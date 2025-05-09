import csv
import time
from operator import itemgetter

import requests


class Normalizer:
    def __init__(self) -> None:
        self.session = requests.Session()

    def normalize(self, src: str, dest: str) -> None:
        """Normalize src file into dest file"""
        count = 0
        with (
            open(src, newline="", encoding="utf-8") as fin,
            open(dest, newline="", encoding="utf-8", mode="w") as fout,
        ):
            reader = csv.reader(fin)
            writer = csv.writer(fout)

            header = ["text", "pos", "definition"]
            writer.writerow(header)

            for row in reader:
                if not row or len(row) != 3:
                    continue

                # If word contains symbol, spaces
                word = row[0]
                if not word.isalpha():
                    continue

                # Normalize each rows
                row_normalized = self._normalize_row(row)

                if row_normalized[1] != "undefined":
                    writer.writerow(row_normalized)

                count += 1

                if count % 100 == 0:
                    print(f"Row #{count} done")

        # Ensure words are sorted
        self._sort(dest)

    def fetch(self, src: str, dest: str) -> None:
        """Fetch data from API for each word in src txt file and store in dest csv file"""
        with open(src, encoding="utf-8") as fin:
            words = [line.strip() for line in fin if line.strip()]

        entries = []
        for word in words:
            try:
                word_entries = self._fetch_data(word)
                entries.extend(word_entries)
                time.sleep(3)  # Wait 5 seconds between requests to avoid rate limiting
            except Exception as e:
                print(f"Error processing {word}: {e}")
                entries.append((word, "undefined", "Not found"))

        # Write to CSV with header
        with open(dest, "w", newline="", encoding="utf-8") as fout:
            writer = csv.writer(fout)
            writer.writerow(["text", "pos", "definition"])
            for entry in entries:
                writer.writerow(entry)

    def merge(self, src1: str, src2: str, dest: str) -> None:
        """Merge two csv to avoid duplicate entries"""
        rows = []
        for src in [src1, src2]:
            with open(src, newline="", encoding="utf-8") as fin:
                reader = csv.reader(fin)
                next(reader)  # Skip header
                for row in reader:
                    if row and len(row) == 3:
                        rows.append(row)

        # Sort the combined rows
        rows.sort(key=itemgetter(0))

        # Write to destination CSV with header
        with open(dest, "w", newline="", encoding="utf-8") as fout:
            writer = csv.writer(fout)
            writer.writerow(["text", "pos", "definition"])
            for row in rows:
                writer.writerow(row)

        print(f"Merged and sorted {len(rows)} rows into {dest}")

    def _normalize_pos(self, pos: str, definition: str) -> str:
        pos = pos.lower().strip()

        if any(
            x in pos
            for x in ["verb", "v.", "vb.", "p. pr.", "imp.", "p. p.", "v. t.", "v. i.", "3d sing. pres.", "obs. p. p."]
        ):
            return "verb"
        if any(x in pos for x in ["noun", "n.", "pl.", "n. pl.", "vb. n."]):
            if definition.strip().startswith("To "):
                return "verb"
            return "noun"
        if any(x in pos for x in ["adjective", "a.", "adj.", "super.", "determiner", "article"]):
            return "adjective"
        if "adverb" in pos or "adv." in pos:
            return "adverb"
        if "pronoun" in pos or "pron." in pos:
            return "pronoun"
        if "preposition" in pos or "prep." in pos:
            return "preposition"
        if "conjunction" in pos or "conj." in pos:
            return "conjunction"
        if "interjection" in pos or "interj." in pos:
            return "interjection"
        return "undefined"

    def _normalize_row(self, row: list[str]) -> list[str]:
        word, pos, definition = row
        word = word.lower()
        pos = self._normalize_pos(pos, definition)
        definition = definition.replace("--", "")
        return [word, pos, definition]

    def _sort(self, file: str) -> None:
        rows = []
        with open(file, newline="", encoding="utf-8") as fin:
            reader = csv.reader(fin)
            next(reader)  # Skip header
            for row in reader:
                if not row or len(row) != 3:
                    continue
                rows.append(row)

        rows.sort(key=itemgetter(0))

        with open(file, newline="", encoding="utf-8", mode="w") as fout:
            writer = csv.writer(fout)
            writer.writerow(["text", "pos", "definition"])
            for row in rows:
                writer.writerow(row)

        print(f"Sorted {len(rows)} rows in {file}")

    def _fetch_data(self, word: str) -> list[tuple[str, str, str]]:
        """Fetch word data from the API and return a list of entries"""
        entries = []
        try:
            response = self.session.get(f"https://api.dictionaryapi.dev/api/v2/entries/en/{word}", timeout=5)
            response.raise_for_status()
            data = response.json()
            for entry in data:
                for meaning in entry.get("meanings", []):
                    pos = meaning.get("partOfSpeech", "undefined")
                    definitions = meaning.get("definitions", [])
                    for definition in definitions:
                        definition_text = definition.get("definition", "Not found")
                        entries.append((word, pos, definition_text))
            if not entries:
                entries.append((word, "undefined", "Not found"))

            print(f"Entry {word} done.")
            return entries
        except requests.RequestException as e:
            print(f"Error fetching data for {word}: {e}")
            return [(word, "undefined", "Not found")]


if __name__ == "__main__":
    normalizer = Normalizer()
    normalizer._sort("dictionary.csv")
