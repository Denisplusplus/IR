import os
import json
from pathlib import Path
from trash import trash
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
nltk.download("stopwords")

russian_stopwords = stopwords.words("russian")

def replacer(text, dic):
    for i, j in dic.items():
        text = text.replace(i, j)
    return text


def preprocess_text(text):
    text = text.lower()
    text = replacer(text, trash)

    tokens = word_tokenize(text, language="russian")
    tokens = [token for token in tokens if token not in russian_stopwords]

    text = " ".join(tokens)
    text = " ".join(text.split())
    return text


if __name__ == '__main__':

    raw_dir_path = "/Users/denis/MAI/IR/lab3/wikipedia_articles_one_by_one"
    raw_dir_list = [(raw_dir_path + "/" +  i) for i in sorted(
        os.listdir(raw_dir_path),
        key=lambda x: int(x.replace(".json", "").replace("wiki_article_", ""))
        )]

    new_path = "/Users/denis/MAI/IR/lab3/wikipedia_articles_tokenized"

    for filepath in raw_dir_list:
        with open(filepath) as file:
            data = json.load(file)
            data["text"] = preprocess_text(data["text"])        
            new_name = f"wiki_article_{data['id']}.json"
            with open(new_path + "/" + new_name, "w", encoding='utf8') as new_file:
                json.dump(data, new_file, ensure_ascii=False)

