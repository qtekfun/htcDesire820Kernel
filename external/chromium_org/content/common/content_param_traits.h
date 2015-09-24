// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_COMMON_CONTENT_PARAM_TRAITS_H_
#define CONTENT_COMMON_CONTENT_PARAM_TRAITS_H_

#include "content/common/content_param_traits_macros.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"
#include "webkit/common/cursors/webcursor.h"

namespace gfx {
class Range;
}

namespace net {
class IPEndPoint;
}

namespace IPC {

template <>
struct ParamTraits<net::IPEndPoint> {
  typedef net::IPEndPoint param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* p);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<gfx::Range> {
  typedef gfx::Range param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<WebCursor> {
  typedef WebCursor param_type;
  static void Write(Message* m, const param_type& p) {
    p.Serialize(m);
  }
  static bool Read(const Message* m, PickleIterator* iter, param_type* r)  {
    return r->Deserialize(iter);
  }
  static void Log(const param_type& p, std::string* l) {
    l->append("<WebCursor>");
  }
};

typedef const blink::WebInputEvent* WebInputEventPointer;
template <>
struct ParamTraits<WebInputEventPointer> {
  typedef WebInputEventPointer param_type;
  static void Write(Message* m, const param_type& p);
  
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

}  

#endif  
