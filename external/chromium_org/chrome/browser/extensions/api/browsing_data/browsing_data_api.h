// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_BROWSING_DATA_BROWSING_DATA_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_BROWSING_DATA_BROWSING_DATA_API_H_

#include <string>

#include "chrome/browser/browsing_data/browsing_data_remover.h"
#include "chrome/browser/extensions/chrome_extension_function.h"

class PluginPrefs;

namespace extension_browsing_data_api_constants {

extern const char kDataRemovalPermittedKey[];
extern const char kDataToRemoveKey[];
extern const char kOptionsKey[];

extern const char kAppCacheKey[];
extern const char kCacheKey[];
extern const char kCookiesKey[];
extern const char kDownloadsKey[];
extern const char kFileSystemsKey[];
extern const char kFormDataKey[];
extern const char kHistoryKey[];
extern const char kIndexedDBKey[];
extern const char kPluginDataKey[];
extern const char kLocalStorageKey[];
extern const char kPasswordsKey[];
extern const char kWebSQLKey[];

extern const char kExtensionsKey[];
extern const char kOriginTypesKey[];
extern const char kProtectedWebKey[];
extern const char kSinceKey[];
extern const char kUnprotectedWebKey[];

extern const char kBadDataTypeDetails[];
extern const char kDeleteProhibitedError[];
extern const char kOneAtATimeError[];

}  

class BrowsingDataSettingsFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.settings", BROWSINGDATA_SETTINGS)

  
  virtual bool RunImpl() OVERRIDE;

 protected:
  virtual ~BrowsingDataSettingsFunction() {}

 private:
  
  
  
  
  void SetDetails(base::DictionaryValue* selected_dict,
                  base::DictionaryValue* permitted_dict,
                  const char* data_type,
                  bool is_selected);
};

class BrowsingDataRemoverFunction : public ChromeAsyncExtensionFunction,
                                    public BrowsingDataRemover::Observer {
 public:
  
  virtual void OnBrowsingDataRemoverDone() OVERRIDE;

  
  virtual bool RunImpl() OVERRIDE;

 protected:
  virtual ~BrowsingDataRemoverFunction() {}

  
  
  virtual int GetRemovalMask() = 0;

 private:
  
  
  void CheckRemovingPluginDataSupported(
      scoped_refptr<PluginPrefs> plugin_prefs);

  
  
  int ParseOriginSetMask(const base::DictionaryValue& options);

  
  void StartRemoving();

  base::Time remove_since_;
  int removal_mask_;
  int origin_set_mask_;
};

class BrowsingDataRemoveAppcacheFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeAppcache",
                             BROWSINGDATA_REMOVEAPPCACHE)

 protected:
  virtual ~BrowsingDataRemoveAppcacheFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.remove", BROWSINGDATA_REMOVE)

 protected:
  virtual ~BrowsingDataRemoveFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveCacheFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeCache",
                             BROWSINGDATA_REMOVECACHE)

 protected:
  virtual ~BrowsingDataRemoveCacheFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveCookiesFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeCookies",
                             BROWSINGDATA_REMOVECOOKIES)

 protected:
  virtual ~BrowsingDataRemoveCookiesFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveDownloadsFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeDownloads",
                             BROWSINGDATA_REMOVEDOWNLOADS)

 protected:
  virtual ~BrowsingDataRemoveDownloadsFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveFileSystemsFunction
    : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeFileSystems",
                             BROWSINGDATA_REMOVEFILESYSTEMS)

 protected:
  virtual ~BrowsingDataRemoveFileSystemsFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveFormDataFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeFormData",
                             BROWSINGDATA_REMOVEFORMDATA)

 protected:
  virtual ~BrowsingDataRemoveFormDataFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveHistoryFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeHistory",
                             BROWSINGDATA_REMOVEHISTORY)

 protected:
  virtual ~BrowsingDataRemoveHistoryFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveIndexedDBFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeIndexedDB",
                             BROWSINGDATA_REMOVEINDEXEDDB)

 protected:
  virtual ~BrowsingDataRemoveIndexedDBFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveLocalStorageFunction
    : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeLocalStorage",
                             BROWSINGDATA_REMOVELOCALSTORAGE)

 protected:
  virtual ~BrowsingDataRemoveLocalStorageFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemovePluginDataFunction
    : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removePluginData",
                             BROWSINGDATA_REMOVEPLUGINDATA)

 protected:
  virtual ~BrowsingDataRemovePluginDataFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemovePasswordsFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removePasswords",
                             BROWSINGDATA_REMOVEPASSWORDS)

 protected:
  virtual ~BrowsingDataRemovePasswordsFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

class BrowsingDataRemoveWebSQLFunction : public BrowsingDataRemoverFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browsingData.removeWebSQL",
                             BROWSINGDATA_REMOVEWEBSQL)

 protected:
  virtual ~BrowsingDataRemoveWebSQLFunction() {}

  
  virtual int GetRemovalMask() OVERRIDE;
};

#endif  
