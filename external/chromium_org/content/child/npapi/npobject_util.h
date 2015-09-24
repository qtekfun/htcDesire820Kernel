// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_NPAPI_CHILD_NPOBJECT_UTIL_H_
#define CONTENT_NPAPI_CHILD_NPOBJECT_UTIL_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

#include "content/child/npapi/npobject_stub.h"

class GURL;

struct _NPVariant;

typedef _NPVariant NPVariant;
typedef void *NPIdentifier;

namespace content {
class NPChannelBase;
struct NPIdentifier_Param;
struct NPVariant_Param;

void PatchNPNFunctions();

bool IsPluginProcess();

void CreateNPIdentifierParam(NPIdentifier id, NPIdentifier_Param* param);

NPIdentifier CreateNPIdentifier(const  NPIdentifier_Param& param);

void CreateNPVariantParam(const NPVariant& variant,
                          NPChannelBase* channel,
                          NPVariant_Param* param,
                          bool release,
                          int render_view_id,
                          const GURL& page_url);

bool CreateNPVariant(const NPVariant_Param& param,
                     NPChannelBase* channel,
                     NPVariant* result,
                     int render_view_id,
                     const GURL& page_url);

#if defined(OS_WIN)
HANDLE GetMessageBoxEvent(HWND hwnd);
#endif  

}  

#endif  
