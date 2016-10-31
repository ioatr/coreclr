#include <Python.h>
// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include <stdio.h>
#include <qbattlecommon.h>
#include <string>
#include <string.h>
#include <sys/stat.h>

static PyObject *QBattleError;
static QBATTLE_INIT_FUNC init;
static QBATTLE_START_FUNC start;
static QBATTLE_UPDATE_FUNC update;
static QBATTLE_FREE_HGLOBAL_FUNC freeHGlobal;


static PyObject * qbattle_start(PyObject *self, PyObject *args)
{
        start();
        return PyLong_FromLong(0);
}

static PyObject * qbattle_init(PyObject *self, PyObject *args)
{
	init();
	return PyLong_FromLong(0);
}

static PyObject * qbattle_update(PyObject *self, PyObject *args)
{
	float param[5];
	if (!PyArg_ParseTuple(args, "fffff", &param[0], &param[1], &param[2], &param[3], &param[4]))
		return NULL;

	const char * ret_cstr = update(param);
	PyObject* ret = PyString_FromString(ret_cstr);
	freeHGlobal(ret_cstr);
	return ret;
}

static void setup()
{
    const char* clrFilesPath;
    const char* managedAssemblyPath;

    char pathBuf[2048];
    char qbattlePath[2048];
    getcwd(pathBuf, 2048);
    strcat(pathBuf, "/coreclr");

    printf("%s\n",pathBuf);
    sprintf(qbattlePath, "%s/qbattlelib.so", pathBuf);

    // Make sure we have a full path for argv[0].
    std::string argv0AbsolutePath;
    if (!GetAbsolutePath(qbattlePath, argv0AbsolutePath))
    {
        perror("Could not get full path");
    }

    std::string clrFilesAbsolutePath;
    if(!GetClrFilesAbsolutePath(argv0AbsolutePath.c_str(), pathBuf, clrFilesAbsolutePath))
    {
        perror("Could not get clr path");
    }

    strcat(pathBuf, "/qbattle/qbattle.dll");

    std::string managedAssemblyAbsolutePath;
    if (!GetAbsolutePath(pathBuf, managedAssemblyAbsolutePath))
    {
        perror("Failed to convert managed assembly path to absolute path");
    }

    int exitCode = initAssembly(
                            argv0AbsolutePath.c_str(),
                            clrFilesAbsolutePath.c_str(),
                            managedAssemblyAbsolutePath.c_str(),
                            &init,
                            &start,
                            &update,
                            &freeHGlobal);

    fprintf(stderr, "setup complete\n");
}


static PyMethodDef QBattleMethods[] = {
    {"init",  qbattle_init, METH_VARARGS, "init"},
    {"start",  qbattle_start, METH_VARARGS, "start"},
    {"update",  qbattle_update, METH_VARARGS, "update"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initqbattlelib(void)
{
    PyObject *m;

    m = Py_InitModule("qbattlelib", QBattleMethods);
    if (m == NULL)
        return;

    setup();

    QBattleError = PyErr_NewException((char*)"qbattlelib.error", NULL, NULL);
    Py_INCREF(QBattleError);
    PyModule_AddObject(m, "error", QBattleError);
}
