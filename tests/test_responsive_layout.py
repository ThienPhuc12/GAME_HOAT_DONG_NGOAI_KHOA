"""Real-browser checks for the three time-sensitive player screens."""

import shutil
import unittest
from pathlib import Path

try:
    from playwright.sync_api import sync_playwright
except ImportError:
    sync_playwright = None


HTML = (Path(__file__).resolve().parents[1] / "web" / "player.html").as_uri()
CHROME = next(
    (
        path
        for path in (
            shutil.which("chrome"),
            shutil.which("google-chrome"),
            r"C:\Program Files\Google\Chrome\Application\chrome.exe",
        )
        if path and Path(path).is_file()
    ),
    None,
)


@unittest.skipUnless(sync_playwright and CHROME, "Playwright and Chrome required")
class ResponsiveLayoutTest(unittest.TestCase):
    def test_time_sensitive_controls_fit_viewport(self):
        with sync_playwright() as playwright:
            browser = playwright.chromium.launch(
                executable_path=CHROME, headless=True, args=["--no-sandbox"]
            )
            try:
                for width, height in ((320, 568), (390, 844), (667, 375), (844, 390), (1440, 900)):
                    with self.subTest(viewport=(width, height)):
                        page = browser.new_page(viewport={"width": width, "height": height})
                        page.goto(HTML, wait_until="domcontentloaded")
                        for state, expected_buttons in (
                            ("TRIVIA_10S", 4),
                            ("CHAR_SELECT_8S", 2),
                            ("AIM_STANDOFF", 3),
                        ):
                            with self.subTest(state=state):
                                page.evaluate(
                                    """state => {
                                      app.identityReady = true;
                                      app.me = {role: 0, team: 1, name: 'Lan'};
                                      app.game = {
                                        state, shootingTeam: 1, scoreFR: 0, scoreVN: 0,
                                        blockedLane: -1, remainingMs: 7000, triviaMs: 10000,
                                        qFr: 'Quel monument célèbre représente la France et où se trouve-t-il ?',
                                        qVi: 'Công trình nổi tiếng nào đại diện cho nước Pháp và nằm ở đâu?',
                                        options: [
                                          'La tour Eiffel à Paris | Tháp Eiffel ở Paris',
                                          'Le pont du Dragon à Da Nang | Cầu Rồng ở Đà Nẵng',
                                          'Le musée du Louvre à Paris | Bảo tàng Louvre ở Paris',
                                          'La cathédrale Notre-Dame de Paris | Nhà thờ Đức Bà Paris'
                                        ]
                                      };
                                      app.deadline = performance.now() + 7000;
                                      render();
                                    }""",
                                    state,
                                )
                                result = page.evaluate(
                                    """() => {
                                      const buttons = [...document.querySelectorAll('#screen button')];
                                      const hidden = buttons.filter(button => {
                                        const rect = button.getBoundingClientRect();
                                        return rect.left < 0 || rect.right > innerWidth ||
                                          rect.top < 0 || rect.bottom > innerHeight;
                                      });
                                      return {
                                        pageScroll: document.documentElement.scrollHeight > innerHeight + 1,
                                        buttons: buttons.length, hidden: hidden.length
                                      };
                                    }"""
                                )
                                self.assertFalse(result["pageScroll"])
                                self.assertEqual(result["buttons"], expected_buttons)
                                self.assertEqual(result["hidden"], 0)
                        page.close()
            finally:
                browser.close()

    def test_desktop_host_rosters_flank_arena(self):
        with sync_playwright() as playwright:
            browser = playwright.chromium.launch(
                executable_path=CHROME, headless=True, args=["--no-sandbox"]
            )
            try:
                page = browser.new_page(viewport={"width": 1440, "height": 900})
                page.goto(HTML, wait_until="domcontentloaded")
                page.evaluate(
                    """() => {
                      app.identityReady = true;
                      app.me = {role: 2, host: true, team: -1};
                      app.game = {state: 'LOBBY', scoreFR: 0, scoreVN: 0,
                        roster: [{team: 0, role: 0, name: 'Pierre'},
                                 {team: 1, role: 0, name: 'Lan'}]};
                      render();
                    }"""
                )
                result = page.evaluate(
                    """() => {
                      const box = selector => document.querySelector(selector).getBoundingClientRect();
                      return {
                        left: box('#desktopRosterFR').right,
                        centerLeft: box('#screen').left,
                        centerRight: box('#screen').right,
                        right: box('#desktopRosterVN').left,
                        canvasWidth: box('.stage').width,
                        scroll: document.documentElement.scrollHeight > innerHeight + 1
                      };
                    }"""
                )
                self.assertLess(result["left"], result["centerLeft"])
                self.assertLess(result["centerRight"], result["right"])
                self.assertLessEqual(result["canvasWidth"], 960)
                self.assertFalse(result["scroll"])
            finally:
                browser.close()


if __name__ == "__main__":
    unittest.main()
