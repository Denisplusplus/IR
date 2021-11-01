import time
import re
import mwxml
import json

import os

import logging

DUMP_PATH = './ruwiki-20211001-pages-articles.xml'
OUTPUT_PATH = './parsed_dump/'


def process_dump(wiki_xml_dump_path, articles_output_directory):
    start_time = time.time()
    logger.info(f'Start processing wiki dump. Input xml file = {wiki_xml_dump_path}. Saving results to'
                f' {articles_output_directory}')

    dump = mwxml.Dump.from_file(open(wiki_xml_dump_path))

    pages_count = 0
    bytes_count = 0
    for page in dump:
        pages_count += 1

        for revision in page:
            bytes_count += revision.bytes


            data_to_write = {
                "id": page.id,
                "text": stripped,
                "text_size": revision.bytes,
            }

            json_object = json.dumps(data_to_write, ensure_ascii=False, indent=4)
            page_output_path = os.path.join(articles_output_directory, str(page.id) + '.json')

            with open(page_output_path, 'w') as outfile:
                outfile.write(json_object)

    logger.info(f'Number of pages = {pages_count}')
    logger.info(f'Size of text (bytes) = {bytes_count}')
    logger.info(f'Taken time to process the dump: {(time.time() - start_time)}s')


if __name__ == '__main__':
    sum_images = 1
    app_name = os.path.splitext(os.path.basename(__file__))[0]

    logger = logging.getLogger(app_name)
    formatter = logging.Formatter(fmt='%(asctime)s %(levelname)s %(name)s: %(message)s', datefmt='%y/%m/%d %H:%M:%S')
    streamHandler = logging.StreamHandler()
    streamHandler.setFormatter(formatter)
    logger.addHandler(streamHandler)
    logger.setLevel(logging.INFO)

    process_dump(DUMP_PATH, OUTPUT_PATH)
