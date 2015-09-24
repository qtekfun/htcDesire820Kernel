// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_PROMPT_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_PROMPT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/extensions/crx_installer_error.h"
#include "extensions/common/url_pattern.h"
#include "google_apis/gaia/oauth2_mint_token_flow.h"
#include "google_apis/gaia/oauth2_token_service.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class ExtensionInstallUI;
class InfoBarDelegate;
class Profile;

namespace base {
class DictionaryValue;
class MessageLoop;
}  

namespace content {
class PageNavigator;
class WebContents;
}

namespace extensions {
class BundleInstaller;
class Extension;
class ExtensionWebstorePrivateApiTest;
class MockGetAuthTokenFunction;
class PermissionSet;
}  

class ExtensionInstallPrompt
    : public OAuth2MintTokenFlow::Delegate,
      public OAuth2TokenService::Consumer,
      public base::SupportsWeakPtr<ExtensionInstallPrompt> {
 public:
  enum PromptType {
    UNSET_PROMPT_TYPE = -1,
    INSTALL_PROMPT = 0,
    INLINE_INSTALL_PROMPT,
    BUNDLE_INSTALL_PROMPT,
    RE_ENABLE_PROMPT,
    PERMISSIONS_PROMPT,
    EXTERNAL_INSTALL_PROMPT,
    POST_INSTALL_PERMISSIONS_PROMPT,
    LAUNCH_PROMPT,
    NUM_PROMPT_TYPES
  };

  enum DetailsType {
    PERMISSIONS_DETAILS = 0,
    OAUTH_DETAILS,
    RETAINED_FILES_DETAILS,
  };

  
  
  
  
  class Prompt {
   public:
    explicit Prompt(PromptType type);
    ~Prompt();

    
    void SetPermissions(const std::vector<base::string16>& permissions);
    
    void SetPermissionsDetails(const std::vector<base::string16>& details);
    void SetIsShowingDetails(DetailsType type,
                             size_t index,
                             bool is_showing_details);
    void SetInlineInstallWebstoreData(const std::string& localized_user_count,
                                      bool show_user_count,
                                      double average_rating,
                                      int rating_count);
    void SetOAuthIssueAdvice(const IssueAdviceInfo& issue_advice);
    void SetUserNameFromProfile(Profile* profile);

    PromptType type() const { return type_; }
    void set_type(PromptType type) { type_ = type; }

    
    base::string16 GetDialogTitle() const;
    base::string16 GetHeading() const;
    int GetDialogButtons() const;
    bool HasAcceptButtonLabel() const;
    base::string16 GetAcceptButtonLabel() const;
    bool HasAbortButtonLabel() const;
    base::string16 GetAbortButtonLabel() const;
    base::string16 GetPermissionsHeading() const;
    base::string16 GetOAuthHeading() const;
    base::string16 GetRetainedFilesHeading() const;

    bool ShouldShowPermissions() const;

    
    

    
    
    
    
    typedef void(*StarAppender)(const gfx::ImageSkia*, void*);
    void AppendRatingStars(StarAppender appender, void* data) const;
    base::string16 GetRatingCount() const;
    base::string16 GetUserCount() const;
    size_t GetPermissionCount() const;
    size_t GetPermissionsDetailsCount() const;
    base::string16 GetPermission(size_t index) const;
    base::string16 GetPermissionsDetails(size_t index) const;
    bool GetIsShowingDetails(DetailsType type, size_t index) const;
    size_t GetOAuthIssueCount() const;
    const IssueAdviceInfoEntry& GetOAuthIssue(size_t index) const;
    size_t GetRetainedFileCount() const;
    base::string16 GetRetainedFile(size_t index) const;

    
    const extensions::BundleInstaller* bundle() const { return bundle_; }
    void set_bundle(const extensions::BundleInstaller* bundle) {
      bundle_ = bundle;
    }

    
    const extensions::Extension* extension() const { return extension_; }
    void set_extension(const extensions::Extension* extension) {
      extension_ = extension;
    }

    
    void set_retained_files(const std::vector<base::FilePath>& retained_files) {
      retained_files_ = retained_files;
    }

    const gfx::Image& icon() const { return icon_; }
    void set_icon(const gfx::Image& icon) { icon_ = icon; }

   private:
    bool ShouldDisplayRevokeFilesButton() const;

    PromptType type_;

    
    
    std::vector<base::string16> permissions_;
    std::vector<base::string16> details_;
    std::vector<bool> is_showing_details_for_permissions_;
    std::vector<bool> is_showing_details_for_oauth_;
    bool is_showing_details_for_retained_files_;

    
    
    IssueAdviceInfo oauth_issue_advice_;

    
    base::string16 oauth_user_name_;

    
    const extensions::Extension* extension_;
    const extensions::BundleInstaller* bundle_;

    
    gfx::Image icon_;

    
    
    
    std::string localized_user_count_;
    
    double average_rating_;
    int rating_count_;

    
    
    bool show_user_count_;

    std::vector<base::FilePath> retained_files_;
  };

  static const int kMinExtensionRating = 0;
  static const int kMaxExtensionRating = 5;

  class Delegate {
   public:
    
    virtual void InstallUIProceed() = 0;

    
    
    virtual void InstallUIAbort(bool user_initiated) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  struct ShowParams {
    explicit ShowParams(content::WebContents* contents);
    ShowParams(gfx::NativeWindow window, content::PageNavigator* navigator);

    
    content::WebContents* parent_web_contents;

    
    
    gfx::NativeWindow parent_window;
    content::PageNavigator* navigator;
  };

  typedef base::Callback<void(const ExtensionInstallPrompt::ShowParams&,
                              ExtensionInstallPrompt::Delegate*,
                              const ExtensionInstallPrompt::Prompt&)>
      ShowDialogCallback;

  
  
  static ShowDialogCallback GetDefaultShowDialogCallback();

  
  
  static scoped_refptr<extensions::Extension> GetLocalizedExtensionForDisplay(
      const base::DictionaryValue* manifest,
      int flags,  
      const std::string& id,
      const std::string& localized_name,
      const std::string& localized_description,
      std::string* error);

  
  explicit ExtensionInstallPrompt(content::WebContents* contents);

  
  ExtensionInstallPrompt(Profile* profile,
                         gfx::NativeWindow native_window,
                         content::PageNavigator* navigator);

  virtual ~ExtensionInstallPrompt();

  ExtensionInstallUI* install_ui() const { return install_ui_.get(); }

  bool record_oauth2_grant() const { return record_oauth2_grant_; }

  content::WebContents* parent_web_contents() const {
    return show_params_.parent_web_contents;
  }

  
  
  
  
  virtual void ConfirmBundleInstall(
      extensions::BundleInstaller* bundle,
      const extensions::PermissionSet* permissions);

  
  
  
  
  virtual void ConfirmStandaloneInstall(Delegate* delegate,
                                        const extensions::Extension* extension,
                                        SkBitmap* icon,
                                        const Prompt& prompt);

  
  
  
  
  
  virtual void ConfirmWebstoreInstall(
      Delegate* delegate,
      const extensions::Extension* extension,
      const SkBitmap* icon,
      const ShowDialogCallback& show_dialog_callback);

  
  
  
  
  
  virtual void ConfirmInstall(Delegate* delegate,
                              const extensions::Extension* extension,
                              const ShowDialogCallback& show_dialog_callback);

  
  
  
  
  virtual void ConfirmReEnable(Delegate* delegate,
                               const extensions::Extension* extension);

  
  
  
  
  virtual void ConfirmExternalInstall(
      Delegate* delegate,
      const extensions::Extension* extension,
      const ShowDialogCallback& show_dialog_callback);

  
  
  
  
  virtual void ConfirmPermissions(Delegate* delegate,
                                  const extensions::Extension* extension,
                                  const extensions::PermissionSet* permissions);

  
  
  
  
  virtual void ConfirmIssueAdvice(Delegate* delegate,
                                  const extensions::Extension* extension,
                                  const IssueAdviceInfo& issue_advice);

  
  
  
  
  virtual void ReviewPermissions(
      Delegate* delegate,
      const extensions::Extension* extension,
      const std::vector<base::FilePath>& retained_file_paths);

  
  virtual void OnInstallSuccess(const extensions::Extension* extension,
                                SkBitmap* icon);

  
  virtual void OnInstallFailure(const extensions::CrxInstallerError& error);

 protected:
  friend class extensions::ExtensionWebstorePrivateApiTest;
  friend class extensions::MockGetAuthTokenFunction;
  friend class WebstoreStartupInstallUnpackFailureTest;

  
  bool record_oauth2_grant_;

 private:
  friend class GalleryInstallApiTestObserver;

  
  
  void SetIcon(const SkBitmap* icon);

  
  void OnImageLoaded(const gfx::Image& image);

  
  
  
  void LoadImageIfNeeded();

  
  virtual void OnGetTokenSuccess(const OAuth2TokenService::Request* request,
                                 const std::string& access_token,
                                 const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(const OAuth2TokenService::Request* request,
                                 const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnIssueAdviceSuccess(
      const IssueAdviceInfo& issue_advice) OVERRIDE;
  virtual void OnMintTokenFailure(
      const GoogleServiceAuthError& error) OVERRIDE;

  
  void ShowConfirmation();

  base::MessageLoop* ui_loop_;

  
  SkBitmap icon_;

  
  
  const extensions::Extension* extension_;

  
  const extensions::BundleInstaller* bundle_;

  
  scoped_refptr<const extensions::PermissionSet> permissions_;

  
  scoped_ptr<ExtensionInstallUI> install_ui_;

  
  ShowParams show_params_;

  
  Delegate* delegate_;

  
  Prompt prompt_;

  scoped_ptr<OAuth2TokenService::Request> login_token_request_;
  scoped_ptr<OAuth2MintTokenFlow> token_flow_;

  
  ShowDialogCallback show_dialog_callback_;
};

#endif  
