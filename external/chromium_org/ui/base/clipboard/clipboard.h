// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_CLIPBOARD_CLIPBOARD_H_
#define UI_BASE_CLIPBOARD_CLIPBOARD_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "base/strings/string16.h"
#include "base/threading/platform_thread.h"
#include "base/threading/thread_checker.h"
#include "ui/base/clipboard/clipboard_types.h"
#include "ui/base/ui_export.h"

#if defined(TOOLKIT_GTK)
#include <gdk/gdk.h>
#endif

#if defined(OS_WIN)
#include <objidl.h>
#elif defined(OS_ANDROID)
#include <jni.h>

#include "base/android/jni_android.h"
#include "base/android/scoped_java_ref.h"
#endif

#if defined(USE_AURA) && defined(USE_X11)
#include "base/memory/scoped_ptr.h"
#endif

namespace base {
class FilePath;

namespace win {
class MessageWindow;
}  
}  

namespace gfx {
class Size;
}

class SkBitmap;

#if defined(TOOLKIT_GTK)
typedef struct _GtkClipboard GtkClipboard;
#endif

#ifdef __OBJC__
@class NSString;
#else
class NSString;
#endif

namespace ui {
class ClipboardTest;

class UI_EXPORT Clipboard : NON_EXPORTED_BASE(public base::ThreadChecker) {
 public:
  
  static const char kMimeTypeText[];
  static const char kMimeTypeURIList[];
  static const char kMimeTypeDownloadURL[];
  static const char kMimeTypeHTML[];
  static const char kMimeTypeRTF[];
  static const char kMimeTypePNG[];

  
  struct UI_EXPORT FormatType {
    FormatType();
    ~FormatType();

    
    std::string Serialize() const;
    static FormatType Deserialize(const std::string& serialization);

#if defined(OS_WIN) || defined(USE_AURA)
    
    bool operator<(const FormatType& other) const;
#endif

#if defined(OS_WIN)
    const FORMATETC& ToFormatEtc() const { return data_; }
#elif defined(OS_MACOSX)
    
    FormatType(const FormatType& other);
    FormatType& operator=(const FormatType& other);
#elif defined(USE_AURA)
    const std::string& ToString() const { return data_; }
#endif

   private:
    friend class Clipboard;

    bool Equals(const FormatType& other) const;

    
    
    
    
    
    
    
    
#if defined(OS_WIN)
    explicit FormatType(UINT native_format);
    FormatType(UINT native_format, LONG index);
    UINT ToUINT() const { return data_.cfFormat; }
    FORMATETC data_;
#elif defined(OS_MACOSX)
    explicit FormatType(NSString* native_format);
    NSString* ToNSString() const { return data_; }
    NSString* data_;
#elif defined(USE_AURA)
    explicit FormatType(const std::string& native_format);
    std::string data_;
#elif defined(TOOLKIT_GTK)
    explicit FormatType(const std::string& native_format);
    explicit FormatType(const GdkAtom& native_format);
    const GdkAtom& ToGdkAtom() const { return data_; }
    GdkAtom data_;
#elif defined(OS_ANDROID)
    explicit FormatType(const std::string& native_format);
    const std::string& data() const { return data_; }
    std::string data_;
#else
#error No FormatType definition.
#endif

    
  };

  
  
  
  
  
  
  enum ObjectType {
    CBF_TEXT,
    CBF_HTML,
    CBF_RTF,
    CBF_BOOKMARK,
    CBF_WEBKIT,
    CBF_SMBITMAP,  
    CBF_DATA,  
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef std::vector<char> ObjectMapParam;
  typedef std::vector<ObjectMapParam> ObjectMapParams;
  typedef std::map<int , ObjectMapParams> ObjectMap;

  static bool IsSupportedClipboardType(int32 type) {
    switch (type) {
      case CLIPBOARD_TYPE_COPY_PASTE:
        return true;
#if defined(USE_X11) && !defined(OS_CHROMEOS)
      case CLIPBOARD_TYPE_SELECTION:
        return true;
#endif
    }
    return false;
  }

  static ClipboardType FromInt(int32 type) {
    return static_cast<ClipboardType>(type);
  }

  
  static void SetAllowedThreads(
      const std::vector<base::PlatformThreadId>& allowed_threads);

  
  
  
  
  
  
  static Clipboard* GetForCurrentThread();

  
  
  
  static void DestroyClipboardForCurrentThread();

  
  
  
  
  void WriteObjects(ClipboardType type, const ObjectMap& objects);

  
  
  
  uint64 GetSequenceNumber(ClipboardType type);

  
  bool IsFormatAvailable(const FormatType& format, ClipboardType type) const;

  
  void Clear(ClipboardType type);

  void ReadAvailableTypes(ClipboardType type,
                          std::vector<base::string16>* types,
                          bool* contains_filenames) const;

  
  void ReadText(ClipboardType type, base::string16* result) const;

  
  void ReadAsciiText(ClipboardType type, std::string* result) const;

  
  
  
  
  void ReadHTML(ClipboardType type,
                base::string16* markup,
                std::string* src_url,
                uint32* fragment_start,
                uint32* fragment_end) const;

  
  
  void ReadRTF(ClipboardType type, std::string* result) const;

  
  SkBitmap ReadImage(ClipboardType type) const;

  void ReadCustomData(ClipboardType clipboard_type,
                      const base::string16& type,
                      base::string16* result) const;

  
  void ReadBookmark(base::string16* title, std::string* url) const;

  
  
  void ReadData(const FormatType& format, std::string* result) const;

  
  
  
  static FormatType GetFormatType(const std::string& format_string);

  
  static const FormatType& GetUrlFormatType();
  static const FormatType& GetUrlWFormatType();
  static const FormatType& GetMozUrlFormatType();
  static const FormatType& GetPlainTextFormatType();
  static const FormatType& GetPlainTextWFormatType();
  static const FormatType& GetFilenameFormatType();
  static const FormatType& GetFilenameWFormatType();
  static const FormatType& GetWebKitSmartPasteFormatType();
  
  static const FormatType& GetHtmlFormatType();
  static const FormatType& GetRtfFormatType();
  static const FormatType& GetBitmapFormatType();
  
  
  static const FormatType& GetWebCustomDataFormatType();
  static const FormatType& GetPepperCustomDataFormatType();

  
  
  
  
  
  static bool ReplaceSharedMemHandle(ObjectMap* objects,
                                     base::SharedMemoryHandle bitmap_handle,
                                     base::ProcessHandle process)
      WARN_UNUSED_RESULT;
#if defined(OS_WIN)
  
  static const FormatType& GetTextHtmlFormatType();
  static const FormatType& GetCFHDropFormatType();
  static const FormatType& GetFileDescriptorFormatType();
  static const FormatType& GetFileContentZeroFormatType();
  static const FormatType& GetIDListFormatType();
#endif

 private:
  FRIEND_TEST_ALL_PREFIXES(ClipboardTest, SharedBitmapTest);
  FRIEND_TEST_ALL_PREFIXES(ClipboardTest, EmptyHTMLTest);
  friend class ClipboardTest;

  Clipboard();
  ~Clipboard();

  void DispatchObject(ObjectType type, const ObjectMapParams& params);

  void WriteText(const char* text_data, size_t text_len);

  void WriteHTML(const char* markup_data,
                 size_t markup_len,
                 const char* url_data,
                 size_t url_len);

  void WriteRTF(const char* rtf_data, size_t data_len);

  void WriteBookmark(const char* title_data,
                     size_t title_len,
                     const char* url_data,
                     size_t url_len);

  void WriteWebSmartPaste();

  void WriteBitmap(const SkBitmap& bitmap);

  void WriteData(const FormatType& format,
                 const char* data_data,
                 size_t data_len);
#if defined(OS_WIN)
  void WriteBitmapFromHandle(HBITMAP source_hbitmap,
                             const gfx::Size& size);

  
  void WriteToClipboard(unsigned int format, HANDLE handle);

  static void ParseBookmarkClipboardFormat(const base::string16& bookmark,
                                           base::string16* title,
                                           std::string* url);

  
  static void FreeData(unsigned int format, HANDLE data);

  
  
  HWND GetClipboardWindow() const;

  
  mutable scoped_ptr<base::win::MessageWindow> clipboard_owner_;

#elif defined(TOOLKIT_GTK)
  
  
  
  
  
  
  

 public:
  typedef std::map<std::string, std::pair<char*, size_t> > TargetMap;

 private:
  
  void SetGtkClipboard(ClipboardType type);
  
  void InsertMapping(const char* key, char* data, size_t data_len);

  
  GtkClipboard* LookupBackingClipboard(ClipboardType type) const;

  TargetMap* clipboard_data_;
  GtkClipboard* clipboard_;
  GtkClipboard* primary_selection_;
#elif defined(USE_CLIPBOARD_AURAX11)
 private:
  
  
  class AuraX11Details;
  scoped_ptr<AuraX11Details> aurax11_details_;
#endif

  DISALLOW_COPY_AND_ASSIGN(Clipboard);
};

}  

#endif  
