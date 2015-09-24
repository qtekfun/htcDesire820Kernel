// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CUSTOM_HANDLERS_PROTOCOL_HANDLER_H_
#define CHROME_COMMON_CUSTOM_HANDLERS_PROTOCOL_HANDLER_H_

#include <string>

#include "base/values.h"
#include "url/gurl.h"

// given protocol should be rewritten to be handled.

class ProtocolHandler {
 public:
  static ProtocolHandler CreateProtocolHandler(const std::string& protocol,
                                               const GURL& url,
                                               const base::string16& title);

  
  
  static ProtocolHandler CreateProtocolHandler(
      const base::DictionaryValue* value);

  
  
  static bool IsValidDict(const base::DictionaryValue* value);

  
  bool IsSameOrigin(const ProtocolHandler& handler) const;

  
  static const ProtocolHandler& EmptyProtocolHandler();

  
  GURL TranslateUrl(const GURL& url) const;

  
  
  
  bool IsEquivalent(const ProtocolHandler& other) const;

  
  
  base::DictionaryValue* Encode() const;

  const std::string& protocol() const { return protocol_; }
  const GURL& url() const { return url_;}
  const base::string16& title() const { return title_; }

  bool IsEmpty() const {
    return protocol_.empty();
  }

#if !defined(NDEBUG)
  
  std::string ToString() const;
#endif


  bool operator==(const ProtocolHandler& other) const;
  bool operator<(const ProtocolHandler& other) const;

 private:
  ProtocolHandler(const std::string& protocol,
                  const GURL& url,
                  const base::string16& title);
  ProtocolHandler();

  std::string protocol_;
  GURL url_;
  base::string16 title_;
};

#endif  
