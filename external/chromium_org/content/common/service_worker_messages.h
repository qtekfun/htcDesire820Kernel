// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/strings/string16.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "third_party/WebKit/public/platform/WebServiceWorkerError.h"
#include "url/gurl.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START ServiceWorkerMsgStart

IPC_ENUM_TRAITS(blink::WebServiceWorkerError::ErrorType)


IPC_MESSAGE_CONTROL4(ServiceWorkerHostMsg_RegisterServiceWorker,
                     int32 ,
                     int32 ,
                     GURL ,
                     GURL )

IPC_MESSAGE_CONTROL3(ServiceWorkerHostMsg_UnregisterServiceWorker,
                     int32 ,
                     int32 ,
                     GURL )


IPC_MESSAGE_CONTROL3(ServiceWorkerMsg_ServiceWorkerRegistered,
                     int32 ,
                     int32 ,
                     int64 )

IPC_MESSAGE_CONTROL2(ServiceWorkerMsg_ServiceWorkerUnregistered,
                     int32 ,
                     int32 )

IPC_MESSAGE_CONTROL4(ServiceWorkerMsg_ServiceWorkerRegistrationError,
                     int32 ,
                     int32 ,
                     blink::WebServiceWorkerError::ErrorType ,
                     base::string16 )

IPC_MESSAGE_CONTROL1(ServiceWorkerHostMsg_ProviderCreated,
                     int )

IPC_MESSAGE_CONTROL1(ServiceWorkerHostMsg_ProviderDestroyed,
                     int )


IPC_MESSAGE_CONTROL3(ServiceWorkerMsg_StartWorker,
                     int ,
                     int64 ,
                     GURL )

IPC_MESSAGE_CONTROL1(ServiceWorkerMsg_TerminateWorker,
                     int )
