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
    def test_all_fifty_questions_and_their_answers_are_preserved(self):
        rows = generator.parse_markdown((ROOT / "File _cau_hoi_moi.md").read_text(encoding="utf-8"))
        self.assertEqual([row.id for row in rows], list(range(1, 51)))
        self.assertEqual(rows[0].correct_idx, 1)
        self.assertEqual(rows[-1].correct_idx, 1)
        self.assertIn("Guerriers de l'Étoile d'Or", rows[0].options[1][0])
        self.assertIn("Những chiến binh Sao Vàng", rows[0].options[1][1])
        rendered = generator.render(rows)
        self.assertEqual(rendered.count("Question{"), 50)
        self.assertEqual(len(re.findall(r"static const char q_\d+_fr\[\] PROGMEM", rendered)), 50)
        self.assertIn("QUESTION_COUNT = 50", rendered)

    def test_generation_is_reproducible(self):
        with tempfile.TemporaryDirectory() as d:
            dest = Path(d) / "questions.h"
            subprocess.run([sys.executable, str(ROOT / "gen_questions_h.py"), "--output", str(dest)], check=True)
            self.assertEqual(dest.read_bytes(), (ROOT / "questions.h").read_bytes())

    def test_missing_answer_is_rejected(self):
        source = "**Câu 1: Một?**\n*(Question 1 : Un ?)*\nA. A\nB. B\nC. C\nD. D\n"
        with self.assertRaisesRegex(ValueError, "đáp án"):
            generator.parse_markdown(source)


if __name__ == "__main__":
    unittest.main()
