#include <stdlib.h>
#include <stdio.h>

#include <Python.h>

void slice(const char* str, char* result, size_t start, size_t end)
{   

    strncpy(result, str + start, end - start);
}

int isDigits(const char* str)
{
    int i = 0;
    while (str[i] != '\0') {
        if (isalpha(str[i]))
            return 0;
        i++;
    }
    return 1;
}

PyObject* cjson_loads(PyObject* self, PyObject* args)
{
    // printf("start\n");
    char *jsonString;

    if(!PyArg_ParseTuple(args, "s", &jsonString))
    {
        printf("ERROR: Failed to read json string\n");
        return NULL;
    }

    int i = 1;
    int start = 1;

    PyObject *dict = NULL;
    if (!(dict = PyDict_New())) {
        printf("ERROR: Failed to create Dict Object\n");
        return NULL;
    }

    PyObject *key = NULL;
    PyObject *value = NULL;

    char *ckey = realloc(NULL, sizeof(char));
    char *cval = realloc(NULL, sizeof(char));

    while (jsonString[i] != '\0') {
        // printf("%c\n", jsonString[i]);
        if (jsonString[i] == '"') {
            if (jsonString[i-1] == ' ') {
                start ++;
            }
            i ++;
            continue;
        }
        if (jsonString[i] == ':') {
            ckey = realloc(ckey, sizeof(char)*(i-start-1));
            ckey[i-start-2]='\0';
            slice(jsonString, ckey, start+1, i-1);
            start = i + 1;

            // ckey[i-start] = '\0'
            
            // if (jsonString[start] == ' ') {
            //     start++;
            // }
        }
        if (jsonString[i] == ',' || jsonString[i] == '}') {
            if (jsonString[i-1] == '"') {
                cval = realloc(cval, sizeof(char)*(i-start-1));
                cval[i-start-2]='\0';
                slice(jsonString, cval, start+1, i-1);
            } else {
                cval = realloc(cval, sizeof(char)*(i-start));
                cval[i-start-1]='\0';
                slice(jsonString, cval, start+1, i);
            }
            start = i + 1;

            // cval[i-start] = '\0'
            
            // if (jsonString[start] == ' ') {
            //     start++;
            // }       

            if (!(key = Py_BuildValue("s", ckey))) {
                printf("ERROR: Failed to build string value\n");
                return NULL;
            }
            if (isDigits(cval))
            {
                if (!(value = Py_BuildValue("i", atoi(cval)))) {
                    printf("ERROR: Failed to build string value\n");
                    return NULL;
                }
            }
            else
            {
                if (!(value = Py_BuildValue("s", cval))) {
                    printf("ERROR: Failed to build string value\n");
                    return NULL;
                }
            }
            if (PyDict_SetItem(dict, key, value) < 0) {
                printf("ERROR: Failed to set item\n");
                return NULL;
            }

        }
        i++;
    }

    return dict;
}

PyObject* cjson_dumps(PyObject* self, PyObject* args)
{
    PyObject *dict;

    if(!PyArg_ParseTuple(args, "O", &dict))
    {
        printf("ERROR: Failed to read json string\n");
        return NULL;
    }

    int i = 0;
    int j = 0;

    char string[100024] = "";

    PyObject *key, *value;
    Py_ssize_t pos = 0;

    string[i] = '{';
    i += 1;

    while (PyDict_Next(dict, &pos, &key, &value)) {
        // printf("%zd\n", pos);
        string[i] = '"';
        i++;
        wchar_t * stringKey = PyUnicode_AS_UNICODE(key);
        while (stringKey[j] != '\0') {
            string[i] = stringKey[j];
            j++;
            i++;
        }
        string[i] = '"';
        i++;

        j = 0;
        string[i] = ':';
        i++;
        string[i] = ' ';
        i++;



        if (PyLong_Check(value)) {
            long longValue = PyLong_AsLong(value);
            char stringVal[2560];
            sprintf(stringVal, "%lld", longValue);
            while (stringVal[j] != '\0') {
                string[i] = stringVal[j];
                j++;
                i++;
            }

            j = 0;
            string[i] = ',';
            i++;
            string[i] = ' ';
            i++;
        }
        else {
            string[i] = '"';
            i++;
            wchar_t * stringVal = PyUnicode_AS_UNICODE(value);
            while (stringVal[j] != '\0') {
                string[i] = stringVal[j];
                j++;
                i++;
            }
            string[i] = '"';
            i++;

            j = 0;
            string[i] = ',';
            i++;
            string[i] = ' ';
            i++;
        }


    }
    i--;
    string[i] = '\0';
    i--;
    string[i] = '}';

    return PyUnicode_FromString(string);
}


static PyMethodDef methods[] = {
    {"loads", cjson_loads, METH_VARARGS, "Parse json to python dict"},
    {"dumps", cjson_dumps, METH_VARARGS, "Convert dict to json string"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cjsonmodule = {
    PyModuleDef_HEAD_INIT,
    "cjson",
    "Module for my first c api code.",
    -1,
    methods
};

PyMODINIT_FUNC PyInit_cjson(void)
{
    return PyModule_Create( &cjsonmodule );
}
