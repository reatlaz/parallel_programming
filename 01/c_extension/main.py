#! /usr/bin/env python3

import cjson


def main():
    # res = cjson.loads('{"key1":"Word1 word2","key2":"word2 word3"}')
    # print(res)
    print(cjson.dumps({"key1": "Word1 word2", "key2": "word2 word3"}))


if __name__ == "__main__":
    main()
