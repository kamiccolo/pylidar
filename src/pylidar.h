/*
 * pylidar.h
 *
 *
 * This file is part of PyLidar
 * Copyright (C) 2015 John Armston, Pete Bunting, Neil Flood, Sam Gillingham
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
A collection of C functions for dealing with numpy structured 
arrays from C/C++.
*/

#ifndef PYLIDAR_H
#define PYLIDAR_H

#include <Python.h>
#include "numpy/arrayobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/* call first - preferably in the init() of your module
 this sets up the connection to numpy */
#if PY_MAJOR_VERSION >= 3
PyObject *pylidar_init();
#else
void pylidar_init();
#endif
/* print error - used internally */
void pylidar_error(char *errstr, ...);

/* Helper function to get information about a named field within an array
 pass null for params you not interested in */
int pylidar_getFieldDescr(PyObject *pArray, const char *pszName, int *pnOffset, char *pcKind, int *pnSize, int *pnLength);

/* return a field as a npy_int64 array. Caller to delete */
npy_int64 *pylidar_getFieldAsInt64(PyObject *pArray, const char *pszName);

/* return a field as a double array. Caller to delete */
double *pylidar_getFieldAsFloat64(PyObject *pArray, const char *pszName);


/* structure for defining a numpy structured array from a C one
 create using CREATE_FIELD_DEFN below */
typedef struct
{
    const char *pszName;
    char cKind; // 'i' for signed int, 'u' for unsigned int, 'f' for float
    int nSize;
    int nOffset;
    int nStructTotalSize;
} SpylidarFieldDefn;

#define CREATE_FIELD_DEFN(STRUCT, FIELD, KIND) \
    {#FIELD, KIND, sizeof(((STRUCT*)0)->FIELD), offsetof(STRUCT, FIELD), sizeof(STRUCT)}

/* 
Here is an example of use:
//Say you had a structure like this:
typedef struct {
    double x,
    double y,
    int count
} SMyStruct;

//Create an array of structures defining the fields like this:
static SpylidarFieldDefn fields[] = {
    CREATE_FIELD_DEFN(SMyStruct, x, 'f'),
    CREATE_FIELD_DEFN(SMyStruct, y, 'f'),
    CREATE_FIELD_DEFN(SMyStruct, count, 'i'),
    {NULL} // Sentinel 
};

// Kind is one of the following (see http://docs.scipy.org/doc/numpy/reference/arrays.dtypes.html)
'b'     boolean
'i'     (signed) integer
'u'     unsigned integer
'f'     floating-point
'c'     complex-floating point
'O'     (Python) objects
'S', 'a'    (byte-)string
'U'     Unicode
'V'     raw data (void)

*/
/* Wrap an existing C array of structures and return as a numpy array */
/* Python will free data when finished */
PyObject *pylidar_structArrayToNumpy(void *pStructArray, npy_intp nElems, SpylidarFieldDefn *pDefn);

#ifdef __cplusplus
} // extern C
#endif

#endif /*PYLIDAR_H*/

