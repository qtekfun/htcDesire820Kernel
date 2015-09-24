
#ifndef _directx_h
#define _directx_h


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#ifndef WIN32
#define WIN32
#endif
#undef  WINNT

#ifndef FAR
#define FAR
#endif

#ifndef MAKE_HRESULT
#define MAKE_HRESULT(sev,fac,code) \
	((HRESULT)(((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))))
#endif

#ifndef S_OK
#define S_OK		(HRESULT)0x00000000L
#endif

#ifndef SUCCEEDED
#define SUCCEEDED(x)	((HRESULT)(x) >= 0)
#endif
#ifndef FAILED
#define FAILED(x)	((HRESULT)(x)<0)
#endif

#ifndef E_FAIL
#define E_FAIL		(HRESULT)0x80000008L
#endif
#ifndef E_NOINTERFACE
#define E_NOINTERFACE	(HRESULT)0x80004002L
#endif
#ifndef E_OUTOFMEMORY
#define E_OUTOFMEMORY	(HRESULT)0x8007000EL
#endif
#ifndef E_INVALIDARG
#define E_INVALIDARG	(HRESULT)0x80070057L
#endif
#ifndef E_NOTIMPL
#define E_NOTIMPL	(HRESULT)0x80004001L
#endif
#ifndef REGDB_E_CLASSNOTREG
#define REGDB_E_CLASSNOTREG	(HRESULT)0x80040154L
#endif

#ifndef SEVERITY_ERROR
#define SEVERITY_ERROR	1
#endif

#ifndef FACILITY_WIN32
#define FACILITY_WIN32	7
#endif

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field)    ((LONG)&(((type *)0)->field))
#endif

#define DIRECTDRAW_VERSION  0x0700
#define DIRECTSOUND_VERSION 0x0500
#define DIRECTINPUT_VERSION 0x0500

#include <ddraw.h>
#include <dsound.h>
#include <dinput.h>

#endif 
