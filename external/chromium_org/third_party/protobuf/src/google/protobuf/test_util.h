// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_TEST_UTIL_H__
#define GOOGLE_PROTOBUF_TEST_UTIL_H__

#include <stack>
#include <string>
#include <google/protobuf/message.h>
#include <google/protobuf/unittest.pb.h>

namespace google {
namespace protobuf {

namespace unittest = ::protobuf_unittest;
namespace unittest_import = protobuf_unittest_import;

class TestUtil {
 public:
  
  static void SetAllFields(unittest::TestAllTypes* message);
  static void SetOptionalFields(unittest::TestAllTypes* message);
  static void AddRepeatedFields1(unittest::TestAllTypes* message);
  static void AddRepeatedFields2(unittest::TestAllTypes* message);
  static void SetDefaultFields(unittest::TestAllTypes* message);
  static void SetAllExtensions(unittest::TestAllExtensions* message);
  static void SetAllFieldsAndExtensions(unittest::TestFieldOrderings* message);
  static void SetPackedFields(unittest::TestPackedTypes* message);
  static void SetPackedExtensions(unittest::TestPackedExtensions* message);
  static void SetUnpackedFields(unittest::TestUnpackedTypes* message);

  
  
  
  
  static void ModifyRepeatedFields(unittest::TestAllTypes* message);
  static void ModifyRepeatedExtensions(unittest::TestAllExtensions* message);
  static void ModifyPackedFields(unittest::TestPackedTypes* message);
  static void ModifyPackedExtensions(unittest::TestPackedExtensions* message);

  
  
  static void ExpectAllFieldsSet(const unittest::TestAllTypes& message);
  static void ExpectAllExtensionsSet(
      const unittest::TestAllExtensions& message);
  static void ExpectPackedFieldsSet(const unittest::TestPackedTypes& message);
  static void ExpectPackedExtensionsSet(
      const unittest::TestPackedExtensions& message);
  static void ExpectUnpackedFieldsSet(
      const unittest::TestUnpackedTypes& message);

  
  
  static void ExpectRepeatedFieldsModified(
      const unittest::TestAllTypes& message);
  static void ExpectRepeatedExtensionsModified(
      const unittest::TestAllExtensions& message);
  static void ExpectPackedFieldsModified(
      const unittest::TestPackedTypes& message);
  static void ExpectPackedExtensionsModified(
      const unittest::TestPackedExtensions& message);

  
  static void ExpectClear(const unittest::TestAllTypes& message);
  static void ExpectExtensionsClear(const unittest::TestAllExtensions& message);
  static void ExpectPackedClear(const unittest::TestPackedTypes& message);
  static void ExpectPackedExtensionsClear(
      const unittest::TestPackedExtensions& message);

  
  
  
  static void ExpectAllFieldsAndExtensionsInOrder(const string& serialized);

  
  static void ExpectLastRepeatedsRemoved(
      const unittest::TestAllTypes& message);
  static void ExpectLastRepeatedExtensionsRemoved(
      const unittest::TestAllExtensions& message);
  static void ExpectLastRepeatedsReleased(
      const unittest::TestAllTypes& message);
  static void ExpectLastRepeatedExtensionsReleased(
      const unittest::TestAllExtensions& message);

  
  
  static void ExpectRepeatedsSwapped(const unittest::TestAllTypes& message);
  static void ExpectRepeatedExtensionsSwapped(
      const unittest::TestAllExtensions& message);

  
  class ReflectionTester {
   public:
    
    
    
    
    
    explicit ReflectionTester(const Descriptor* base_descriptor);

    void SetAllFieldsViaReflection(Message* message);
    void ModifyRepeatedFieldsViaReflection(Message* message);
    void ExpectAllFieldsSetViaReflection(const Message& message);
    void ExpectClearViaReflection(const Message& message);

    void SetPackedFieldsViaReflection(Message* message);
    void ModifyPackedFieldsViaReflection(Message* message);
    void ExpectPackedFieldsSetViaReflection(const Message& message);
    void ExpectPackedClearViaReflection(const Message& message);

    void RemoveLastRepeatedsViaReflection(Message* message);
    void ReleaseLastRepeatedsViaReflection(
        Message* message, bool expect_extensions_notnull);
    void SwapRepeatedsViaReflection(Message* message);

    enum MessageReleaseState {
      IS_NULL,
      CAN_BE_NULL,
      NOT_NULL,
    };
    void ExpectMessagesReleasedViaReflection(
        Message* message, MessageReleaseState expected_release_state);

   private:
    const FieldDescriptor* F(const string& name);

    const Descriptor* base_descriptor_;

    const FieldDescriptor* group_a_;
    const FieldDescriptor* repeated_group_a_;
    const FieldDescriptor* nested_b_;
    const FieldDescriptor* foreign_c_;
    const FieldDescriptor* import_d_;
    const FieldDescriptor* import_e_;

    const EnumValueDescriptor* nested_foo_;
    const EnumValueDescriptor* nested_bar_;
    const EnumValueDescriptor* nested_baz_;
    const EnumValueDescriptor* foreign_foo_;
    const EnumValueDescriptor* foreign_bar_;
    const EnumValueDescriptor* foreign_baz_;
    const EnumValueDescriptor* import_foo_;
    const EnumValueDescriptor* import_bar_;
    const EnumValueDescriptor* import_baz_;

    
    
    void ExpectAllFieldsSetViaReflection1(const Message& message);
    void ExpectAllFieldsSetViaReflection2(const Message& message);
    void ExpectAllFieldsSetViaReflection3(const Message& message);

    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ReflectionTester);
  };

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TestUtil);
};

}  

}  
#endif  
