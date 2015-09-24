// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/shared_memory.h"
#include "base/values.h"
#include "chrome/common/extensions/extension.h"
#include "chrome/common/extensions/extension_extent.h"
#include "chrome/common/extensions/url_pattern.h"
#include "chrome/common/web_apps.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START ExtensionMsgStart

IPC_STRUCT_BEGIN(ExtensionHostMsg_DomMessage_Params)
  
  IPC_STRUCT_MEMBER(std::string, name)

  
  IPC_STRUCT_MEMBER(ListValue, arguments)

  
  IPC_STRUCT_MEMBER(GURL, source_url)

  
  IPC_STRUCT_MEMBER(int, request_id)

  
  IPC_STRUCT_MEMBER(bool, has_callback)

  
  IPC_STRUCT_MEMBER(bool, user_gesture)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ExtensionMsg_ExecuteCode_Params)
  
  IPC_STRUCT_MEMBER(int, request_id)

  
  
  IPC_STRUCT_MEMBER(std::string, extension_id)

  
  IPC_STRUCT_MEMBER(bool, is_javascript)

  
  IPC_STRUCT_MEMBER(std::string, code)

  
  IPC_STRUCT_MEMBER(bool, all_frames)

  
  
  IPC_STRUCT_MEMBER(bool, in_main_world)
IPC_STRUCT_END()

IPC_STRUCT_TRAITS_BEGIN(WebApplicationInfo::IconInfo)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(width)
  IPC_STRUCT_TRAITS_MEMBER(height)
  IPC_STRUCT_TRAITS_MEMBER(data)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(WebApplicationInfo)
  IPC_STRUCT_TRAITS_MEMBER(title)
  IPC_STRUCT_TRAITS_MEMBER(description)
  IPC_STRUCT_TRAITS_MEMBER(app_url)
  IPC_STRUCT_TRAITS_MEMBER(icons)
  IPC_STRUCT_TRAITS_MEMBER(permissions)
  IPC_STRUCT_TRAITS_MEMBER(launch_container)
IPC_STRUCT_TRAITS_END()

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_MESSAGES_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_MESSAGES_H_

typedef std::map<std::string, std::string> SubstitutionMap;

struct ExtensionMsg_Loaded_Params {
  ExtensionMsg_Loaded_Params();
  ~ExtensionMsg_Loaded_Params();
  explicit ExtensionMsg_Loaded_Params(const Extension* extension);

  
  
  ExtensionMsg_Loaded_Params(const ExtensionMsg_Loaded_Params& other);

  
  scoped_refptr<Extension> ConvertToExtension() const;

  
  scoped_ptr<DictionaryValue> manifest;

  
  Extension::Location location;

  
  
  FilePath path;

  
  std::string id;
};

namespace IPC {

template <>
struct ParamTraits<URLPattern> {
  typedef URLPattern param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* p);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<ExtensionExtent> {
  typedef ExtensionExtent param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* p);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<ExtensionMsg_Loaded_Params> {
  typedef ExtensionMsg_Loaded_Params param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* p);
  static void Log(const param_type& p, std::string* l);
};

}  

#endif  


IPC_MESSAGE_ROUTED4(ExtensionMsg_Response,
                    int ,
                    bool ,
                    std::string ,
                    std::string )

IPC_MESSAGE_ROUTED4(ExtensionMsg_MessageInvoke,
                    std::string ,
                    std::string ,
                    ListValue ,
                    GURL )

IPC_MESSAGE_CONTROL1(ExtensionMsg_SetFunctionNames,
                     std::vector<std::string>)


IPC_MESSAGE_CONTROL2(ExtensionMsg_SetAPIPermissions,
                     std::string ,
                     std::set<std::string> )

IPC_MESSAGE_CONTROL2(ExtensionMsg_SetHostPermissions,
                     GURL ,
                     
                     std::vector<URLPattern>)

IPC_MESSAGE_CONTROL2(ExtensionMsg_UpdatePageActions,
                     std::string ,
                     std::vector<std::string> )

IPC_MESSAGE_CONTROL1(ExtensionMsg_Loaded,
                     ExtensionMsg_Loaded_Params)

IPC_MESSAGE_CONTROL1(ExtensionMsg_Unloaded,
                     std::string)

IPC_MESSAGE_CONTROL1(ExtensionMsg_SetScriptingWhitelist,
                     Extension::ScriptingWhitelist )

IPC_MESSAGE_ROUTED1(ExtensionMsg_ExecuteCode,
                    ExtensionMsg_ExecuteCode_Params)

IPC_MESSAGE_CONTROL1(ExtensionMsg_UpdateUserScripts,
                     base::SharedMemoryHandle)

IPC_MESSAGE_ROUTED1(ExtensionMsg_GetApplicationInfo,
                    int32 )


IPC_MESSAGE_ROUTED1(ExtensionHostMsg_Request,
                    ExtensionHostMsg_DomMessage_Params)

IPC_MESSAGE_CONTROL2(ExtensionHostMsg_AddListener,
                     std::string ,
                     std::string )

IPC_MESSAGE_CONTROL2(ExtensionHostMsg_RemoveListener,
                     std::string ,
                     std::string )

IPC_SYNC_MESSAGE_CONTROL4_1(ExtensionHostMsg_OpenChannelToExtension,
                            int ,
                            std::string ,
                            std::string ,
                            std::string ,
                            int )

IPC_SYNC_MESSAGE_CONTROL4_1(ExtensionHostMsg_OpenChannelToTab,
                            int ,
                            int ,
                            std::string ,
                            std::string ,
                            int )

IPC_MESSAGE_ROUTED2(ExtensionHostMsg_PostMessage,
                    int ,
                    std::string )

IPC_MESSAGE_CONTROL1(ExtensionHostMsg_CloseChannel,
                     int )

IPC_SYNC_MESSAGE_CONTROL1_1(ExtensionHostMsg_GetMessageBundle,
                            std::string ,
                            SubstitutionMap )

IPC_MESSAGE_ROUTED3(ExtensionHostMsg_ExecuteCodeFinished,
                    int ,
                    bool ,
                    std::string )

IPC_MESSAGE_ROUTED2(ExtensionHostMsg_DidGetApplicationInfo,
                    int32 ,
                    WebApplicationInfo)

IPC_MESSAGE_ROUTED1(ExtensionHostMsg_InstallApplication,
                    WebApplicationInfo)
