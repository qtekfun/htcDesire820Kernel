
#include "Python.h"


#ifdef MS_WINDOWS

#define _WIN32_WINNT 0x0400

#include <windows.h>


typedef BOOL (WINAPI *CRYPTACQUIRECONTEXTA)(HCRYPTPROV *phProv,\
              LPCSTR pszContainer, LPCSTR pszProvider, DWORD dwProvType,\
              DWORD dwFlags );
typedef BOOL (WINAPI *CRYPTGENRANDOM)(HCRYPTPROV hProv, DWORD dwLen,\
              BYTE *pbBuffer );
typedef BOOL (WINAPI *CRYPTRELEASECONTEXT)(HCRYPTPROV hProv,\
              DWORD dwFlags);


static PyObject* entropy(PyObject *self, PyObject *args)
{
    int howMany = 0;
    HINSTANCE hAdvAPI32 = NULL;
    CRYPTACQUIRECONTEXTA pCryptAcquireContextA = NULL;
    CRYPTGENRANDOM pCryptGenRandom = NULL;
    CRYPTRELEASECONTEXT pCryptReleaseContext = NULL;
    HCRYPTPROV hCryptProv = 0;
    unsigned char* bytes = NULL;
    PyObject* returnVal = NULL;


    
    if (!PyArg_ParseTuple(args, "i", &howMany))
        return(NULL);

	if( (hAdvAPI32 = GetModuleHandle("advapi32.dll")) == NULL) {
        PyErr_Format(PyExc_SystemError,
            "Advapi32.dll not found");
        return NULL;
	}

	pCryptAcquireContextA = (CRYPTACQUIRECONTEXTA)GetProcAddress(hAdvAPI32,\
	                        "CryptAcquireContextA");
	pCryptGenRandom = (CRYPTGENRANDOM)GetProcAddress(hAdvAPI32,\
	                  "CryptGenRandom");
	pCryptReleaseContext = (CRYPTRELEASECONTEXT) GetProcAddress(hAdvAPI32,\
	                       "CryptReleaseContext");
	if (pCryptAcquireContextA == NULL || pCryptGenRandom == NULL ||
	                                     pCryptReleaseContext == NULL) {
        PyErr_Format(PyExc_NotImplementedError,
            "CryptoAPI not available on this version of Windows");
        return NULL;
	}

    
    if ((bytes = (unsigned char*)PyMem_Malloc(howMany)) == NULL)
        return PyErr_NoMemory();


    
    if(!pCryptAcquireContextA(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                            CRYPT_VERIFYCONTEXT)) {
        PyErr_Format(PyExc_SystemError,
                     "CryptAcquireContext failed, error %d", GetLastError());
        PyMem_Free(bytes);
        return NULL;
    }

    
    if(!pCryptGenRandom(hCryptProv, howMany, bytes)) {
        PyErr_Format(PyExc_SystemError,
                     "CryptGenRandom failed, error %d", GetLastError());
        PyMem_Free(bytes);
        CryptReleaseContext(hCryptProv, 0);
        return NULL;
    }

    
    returnVal = Py_BuildValue("s#", bytes, howMany);
    PyMem_Free(bytes);

    
    if (!pCryptReleaseContext(hCryptProv, 0)) {
        PyErr_Format(PyExc_SystemError,
                     "CryptReleaseContext failed, error %d", GetLastError());
        return NULL;
    }

    return returnVal;
}

#elif defined(HAVE_UNISTD_H) && defined(HAVE_FCNTL_H)

#include <unistd.h>
#include <fcntl.h>

static PyObject* entropy(PyObject *self, PyObject *args)
{
    int howMany;
    int fd;
    unsigned char* bytes = NULL;
    PyObject* returnVal = NULL;


    
    if (!PyArg_ParseTuple(args, "i", &howMany))
        return(NULL);

    
    if ((bytes = (unsigned char*)PyMem_Malloc(howMany)) == NULL)
        return PyErr_NoMemory();

    
    if ((fd = open("/dev/urandom", O_RDONLY, 0)) == -1) {
        PyErr_Format(PyExc_NotImplementedError,
            "No entropy source found");
        PyMem_Free(bytes);
        return NULL;
    }

    
    if (read(fd, bytes, howMany) < howMany) {
        PyErr_Format(PyExc_SystemError,
            "Reading from /dev/urandom failed");
        PyMem_Free(bytes);
        close(fd);
        return NULL;
    }

    
    returnVal = Py_BuildValue("s#", bytes, howMany);
    PyMem_Free(bytes);

    
    close(fd);

    return returnVal;
}

#else

static PyObject* entropy(PyObject *self, PyObject *args)
{
    PyErr_Format(PyExc_NotImplementedError,
            "Function not supported");
    return NULL;
}

#endif




static struct PyMethodDef entropy_functions[] = {
    {"entropy", (PyCFunction)entropy, METH_VARARGS, "Return a string of random bytes produced by a platform-specific\nentropy source."},
    {NULL,  NULL}        
};



PyMODINIT_FUNC initentropy(void)
{
    Py_InitModule("entropy", entropy_functions);
}
