// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/browsing_data/browsing_data_database_helper.h"
#include "chrome/browser/browsing_data/browsing_data_indexed_db_helper.h"
#include "chrome/browser/browsing_data/browsing_data_local_storage_helper.h"
#include "webkit/browser/appcache/appcache_service.h"

class CookieTreeNode;

namespace net {
class CanonicalCookie;
}

enum CocoaCookieDetailsType {
  
  kCocoaCookieDetailsTypeFolder = 0,

  
  
  kCocoaCookieDetailsTypeCookie,

  
  
  kCocoaCookieDetailsTypeTreeDatabase,

  
  
  kCocoaCookieDetailsTypeTreeLocalStorage,

  
  
  kCocoaCookieDetailsTypeTreeAppCache,

  
  
  kCocoaCookieDetailsTypeTreeIndexedDB,

  
  
  kCocoaCookieDetailsTypePromptDatabase,

  
  
  kCocoaCookieDetailsTypePromptLocalStorage,

  
  
  kCocoaCookieDetailsTypePromptAppCache
};

// If this system is rewritten to not use bindings, this class should be
@interface CocoaCookieDetails : NSObject {
 @private
  CocoaCookieDetailsType type_;

  
  
  BOOL canEditExpiration_;

  
  
  BOOL hasExpiration_;

  
  base::scoped_nsobject<NSString> content_;
  base::scoped_nsobject<NSString> path_;
  base::scoped_nsobject<NSString> sendFor_;
  
  base::scoped_nsobject<NSString> expires_;

  
  
  base::scoped_nsobject<NSString> created_;

  
  
  base::scoped_nsobject<NSString> name_;

  
  
  
  
  
  base::scoped_nsobject<NSString> fileSize_;

  
  
  
  base::scoped_nsobject<NSString> lastModified_;

  
  base::scoped_nsobject<NSString> lastAccessed_;

  
  
  
  
  base::scoped_nsobject<NSString> domain_;

  
  
  base::scoped_nsobject<NSString> databaseDescription_;

  
  base::scoped_nsobject<NSString> localStorageKey_;
  base::scoped_nsobject<NSString> localStorageValue_;

  
  
  base::scoped_nsobject<NSString> manifestURL_;
}

@property(nonatomic, readonly) BOOL canEditExpiration;
@property(nonatomic) BOOL hasExpiration;
@property(nonatomic, readonly) CocoaCookieDetailsType type;

- (BOOL)shouldHideCookieDetailsView;
- (BOOL)shouldShowLocalStorageTreeDetailsView;
- (BOOL)shouldShowLocalStoragePromptDetailsView;
- (BOOL)shouldShowDatabaseTreeDetailsView;
- (BOOL)shouldShowDatabasePromptDetailsView;
- (BOOL)shouldShowAppCachePromptDetailsView;
- (BOOL)shouldShowAppCacheTreeDetailsView;
- (BOOL)shouldShowIndexedDBTreeDetailsView;

- (NSString*)name;
- (NSString*)content;
- (NSString*)domain;
- (NSString*)path;
- (NSString*)sendFor;
- (NSString*)created;
- (NSString*)expires;
- (NSString*)fileSize;
- (NSString*)lastModified;
- (NSString*)lastAccessed;
- (NSString*)databaseDescription;
- (NSString*)localStorageKey;
- (NSString*)localStorageValue;
- (NSString*)manifestURL;

- (id)initAsFolder;

- (id)initWithCookie:(const net::CanonicalCookie*)treeNode
   canEditExpiration:(BOOL)canEditExpiration;

- (id)initWithDatabase:
    (const BrowsingDataDatabaseHelper::DatabaseInfo*)databaseInfo;

- (id)initWithLocalStorage:
    (const BrowsingDataLocalStorageHelper::LocalStorageInfo*)localStorageInfo;

- (id)initWithDatabase:(const std::string&)domain
          databaseName:(const base::string16&)databaseName
   databaseDescription:(const base::string16&)databaseDescription
              fileSize:(unsigned long)fileSize;

- (id)initWithAppCacheInfo:(const appcache::AppCacheInfo*)appcacheInfo;

- (id)initWithLocalStorage:(const std::string&)domain
                       key:(const base::string16&)key
                     value:(const base::string16&)value;

- (id)initWithAppCacheManifestURL:(const std::string&)manifestURL;

- (id)initWithIndexedDBInfo:
    (const content::IndexedDBInfo*)indexedDB;

+ (CocoaCookieDetails*)createFromCookieTreeNode:(CookieTreeNode*)treeNode;

@end

@interface CookiePromptContentDetailsAdapter : NSObject {
 @private
  base::scoped_nsobject<CocoaCookieDetails> details_;
}

- (CocoaCookieDetails*)details;

- (id)initWithDetails:(CocoaCookieDetails*)details;
@end
