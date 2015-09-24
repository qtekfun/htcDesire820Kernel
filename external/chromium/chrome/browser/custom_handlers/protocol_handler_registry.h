// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CUSTOM_HANDLERS_PROTOCOL_HANDLER_REGISTRY_H_
#define CHROME_BROWSER_CUSTOM_HANDLERS_PROTOCOL_HANDLER_REGISTRY_H_
#pragma once

#include <string>
#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "chrome/browser/custom_handlers/protocol_handler.h"
#include "chrome/browser/profiles/profile.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job.h"




class ProtocolHandlerRegistry
    : public base::RefCountedThreadSafe<ProtocolHandlerRegistry> {
 public:
  explicit ProtocolHandlerRegistry(Profile* profile);

  
  void OnAcceptRegisterProtocolHandler(ProtocolHandler* handler);

  
  void OnDenyRegisterProtocolHandler(ProtocolHandler* handler);

  
  void Load();

  
  void Save();

  
  ProtocolHandler* GetHandlerFor(const std::string& scheme) const;

  
  
  bool CanSchemeBeOverridden(const std::string& scheme) const;

  
  bool IsAlreadyRegistered(const ProtocolHandler* handler) const;

  
  
  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     const std::string& scheme);

  
  static void RegisterPrefs(PrefService* prefService);

  
  
  net::URLRequestJob* MaybeCreateJob(net::URLRequest* request) const;

 private:
  typedef std::map<std::string, ProtocolHandler*> ProtocolHandlerMap;

  friend class base::RefCountedThreadSafe<ProtocolHandlerRegistry>;
  ~ProtocolHandlerRegistry();

  
  
  Value* Encode();

  
  void RegisterProtocolHandler(ProtocolHandler* handler);

  
  void RegisterHandlerFromValue(const DictionaryValue* value);

  
  ProtocolHandlerMap protocolHandlers_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ProtocolHandlerRegistry);
};
#endif  

