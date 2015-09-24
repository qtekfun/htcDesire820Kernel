// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OPTIONS_VPN_CONFIG_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_OPTIONS_VPN_CONFIG_VIEW_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/chromeos/options/cert_library.h"
#include "chrome/browser/chromeos/options/network_config_view.h"
#include "chrome/browser/chromeos/options/network_property_ui_data.h"
#include "chrome/browser/chromeos/options/passphrase_textfield.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/view.h"

namespace base {
class DictionaryValue;
}

namespace views {
class Checkbox;
class GridLayout;
class Label;
}

namespace chromeos {

class NetworkState;

namespace internal {
class ProviderTypeComboboxModel;
class VpnServerCACertComboboxModel;
class VpnUserCertComboboxModel;
}

class VPNConfigView : public ChildNetworkConfigView,
                      public views::TextfieldController,
                      public views::ButtonListener,
                      public views::ComboboxListener,
                      public CertLibrary::Observer {
 public:
  VPNConfigView(NetworkConfigView* parent, const std::string& service_path);
  virtual ~VPNConfigView();

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const base::string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnSelectedIndexChanged(views::Combobox* combobox) OVERRIDE;

  
  virtual void OnCertificatesLoaded(bool initial_load) OVERRIDE;

  
  virtual base::string16 GetTitle() const OVERRIDE;
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual bool CanLogin() OVERRIDE;
  virtual bool Login() OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual void InitFocus() OVERRIDE;

 private:
  
  void Init();

  
  void InitFromProperties(const std::string& service_path,
                          const base::DictionaryValue& dictionary);
  void ParseUIProperties(const NetworkState* vpn);
  void GetPropertiesError(const std::string& error_name,
                          scoped_ptr<base::DictionaryValue> error_data);

  
  void SetConfigProperties(base::DictionaryValue* properties);

  
  void Refresh();

  
  void UpdateControlsToEnable();
  void UpdateControls();
  void UpdateErrorLabel();

  
  void UpdateCanLogin();

  
  bool HaveUserCerts() const;

  
  bool IsUserCertValid() const;

  
  const std::string GetTextFromField(views::Textfield* textfield,
                                     bool trim_whitespace) const;

  
  const std::string GetPassphraseFromField(
      PassphraseTextfield* textfield) const;

  
  const std::string GetService() const;
  const std::string GetServer() const;
  const std::string GetPSKPassphrase() const;
  const std::string GetUsername() const;
  const std::string GetUserPassphrase() const;
  const std::string GetOTP() const;
  const std::string GetGroupName() const;
  const std::string GetServerCACertPEM() const;
  const std::string GetUserCertID() const;
  bool GetSaveCredentials() const;
  int GetProviderTypeIndex() const;
  std::string GetProviderTypeString() const;

  
  
  void ParseVPNUIProperty(const NetworkState* network,
                          const std::string& dict_key,
                          const std::string& key,
                          NetworkPropertyUIData* property_ui_data);

  base::string16 service_name_from_server_;
  bool service_text_modified_;

  

  bool enable_psk_passphrase_;
  bool enable_user_cert_;
  bool enable_server_ca_cert_;
  bool enable_otp_;
  bool enable_group_name_;

  NetworkPropertyUIData ca_cert_ui_data_;
  NetworkPropertyUIData psk_passphrase_ui_data_;
  NetworkPropertyUIData user_cert_ui_data_;
  NetworkPropertyUIData username_ui_data_;
  NetworkPropertyUIData user_passphrase_ui_data_;
  NetworkPropertyUIData group_name_ui_data_;
  NetworkPropertyUIData save_credentials_ui_data_;

  int title_;

  views::GridLayout* layout_;
  views::Textfield* server_textfield_;
  views::Label* service_text_;
  views::Textfield* service_textfield_;
  scoped_ptr<internal::ProviderTypeComboboxModel> provider_type_combobox_model_;
  views::Combobox* provider_type_combobox_;
  views::Label* provider_type_text_label_;
  views::Label* psk_passphrase_label_;
  PassphraseTextfield* psk_passphrase_textfield_;
  views::Label* user_cert_label_;
  scoped_ptr<internal::VpnUserCertComboboxModel> user_cert_combobox_model_;
  views::Combobox* user_cert_combobox_;
  views::Label* server_ca_cert_label_;
  scoped_ptr<internal::VpnServerCACertComboboxModel>
      server_ca_cert_combobox_model_;
  views::Combobox* server_ca_cert_combobox_;
  views::Textfield* username_textfield_;
  PassphraseTextfield* user_passphrase_textfield_;
  views::Label* otp_label_;
  views::Textfield* otp_textfield_;
  views::Label* group_name_label_;
  views::Textfield* group_name_textfield_;
  views::Checkbox* save_credentials_checkbox_;
  views::Label* error_label_;

  
  int provider_type_index_;
  std::string ca_cert_pem_;
  std::string client_cert_id_;

  base::WeakPtrFactory<VPNConfigView> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(VPNConfigView);
};

}  

#endif  
