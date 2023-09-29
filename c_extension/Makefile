all: # работает со второго раза (не знаю почему)
	python -m venv venv
	source venv/bin/activate && pip install -r requirements.txt && python setup.py build && python setup.py install
test:
	source venv/bin/activate && pytest -s -vv
clear:
	rm -rf venv
	rm -rf build
	rm -rf dist
	rm -rf .pytest_cache
	rm -rf __pycache__
	rm -rf cjson.egg-info
