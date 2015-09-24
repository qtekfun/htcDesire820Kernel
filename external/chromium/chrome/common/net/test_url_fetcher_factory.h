// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NET_TEST_URL_FETCHER_FACTORY_H_
#define CHROME_COMMON_NET_TEST_URL_FETCHER_FACTORY_H_
#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>

#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"


class TestURLFetcher : public URLFetcher {
 public:
  TestURLFetcher(int id,
                 const GURL& url,
                 RequestType request_type,
                 Delegate* d);
  ~TestURLFetcher();

  
  virtual void Start() {}

  
  
  virtual void AppendChunkToUpload(const std::string& data, bool is_last_chunk);

  
  int id() const { return id_; }

  
  
  
  const GURL& original_url() const { return original_url_; }

  
  const std::string& upload_data() const { return URLFetcher::upload_data(); }

  
  const std::list<std::string>& upload_chunks() const { return chunks_; }

  
  Delegate* delegate() const { return URLFetcher::delegate(); }

 private:
  const int id_;
  const GURL original_url_;
  std::list<std::string> chunks_;
  bool did_receive_last_chunk_;

  DISALLOW_COPY_AND_ASSIGN(TestURLFetcher);
};

class TestURLFetcherFactory : public URLFetcher::Factory {
 public:
  TestURLFetcherFactory();
  virtual ~TestURLFetcherFactory();

  virtual URLFetcher* CreateURLFetcher(int id,
                                       const GURL& url,
                                       URLFetcher::RequestType request_type,
                                       URLFetcher::Delegate* d);
  TestURLFetcher* GetFetcherByID(int id) const;
  void RemoveFetcherFromMap(int id);

 private:
  
  typedef std::map<int, TestURLFetcher*> Fetchers;
  Fetchers fetchers_;

  DISALLOW_COPY_AND_ASSIGN(TestURLFetcherFactory);
};


class FakeURLFetcherFactory : public URLFetcher::Factory {
 public:
  FakeURLFetcherFactory();
  virtual ~FakeURLFetcherFactory();

  
  
  
  virtual URLFetcher* CreateURLFetcher(int id,
                                       const GURL& url,
                                       URLFetcher::RequestType request_type,
                                       URLFetcher::Delegate* d);

  
  
  void SetFakeResponse(const std::string& url,
                       const std::string& response_data,
                       bool success);

  
  
  void ClearFakeReponses();

 private:
  typedef std::map<GURL, std::pair<std::string, bool> > FakeResponseMap;
  FakeResponseMap fake_responses_;

  DISALLOW_COPY_AND_ASSIGN(FakeURLFetcherFactory);
};

#endif  
