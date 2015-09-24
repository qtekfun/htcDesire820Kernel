// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_FILE_BROWSER_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_FILE_BROWSER_PRIVATE_API_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/platform_file.h"
#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "googleurl/src/url_util.h"
#include "webkit/fileapi/file_system_callback_dispatcher.h"

class GURL;
class HtmlDialogView;

class RequestLocalFileSystemFunction : public AsyncExtensionFunction {
 protected:
  friend class LocalFileSystemCallbackDispatcher;
  
  virtual bool RunImpl() OVERRIDE;
  void RespondSuccessOnUIThread(const std::string& name,
                                const GURL& root_path);
  void RespondFailedOnUIThread(base::PlatformFileError error_code);
  void RequestOnFileThread(const GURL& source_url, int child_id);
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.requestLocalFileSystem");
};

class GetFileTasksFileBrowserFunction : public AsyncExtensionFunction {
 protected:
  
  virtual bool RunImpl() OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.getFileTasks");
};


class ExecuteTasksFileBrowserFunction : public AsyncExtensionFunction {
 protected:
  
  virtual bool RunImpl() OVERRIDE;

 private:
  struct FileDefinition {
    GURL target_file_url;
    FilePath virtual_path;
    bool is_directory;
  };
  typedef std::vector<FileDefinition> FileDefinitionList;
  friend class ExecuteTasksFileSystemCallbackDispatcher;
  
  
  bool InitiateFileTaskExecution(const std::string& task_id,
                                 ListValue* files_list);
  void RequestFileEntryOnFileThread(const GURL& source_url,
                                    const std::string& task_id,
                                    const std::vector<GURL>& file_urls);
  void RespondFailedOnUIThread(base::PlatformFileError error_code);
  void ExecuteFileActionsOnUIThread(const std::string& task_id,
                                    const std::string& file_system_name,
                                    const GURL& file_system_root,
                                    const FileDefinitionList& file_list);
  void ExecuteFailedOnUIThread();
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.executeTask");
};

class FileDialogFunction
    : public AsyncExtensionFunction {
 public:
  FileDialogFunction();

  
  
  
  class Callback {
   public:
    Callback(SelectFileDialog::Listener* listener,
             HtmlDialogView* dialog,
             void* params)
        : listener_(listener),
          dialog_(dialog),
          params_(params) {
    }
    SelectFileDialog::Listener* listener() const { return listener_; }
    HtmlDialogView* dialog() const { return dialog_; }
    void* params() const { return params_; }
    bool IsNull() const { return listener_ == NULL; }

    static void Add(int32 tab_id,
                    SelectFileDialog::Listener* listener,
                    HtmlDialogView* dialog,
                    void* params);
    static void Remove(int32 tab_id);
    static const Callback& Find(int32 tab_id);
    static const Callback& null() { return null_; }

   private:

    SelectFileDialog::Listener* listener_;
    HtmlDialogView* dialog_;
    void* params_;

    
    typedef std::map<int32, Callback> Map;
    static Map map_;
    static Callback null_;
  };

 protected:
  typedef std::vector<GURL> UrlList;
  typedef std::vector<FilePath> FilePathList;

  virtual ~FileDialogFunction();

  
  void GetLocalPathsOnFileThread(const UrlList& file_urls,
                                 const std::string& internal_task_id);

  
  virtual void GetLocalPathsResponseOnUIThread(const FilePathList& files,
      const std::string& internal_task_id) {}

  
  const Callback& GetCallback() const;

  
  void CloseDialog(HtmlDialogView* dialog);

 private:
  
  int32 GetTabId() const;
};

class SelectFileFunction
    : public FileDialogFunction {
 public:
  SelectFileFunction() {}

 protected:
  virtual ~SelectFileFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  
  virtual void GetLocalPathsResponseOnUIThread(
      const FilePathList& files, const std::string& internal_task_id) OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.selectFile");
};

class ViewFilesFunction
    : public FileDialogFunction {
 public:
  ViewFilesFunction();

 protected:
  virtual ~ViewFilesFunction();

  
  virtual bool RunImpl() OVERRIDE;

  
  virtual void GetLocalPathsResponseOnUIThread(
      const FilePathList& files, const std::string& internal_task_id) OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.viewFiles");
};

class SelectFilesFunction
    : public FileDialogFunction {
 public:
  SelectFilesFunction();

 protected:
  virtual ~SelectFilesFunction();

  
  virtual bool RunImpl() OVERRIDE;

  
  virtual void GetLocalPathsResponseOnUIThread(
      const FilePathList& files, const std::string& internal_task_id) OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.selectFiles");
};

class CancelFileDialogFunction
    : public FileDialogFunction {
 public:
  CancelFileDialogFunction() {}

 protected:
  virtual ~CancelFileDialogFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.cancelDialog");
};

class FileDialogStringsFunction : public SyncExtensionFunction {
 public:
  FileDialogStringsFunction() {}

 protected:
  virtual ~FileDialogStringsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION_NAME("fileBrowserPrivate.getStrings");
};

#endif  
