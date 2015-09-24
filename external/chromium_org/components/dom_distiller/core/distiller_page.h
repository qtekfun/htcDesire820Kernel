// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_DISTILLER_PAGE_H_
#define COMPONENTS_DOM_DISTILLER_CORE_DISTILLER_PAGE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "url/gurl.h"

namespace dom_distiller {

class DistillerPage {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    virtual void OnLoadURLDone() {}
    virtual void OnExecuteJavaScriptDone(const base::Value* value) {}
  };

  explicit DistillerPage(Delegate* delegate);

  virtual ~DistillerPage();


  
  
  void Init();

  
  
  void LoadURL(const GURL& url);
  virtual void OnLoadURLDone();
  virtual void OnLoadURLFailed();

  
  
  
  void ExecuteJavaScript(const std::string& script);

  
  
  virtual void OnExecuteJavaScriptDone(const base::Value* value);

 protected:
  enum State {
    
    NO_CONTEXT,
    
    IDLE,
    
    LOADING_PAGE,
    
    PAGE_AVAILABLE,
    
    PAGELOAD_FAILED,
    
    
    EXECUTING_JAVASCRIPT
  };

  
  
  virtual void InitImpl() = 0;

  
  
  virtual void LoadURLImpl(const GURL& gurl) = 0;

  
  
  virtual void ExecuteJavaScriptImpl(const std::string& script) = 0;



  
  State state_;

 private:
  Delegate* delegate_;
  DISALLOW_COPY_AND_ASSIGN(DistillerPage);
};

class DistillerPageFactory {
 public:
  virtual ~DistillerPageFactory();

  virtual scoped_ptr<DistillerPage> CreateDistillerPage(
      DistillerPage::Delegate* delegate) const = 0;
};

}  

#endif  
