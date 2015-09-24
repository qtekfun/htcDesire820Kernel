// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "googleurl/src/gurl.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"

#define IPC_MESSAGE_START SafeBrowsingMsgStart

IPC_STRUCT_BEGIN(SafeBrowsingHostMsg_MalwareDOMDetails_Node)
  
  IPC_STRUCT_MEMBER(GURL, url)

  
  
  IPC_STRUCT_MEMBER(std::string, tag_name)

  
  IPC_STRUCT_MEMBER(GURL, parent)

  
  IPC_STRUCT_MEMBER(std::vector<GURL>, children)
IPC_STRUCT_END()


IPC_MESSAGE_ROUTED1(SafeBrowsingHostMsg_DetectedPhishingSite,
                    std::string )

IPC_MESSAGE_ROUTED1(SafeBrowsingHostMsg_MalwareDOMDetails,
                    std::vector<SafeBrowsingHostMsg_MalwareDOMDetails_Node>)


IPC_MESSAGE_CONTROL1(SafeBrowsingMsg_SetPhishingModel,
                     IPC::PlatformFileForTransit )

IPC_MESSAGE_ROUTED0(SafeBrowsingMsg_GetMalwareDOMDetails)

IPC_MESSAGE_ROUTED1(SafeBrowsingMsg_StartPhishingDetection,
                    GURL)
