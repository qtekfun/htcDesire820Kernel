// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/browsing_data_database_helper.h"
#include "chrome/browser/browsing_data_indexed_db_helper.h"
#include "chrome/browser/browsing_data_local_storage_helper.h"
#include "net/base/cookie_monster.h"
#include "webkit/appcache/appcache_service.h"

class CookieTreeNode;
class CookiePromptModalDialog;

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

  
  scoped_nsobject<NSString> content_;
  scoped_nsobject<NSString> path_;
  scoped_nsobject<NSString> sendFor_;
  
  scoped_nsobject<NSString> expires_;

  
  
  scoped_nsobject<NSString> created_;

  
  
  scoped_nsobject<NSString> name_;

  
  
  
  
  
  scoped_nsobject<NSString> fileSize_;

  
  
  
  scoped_nsobject<NSString> lastModified_;

  
  scoped_nsobject<NSString> lastAccessed_;

  
  
  
  
  scoped_nsobject<NSString> domain_;

  
  
  scoped_nsobject<NSString> databaseDescription_;

  
  scoped_nsobject<NSString> localStorageKey_;
  scoped_nsobject<NSString> localStorageValue_;

  
  
  scoped_nsobject<NSString> manifestURL_;
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

- (id)initWithCookie:(const net::CookieMonster::CanonicalCookie*)treeNode
              origin:(NSString*)origin
   canEditExpiration:(BOOL)canEditExpiration;

- (id)initWithDatabase:
    (const BrowsingDataDatabaseHelper::DatabaseInfo*)databaseInfo;

- (id)initWithLocalStorage:
    (const BrowsingDataLocalStorageHelper::LocalStorageInfo*)localStorageInfo;

- (id)initWithDatabase:(const std::string&)domain
          databaseName:(const string16&)databaseName
   databaseDescription:(const string16&)databaseDescription
              fileSize:(unsigned long)fileSize;

- (id)initWithAppCacheInfo:(const appcache::AppCacheInfo*)appcacheInfo;

- (id)initWithLocalStorage:(const std::string&)domain
                       key:(const string16&)key
                     value:(const string16&)value;

- (id)initWithAppCacheManifestURL:(const std::string&)manifestURL;

- (id)initWithIndexedDBInfo:
    (const BrowsingDataIndexedDBHelper::IndexedDBInfo*)indexedDB;

+ (CocoaCookieDetails*)createFromCookieTreeNode:(CookieTreeNode*)treeNode;

@end

@interface CookiePromptContentDetailsAdapter : NSObject {
 @private
  scoped_nsobject<CocoaCookieDetails> details_;
}

- (CocoaCookieDetails*)details;

- (id)initWithDetails:(CocoaCookieDetails*)details;
@end

