// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PRINCIPALS_PRIVATE_PRINCIPALS_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_PRINCIPALS_PRIVATE_PRINCIPALS_PRIVATE_API_H_

#include "chrome/browser/extensions/chrome_extension_function.h"


namespace extensions {

class PrincipalsPrivateExtensionFunction : public ChromeSyncExtensionFunction {
 public:
  PrincipalsPrivateExtensionFunction() {}

 protected:
  virtual ~PrincipalsPrivateExtensionFunction() {}

  
  
  
  virtual bool RunImpl() OVERRIDE FINAL;

 private:
  virtual bool RunImplSafe() = 0;
};

class PrincipalsPrivateSignOutFunction
    : public PrincipalsPrivateExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("principalsPrivate.signOut",
                             PRINCIPALSPRIVATE_SIGNOUT);
  PrincipalsPrivateSignOutFunction() {}

 protected:
  virtual ~PrincipalsPrivateSignOutFunction() {}

 private:
  
  virtual bool RunImplSafe() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(PrincipalsPrivateSignOutFunction);
};

class PrincipalsPrivateShowAvatarBubbleFunction
    : public PrincipalsPrivateExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("principalsPrivate.showAvatarBubble",
                             PRINCIPALSPRIVATE_SHOWAVATARBUBBLE);
  PrincipalsPrivateShowAvatarBubbleFunction() {}

 protected:
  virtual ~PrincipalsPrivateShowAvatarBubbleFunction() {}

 private:
  
  virtual bool RunImplSafe() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(PrincipalsPrivateShowAvatarBubbleFunction);
};

}  

#endif  

