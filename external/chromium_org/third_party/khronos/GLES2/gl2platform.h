#ifndef __gl2platform_h_
#define __gl2platform_h_


/*
 * This document is licensed under the SGI Free Software B License Version
 * 2.0. For details, see http://oss.sgi.com/projects/FreeB/ .
 */


#include <KHR/khrplatform.h>

#if defined(COMPONENT_BUILD)
#if defined(WIN32)
#if defined(GLES2_C_LIB_IMPLEMENTATION)
#define GL_APICALL __declspec(dllexport)
#else
#define GL_APICALL __declspec(dllimport)
#endif  
#else 
#define GL_APICALL __attribute__((visibility("default")))
#endif
#else
#   define GL_APICALL
#endif

#ifndef GL_APIENTRY
#define GL_APIENTRY KHRONOS_APIENTRY
#endif

#undef GL_APIENTRY
#define GL_APIENTRY

#endif 
