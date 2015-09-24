// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_TEST_UTIL_LITE_H__
#define GOOGLE_PROTOBUF_TEST_UTIL_LITE_H__

#include <google/protobuf/unittest_lite.pb.h>

namespace google {
namespace protobuf {

namespace unittest = protobuf_unittest;
namespace unittest_import = protobuf_unittest_import;

class TestUtilLite {
 public:
  
  static void SetAllFields(unittest::TestAllTypesLite* message);
  static void SetAllExtensions(unittest::TestAllExtensionsLite* message);
  static void SetPackedFields(unittest::TestPackedTypesLite* message);
  static void SetPackedExtensions(unittest::TestPackedExtensionsLite* message);

  
  
  
  
  static void ModifyRepeatedFields(unittest::TestAllTypesLite* message);
  static void ModifyRepeatedExtensions(
      unittest::TestAllExtensionsLite* message);
  static void ModifyPackedFields(unittest::TestPackedTypesLite* message);
  static void ModifyPackedExtensions(
      unittest::TestPackedExtensionsLite* message);

  
  
  static void ExpectAllFieldsSet(const unittest::TestAllTypesLite& message);
  static void ExpectAllExtensionsSet(
      const unittest::TestAllExtensionsLite& message);
  static void ExpectPackedFieldsSet(
      const unittest::TestPackedTypesLite& message);
  static void ExpectPackedExtensionsSet(
      const unittest::TestPackedExtensionsLite& message);

  
  
  static void ExpectRepeatedFieldsModified(
      const unittest::TestAllTypesLite& message);
  static void ExpectRepeatedExtensionsModified(
      const unittest::TestAllExtensionsLite& message);
  static void ExpectPackedFieldsModified(
      const unittest::TestPackedTypesLite& message);
  static void ExpectPackedExtensionsModified(
      const unittest::TestPackedExtensionsLite& message);

  
  static void ExpectClear(const unittest::TestAllTypesLite& message);
  static void ExpectExtensionsClear(
      const unittest::TestAllExtensionsLite& message);
  static void ExpectPackedClear(const unittest::TestPackedTypesLite& message);
  static void ExpectPackedExtensionsClear(
      const unittest::TestPackedExtensionsLite& message);

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TestUtilLite);
};

}  

}  
#endif  
