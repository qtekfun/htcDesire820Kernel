// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_IMPORT_DATA_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_IMPORT_DATA_HANDLER_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/importer/importer_data_types.h"
#include "chrome/browser/importer/importer_list.h"
#include "chrome/browser/importer/importer_progress_observer.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class ImporterHost;

class ImportDataHandler : public OptionsPageUIHandler,
                          public ImporterList::Observer,
                          public importer::ImporterProgressObserver {
 public:
  ImportDataHandler();
  virtual ~ImportDataHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Initialize();

  
  virtual void RegisterMessages();

 private:
  void ImportData(const ListValue* args);

  
  virtual void OnSourceProfilesLoaded() OVERRIDE;

  
  virtual void ImportStarted() OVERRIDE;
  virtual void ImportItemStarted(importer::ImportItem item) OVERRIDE;
  virtual void ImportItemEnded(importer::ImportItem item) OVERRIDE;
  virtual void ImportEnded() OVERRIDE;

  scoped_refptr<ImporterList> importer_list_;

  
  
  ImporterHost* importer_host_;  

  DISALLOW_COPY_AND_ASSIGN(ImportDataHandler);
};

#endif  
