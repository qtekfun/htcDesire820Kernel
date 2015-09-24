// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_WEBKIT_GLUE_H_
#define WEBKIT_GLUE_WEBKIT_GLUE_H_

#include "base/basictypes.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/platform_file.h"
#include "base/string16.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebCanvas.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebFileError.h"
#include "ui/base/clipboard/clipboard.h"

class GURL;
class SkBitmap;

namespace base {
class StringPiece;
}

namespace skia {
class PlatformCanvas;
}

namespace WebKit {
class WebFrame;
class WebString;
class WebView;
}

namespace webkit {
namespace npapi {
struct WebPluginInfo;
}
}

namespace webkit_glue {



void SetJavaScriptFlags(const std::string& flags);

void EnableWebCoreLogChannels(const std::string& channels);

string16 DumpDocumentText(WebKit::WebFrame* web_frame);

string16 DumpFramesAsText(WebKit::WebFrame* web_frame, bool recursive);

string16 DumpRenderer(WebKit::WebFrame* web_frame);

bool CounterValueForElementById(WebKit::WebFrame* web_frame,
                                const std::string& id,
                                string16* counter_value);

int PageNumberForElementById(WebKit::WebFrame* web_frame,
                             const std::string& id,
                             float page_width_in_pixels,
                             float page_height_in_pixels);

int NumberOfPages(WebKit::WebFrame* web_frame,
                  float page_width_in_pixels,
                  float page_height_in_pixels);

string16 DumpFrameScrollPosition(WebKit::WebFrame* web_frame, bool recursive);

string16 DumpHistoryState(const std::string& history_state, int indent,
                          bool is_current);

std::string GetWebKitVersion();

void SetUserAgent(const std::string& new_user_agent);

const std::string& GetUserAgent(const GURL& url);

std::string CreateHistoryStateForURL(const GURL& url);

std::string RemoveFormDataFromHistoryState(const std::string& content_state);

std::string RemoveScrollOffsetFromHistoryState(
    const std::string& content_state);

#ifndef NDEBUG
void CheckForLeaks();
#endif

bool DecodeImage(const std::string& image_data, SkBitmap* image);

void SetForcefullyTerminatePluginProcess(bool value);

bool ShouldForcefullyTerminatePluginProcess();

FilePath::StringType WebStringToFilePathString(const WebKit::WebString& str);
WebKit::WebString FilePathStringToWebString(const FilePath::StringType& str);
FilePath WebStringToFilePath(const WebKit::WebString& str);
WebKit::WebString FilePathToWebString(const FilePath& file_path);

WebKit::WebFileError PlatformFileErrorToWebFileError(
    base::PlatformFileError error_code);

WebKit::WebCanvas* ToWebCanvas(skia::PlatformCanvas*);

int GetGlyphPageCount();




void AppendToLog(const char* filename, int line, const char* message);


string16 GetLocalizedString(int message_id);

base::StringPiece GetDataResource(int resource_id);

#if defined(OS_WIN)
HCURSOR LoadCursor(int cursor_id);
#endif


ui::Clipboard* ClipboardGetClipboard();

bool ClipboardIsFormatAvailable(const ui::Clipboard::FormatType& format,
                                ui::Clipboard::Buffer buffer);

void ClipboardReadAvailableTypes(ui::Clipboard::Buffer buffer,
                                 std::vector<string16>* types,
                                 bool* contains_filenames);

void ClipboardReadText(ui::Clipboard::Buffer buffer, string16* result);

void ClipboardReadAsciiText(ui::Clipboard::Buffer buffer, std::string* result);

void ClipboardReadHTML(ui::Clipboard::Buffer buffer, string16* markup,
                       GURL* url);

void ClipboardReadImage(ui::Clipboard::Buffer buffer, std::string* data);

bool ClipboardReadData(ui::Clipboard::Buffer buffer, const string16& type,
                       string16* data, string16* metadata);

bool ClipboardReadFilenames(ui::Clipboard::Buffer buffer,
                            std::vector<string16>* filenames);

bool GetApplicationDirectory(FilePath* path);

bool GetExeDirectory(FilePath* path);

void GetPlugins(bool refresh,
                std::vector<webkit::npapi::WebPluginInfo>* plugins);

bool IsPluginRunningInRendererProcess();

bool IsDefaultPluginEnabled();

bool IsProtocolSupportedForMedia(const GURL& url);

#if defined(OS_WIN)
bool DownloadUrl(const std::string& url, HWND caller_window);
#endif

bool GetPluginFinderURL(std::string* plugin_finder_url);

bool FindProxyForUrl(const GURL& url, std::string* proxy_list);

std::string GetWebKitLocale();

void CloseCurrentConnections();

void SetCacheMode(bool enabled);

void ClearCache(bool preserve_ssl_host_info);

void ClearHostResolverCache();

void ClearPredictorCache();

std::string GetProductVersion();

bool IsSingleProcess();

void EnableSpdy(bool enable);

void UserMetricsRecordAction(const std::string& action);

#if !defined(DISABLE_NACL)
bool LaunchSelLdr(const char* alleged_url, int socket_count, void* imc_handles,
                  void* nacl_process_handle, int* nacl_process_id);
#endif

#if defined(OS_LINUX)
int MatchFontWithFallback(const std::string& face, bool bold,
                          bool italic, int charset);

bool GetFontTable(int fd, uint32_t table, uint8_t* output,
                  size_t* output_length);
#endif



} 

#endif  
