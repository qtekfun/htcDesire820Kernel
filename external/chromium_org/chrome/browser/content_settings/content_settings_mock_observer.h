// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_MOCK_OBSERVER_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_MOCK_OBSERVER_H_

#include <string>

#include "chrome/browser/content_settings/content_settings_observer.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace content_settings {

class MockObserver : public Observer {
 public:
  MockObserver();
  virtual ~MockObserver();

  MOCK_METHOD4(OnContentSettingChanged,
               void(const ContentSettingsPattern& primary_pattern,
                    const ContentSettingsPattern& secondary_pattern,
                    ContentSettingsType content_type,
                    std::string resource_identifier));
 private:
  DISALLOW_COPY_AND_ASSIGN(MockObserver);
};

}  

#endif  
