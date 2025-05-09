# Find word without meanings


with open("raw/opted.txt") as file:
    opted_words = file.readlines()

distinct_words = []

with open("words.txt") as file:
    words = file.readlines()

    for word in words:
        if word not in opted_words:
            distinct_words.append(word)

with open("distinct.txt", "w") as file:
    file.writelines(distinct_words)
