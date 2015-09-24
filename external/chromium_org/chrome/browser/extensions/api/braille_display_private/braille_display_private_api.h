// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRAILLE_DISPLAY_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRAILLE_DISPLAY_PRIVATE_API_H_

#include "base/scoped_observer.h"
#include "chrome/browser/extensions/api/api_function.h"
#include "chrome/browser/extensions/api/braille_display_private/braille_controller.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/common/extensions/api/braille_display_private.h"
#include "extensions/browser/event_router.h"

namespace extensions {
namespace api {
namespace braille_display_private {
class BrailleDisplayPrivateAPIUserTest;
}  
}  

class BrailleDisplayPrivateAPI
    : public ProfileKeyedAPI,
      api::braille_display_private::BrailleObserver,
      EventRouter::Observer {
 public:
  explicit BrailleDisplayPrivateAPI(Profile* profile);
  virtual ~BrailleDisplayPrivateAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<BrailleDisplayPrivateAPI>* GetFactoryInstance();

  
  virtual void OnDisplayStateChanged(
      const api::braille_display_private::DisplayState& display_state) OVERRIDE;
  virtual void OnKeyEvent(
      const api::braille_display_private::KeyEvent& keyEvent) OVERRIDE;

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;
  virtual void OnListenerRemoved(const EventListenerInfo& details) OVERRIDE;


 private:
  friend class ProfileKeyedAPIFactory<BrailleDisplayPrivateAPI>;
  friend class api::braille_display_private::BrailleDisplayPrivateAPIUserTest;

  class EventDelegate {
   public:
    virtual ~EventDelegate() {}
    virtual void BroadcastEvent(scoped_ptr<Event> event) = 0;
    virtual bool HasListener() = 0;
  };

  class DefaultEventDelegate;

  
  
  bool IsProfileActive();

  void SetEventDelegateForTest(scoped_ptr<EventDelegate> delegate);

  Profile* profile_;
  ScopedObserver<api::braille_display_private::BrailleController,
                 BrailleObserver> scoped_observer_;
  scoped_ptr<EventDelegate> event_delegate_;

  
  static const char* service_name() {
    return "BrailleDisplayPrivateAPI";
  }
  
  static const bool kServiceIsNULLWhileTesting = true;
};

namespace api {

class BrailleDisplayPrivateGetDisplayStateFunction : public AsyncApiFunction {
  DECLARE_EXTENSION_FUNCTION("brailleDisplayPrivate.getDisplayState",
                             BRAILLEDISPLAYPRIVATE_GETDISPLAYSTATE)
 protected:
  virtual ~BrailleDisplayPrivateGetDisplayStateFunction() {}
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;
  virtual bool Respond() OVERRIDE;
};

class BrailleDisplayPrivateWriteDotsFunction : public AsyncApiFunction {
  DECLARE_EXTENSION_FUNCTION("brailleDisplayPrivate.writeDots",
                             BRAILLEDISPLAYPRIVATE_WRITEDOTS);
 public:
  BrailleDisplayPrivateWriteDotsFunction();

 protected:
  virtual ~BrailleDisplayPrivateWriteDotsFunction();
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;
  virtual bool Respond() OVERRIDE;

 private:
  scoped_ptr<braille_display_private::WriteDots::Params> params_;
};

}  
}  

#endif  
