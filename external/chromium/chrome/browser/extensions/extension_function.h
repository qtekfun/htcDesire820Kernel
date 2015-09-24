// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_H_
#pragma once

#include <string>
#include <list>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_function_dispatcher.h"

class ExtensionFunctionDispatcher;
class ListValue;
class Profile;
class QuotaLimitHeuristic;
class Value;

#define EXTENSION_FUNCTION_VALIDATE(test) do { \
    if (!(test)) { \
      bad_message_ = true; \
      return false; \
    } \
  } while (0)

#define EXTENSION_FUNCTION_ERROR(error) do { \
    error_ = error; \
    bad_message_ = true; \
    return false; \
  } while (0)

#define DECLARE_EXTENSION_FUNCTION_NAME(name) \
  public: static const char* function_name() { return name; }

class ExtensionFunction : public base::RefCountedThreadSafe<ExtensionFunction> {
 public:
  ExtensionFunction();

  
  void set_name(const std::string& name) { name_ = name; }
  const std::string name() const { return name_; }

  
  
  void set_profile(Profile* profile) { profile_ = profile; }
  Profile* profile() const { return profile_; }

  
  void set_extension_id(std::string extension_id) {
    extension_id_ = extension_id;
  }
  std::string extension_id() const { return extension_id_; }

  
  virtual void SetArgs(const ListValue* args) = 0;

  
  
  virtual const std::string GetResult() = 0;

  
  virtual const std::string GetError() = 0;

  
  
  virtual void GetQuotaLimitHeuristics(
      std::list<QuotaLimitHeuristic*>* heuristics) const {}

  void set_dispatcher_peer(ExtensionFunctionDispatcher::Peer* peer) {
    peer_ = peer;
  }
  ExtensionFunctionDispatcher* dispatcher() const {
    return peer_->dispatcher_;
  }

  void set_request_id(int request_id) { request_id_ = request_id; }
  int request_id() { return request_id_; }

  void set_source_url(const GURL& source_url) { source_url_ = source_url; }
  const GURL& source_url() { return source_url_; }

  void set_has_callback(bool has_callback) { has_callback_ = has_callback; }
  bool has_callback() { return has_callback_; }

  void set_include_incognito(bool include) { include_incognito_ = include; }
  bool include_incognito() { return include_incognito_; }

  void set_user_gesture(bool user_gesture) { user_gesture_ = user_gesture; }
  bool user_gesture() const { return user_gesture_; }

  
  
  
  
  virtual void Run() = 0;

 protected:
  friend class base::RefCountedThreadSafe<ExtensionFunction>;

  virtual ~ExtensionFunction();

  
  
  
  const Extension* GetExtension();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Browser* GetCurrentBrowser();

  
  scoped_refptr<ExtensionFunctionDispatcher::Peer> peer_;

  
  int request_id_;

  
  Profile* profile_;

  
  std::string extension_id_;

  
  std::string name_;

  
  GURL source_url_;

  
  
  bool has_callback_;

  
  
  
  
  bool include_incognito_;

  
  bool user_gesture_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionFunction);
};

class AsyncExtensionFunction : public ExtensionFunction {
 public:
  AsyncExtensionFunction();

  virtual void SetArgs(const ListValue* args);
  virtual const std::string GetResult();
  virtual const std::string GetError();
  virtual void Run();

  
  
  virtual bool RunImpl() = 0;

 protected:
  virtual ~AsyncExtensionFunction();

  void SendResponse(bool success);

  
  
  bool HasOptionalArgument(size_t index);

  
  scoped_ptr<ListValue> args_;

  
  
  scoped_ptr<Value> result_;

  
  
  std::string error_;

  
  
  bool bad_message_;

  DISALLOW_COPY_AND_ASSIGN(AsyncExtensionFunction);
};

class SyncExtensionFunction : public AsyncExtensionFunction {
 public:
  SyncExtensionFunction();

  
  
  virtual bool RunImpl() = 0;

  virtual void Run();

 protected:
  virtual ~SyncExtensionFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(SyncExtensionFunction);
};

#endif  
