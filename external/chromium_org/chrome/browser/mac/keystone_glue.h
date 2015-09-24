// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MAC_KEYSTONE_GLUE_H_
#define CHROME_BROWSER_MAC_KEYSTONE_GLUE_H_

#include "base/strings/string16.h"

#if defined(__OBJC__)

#import <Foundation/Foundation.h>

#include "base/mac/scoped_authorizationref.h"
#import "base/mac/scoped_nsobject.h"

enum AutoupdateStatus {
  kAutoupdateNone = 0,        
  kAutoupdateRegistering,     
  kAutoupdateRegistered,      
  kAutoupdateChecking,        
  kAutoupdateCurrent,         
  kAutoupdateAvailable,       
  kAutoupdateInstalling,      
  kAutoupdateInstalled,       
  kAutoupdatePromoting,       
  kAutoupdatePromoted,        
  kAutoupdateRegisterFailed,  
  kAutoupdateCheckFailed,     
  kAutoupdateInstallFailed,   
  kAutoupdatePromoteFailed,   
  kAutoupdateNeedsPromotion,  
};

extern NSString* const kAutoupdateStatusNotification;
extern NSString* const kAutoupdateStatusStatus;
extern NSString* const kAutoupdateStatusVersion;

namespace {

enum BrandFileType {
  kBrandFileTypeNotDetermined = 0,
  kBrandFileTypeNone,
  kBrandFileTypeUser,
  kBrandFileTypeSystem,
};

} 


@class KSRegistration;

@interface KeystoneGlue : NSObject {
 @protected

  
  NSString* productID_;
  NSString* appPath_;
  NSString* url_;
  NSString* version_;
  NSString* channel_;  
  BrandFileType brandFileType_;

  
  KSRegistration* registration_;  
  NSTimer* timer_;  

  
  base::scoped_nsobject<NSNotification> recentNotification_;

  
  
  base::mac::ScopedAuthorizationRef authorization_;

  
  
  BOOL synchronousPromotion_;

  
  BOOL updateSuccessfullyInstalled_;
}

+ (id)defaultKeystoneGlue;

- (void)registerWithKeystone;

- (void)checkForUpdate;
- (void)installUpdate;

- (NSNotification*)recentNotification;

- (AutoupdateStatus)recentStatus;

- (BOOL)asyncOperationPending;

- (BOOL)isOnReadOnlyFilesystem;

- (BOOL)needsPromotion;
- (BOOL)wantsPromotion;

- (void)promoteTicketWithAuthorization:(AuthorizationRef)authorization_arg
                           synchronous:(BOOL)synchronous;

- (void)promoteTicket;

- (void)setAppPath:(NSString*)appPath;

@end  

@interface KeystoneGlue(ExposedForTesting)

- (void)loadParameters;

- (BOOL)loadKeystoneRegistration;

- (void)stopTimer;

- (void)checkForUpdateComplete:(NSNotification*)notification;

- (void)installUpdateComplete:(NSNotification*)notification;

@end  

#endif  

namespace keystone_glue {

std::string BrandCode();

bool KeystoneEnabled();

base::string16 CurrentlyInstalledVersion();

}  

#endif  
