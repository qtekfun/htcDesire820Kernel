// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MAC_SECURITY_WRAPPERS_H_
#define CHROME_BROWSER_MAC_SECURITY_WRAPPERS_H_

#include <Security/Security.h>
#include <Security/SecRequirement.h>

#include "base/basictypes.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/memory/scoped_ptr.h"

namespace chrome {

class ScopedSecKeychainSetUserInteractionAllowed {
 public:
  explicit ScopedSecKeychainSetUserInteractionAllowed(Boolean allowed);
  ~ScopedSecKeychainSetUserInteractionAllowed();

 private:
  Boolean old_allowed_;

  DISALLOW_COPY_AND_ASSIGN(ScopedSecKeychainSetUserInteractionAllowed);
};

class CrSKeychainItemAndAccess {
 public:
  CrSKeychainItemAndAccess(SecKeychainItemRef item, SecAccessRef access);
  CrSKeychainItemAndAccess(const CrSKeychainItemAndAccess& that);

  ~CrSKeychainItemAndAccess();

  void operator=(const CrSKeychainItemAndAccess& that);

  SecKeychainItemRef item() const { return item_; }
  SecAccessRef access() const { return access_; }

 private:
  base::ScopedCFTypeRef<SecKeychainItemRef> item_;
  base::ScopedCFTypeRef<SecAccessRef> access_;
};

struct CrSACLSimpleContents {
  CrSACLSimpleContents();
  ~CrSACLSimpleContents();

  base::ScopedCFTypeRef<CFArrayRef> application_list;
  base::ScopedCFTypeRef<CFStringRef> description;
  CSSM_ACL_KEYCHAIN_PROMPT_SELECTOR prompt_selector;
};

class ScopedSecKeychainAttributeInfo {
 public:
  explicit ScopedSecKeychainAttributeInfo(
      SecKeychainAttributeInfo* attribute_info);
  ~ScopedSecKeychainAttributeInfo();

  operator SecKeychainAttributeInfo*() const {
    return attribute_info_;
  }

 private:
  SecKeychainAttributeInfo* attribute_info_;
};

struct CrSKeychainItemAttributesAndData {
  SecItemClass item_class;
  SecKeychainAttributeList* attribute_list;
  UInt32 length;
  void* data;
};

class ScopedCrSKeychainItemAttributesAndData {
 public:
  ScopedCrSKeychainItemAttributesAndData(
      CrSKeychainItemAttributesAndData* attributes_and_data);
  ~ScopedCrSKeychainItemAttributesAndData();

  CrSKeychainItemAttributesAndData* get() const {
    return attributes_and_data_.get();
  }

  CrSKeychainItemAttributesAndData* release() {
    return attributes_and_data_.release();
  }

  SecItemClass item_class() const {
    return attributes_and_data_->item_class;
  }

  SecItemClass* item_class_ptr() const {
    return &attributes_and_data_->item_class;
  }

  SecKeychainAttributeList* attribute_list() const {
    return attributes_and_data_->attribute_list;
  }

  SecKeychainAttributeList** attribute_list_ptr() const {
    return &attributes_and_data_->attribute_list;
  }

  UInt32 length() const {
    return attributes_and_data_->length;
  }

  UInt32* length_ptr() const {
    return &attributes_and_data_->length;
  }

  void* data() const {
    return attributes_and_data_->data;
  }

  void** data_ptr() const {
    return &attributes_and_data_->data;
  }

 private:
  scoped_ptr<CrSKeychainItemAttributesAndData> attributes_and_data_;
};

SecKeychainSearchRef CrSKeychainSearchCreateFromAttributes(
    CFTypeRef keychain_or_array,
    SecItemClass item_class,
    const SecKeychainAttributeList* attribute_list);

SecKeychainItemRef CrSKeychainSearchCopyNext(SecKeychainSearchRef search);

void CrSKeychainItemFreeAttributesAndData(
    SecKeychainAttributeList* attribute_list,
    void* data);

bool CrSKeychainItemTestAccess(SecKeychainItemRef item);

SecAccessRef CrSKeychainItemCopyAccess(SecKeychainItemRef item);

CFArrayRef CrSAccessCopyACLList(SecAccessRef access);

CrSACLSimpleContents* CrSACLCopySimpleContents(SecACLRef acl);

SecRequirementRef CrSTrustedApplicationCopyRequirement(
    SecTrustedApplicationRef application);

CFStringRef CrSRequirementCopyString(SecRequirementRef requirement,
                                     SecCSFlags flags);

SecTrustedApplicationRef CrSTrustedApplicationCreateFromPath(const char* path);

bool CrSACLSetSimpleContents(SecACLRef acl,
                             const CrSACLSimpleContents& acl_simple_contents);

SecKeychainRef CrSKeychainItemCopyKeychain(SecKeychainItemRef item);

SecKeychainAttributeInfo* CrSKeychainAttributeInfoForItemID(
    SecKeychainRef keychain,
    UInt32 item_id);

CrSKeychainItemAttributesAndData* CrSKeychainItemCopyAttributesAndData(
    SecKeychainRef keychain,
    SecKeychainItemRef item);

bool CrSKeychainItemDelete(SecKeychainItemRef item);

SecKeychainItemRef CrSKeychainItemCreateFromContent(
    const CrSKeychainItemAttributesAndData& attributes_and_data,
    SecKeychainRef keychain,
    SecAccessRef access);

}  

#endif  
