import re
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
import gen_questions_h as generator


class QuestionsTest(unittest.TestCase):
    def test_all_source_questions_have_curated_french_and_four_options(self):
        rows = generator.parse_markdown((ROOT / "cau-hoi-van-hoa-viet-phap.md").read_text(encoding="utf-8"))
        self.assertEqual(len(rows), 20)
        rendered = generator.render(rows)
        self.assertEqual(rendered.count("Question{"), 20)
        self.assertEqual(len(re.findall(r"static const char q_\d+_fr\[\] PROGMEM", rendered)), 20)
        self.assertIn("QUESTION_COUNT = 20", rendered)

    def test_generation_is_reproducible(self):
        with tempfile.TemporaryDirectory() as d:
            dest = Path(d) / "questions.h"
            subprocess.run([sys.executable, str(ROOT / "gen_questions_h.py"), "--output", str(dest)], check=True)
            self.assertEqual(dest.read_bytes(), (ROOT / "questions.h").read_bytes())

    def test_new_untranslated_question_fails_closed(self):
        with self.assertRaisesRegex(ValueError, "biên tập"):
            generator.render([(21, "Câu mới?", "Đáp án")])


if __name__ == "__main__":
    unittest.main()
