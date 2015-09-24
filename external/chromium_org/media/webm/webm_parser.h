// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_WEBM_WEBM_PARSER_H_
#define MEDIA_WEBM_WEBM_PARSER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT WebMParserClient {
 public:
  virtual ~WebMParserClient();

  virtual WebMParserClient* OnListStart(int id);
  virtual bool OnListEnd(int id);
  virtual bool OnUInt(int id, int64 val);
  virtual bool OnFloat(int id, double val);
  virtual bool OnBinary(int id, const uint8* data, int size);
  virtual bool OnString(int id, const std::string& str);

 protected:
  WebMParserClient();

  DISALLOW_COPY_AND_ASSIGN(WebMParserClient);
};

struct ListElementInfo;

class MEDIA_EXPORT WebMListParser {
 public:
  
  
  WebMListParser(int id, WebMParserClient* client);
  ~WebMListParser();

  
  void Reset();

  
  
  
  
  
  int Parse(const uint8* buf, int size);

  
  bool IsParsingComplete() const;

 private:
  enum State {
    NEED_LIST_HEADER,
    INSIDE_LIST,
    DONE_PARSING_LIST,
    PARSE_ERROR,
  };

  struct ListState {
    int id_;
    int64 size_;
    int64 bytes_parsed_;
    const ListElementInfo* element_info_;
    WebMParserClient* client_;
  };

  void ChangeState(State new_state);

  
  
  
  
  
  
  
  
  
  
  
  
  int ParseListElement(int header_size,
                       int id, int64 element_size,
                       const uint8* data, int size);

  
  
  
  
  
  
  
  
  bool OnListStart(int id, int64 size);

  
  
  
  
  
  bool OnListEnd();

  
  bool IsSiblingOrAncestor(int id_a, int id_b) const;

  State state_;

  
  const int root_id_;

  
  
  const int root_level_;

  
  WebMParserClient* const root_client_;

  
  
  std::vector<ListState> list_state_stack_;

  DISALLOW_COPY_AND_ASSIGN(WebMListParser);
};

int MEDIA_EXPORT WebMParseElementHeader(const uint8* buf, int size,
                                        int* id, int64* element_size);

}  

#endif  
