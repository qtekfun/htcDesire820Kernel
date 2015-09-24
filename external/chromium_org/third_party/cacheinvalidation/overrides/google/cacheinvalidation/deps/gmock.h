// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_CACHEINVALIDATION_DEPS_GMOCK_H_
#define GOOGLE_CACHEINVALIDATION_DEPS_GMOCK_H_

#include "testing/gmock/include/gmock/gmock.h"

namespace testing {
namespace internal {


template <class Proto>
class WhenDeserializedAsMatcher {
 public:
  typedef Matcher<const Proto&> InnerMatcher;

  explicit WhenDeserializedAsMatcher(const InnerMatcher& proto_matcher)
      : proto_matcher_(proto_matcher) {}

  virtual ~WhenDeserializedAsMatcher() {}

  
  
  Proto* Deserialize(const string& str) const {
    Proto* proto = new Proto;
    if (proto->ParsePartialFromString(str)) {
      return proto;
    } else {
      delete proto;
      return NULL;
    }
  }

  void DescribeTo(::std::ostream* os) const {
    *os << "can be deserialized as a protobuf that ";
    proto_matcher_.DescribeTo(os);
  }

  void DescribeNegationTo(::std::ostream* os) const {
    *os << "cannot be deserialized as a protobuf that ";
    proto_matcher_.DescribeTo(os);
  }

  bool MatchAndExplain(const string& arg, MatchResultListener* listener) const {
    
    
    scoped_ptr<const Proto> deserialized_arg(Deserialize(arg));
    
    return (deserialized_arg.get() != NULL) &&
        proto_matcher_.Matches(*deserialized_arg);
  }

 private:
  const InnerMatcher proto_matcher_;
};

}  

namespace proto {

template <class Proto, class InnerMatcher>
inline PolymorphicMatcher<internal::WhenDeserializedAsMatcher<Proto> >
WhenDeserializedAs(const InnerMatcher& inner_matcher) {
  return MakePolymorphicMatcher(
      internal::WhenDeserializedAsMatcher<Proto>(
          SafeMatcherCast<const Proto&>(inner_matcher)));
}

}  

MATCHER_P(EqualsProto, message, "") {
  
  
  
  
  std::string expected_serialized, actual_serialized;
  message.SerializeToString(&expected_serialized);
  arg.SerializeToString(&actual_serialized);
  return expected_serialized == actual_serialized;
}

}  

#endif  
