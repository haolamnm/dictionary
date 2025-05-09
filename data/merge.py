# Merge all the raw text files into 1 file

import os

words: set[str] = set()

words_path = "all/words.txt"

for file in os.listdir("raw"):
    # if file == "opted.txt":
    #     continue
    filepath = os.path.join("raw", file)
    with open(filepath) as f:
        lines = f.readlines()
        print(len(lines))
        for word in lines:
            words.add(word.strip().lower())

temp = words.copy()
for word in temp:
    if not word.isalpha():
        words.remove(word)
del temp

with open(words_path, mode="w") as file:
    file.writelines(word + "\n" for word in sorted(words))
