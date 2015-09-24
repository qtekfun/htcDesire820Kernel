// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_EXTENSION_ERROR_H_
#define EXTENSIONS_BROWSER_EXTENSION_ERROR_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "extensions/common/stack_frame.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

class ExtensionError {
 public:
  enum Type {
    MANIFEST_ERROR,
    RUNTIME_ERROR
  };

  virtual ~ExtensionError();

  
  virtual scoped_ptr<base::DictionaryValue> ToValue() const;

  virtual std::string PrintForTest() const;

  
  
  bool IsEqual(const ExtensionError* rhs) const;

  Type type() const { return type_; }
  const std::string& extension_id() const { return extension_id_; }
  bool from_incognito() const { return from_incognito_; }
  logging::LogSeverity level() const { return level_; }
  const base::string16& source() const { return source_; }
  const base::string16& message() const { return message_; }
  size_t occurrences() const { return occurrences_; }
  void set_occurrences(size_t occurrences) { occurrences_ = occurrences; }

  
  static const char kExtensionIdKey[];
  static const char kFromIncognitoKey[];
  static const char kLevelKey[];
  static const char kMessageKey[];
  static const char kSourceKey[];
  static const char kTypeKey[];

 protected:
  ExtensionError(Type type,
                 const std::string& extension_id,
                 bool from_incognito,
                 logging::LogSeverity level,
                 const base::string16& source,
                 const base::string16& message);

  virtual bool IsEqualImpl(const ExtensionError* rhs) const = 0;

  
  Type type_;
  
  std::string extension_id_;
  
  bool from_incognito_;
  
  logging::LogSeverity level_;
  
  
  
  base::string16 source_;
  
  base::string16 message_;
  
  size_t occurrences_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionError);
};

class ManifestError : public ExtensionError {
 public:
  ManifestError(const std::string& extension_id,
                const base::string16& message,
                const base::string16& manifest_key,
                const base::string16& manifest_specific);
  virtual ~ManifestError();

  virtual scoped_ptr<base::DictionaryValue> ToValue() const OVERRIDE;

  virtual std::string PrintForTest() const OVERRIDE;

  const base::string16& manifest_key() const { return manifest_key_; }
  const base::string16& manifest_specific() const { return manifest_specific_; }

  
  static const char kManifestKeyKey[];
  static const char kManifestSpecificKey[];

 private:
  virtual bool IsEqualImpl(const ExtensionError* rhs) const OVERRIDE;

  
  
  base::string16 manifest_key_;
  
  
  base::string16 manifest_specific_;

  DISALLOW_COPY_AND_ASSIGN(ManifestError);
};

class RuntimeError : public ExtensionError {
 public:
  RuntimeError(const std::string& extension_id,  
               bool from_incognito,
               const base::string16& source,
               const base::string16& message,
               const StackTrace& stack_trace,
               const GURL& context_url,
               logging::LogSeverity level,
               int render_view_id,
               int render_process_id);
  virtual ~RuntimeError();

  virtual scoped_ptr<base::DictionaryValue> ToValue() const OVERRIDE;

  virtual std::string PrintForTest() const OVERRIDE;

  const GURL& context_url() const { return context_url_; }
  const StackTrace& stack_trace() const { return stack_trace_; }
  int render_view_id() const { return render_view_id_; }
  int render_process_id() const { return render_process_id_; }

  
  static const char kColumnNumberKey[];
  static const char kContextUrlKey[];
  static const char kFunctionNameKey[];
  static const char kLineNumberKey[];
  static const char kStackTraceKey[];
  static const char kUrlKey[];
  static const char kRenderProcessIdKey[];
  static const char kRenderViewIdKey[];

 private:
  virtual bool IsEqualImpl(const ExtensionError* rhs) const OVERRIDE;

  
  
  
  void CleanUpInit();

  GURL context_url_;
  StackTrace stack_trace_;

  
  
  int render_view_id_;
  int render_process_id_;

  DISALLOW_COPY_AND_ASSIGN(RuntimeError);
};

}  

#endif  
