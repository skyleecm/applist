#include <e32base.h>	// CBase
#include <apgcli.h>		// RApaLsSession, link against apgrfx.lib

#include <Python.h>
#include "symbian_python_ext_util.h"

/*
Get list of applications info.
Returns a list of tuples (Uid, caption, fullName)

import applist
apps = applist.applist()

    @copyright: 2007 -  Lee Chee Meng skyleecm@gmail.com
    @License: LGPL
*/

static PyObject *
applist(PyObject *self, PyObject *args)
{
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);
	
	User::LeaveIfError(ls.GetAllApps());
	TInt iAppCount = 0;
	ls.AppCount(iAppCount);
 
	TInt Errnono(KErrNone);
	TApaAppInfo appInfo;
	
	PyObject * tup;
	PyObject * list;
	int i = 0;
	list = PyList_New(iAppCount);
	do
	{
		Errnono = ls.GetNextApp(appInfo);
		if (KErrNone == Errnono && appInfo.iCaption.Length())
		{	
			tup = Py_BuildValue("(i,u#,u#)", appInfo.iUid.iUid, appInfo.iCaption.Ptr(), appInfo.iCaption.Length(), 
				appInfo.iFullName.Ptr(), appInfo.iFullName.Length());
			PyList_SetItem(list, i, tup);
			i++;
		}
 
	} while(KErrNone == Errnono);
	
	CleanupStack::PopAndDestroy(); // ls
	
	return list;
}

static const PyMethodDef Methods[] = {
    {"applist",        applist,       METH_VARARGS, "Get list of applications info. Returns a list of tuples (Uid, caption, fullName)"}
   ,{NULL, NULL, 0, NULL}        /* Sentinel */
};

#define PyMODINIT_FUNC extern "C" DL_EXPORT(void) // PyMODINIT_FUNC not defined in PyS60 ??

PyMODINIT_FUNC
initmodule()
{
    Py_InitModule("applist", Methods);
}

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason)
{
  return KErrNone;
}
#endif

