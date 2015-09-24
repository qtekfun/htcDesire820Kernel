// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/platform_file.h"
#include "base/values.h"
#include "chrome/common/extensions/update_manifest.h"
#include "content/common/common_param_traits.h"
#include "content/common/indexed_db_key.h"
#include "content/common/indexed_db_param_traits.h"
#include "content/common/serialized_script_value.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"
#include "printing/backend/print_backend.h"
#include "printing/page_range.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/rect.h"

#define IPC_MESSAGE_START UtilityMsgStart

IPC_STRUCT_TRAITS_BEGIN(printing::PageRange)
  IPC_STRUCT_TRAITS_MEMBER(from)
  IPC_STRUCT_TRAITS_MEMBER(to)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(printing::PrinterCapsAndDefaults)
  IPC_STRUCT_TRAITS_MEMBER(printer_capabilities)
  IPC_STRUCT_TRAITS_MEMBER(caps_mime_type)
  IPC_STRUCT_TRAITS_MEMBER(printer_defaults)
  IPC_STRUCT_TRAITS_MEMBER(defaults_mime_type)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(UpdateManifest::Result)
  IPC_STRUCT_TRAITS_MEMBER(extension_id)
  IPC_STRUCT_TRAITS_MEMBER(version)
  IPC_STRUCT_TRAITS_MEMBER(browser_min_version)
  IPC_STRUCT_TRAITS_MEMBER(package_hash)
  IPC_STRUCT_TRAITS_MEMBER(crx_url)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(UpdateManifest::Results)
  IPC_STRUCT_TRAITS_MEMBER(list)
  IPC_STRUCT_TRAITS_MEMBER(daystart_elapsed_seconds)
IPC_STRUCT_TRAITS_END()

IPC_MESSAGE_CONTROL1(UtilityMsg_UnpackExtension,
                     FilePath )

IPC_MESSAGE_CONTROL1(UtilityMsg_UnpackWebResource,
                     std::string )

IPC_MESSAGE_CONTROL1(UtilityMsg_ParseUpdateManifest,
                     std::string )

IPC_MESSAGE_CONTROL1(UtilityMsg_DecodeImage,
                     std::vector<unsigned char>)  

IPC_MESSAGE_CONTROL1(UtilityMsg_DecodeImageBase64,
                     std::string)  

IPC_MESSAGE_CONTROL5(UtilityMsg_RenderPDFPagesToMetafile,
                     base::PlatformFile,       
                     FilePath,                 
                     gfx::Rect,                
                     int,                      
                     std::vector<printing::PageRange>)

IPC_MESSAGE_CONTROL3(UtilityMsg_IDBKeysFromValuesAndKeyPath,
                     int,     
                     std::vector<SerializedScriptValue>,
                     string16)  

IPC_MESSAGE_CONTROL3(UtilityMsg_InjectIDBKey,
                     IndexedDBKey ,
                     SerializedScriptValue ,
                     string16 )

IPC_MESSAGE_CONTROL1(UtilityMsg_ParseJSON,
                     std::string )

IPC_MESSAGE_CONTROL0(UtilityMsg_BatchMode_Started)

IPC_MESSAGE_CONTROL0(UtilityMsg_BatchMode_Finished)

IPC_MESSAGE_CONTROL1(UtilityMsg_GetPrinterCapsAndDefaults,
                     std::string )

// The unpacker should also have written out files containing the decoded
IPC_MESSAGE_CONTROL1(UtilityHostMsg_UnpackExtension_Succeeded,
                     DictionaryValue )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_UnpackExtension_Failed,
                     std::string )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_UnpackWebResource_Succeeded,
                     DictionaryValue )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_UnpackWebResource_Failed,
                     std::string )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_ParseUpdateManifest_Succeeded,
                     UpdateManifest::Results )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_ParseUpdateManifest_Failed,
                     std::string )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_DecodeImage_Succeeded,
                     SkBitmap)  

IPC_MESSAGE_CONTROL0(UtilityHostMsg_DecodeImage_Failed)

IPC_MESSAGE_CONTROL1(UtilityHostMsg_RenderPDFPagesToMetafile_Succeeded,
                     int)       

IPC_MESSAGE_CONTROL0(UtilityHostMsg_RenderPDFPagesToMetafile_Failed)

#if defined(OS_WIN)
IPC_SYNC_MESSAGE_CONTROL1_0(UtilityHostMsg_PreCacheFont,
                            LOGFONT )
#endif  

IPC_MESSAGE_CONTROL2(UtilityHostMsg_IDBKeysFromValuesAndKeyPath_Succeeded,
                     int ,
                     std::vector<IndexedDBKey> )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_IDBKeysFromValuesAndKeyPath_Failed,
                     int )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_InjectIDBKey_Finished,
                     SerializedScriptValue )

IPC_MESSAGE_CONTROL1(UtilityHostMsg_ParseJSON_Succeeded,
                     ListValue)

IPC_MESSAGE_CONTROL1(UtilityHostMsg_ParseJSON_Failed,
                     std::string )

IPC_MESSAGE_CONTROL2(UtilityHostMsg_GetPrinterCapsAndDefaults_Succeeded,
                     std::string ,
                     printing::PrinterCapsAndDefaults)

IPC_MESSAGE_CONTROL1(UtilityHostMsg_GetPrinterCapsAndDefaults_Failed,
                     std::string )
