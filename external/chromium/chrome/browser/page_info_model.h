// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PAGE_INFO_MODEL_H_
#define CHROME_BROWSER_PAGE_INFO_MODEL_H_
#pragma once

#include <vector>

#include "base/string16.h"
#include "chrome/browser/history/history.h"
#include "content/browser/cancelable_request.h"
#include "content/browser/tab_contents/navigation_entry.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/image.h"

class PrefService;
class Profile;

class PageInfoModel {
 public:
  class PageInfoModelObserver {
   public:
    virtual ~PageInfoModelObserver() {}

    virtual void ModelChanged() = 0;
  };

  enum SectionInfoType {
    SECTION_INFO_IDENTITY = 0,
    SECTION_INFO_CONNECTION,
    SECTION_INFO_FIRST_VISIT,
  };

  
  
  enum SectionStateIcon {
    
    ICON_NONE = -1,
    
    ICON_STATE_OK,
    
    ICON_STATE_WARNING_MINOR,
    
    ICON_STATE_WARNING_MAJOR,
    
    ICON_STATE_ERROR,
    
    ICON_STATE_INFO
  };

  struct SectionInfo {
    SectionInfo(SectionStateIcon icon_id,
                const string16& headline,
                const string16& description,
                SectionInfoType type)
        : icon_id(icon_id),
          headline(headline),
          description(description),
          type(type) {
    }

    
    SectionStateIcon icon_id;

    
    string16 headline;

    
    string16 description;

    
    
    SectionInfoType type;
  };

  PageInfoModel(Profile* profile,
                const GURL& url,
                const NavigationEntry::SSLStatus& ssl,
                bool show_history,
                PageInfoModelObserver* observer);
  ~PageInfoModel();

  int GetSectionCount();
  SectionInfo GetSectionInfo(int index);

  
  gfx::Image* GetIconImage(SectionStateIcon icon_id);

  
  void OnGotVisitCountToHost(HistoryService::Handle handle,
                             bool found_visits,
                             int count,
                             base::Time first_visit);

 protected:
  
  PageInfoModel();

  
  void Init();

  PageInfoModelObserver* observer_;

  std::vector<SectionInfo> sections_;

  
  std::vector<gfx::Image*> icons_;

  
  CancelableRequestConsumer request_consumer_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PageInfoModel);
};

#endif  
