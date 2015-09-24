// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FILE_SELECT_HELPER_H_
#define CHROME_BROWSER_FILE_SELECT_HELPER_H_
#pragma once

#include <map>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "net/base/directory_lister.h"

class Profile;
class RenderViewHost;
struct ViewHostMsg_RunFileChooser_Params;


class FileSelectHelper
    : public SelectFileDialog::Listener,
      public NotificationObserver {
 public:
  explicit FileSelectHelper(Profile* profile);
  ~FileSelectHelper();

  
  void RunFileChooser(RenderViewHost* render_view_host,
                      TabContents* tab_contents,
                      const ViewHostMsg_RunFileChooser_Params& params);

  
  void EnumerateDirectory(int request_id,
                          RenderViewHost* render_view_host,
                          const FilePath& path);

 private:
  
  
  class DirectoryListerDispatchDelegate
      : public net::DirectoryLister::DirectoryListerDelegate {
   public:
    DirectoryListerDispatchDelegate(FileSelectHelper* parent, int id)
        : parent_(parent),
          id_(id) {}
    ~DirectoryListerDispatchDelegate() {}
    virtual void OnListFile(
        const net::DirectoryLister::DirectoryListerData& data) {
      parent_->OnListFile(id_, data);
    }
    virtual void OnListDone(int error) {
      parent_->OnListDone(id_, error);
    }
   private:
    
    FileSelectHelper* parent_;
    int id_;

    DISALLOW_COPY_AND_ASSIGN(DirectoryListerDispatchDelegate);
  };

  
  virtual void FileSelected(
      const FilePath& path, int index, void* params) OVERRIDE;
  virtual void MultiFilesSelected(const std::vector<FilePath>& files,
                                  void* params) OVERRIDE;
  virtual void FileSelectionCanceled(void* params) OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  void StartNewEnumeration(const FilePath& path,
                           int request_id,
                           RenderViewHost* render_view_host);

  
  virtual void OnListFile(
      int id,
      const net::DirectoryLister::DirectoryListerData& data);
  virtual void OnListDone(int id, int error);

  
  
  
  SelectFileDialog::FileTypeInfo* GetFileTypesFromAcceptType(
      const string16& accept_types);

  
  Profile* profile_;

  
  
  RenderViewHost* render_view_host_;

  
  scoped_refptr<SelectFileDialog> select_file_dialog_;

  
  SelectFileDialog::Type dialog_type_;

  
  
  
  struct ActiveDirectoryEnumeration;
  std::map<int, ActiveDirectoryEnumeration*> directory_enumerations_;

  
  NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(FileSelectHelper);
};

class FileSelectObserver : public TabContentsObserver {
 public:
  explicit FileSelectObserver(TabContents* tab_contents);
  ~FileSelectObserver();

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void OnRunFileChooser(const ViewHostMsg_RunFileChooser_Params& params);

  
  void OnEnumerateDirectory(int request_id, const FilePath& path);

  
  scoped_ptr<FileSelectHelper> file_select_helper_;

  DISALLOW_COPY_AND_ASSIGN(FileSelectObserver);
};

#endif  
