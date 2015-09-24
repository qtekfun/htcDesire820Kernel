// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRAILLE_CONTROLLER_BRLAPI_H_
#define CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRAILLE_CONTROLLER_BRLAPI_H_

#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "chrome/browser/extensions/api/braille_display_private/braille_controller.h"
#include "chrome/browser/extensions/api/braille_display_private/brlapi_connection.h"
#include "library_loaders/libbrlapi.h"

namespace extensions {
namespace api {
namespace braille_display_private {

class BrailleControllerImpl : public BrailleController {
 public:
  static BrailleControllerImpl* GetInstance();
  virtual scoped_ptr<DisplayState> GetDisplayState() OVERRIDE;
  virtual void WriteDots(const std::string& cells) OVERRIDE;
  virtual void AddObserver(BrailleObserver* observer) OVERRIDE;
  virtual void RemoveObserver(BrailleObserver* observer) OVERRIDE;

 private:
  
  friend class BrailleDisplayPrivateApiTest;
  friend class MockBrlapiConnection;

  BrailleControllerImpl();
  virtual ~BrailleControllerImpl();
  void TryLoadLibBrlApi();

  typedef base::Callback<scoped_ptr<BrlapiConnection>()>
      CreateBrlapiConnectionFunction;

  
  
  void SetCreateBrlapiConnectionForTesting(
      const CreateBrlapiConnectionFunction& callback);

  
  
  void PokeSocketDirForTesting();

  
  
  void StartConnecting();
  void StartWatchingSocketDirOnFileThread();
  void OnSocketDirChangedOnFileThread(const base::FilePath& path, bool error);
  void OnSocketDirChangedOnIOThread();
  void TryToConnect();
  void ResetRetryConnectHorizon();
  void ScheduleTryToConnect();
  void Disconnect();
  scoped_ptr<BrlapiConnection> CreateBrlapiConnection();
  void DispatchKeys();
  scoped_ptr<KeyEvent> MapKeyCode(brlapi_keyCode_t code);
  void DispatchKeyEvent(scoped_ptr<KeyEvent> event);
  void DispatchOnDisplayStateChanged(scoped_ptr<DisplayState> new_state);

  CreateBrlapiConnectionFunction create_brlapi_connection_function_;

  
  LibBrlapiLoader libbrlapi_loader_;
  scoped_ptr<BrlapiConnection> connection_;
  bool started_connecting_;
  bool connect_scheduled_;
  base::Time retry_connect_horizon_;

  
  ObserverList<BrailleObserver> observers_;

  
  base::FilePathWatcher file_path_watcher_;

  friend struct DefaultSingletonTraits<BrailleControllerImpl>;

  DISALLOW_COPY_AND_ASSIGN(BrailleControllerImpl);
};

}  
}  
}  

#endif  
