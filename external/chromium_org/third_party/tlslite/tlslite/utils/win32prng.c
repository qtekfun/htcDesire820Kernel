
#include "Python.h"
#define _WIN32_WINNT 0x0400	  
#include <windows.h>


static PyObject* getRandomBytes(PyObject *self, PyObject *args)
{
	int howMany;
	HCRYPTPROV hCryptProv;
	unsigned char* bytes = NULL;
	PyObject* returnVal = NULL;


	
    if (!PyArg_ParseTuple(args, "i", &howMany))
    	return(NULL);

	
	if(CryptAcquireContext(
	   &hCryptProv,
	   NULL,
	   NULL,
	   PROV_RSA_FULL,
	   CRYPT_VERIFYCONTEXT) == 0)
		return Py_BuildValue("s#", NULL, 0);


	
	bytes = malloc(howMany);


	
	if(CryptGenRandom(
	   hCryptProv,
	   howMany,
	   bytes) == 0)
		returnVal = Py_BuildValue("s#", NULL, 0);
	else
		returnVal = Py_BuildValue("s#", bytes, howMany);

	free(bytes);
	CryptReleaseContext(hCryptProv, 0);

	return returnVal;
}




static struct PyMethodDef win32prng_functions[] = {
    {"getRandomBytes", (PyCFunction)getRandomBytes, METH_VARARGS},
    {NULL,  NULL}        
};



DL_EXPORT(void) initwin32prng(void)
{
    Py_InitModule("win32prng", win32prng_functions);
}
