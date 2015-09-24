// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_HANDLE_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_HANDLE_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/prerender/prerender_manager.h"

class GURL;

namespace content {
class SessionStorageNamespace;
}

namespace prerender {

class PrerenderContents;

class PrerenderHandle : public base::NonThreadSafe,
                        public PrerenderContents::Observer {
 public:
  class Observer {
   public:
    
    virtual void OnPrerenderStart(PrerenderHandle* handle) = 0;

    
    virtual void OnPrerenderStopLoading(PrerenderHandle* handle) = 0;

    
    virtual void OnPrerenderStop(PrerenderHandle* handle) = 0;

    
    virtual void OnPrerenderCreatedMatchCompleteReplacement(
        PrerenderHandle* handle) = 0;

   protected:
    Observer();
    virtual ~Observer();
  };

  
  
  virtual ~PrerenderHandle();

  void SetObserver(Observer* observer);

  
  
  
  void OnNavigateAway();

  
  
  
  
  void OnCancel();

  
  bool IsPrerendering() const;

  
  bool IsFinishedLoading() const;

  PrerenderContents* contents() const;

  
  bool Matches(
      const GURL& url,
      const content::SessionStorageNamespace* session_storage_namespace) const;

  
  
  bool RepresentingSamePrerenderAs(PrerenderHandle* other) const;

  
  
  content::SessionStorageNamespace* GetSessionStorageNamespace() const;

  
  int GetChildId() const;

 private:
  friend class PrerenderManager;

  explicit PrerenderHandle(PrerenderManager::PrerenderData* prerender_data);

  void AdoptPrerenderDataFrom(PrerenderHandle* other_handle);

  
  virtual void OnPrerenderStart(PrerenderContents* prerender_contents) OVERRIDE;
  virtual void OnPrerenderStopLoading(PrerenderContents* prerender_contents)
      OVERRIDE;
  virtual void OnPrerenderStop(PrerenderContents* prerender_contents) OVERRIDE;
  virtual void OnPrerenderCreatedMatchCompleteReplacement(
      PrerenderContents* contents, PrerenderContents* replacement) OVERRIDE;

  Observer* observer_;

  base::WeakPtr<PrerenderManager::PrerenderData> prerender_data_;
  base::WeakPtrFactory<PrerenderHandle> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderHandle);
};

}  

#endif  
