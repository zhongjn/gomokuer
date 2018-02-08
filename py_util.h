#pragma once

#include <iostream>
#include <Python.h>
#include <numpy/arrayobject.h>
#include <atlconv.h>

namespace py_util {
	
	void init_python();
	extern PyObject *module;
	void init_py_util();
}

