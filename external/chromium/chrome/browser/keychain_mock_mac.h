// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_KEYCHAIN_MOCK_MAC_H_
#define CHROME_BROWSER_KEYCHAIN_MOCK_MAC_H_
#pragma once

#include <set>
#include <string>
#include <vector>

#include "chrome/browser/keychain_mac.h"

class MockKeychain : public MacKeychain {
 public:
  
  explicit MockKeychain(unsigned int item_capacity);
  virtual ~MockKeychain();
  virtual OSStatus ItemCopyAttributesAndData(
      SecKeychainItemRef itemRef, SecKeychainAttributeInfo *info,
      SecItemClass *itemClass, SecKeychainAttributeList **attrList,
      UInt32 *length, void **outData) const;
  
  virtual OSStatus ItemModifyAttributesAndData(
      SecKeychainItemRef itemRef, const SecKeychainAttributeList *attrList,
      UInt32 length, const void *data) const;
  virtual OSStatus ItemFreeAttributesAndData(SecKeychainAttributeList *attrList,
                                             void *data) const;
  virtual OSStatus ItemDelete(SecKeychainItemRef itemRef) const;
  virtual OSStatus SearchCreateFromAttributes(
      CFTypeRef keychainOrArray, SecItemClass itemClass,
      const SecKeychainAttributeList *attrList,
      SecKeychainSearchRef *searchRef) const;
  virtual OSStatus SearchCopyNext(SecKeychainSearchRef searchRef,
                                  SecKeychainItemRef *itemRef) const;
  
  
  
  virtual OSStatus AddInternetPassword(SecKeychainRef keychain,
                                       UInt32 serverNameLength,
                                       const char *serverName,
                                       UInt32 securityDomainLength,
                                       const char *securityDomain,
                                       UInt32 accountNameLength,
                                       const char *accountName,
                                       UInt32 pathLength, const char *path,
                                       UInt16 port, SecProtocolType protocol,
                                       SecAuthenticationType authenticationType,
                                       UInt32 passwordLength,
                                       const void *passwordData,
                                       SecKeychainItemRef *itemRef) const;
  virtual OSStatus FindGenericPassword(CFTypeRef keychainOrArray,
                                       UInt32 serviceNameLength,
                                       const char *serviceName,
                                       UInt32 accountNameLength,
                                       const char *accountName,
                                       UInt32 *passwordLength,
                                       void **passwordData,
                                       SecKeychainItemRef *itemRef) const;
  virtual OSStatus ItemFreeContent(SecKeychainAttributeList *attrList,
                                   void *data) const;
  virtual OSStatus AddGenericPassword(SecKeychainRef keychain,
                                      UInt32 serviceNameLength,
                                      const char *serviceName,
                                      UInt32 accountNameLength,
                                      const char *accountName,
                                      UInt32 passwordLength,
                                      const void *passwordData,
                                      SecKeychainItemRef *itemRef) const;
  virtual void Free(CFTypeRef ref) const;

  
  
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

  
  
  
  
  
  void set_find_generic_result(OSStatus result) {
    find_generic_result_ = result;
  }

  
  bool called_add_generic() const { return called_add_generic_; }

  
  
  std::string add_generic_password() const { return add_generic_password_; }

  
  int password_data_count() const { return password_data_count_; }

 private:
  
  void SetTestDataBytes(int item, UInt32 tag, const void* data, size_t length);
  
  
  
  
  void SetTestDataString(int item, UInt32 tag, const char* value);
  
  
  
  void SetTestDataPort(int item, UInt32 value);
  void SetTestDataProtocol(int item, SecProtocolType value);
  void SetTestDataAuthType(int item, SecAuthenticationType value);
  void SetTestDataNegativeItem(int item, Boolean value);
  void SetTestDataCreator(int item, OSType value);
  
  void SetTestDataPasswordBytes(int item, const void* data, size_t length);
  
  
  void SetTestDataPasswordString(int item, const char* value);

  
  static SecKeychainAttribute* AttributeWithTag(
      const SecKeychainAttributeList& attribute_list, UInt32 tag);

  static const int kDummySearchRef = 1000;

  typedef struct  {
    void* data;
    UInt32 length;
  } KeychainPasswordData;

  SecKeychainAttributeList* keychain_attr_list_;
  KeychainPasswordData* keychain_data_;
  unsigned int item_capacity_;
  mutable unsigned int item_count_;

  
  
  
  
  mutable std::vector<unsigned int> remaining_search_results_;

  
  
  
  mutable int search_copy_count_;
  mutable int keychain_item_copy_count_;
  mutable int attribute_data_copy_count_;

  
  mutable std::set<unsigned int> added_via_api_;

  
  OSStatus find_generic_result_;

  
  mutable bool called_add_generic_;

  
  
  mutable unsigned int password_data_count_;

  
  mutable std::string add_generic_password_;
};

#endif  
