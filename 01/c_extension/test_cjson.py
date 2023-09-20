from time import time
import json
import random
import string


# import ujson


import cjson


LETTERS = string.ascii_letters


def timer(func):
    def wrapper(*args, **kwargs):
        start = time()
        res = func(*args, **kwargs)
        end = time()
        wrapper.last_execution_time = end - start
        return res
    return wrapper


cjson.dumps = timer(cjson.dumps)
json.dumps = timer(json.dumps)
cjson.loads = timer(cjson.loads)
json.loads = timer(json.loads)


def random_string():
    lst = (random.choice(LETTERS) for _ in range(random.randint(4, 10)))
    return ''.join(lst)


def random_dict():
    data = {}
    for _ in range(random.randint(100, 200)):
        key = random_string()
        # value = random_string()
        if bool(random.getrandbits(1)):
            value = random_string()
        else:
            value = random.randint(- 100000000, 100000000)
        data[key] = value
    return data


def test_loads():
    json_inp = '{"key1": "Word1 word2", "key2": "word2 word3"}'
    assert cjson.loads(json_inp) == json.loads(json_inp)


def test_dumps():
    dictionary = {"key1": "Word1 word2", "key2": "word2 word3"}
    assert cjson.dumps(dictionary) == json.dumps(dictionary)


def test_integers():
    dictionary = {"key1": 1, "key2": "word2 word3", "key3": 3}
    assert cjson.dumps(dictionary) == json.dumps(dictionary)


def test_random_input():
    data = random_dict()

    cjson_string, json_string = cjson.dumps(data), json.dumps(data)
    assert cjson_string == json_string

    print(f'''

    json.dumps execution: {json.dumps.last_execution_time} seconds
    cjson.dumps ecexution: {cjson.dumps.last_execution_time} seconds''')

    cjson_dict, json_dict = cjson.loads(json_string), json.loads(json_string)
    assert cjson_dict == json_dict

    print(f'''
    json.dumps execution: {json.loads.last_execution_time} seconds
    cjson.dumps ecexution: {cjson.loads.last_execution_time} seconds''')


def test_predefined_input():
    with open('test.json', 'r', encoding='utf-8') as file_handle:
        data = json.load(file_handle)
    text = json.dumps(data)
    assert cjson.dumps(data) == text
    print(f'''

    json.dumps execution: {json.dumps.last_execution_time} seconds
    cjson.dumps ecexution: {cjson.dumps.last_execution_time} seconds''')

    assert cjson.loads(text) == json.loads(text)
    print(f'''
    json.loads execution: {json.loads.last_execution_time} seconds
    cjson.loads ecexution: {cjson.loads.last_execution_time} seconds''')
