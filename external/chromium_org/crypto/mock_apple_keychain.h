// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_MOCK_KEYCHAIN_MAC_H_
#define CRYPTO_MOCK_KEYCHAIN_MAC_H_

#include <stdint.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "crypto/apple_keychain.h"

namespace crypto {

class CRYPTO_EXPORT MockAppleKeychain : public AppleKeychain {
 public:
  MockAppleKeychain();
  virtual ~MockAppleKeychain();

  
  virtual OSStatus FindGenericPassword(
      CFTypeRef keychainOrArray,
      UInt32 serviceNameLength,
      const char* serviceName,
      UInt32 accountNameLength,
      const char* accountName,
      UInt32* passwordLength,
      void** passwordData,
      SecKeychainItemRef* itemRef) const OVERRIDE;
  virtual OSStatus ItemFreeContent(SecKeychainAttributeList* attrList,
                                   void* data) const OVERRIDE;
  virtual OSStatus AddGenericPassword(
      SecKeychainRef keychain,
      UInt32 serviceNameLength,
      const char* serviceName,
      UInt32 accountNameLength,
      const char* accountName,
      UInt32 passwordLength,
      const void* passwordData,
      SecKeychainItemRef* itemRef) const OVERRIDE;

#if !defined(OS_IOS)
  virtual OSStatus ItemCopyAttributesAndData(
      SecKeychainItemRef itemRef,
      SecKeychainAttributeInfo* info,
      SecItemClass* itemClass,
      SecKeychainAttributeList** attrList,
      UInt32* length,
      void** outData) const OVERRIDE;
  
  virtual OSStatus ItemModifyAttributesAndData(
      SecKeychainItemRef itemRef,
      const SecKeychainAttributeList* attrList,
      UInt32 length,
      const void* data) const OVERRIDE;
  virtual OSStatus ItemFreeAttributesAndData(SecKeychainAttributeList* attrList,
                                             void* data) const OVERRIDE;
  virtual OSStatus ItemDelete(SecKeychainItemRef itemRef) const OVERRIDE;
  virtual OSStatus SearchCreateFromAttributes(
      CFTypeRef keychainOrArray,
      SecItemClass itemClass,
      const SecKeychainAttributeList* attrList,
      SecKeychainSearchRef* searchRef) const OVERRIDE;
  virtual OSStatus SearchCopyNext(SecKeychainSearchRef searchRef,
                                  SecKeychainItemRef* itemRef) const OVERRIDE;
  
  virtual OSStatus AddInternetPassword(
      SecKeychainRef keychain,
      UInt32 serverNameLength,
      const char* serverName,
      UInt32 securityDomainLength,
      const char* securityDomain,
      UInt32 accountNameLength,
      const char* accountName,
      UInt32 pathLength, const char* path,
      UInt16 port, SecProtocolType protocol,
      SecAuthenticationType authenticationType,
      UInt32 passwordLength,
      const void* passwordData,
      SecKeychainItemRef* itemRef) const OVERRIDE;
  virtual void Free(CFTypeRef ref) const OVERRIDE;

  
  
  int UnfreedSearchCount() const;
  int UnfreedKeychainItemCount() const;
  int UnfreedAttributeDataCount() const;

  
  
  bool CreatorCodesSetForAddedItems() const;

  struct KeychainTestData {
    const SecAuthenticationType auth_type;
    const char* server;
    const SecProtocolType protocol;
    const char* path;
    const UInt32 port;
    const char* security_domain;
    const char* creation_date;
    const char* username;
    const char* password;
    const bool negative_item;
  };
  
  void AddTestItem(const KeychainTestData& item_data);
#endif  

  
  
  
  
  
  void set_find_generic_result(OSStatus result) {
    find_generic_result_ = result;
  }

  
  bool called_add_generic() const { return called_add_generic_; }

  
  
  std::string add_generic_password() const { return add_generic_password_; }

  
  int password_data_count() const { return password_data_count_; }

 private:

  
  typedef uintptr_t MockKeychainItemType;

  
  typedef std::map<MockKeychainItemType, SecKeychainAttributeList>
      MockKeychainAttributesMap;

#if !defined(OS_IOS)
  
  
  bool AlreadyContainsInternetPassword(
      UInt32 serverNameLength,
      const char* serverName,
      UInt32 securityDomainLength,
      const char* securityDomain,
      UInt32 accountNameLength,
      const char* accountName,
      UInt32 pathLength,
      const char* path,
      UInt16 port,
      SecProtocolType protocol,
      SecAuthenticationType authenticationType) const;
  
  void InitializeKeychainData(MockKeychainItemType key) const;
  
  void SetTestDataBytes(
      MockKeychainItemType item,
      UInt32 tag,
      const void* data,
      size_t length);
  
  
  
  
  void SetTestDataString(MockKeychainItemType item,
                         UInt32 tag,
                         const char* value);
  
  
  
  void SetTestDataPort(MockKeychainItemType item, UInt32 value);
  void SetTestDataProtocol(MockKeychainItemType item, SecProtocolType value);
  void SetTestDataAuthType(MockKeychainItemType item,
                           SecAuthenticationType value);
  void SetTestDataNegativeItem(MockKeychainItemType item, Boolean value);
  void SetTestDataCreator(MockKeychainItemType item, OSType value);
  
  void SetTestDataPasswordBytes(MockKeychainItemType item,
                                const void* data,
                                size_t length);
  
  
  void SetTestDataPasswordString(MockKeychainItemType item, const char* value);

  
  static SecKeychainAttribute* AttributeWithTag(
      const SecKeychainAttributeList& attribute_list,
      UInt32 tag);

  static const SecKeychainSearchRef kDummySearchRef;

  typedef struct KeychainPasswordData {
    KeychainPasswordData() : data(NULL), length(0) {}
    void* data;
    UInt32 length;
  } KeychainPasswordData;

  
  
  mutable MockKeychainAttributesMap keychain_attr_list_;
  mutable std::map<MockKeychainItemType,
                   KeychainPasswordData> keychain_data_;
  mutable MockKeychainItemType next_item_key_;

  
  
  
  
  mutable std::vector<MockKeychainItemType> remaining_search_results_;

  
  
  
  mutable int search_copy_count_;
  mutable int keychain_item_copy_count_;
  mutable int attribute_data_copy_count_;

  
  mutable std::set<MockKeychainItemType> added_via_api_;
#endif  

  
  OSStatus find_generic_result_;

  
  mutable bool called_add_generic_;

  
  
  mutable int password_data_count_;

  
  mutable std::string add_generic_password_;
};

}  

#endif  
