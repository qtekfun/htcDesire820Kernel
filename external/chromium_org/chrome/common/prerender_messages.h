// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/common/common_param_traits.h"
#include "content/public/common/referrer.h"
#include "ipc/ipc_message.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

#define IPC_MESSAGE_START PrerenderMsgStart


IPC_MESSAGE_CONTROL5(PrerenderHostMsg_AddLinkRelPrerender,
                     int ,
                     GURL ,
                     content::Referrer,
                     gfx::Size,
                     int )

IPC_MESSAGE_CONTROL1(PrerenderHostMsg_CancelLinkRelPrerender,
                     int )

IPC_MESSAGE_CONTROL1(PrerenderHostMsg_AbandonLinkRelPrerender,
                     int )


IPC_MESSAGE_ROUTED1(PrerenderMsg_SetIsPrerendering,
                    bool )

IPC_MESSAGE_CONTROL1(PrerenderMsg_OnPrerenderStart,
                     int )

IPC_MESSAGE_CONTROL1(PrerenderMsg_OnPrerenderStopLoading,
                     int )

IPC_MESSAGE_CONTROL1(PrerenderMsg_OnPrerenderAddAlias,
                     GURL )

IPC_MESSAGE_CONTROL1(PrerenderMsg_OnPrerenderRemoveAliases,
                     std::vector<GURL> )

IPC_MESSAGE_CONTROL1(PrerenderMsg_OnPrerenderStop,
                     int )
