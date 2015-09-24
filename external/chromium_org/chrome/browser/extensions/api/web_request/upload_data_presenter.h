// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_UPLOAD_DATA_PRESENTER_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_UPLOAD_DATA_PRESENTER_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace extensions {
class FormDataParser;
}

namespace net {
class URLRequest;
class UploadElementReader;
}

namespace extensions {

namespace subtle {


void AppendKeyValuePair(const char* key,
                        base::Value* value,
                        base::ListValue* list);

}  

FORWARD_DECLARE_TEST(WebRequestUploadDataPresenterTest, RawData);

class UploadDataPresenter {
 public:
  virtual ~UploadDataPresenter();
  virtual void FeedNext(const net::UploadElementReader& reader) = 0;
  virtual bool Succeeded() = 0;
  virtual scoped_ptr<base::Value> Result() = 0;

 protected:
  UploadDataPresenter() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(UploadDataPresenter);
};

class RawDataPresenter : public UploadDataPresenter {
 public:
  RawDataPresenter();
  virtual ~RawDataPresenter();

  
  virtual void FeedNext(const net::UploadElementReader& reader) OVERRIDE;
  virtual bool Succeeded() OVERRIDE;
  virtual scoped_ptr<base::Value> Result() OVERRIDE;

 private:
  void FeedNextBytes(const char* bytes, size_t size);
  void FeedNextFile(const std::string& filename);
  FRIEND_TEST_ALL_PREFIXES(WebRequestUploadDataPresenterTest, RawData);

  bool success_;
  scoped_ptr<base::ListValue> list_;
};

class ParsedDataPresenter : public UploadDataPresenter {
 public:
  explicit ParsedDataPresenter(const net::URLRequest& request);
  virtual ~ParsedDataPresenter();

  
  virtual void FeedNext(const net::UploadElementReader& reader) OVERRIDE;
  virtual bool Succeeded() OVERRIDE;
  virtual scoped_ptr<base::Value> Result() OVERRIDE;

  
  
  
  static scoped_ptr<ParsedDataPresenter> CreateForTests();

 private:
  
  explicit ParsedDataPresenter(const std::string& form_type);

  
  void Abort();
  scoped_ptr<FormDataParser> parser_;
  bool success_;
  scoped_ptr<base::DictionaryValue> dictionary_;
};

}  

#endif  
