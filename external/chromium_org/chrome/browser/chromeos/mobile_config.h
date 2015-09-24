// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_MOBILE_CONFIG_H_
#define CHROME_BROWSER_CHROMEOS_MOBILE_CONFIG_H_

#include <map>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/customization_document.h"

namespace base {
class DictionaryValue;
class FilePath;
}

namespace chromeos {

class MobileConfig : public CustomizationDocument  {
 public:
  
  class CarrierDeal {
   public:
    explicit CarrierDeal(const base::DictionaryValue* deal_dict);
    ~CarrierDeal();

    
    
    
    std::string GetLocalizedString(const std::string& locale,
                                   const std::string& id) const;

    const std::string& deal_id() const { return deal_id_; }
    const std::vector<std::string>& locales() const { return locales_; }
    const std::string& info_url() const { return info_url_; }
    int notification_count() const { return notification_count_; }
    base::Time expire_date() const { return expire_date_; }

   private:
    std::string deal_id_;
    std::vector<std::string> locales_;
    std::string info_url_;
    int notification_count_;
    base::Time expire_date_;
    const base::DictionaryValue* localized_strings_;

    DISALLOW_COPY_AND_ASSIGN(CarrierDeal);
  };

  
  class Carrier {
   public:
    Carrier(const base::DictionaryValue* carrier_dict,
            const std::string& initial_locale);
    ~Carrier();

    const std::vector<std::string>& external_ids() { return external_ids_; }
    const std::string& top_up_url() const { return top_up_url_; }
    bool show_portal_button() const { return show_portal_button_; }

    
    
    const CarrierDeal* GetDefaultDeal() const;

    
    const CarrierDeal* GetDeal(const std::string& deal_id) const;

    
    
    void InitFromDictionary(const base::DictionaryValue* carrier_dict,
                            const std::string& initial_locale);

    
    void RemoveDeals();

   private:
    
    typedef std::map<std::string, CarrierDeal*> CarrierDeals;

    
    std::vector<std::string> external_ids_;

    
    
    std::string top_up_url_;

    
    
    
    
    
    bool show_portal_button_;

    CarrierDeals deals_;

    DISALLOW_COPY_AND_ASSIGN(Carrier);
  };

  
  class LocaleConfig {
   public:
    explicit LocaleConfig(base::DictionaryValue* locale_dict);
    ~LocaleConfig();

    const std::string& setup_url() const { return setup_url_; }

    
    
    void InitFromDictionary(base::DictionaryValue* locale_dict);

   private:
    
    
    
    std::string setup_url_;

    DISALLOW_COPY_AND_ASSIGN(LocaleConfig);
  };

  
  typedef std::map<std::string, std::string> CarrierIdMap;

  
  typedef std::map<std::string, Carrier*> Carriers;

  static MobileConfig* GetInstance();

  
  const MobileConfig::Carrier* GetCarrier(const std::string& carrier_id) const;

  
  
  const MobileConfig::LocaleConfig* GetLocaleConfig() const;

 protected:
  virtual bool LoadManifestFromString(const std::string& manifest) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(MobileConfigTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(MobileConfigTest, BadManifest);
  FRIEND_TEST_ALL_PREFIXES(MobileConfigTest, DealOtherLocale);
  FRIEND_TEST_ALL_PREFIXES(MobileConfigTest, OldDeal);
  FRIEND_TEST_ALL_PREFIXES(MobileConfigTest, LocalConfigNoDeals);
  FRIEND_TEST_ALL_PREFIXES(MobileConfigTest, LocalConfig);
  friend struct DefaultSingletonTraits<MobileConfig>;

  
  MobileConfig();

  
  MobileConfig(const std::string& config,
               const std::string& initial_locale);

  virtual ~MobileConfig();

  
  void LoadConfig();

  
  void ProcessConfig(const std::string& global_config,
                     const std::string& local_config);

  
  void ReadConfigInBackground(const base::FilePath& global_config_file,
                              const base::FilePath& local_config_file);

  
  CarrierIdMap carrier_id_map_;

  
  Carriers carriers_;

  
  scoped_ptr<LocaleConfig> locale_config_;

  
  std::string initial_locale_;

  
  
  scoped_ptr<base::DictionaryValue> local_config_root_;

  DISALLOW_COPY_AND_ASSIGN(MobileConfig);
};

}  

#endif  
