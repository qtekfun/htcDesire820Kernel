// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_INPUT_IME_INPUT_IME_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_INPUT_IME_INPUT_IME_API_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/singleton.h"
#include "base/values.h"
#include "chrome/browser/chromeos/input_method/input_method_engine_interface.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/extension_function.h"
#include "extensions/common/extension.h"

class Profile;

namespace chromeos {
class InputMethodEngineInterface;
class ImeObserver;
}

namespace extensions {
struct InputComponentInfo;

class InputImeEventRouter {
 public:
  static InputImeEventRouter* GetInstance();

  bool RegisterIme(Profile* profile,
                   const std::string& extension_id,
                   const extensions::InputComponentInfo& component);
  void UnregisterAllImes(Profile* profile, const std::string& extension_id);
  chromeos::InputMethodEngineInterface* GetEngine(
      const std::string& extension_id,
      const std::string& engine_id);
  chromeos::InputMethodEngineInterface* GetActiveEngine(
      const std::string& extension_id);


  
  void OnKeyEventHandled(const std::string& extension_id,
                         const std::string& request_id,
                         bool handled);

  std::string AddRequest(const std::string& engine_id,
                         chromeos::input_method::KeyEventHandle* key_data);

 private:
  friend struct DefaultSingletonTraits<InputImeEventRouter>;
  typedef std::map<std::string, std::pair<std::string,
          chromeos::input_method::KeyEventHandle*> > RequestMap;

  InputImeEventRouter();
  ~InputImeEventRouter();

  std::map<std::string, std::map<std::string,
                                 chromeos::InputMethodEngineInterface*> >
      engines_;
  std::map<std::string, std::map<std::string, chromeos::ImeObserver*> >
      observers_;

  unsigned int next_request_id_;
  RequestMap request_map_;

  DISALLOW_COPY_AND_ASSIGN(InputImeEventRouter);
};

class InputImeSetCompositionFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.setComposition",
                             INPUT_IME_SETCOMPOSITION)

 protected:
  virtual ~InputImeSetCompositionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeClearCompositionFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.clearComposition",
                             INPUT_IME_CLEARCOMPOSITION)

 protected:
  virtual ~InputImeClearCompositionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeCommitTextFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.commitText", INPUT_IME_COMMITTEXT)

 protected:
  virtual ~InputImeCommitTextFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeSetCandidateWindowPropertiesFunction
    : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.setCandidateWindowProperties",
                             INPUT_IME_SETCANDIDATEWINDOWPROPERTIES)

 protected:
  virtual ~InputImeSetCandidateWindowPropertiesFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeSetCandidatesFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.setCandidates", INPUT_IME_SETCANDIDATES)

 protected:
  virtual ~InputImeSetCandidatesFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeSetCursorPositionFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.setCursorPosition",
                             INPUT_IME_SETCURSORPOSITION)

 protected:
  virtual ~InputImeSetCursorPositionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeSetMenuItemsFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.setMenuItems", INPUT_IME_SETMENUITEMS)

 protected:
  virtual ~InputImeSetMenuItemsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeUpdateMenuItemsFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.updateMenuItems",
                             INPUT_IME_UPDATEMENUITEMS)

 protected:
  virtual ~InputImeUpdateMenuItemsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeDeleteSurroundingTextFunction : public SyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.deleteSurroundingText",
                             INPUT_IME_DELETESURROUNDINGTEXT)
 protected:
  virtual ~InputImeDeleteSurroundingTextFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeKeyEventHandledFunction : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.keyEventHandled",
                             INPUT_IME_KEYEVENTHANDLED)

 protected:
  virtual ~InputImeKeyEventHandledFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeSendKeyEventsFunction : public AsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("input.ime.sendKeyEvents",
                             INPUT_IME_SENDKEYEVENTS)

 protected:
  virtual ~InputImeSendKeyEventsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class InputImeAPI : public ProfileKeyedAPI,
                    public content::NotificationObserver {
 public:
  explicit InputImeAPI(Profile* profile);
  virtual ~InputImeAPI();

  
  static ProfileKeyedAPIFactory<InputImeAPI>* GetFactoryInstance();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<InputImeAPI>;
  InputImeEventRouter* input_ime_event_router();

  
  static const char* service_name() {
    return "InputImeAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  Profile* const profile_;
  content::NotificationRegistrar registrar_;
};

}  

#endif  
