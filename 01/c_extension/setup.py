#! /usr/bin/env python3

from setuptools import setup, Extension


def main():
    setup(name="cjson",
          version="1.0.1",
          author="Rinat Afiatillov",
          ext_modules=[
              Extension('cjson', ['cjson.c'])
              ])


if __name__ == "__main__":
    main()
