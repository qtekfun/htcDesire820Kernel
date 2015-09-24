/*
 * Copyright © 2004, Apple Computer, Inc. and The Mozilla Foundation. 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the names of Apple Computer, Inc. ("Apple") or The Mozilla
 * Foundation ("Mozilla") nor the names of their contributors may be used
 * to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY APPLE, MOZILLA AND THEIR CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE, MOZILLA OR
 * THEIR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Revision 1 (March 4, 2004):
 * Initial proposal.
 *
 * Revision 2 (March 10, 2004):
 * All calls into script were made asynchronous.  Results are
 * provided via the NPScriptResultFunctionPtr callback.
 *
 * Revision 3 (March 10, 2004):
 * Corrected comments to not refer to class retain/release FunctionPtrs.
 *
 * Revision 4 (March 11, 2004):
 * Added additional convenience NPN_SetExceptionWithUTF8().
 * Changed NPHasPropertyFunctionPtr and NPHasMethodFunctionPtr to take NPClass
 * pointers instead of NPObject pointers.
 * Added NPIsValidIdentifier().
 *
 * Revision 5 (March 17, 2004):
 * Added context parameter to result callbacks from ScriptObject functions.
 *
 * Revision 6 (March 29, 2004):
 * Renamed functions implemented by user agent to NPN_*.  Removed _ from
 * type names.
 * Renamed "JavaScript" types to "Script".
 *
 * Revision 7 (April 21, 2004):
 * NPIdentifier becomes a void*, was int32_t
 * Remove NP_IsValidIdentifier, renamed NP_IdentifierFromUTF8 to NP_GetIdentifier
 * Added NPVariant and modified functions to use this new type.
 *
 * Revision 8 (July 9, 2004):
 * Updated to joint Apple-Mozilla license.
 *
 */
#ifndef _NP_RUNTIME_H_
#define _NP_RUNTIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "nptypes.h"

/*
    This API is used to facilitate binding code written in C to script
    objects.  The API in this header does not assume the presence of a
    user agent.  That is, it can be used to bind C code to scripting
    environments outside of the context of a user agent.
    
    However, the normal use of the this API is in the context of a
    scripting environment running in a browser or other user agent.
    In particular it is used to support the extended Netscape
    script-ability API for plugins (NP-SAP).  NP-SAP is an extension
    of the Netscape plugin API.  As such we have adopted the use of
    the "NP" prefix for this API.

    The following NP{N|P}Variables were added to the Netscape plugin
    API (in npapi.h):

    NPNVWindowNPObject
    NPNVPluginElementNPObject
    NPPVpluginScriptableNPObject

    These variables are exposed through NPN_GetValue() and
    NPP_GetValue() (respectively) and are used to establish the
    initial binding between the user agent and native code.  The DOM
    objects in the user agent can be examined and manipulated using
    the NPN_ functions that operate on NPObjects described in this
    header.

    To the extent possible the assumptions about the scripting
    language used by the scripting environment have been minimized.
*/

#define NP_BEGIN_MACRO  do {
#define NP_END_MACRO    } while (0)

typedef struct NPObject NPObject;
typedef struct NPClass NPClass;

typedef char NPUTF8;
typedef struct _NPString {
    const NPUTF8 *utf8characters;
    uint32_t utf8length;
} NPString;

typedef enum {
    NPVariantType_Void,
    NPVariantType_Null,
    NPVariantType_Bool,
    NPVariantType_Int32,
    NPVariantType_Double,
    NPVariantType_String,
    NPVariantType_Object
} NPVariantType;

typedef struct _NPVariant {
    NPVariantType type;
    union {
        bool boolValue;
        uint32_t intValue;
        double doubleValue;
        NPString stringValue;
        NPObject *objectValue;
    } value;
} NPVariant;

void NPN_ReleaseVariantValue(NPVariant *variant);

#define NPVARIANT_IS_VOID(_v)    ((_v).type == NPVariantType_Void)
#define NPVARIANT_IS_NULL(_v)    ((_v).type == NPVariantType_Null)
#define NPVARIANT_IS_BOOLEAN(_v) ((_v).type == NPVariantType_Bool)
#define NPVARIANT_IS_INT32(_v)   ((_v).type == NPVariantType_Int32)
#define NPVARIANT_IS_DOUBLE(_v)  ((_v).type == NPVariantType_Double)
#define NPVARIANT_IS_STRING(_v)  ((_v).type == NPVariantType_String)
#define NPVARIANT_IS_OBJECT(_v)  ((_v).type == NPVariantType_Object)

#define NPVARIANT_TO_BOOLEAN(_v) ((_v).value.boolValue)
#define NPVARIANT_TO_INT32(_v)   ((_v).value.intValue)
#define NPVARIANT_TO_DOUBLE(_v)  ((_v).value.doubleValue)
#define NPVARIANT_TO_STRING(_v)  ((_v).value.stringValue)
#define NPVARIANT_TO_OBJECT(_v)  ((_v).value.objectValue)

#define VOID_TO_NPVARIANT(_v)                                                 \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_Void;                                           \
    (_v).value.objectValue = NULL;                                            \
NP_END_MACRO

#define NULL_TO_NPVARIANT(_v)                                                 \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_Null;                                           \
    (_v).value.objectValue = NULL;                                            \
NP_END_MACRO

#define BOOLEAN_TO_NPVARIANT(_val, _v)                                        \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_Bool;                                           \
    (_v).value.boolValue = !!(_val);                                          \
NP_END_MACRO

#define INT32_TO_NPVARIANT(_val, _v)                                          \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_Int32;                                          \
    (_v).value.intValue = _val;                                               \
NP_END_MACRO

#define DOUBLE_TO_NPVARIANT(_val, _v)                                         \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_Double;                                         \
    (_v).value.doubleValue = _val;                                            \
NP_END_MACRO

#define STRINGZ_TO_NPVARIANT(_val, _v)                                        \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_String;                                         \
    NPString str = { _val, strlen(_val) };                                    \
    (_v).value.stringValue = str;                                             \
NP_END_MACRO

#define STRINGN_TO_NPVARIANT(_val, _len, _v)                                  \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_String;                                         \
    NPString str = { _val, _len };                                            \
    (_v).value.stringValue = str;                                             \
NP_END_MACRO

#define OBJECT_TO_NPVARIANT(_val, _v)                                         \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_Object;                                         \
    (_v).value.objectValue = _val;                                            \
NP_END_MACRO



typedef void *NPIdentifier;

NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name);
void NPN_GetStringIdentifiers(const NPUTF8 **names, int32_t nameCount,
                              NPIdentifier *identifiers);
NPIdentifier NPN_GetIntIdentifier(int32_t intid);
bool NPN_IdentifierIsString(NPIdentifier identifier);

NPUTF8 *NPN_UTF8FromIdentifier(NPIdentifier identifier);

int32_t NPN_IntFromIdentifier(NPIdentifier identifier);

typedef NPObject *(*NPAllocateFunctionPtr)(NPP npp, NPClass *aClass);
typedef void (*NPDeallocateFunctionPtr)(NPObject *npobj);
typedef void (*NPInvalidateFunctionPtr)(NPObject *npobj);
typedef bool (*NPHasMethodFunctionPtr)(NPObject *npobj, NPIdentifier name);
typedef bool (*NPInvokeFunctionPtr)(NPObject *npobj, NPIdentifier name,
                                    const NPVariant *args, uint32_t argCount,
                                    NPVariant *result);
typedef bool (*NPInvokeDefaultFunctionPtr)(NPObject *npobj,
                                           const NPVariant *args,
                                           uint32_t argCount,
                                           NPVariant *result);
typedef bool (*NPHasPropertyFunctionPtr)(NPObject *npobj, NPIdentifier name);
typedef bool (*NPGetPropertyFunctionPtr)(NPObject *npobj, NPIdentifier name,
                                         NPVariant *result);
typedef bool (*NPSetPropertyFunctionPtr)(NPObject *npobj, NPIdentifier name,
                                         const NPVariant *value);
typedef bool (*NPRemovePropertyFunctionPtr)(NPObject *npobj,
                                            NPIdentifier name);
typedef bool (*NPEnumerationFunctionPtr)(NPObject *npobj, NPIdentifier **value,
                                         uint32_t *count);

struct NPClass
{
    uint32_t structVersion;
    NPAllocateFunctionPtr allocate;
    NPDeallocateFunctionPtr deallocate;
    NPInvalidateFunctionPtr invalidate;
    NPHasMethodFunctionPtr hasMethod;
    NPInvokeFunctionPtr invoke;
    NPInvokeDefaultFunctionPtr invokeDefault;
    NPHasPropertyFunctionPtr hasProperty;
    NPGetPropertyFunctionPtr getProperty;
    NPSetPropertyFunctionPtr setProperty;
    NPRemovePropertyFunctionPtr removeProperty;
    NPEnumerationFunctionPtr enumerate;
};

#define NP_CLASS_STRUCT_VERSION      2
#define NP_CLASS_STRUCT_VERSION_ENUM 2

#define NP_CLASS_STRUCT_VERSION_HAS_ENUM(npclass)   \
        ((npclass)->structVersion >= NP_CLASS_STRUCT_VERSION_ENUM)

struct NPObject {
    NPClass *_class;
    uint32_t referenceCount;
};

NPObject *NPN_CreateObject(NPP npp, NPClass *aClass);

NPObject *NPN_RetainObject(NPObject *npobj);

void NPN_ReleaseObject(NPObject *npobj);


bool NPN_Invoke(NPP npp, NPObject *npobj, NPIdentifier methodName,
                const NPVariant *args, uint32_t argCount, NPVariant *result);
bool NPN_InvokeDefault(NPP npp, NPObject *npobj, const NPVariant *args,
                       uint32_t argCount, NPVariant *result);
bool NPN_Evaluate(NPP npp, NPObject *npobj, NPString *script,
                  NPVariant *result);
bool NPN_GetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName,
                     NPVariant *result);
bool NPN_SetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName,
                     const NPVariant *value);
bool NPN_RemoveProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName);
bool NPN_HasProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName);
bool NPN_HasMethod(NPP npp, NPObject *npobj, NPIdentifier methodName);
bool NPN_Enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier,
                   uint32_t *count);

void NPN_SetException(NPObject *npobj, const NPUTF8 *message);

#ifdef __cplusplus
}
#endif

#endif
