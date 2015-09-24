// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_DATA_H_
#define CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_DATA_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/webstore_data_fetcher_delegate.h"
#include "ui/gfx/image/image_skia.h"

class Profile;

namespace base {
class RefCountedString;
}

namespace extensions {
class Extension;
class WebstoreDataFetcher;
}

namespace gfx {
class Image;
}

namespace net {
class URLRequestContextGetter;
}

namespace chromeos {

class KioskAppDataDelegate;

class KioskAppData : public base::SupportsWeakPtr<KioskAppData>,
                     public extensions::WebstoreDataFetcherDelegate {
 public:
  enum Status {
    STATUS_INIT,     
    STATUS_LOADING,  
    STATUS_LOADED,   
    STATUS_ERROR,    
  };

  KioskAppData(KioskAppDataDelegate* delegate,
               const std::string& app_id,
               const std::string& user_id);
  virtual ~KioskAppData();

  
  
  void Load();

  
  void ClearCache();

  
  void LoadFromInstalledApp(Profile* profile, const extensions::Extension* app);

  
  bool IsLoading() const;

  const std::string& app_id() const { return app_id_; }
  const std::string& user_id() const { return user_id_; }
  const std::string& name() const { return name_; }
  const gfx::ImageSkia& icon() const { return icon_; }
  const base::RefCountedString* raw_icon() const {
    return raw_icon_.get();
  }
  Status status() const { return status_; }

 private:
  class IconLoader;
  class WebstoreDataParser;

  void SetStatus(Status status);

  
  net::URLRequestContextGetter* GetRequestContextGetter();

  
  bool LoadFromCache();

  
  void SetCache(const std::string& name, const base::FilePath& icon_path);

  
  void SetCache(const std::string& name, const SkBitmap& icon);

  
  void OnExtensionIconLoaded(const gfx::Image& icon);

  
  void OnIconLoadSuccess(const scoped_refptr<base::RefCountedString>& raw_icon,
                         const gfx::ImageSkia& icon);
  void OnIconLoadFailure();

  
  void OnWebstoreParseSuccess(const SkBitmap& icon);
  void OnWebstoreParseFailure();

  
  void StartFetch();

  
  virtual void OnWebstoreRequestFailure() OVERRIDE;
  virtual void OnWebstoreResponseParseSuccess(
      scoped_ptr<base::DictionaryValue> webstore_data) OVERRIDE;
  virtual void OnWebstoreResponseParseFailure(
      const std::string& error) OVERRIDE;

  
  
  
  bool CheckResponseKeyValue(const base::DictionaryValue* response,
                             const char* key,
                             std::string* value);

  KioskAppDataDelegate* delegate_;  
  Status status_;

  std::string app_id_;
  std::string user_id_;
  std::string name_;
  gfx::ImageSkia icon_;
  scoped_refptr<base::RefCountedString> raw_icon_;

  scoped_ptr<extensions::WebstoreDataFetcher> webstore_fetcher_;
  base::FilePath icon_path_;

  DISALLOW_COPY_AND_ASSIGN(KioskAppData);
};

}  

#endif  
