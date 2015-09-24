// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/platform_file.h"
#include "base/strings/string16.h"
#include "base/tuple.h"
#include "base/values.h"
#include "chrome/common/extensions/update_manifest.h"
#include "chrome/common/media_galleries/iphoto_library.h"
#include "chrome/common/media_galleries/itunes_library.h"
#include "chrome/common/media_galleries/picasa_types.h"
#include "chrome/common/safe_browsing/zip_analyzer.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "printing/backend/print_backend.h"
#include "printing/page_range.h"
#include "printing/pdf_render_settings.h"
#include "third_party/skia/include/core/SkBitmap.h"

#define IPC_MESSAGE_START ChromeUtilityMsgStart

#ifndef CHROME_COMMON_CHROME_UTILITY_MESSAGES_H_
#define CHROME_COMMON_CHROME_UTILITY_MESSAGES_H_

typedef std::vector<Tuple2<SkBitmap, base::FilePath> > DecodedImages;

#endif  

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

IPC_STRUCT_TRAITS_BEGIN(safe_browsing::zip_analyzer::Results)
  IPC_STRUCT_TRAITS_MEMBER(success)
  IPC_STRUCT_TRAITS_MEMBER(has_executable)
  IPC_STRUCT_TRAITS_MEMBER(has_archive)
IPC_STRUCT_TRAITS_END()

#if defined(OS_MACOSX)
IPC_STRUCT_TRAITS_BEGIN(iphoto::parser::Photo)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(location)
  IPC_STRUCT_TRAITS_MEMBER(original_location)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(iphoto::parser::Library)
  IPC_STRUCT_TRAITS_MEMBER(albums)
  IPC_STRUCT_TRAITS_MEMBER(all_photos)
IPC_STRUCT_TRAITS_END()
#endif  

#if defined(OS_WIN) || defined(OS_MACOSX)
IPC_STRUCT_TRAITS_BEGIN(itunes::parser::Track)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(location)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(picasa::AlbumInfo)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(timestamp)
  IPC_STRUCT_TRAITS_MEMBER(uid)
  IPC_STRUCT_TRAITS_MEMBER(path)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(picasa::AlbumTableFilesForTransit)
  IPC_STRUCT_TRAITS_MEMBER(indicator_file)
  IPC_STRUCT_TRAITS_MEMBER(category_file)
  IPC_STRUCT_TRAITS_MEMBER(date_file)
  IPC_STRUCT_TRAITS_MEMBER(filename_file)
  IPC_STRUCT_TRAITS_MEMBER(name_file)
  IPC_STRUCT_TRAITS_MEMBER(token_file)
  IPC_STRUCT_TRAITS_MEMBER(uid_file)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(picasa::FolderINIContents)
  IPC_STRUCT_TRAITS_MEMBER(folder_path)
  IPC_STRUCT_TRAITS_MEMBER(ini_contents)
IPC_STRUCT_TRAITS_END()
#endif  


IPC_MESSAGE_CONTROL4(ChromeUtilityMsg_UnpackExtension,
                     base::FilePath ,
                     std::string ,
                     int ,
                     int )

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_UnpackWebResource,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_ParseUpdateManifest,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_DecodeImage,
                     std::vector<unsigned char>)  

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_DecodeImageBase64,
                     std::string)  

IPC_MESSAGE_CONTROL4(ChromeUtilityMsg_RenderPDFPagesToMetafile,
                     base::PlatformFile,       
                     base::FilePath,           
                     printing::PdfRenderSettings,  
                     std::vector<printing::PageRange>)

IPC_MESSAGE_CONTROL3(ChromeUtilityMsg_RenderPDFPagesToPWGRaster,
                     IPC::PlatformFileForTransit,  
                     printing::PdfRenderSettings,  
                     IPC::PlatformFileForTransit   )

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_RobustJPEGDecodeImage,
                     std::vector<unsigned char>)  

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_ParseJSON,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_GetPrinterCapsAndDefaults,
                     std::string )

#if defined(OS_CHROMEOS)
IPC_MESSAGE_CONTROL3(ChromeUtilityMsg_CreateZipFile,
                     base::FilePath ,
                     std::vector<base::FilePath> ,
                     base::FileDescriptor )
#endif  

IPC_MESSAGE_CONTROL0(ChromeUtilityMsg_StartupPing)

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_AnalyzeZipFileForDownloadProtection,
                     IPC::PlatformFileForTransit )

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_ParseITunesPrefXml,
                     std::string )
#endif  

#if defined(OS_MACOSX)
IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_ParseIPhotoLibraryXmlFile,
                     IPC::PlatformFileForTransit )
#endif  

#if defined(OS_WIN) || defined(OS_MACOSX)
IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_ParseITunesLibraryXmlFile,
                     IPC::PlatformFileForTransit )

IPC_MESSAGE_CONTROL1(ChromeUtilityMsg_ParsePicasaPMPDatabase,
                     picasa::AlbumTableFilesForTransit )

IPC_MESSAGE_CONTROL2(ChromeUtilityMsg_IndexPicasaAlbumsContents,
                     picasa::AlbumUIDSet ,
                     std::vector<picasa::FolderINIContents> )
#endif  

#if !defined(OS_ANDROID) && !defined(OS_IOS)
IPC_MESSAGE_CONTROL2(ChromeUtilityMsg_CheckMediaFile,
                     int64 ,
                     IPC::PlatformFileForTransit )
#endif  


// The unpacker should also have written out files containing the decoded
// images and message catalogs from the extension. The data is written into a
IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_UnpackExtension_Succeeded,
                     base::DictionaryValue )

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_UnpackExtension_Failed,
                     base::string16 )

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_UnpackWebResource_Succeeded,
                     base::DictionaryValue )

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_UnpackWebResource_Failed,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_ParseUpdateManifest_Succeeded,
                     UpdateManifest::Results )

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_ParseUpdateManifest_Failed,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_DecodeImage_Succeeded,
                     SkBitmap)  

IPC_MESSAGE_CONTROL0(ChromeUtilityHostMsg_DecodeImage_Failed)

IPC_MESSAGE_CONTROL2(ChromeUtilityHostMsg_RenderPDFPagesToMetafile_Succeeded,
                     int,          
                     double)       

IPC_MESSAGE_CONTROL0(ChromeUtilityHostMsg_RenderPDFPagesToMetafile_Failed)

IPC_MESSAGE_CONTROL0(ChromeUtilityHostMsg_RenderPDFPagesToPWGRaster_Succeeded)

IPC_MESSAGE_CONTROL0(ChromeUtilityHostMsg_RenderPDFPagesToPWGRaster_Failed)

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_ParseJSON_Succeeded,
                     base::ListValue)

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_ParseJSON_Failed,
                     std::string )

#if defined(ENABLE_FULL_PRINTING)
IPC_MESSAGE_CONTROL2(ChromeUtilityHostMsg_GetPrinterCapsAndDefaults_Succeeded,
                     std::string ,
                     printing::PrinterCapsAndDefaults)
#endif

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_GetPrinterCapsAndDefaults_Failed,
                     std::string )

#if defined(OS_CHROMEOS)
IPC_MESSAGE_CONTROL0(ChromeUtilityHostMsg_CreateZipFile_Succeeded)

IPC_MESSAGE_CONTROL0(ChromeUtilityHostMsg_CreateZipFile_Failed)
#endif  

IPC_MESSAGE_CONTROL0(ChromeUtilityHostMsg_ProcessStarted)

IPC_MESSAGE_CONTROL1(
    ChromeUtilityHostMsg_AnalyzeZipFileForDownloadProtection_Finished,
    safe_browsing::zip_analyzer::Results)

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_GotITunesDirectory,
                     base::FilePath )
#endif  

#if defined(OS_MACOSX)
IPC_MESSAGE_CONTROL2(ChromeUtilityHostMsg_GotIPhotoLibrary,
                     bool ,
                     iphoto::parser::Library )
#endif  

#if defined(OS_WIN) || defined(OS_MACOSX)
IPC_MESSAGE_CONTROL2(ChromeUtilityHostMsg_GotITunesLibrary,
                     bool ,
                     itunes::parser::Library )

IPC_MESSAGE_CONTROL3(ChromeUtilityHostMsg_ParsePicasaPMPDatabase_Finished,
                     bool ,
                     std::vector<picasa::AlbumInfo> ,
                     std::vector<picasa::AlbumInfo> )

IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_IndexPicasaAlbumsContents_Finished,
                     picasa::AlbumImagesMap )
#endif  

#if !defined(OS_ANDROID) && !defined(OS_IOS)
IPC_MESSAGE_CONTROL1(ChromeUtilityHostMsg_CheckMediaFile_Finished,
                     bool )
#endif  
