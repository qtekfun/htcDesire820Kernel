// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_NETWORK_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_NETWORK_LIBRARY_H_
#pragma once

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/string16.h"
#include "base/timer.h"
#include "third_party/cros/chromeos_network.h"

class DictionaryValue;
class Value;

namespace chromeos {

enum ConnectionType {
  TYPE_UNKNOWN   = 0,
  TYPE_ETHERNET  = 1,
  TYPE_WIFI      = 2,
  TYPE_WIMAX     = 3,
  TYPE_BLUETOOTH = 4,
  TYPE_CELLULAR  = 5,
  TYPE_VPN       = 6,
};

enum ConnectionMode {
  MODE_UNKNOWN = 0,
  MODE_MANAGED = 1,
  MODE_ADHOC   = 2,
};

enum ConnectionSecurity {
  SECURITY_UNKNOWN = 0,
  SECURITY_NONE    = 1,
  SECURITY_WEP     = 2,
  SECURITY_WPA     = 3,
  SECURITY_RSN     = 4,
  SECURITY_8021X   = 5,
  SECURITY_PSK     = 6,
};

enum ConnectionState {
  STATE_UNKNOWN            = 0,
  STATE_IDLE               = 1,
  STATE_CARRIER            = 2,
  STATE_ASSOCIATION        = 3,
  STATE_CONFIGURATION      = 4,
  STATE_READY              = 5,
  STATE_DISCONNECT         = 6,
  STATE_FAILURE            = 7,
  STATE_ACTIVATION_FAILURE = 8
};

enum ConnectivityState {
  CONN_STATE_UNKNOWN      = 0,
  CONN_STATE_UNRESTRICTED = 1,
  CONN_STATE_RESTRICTED   = 2,
  CONN_STATE_NONE         = 3
};

enum NetworkTechnology {
  NETWORK_TECHNOLOGY_UNKNOWN      = 0,
  NETWORK_TECHNOLOGY_1XRTT        = 1,
  NETWORK_TECHNOLOGY_EVDO         = 2,
  NETWORK_TECHNOLOGY_GPRS         = 3,
  NETWORK_TECHNOLOGY_EDGE         = 4,
  NETWORK_TECHNOLOGY_UMTS         = 5,
  NETWORK_TECHNOLOGY_HSPA         = 6,
  NETWORK_TECHNOLOGY_HSPA_PLUS    = 7,
  NETWORK_TECHNOLOGY_LTE          = 8,
  NETWORK_TECHNOLOGY_LTE_ADVANCED = 9,
};

enum ActivationState {
  ACTIVATION_STATE_UNKNOWN             = 0,
  ACTIVATION_STATE_ACTIVATED           = 1,
  ACTIVATION_STATE_ACTIVATING          = 2,
  ACTIVATION_STATE_NOT_ACTIVATED       = 3,
  ACTIVATION_STATE_PARTIALLY_ACTIVATED = 4,
};

enum NetworkRoamingState {
  ROAMING_STATE_UNKNOWN = 0,
  ROAMING_STATE_HOME    = 1,
  ROAMING_STATE_ROAMING = 2,
};

enum SIMLockState {
  SIM_UNKNOWN    = 0,
  SIM_UNLOCKED   = 1,
  SIM_LOCKED_PIN = 2,
  SIM_LOCKED_PUK = 3,  
};

enum SIMPinRequire {
  SIM_PIN_REQUIRE_UNKNOWN = 0,
  SIM_PIN_NOT_REQUIRED    = 1,
  SIM_PIN_REQUIRED        = 2,
};

enum PinOperationError {
  PIN_ERROR_NONE           = 0,
  PIN_ERROR_UNKNOWN        = 1,
  PIN_ERROR_INCORRECT_CODE = 2,  
  PIN_ERROR_BLOCKED        = 3,  
};

enum ConnectionError {
  ERROR_NO_ERROR          = 0,
  ERROR_OUT_OF_RANGE      = 1,
  ERROR_PIN_MISSING       = 2,
  ERROR_DHCP_FAILED       = 3,
  ERROR_CONNECT_FAILED    = 4,
  ERROR_BAD_PASSPHRASE    = 5,
  ERROR_BAD_WEPKEY        = 6,
  ERROR_ACTIVATION_FAILED = 7,
  ERROR_NEED_EVDO         = 8,
  ERROR_NEED_HOME_NETWORK = 9,
  ERROR_OTASP_FAILED      = 10,
  ERROR_AAA_FAILED        = 11,
};

enum EAPMethod {
  EAP_METHOD_UNKNOWN = 0,
  EAP_METHOD_PEAP    = 1,
  EAP_METHOD_TLS     = 2,
  EAP_METHOD_TTLS    = 3,
  EAP_METHOD_LEAP    = 4
};

enum EAPPhase2Auth {
  EAP_PHASE_2_AUTH_AUTO     = 0,
  EAP_PHASE_2_AUTH_MD5      = 1,
  EAP_PHASE_2_AUTH_MSCHAPV2 = 2,
  EAP_PHASE_2_AUTH_MSCHAP   = 3,
  EAP_PHASE_2_AUTH_PAP      = 4,
  EAP_PHASE_2_AUTH_CHAP     = 5
};

struct FoundCellularNetwork {
  std::string status;
  std::string network_id;
  std::string short_name;
  std::string long_name;
  std::string technology;
};

typedef std::vector<FoundCellularNetwork> CellularNetworkList;

static const int kCellularDataLowSecs = 60 * 60;

static const int kCellularDataVeryLowSecs = 30 * 60;

static const int kCellularDataLowBytes = 100 * 1024 * 1024;

static const int kCellularDataVeryLowBytes = 50 * 1024 * 1024;

class NetworkDevice {
 public:
  explicit NetworkDevice(const std::string& device_path);

  
  const std::string& device_path() const { return device_path_; }
  const std::string& name() const { return name_; }
  ConnectionType type() const { return type_; }
  bool scanning() const { return scanning_; }
  const std::string& meid() const { return MEID_; }
  const std::string& imei() const { return IMEI_; }
  const std::string& imsi() const { return IMSI_; }
  const std::string& esn() const { return ESN_; }
  const std::string& mdn() const { return MDN_; }
  const std::string& min() const { return MIN_; }
  const std::string& model_id() const { return model_id_; }
  const std::string& manufacturer() const { return manufacturer_; }
  SIMLockState sim_lock_state() const { return sim_lock_state_; }
  bool is_sim_locked() const {
    return sim_lock_state_ == SIM_LOCKED_PIN ||
        sim_lock_state_ == SIM_LOCKED_PUK;
  }
  const int sim_retries_left() const { return sim_retries_left_; }
  SIMPinRequire sim_pin_required() const { return sim_pin_required_; }
  const std::string& firmware_revision() const { return firmware_revision_; }
  const std::string& hardware_revision() const { return hardware_revision_; }
  const unsigned int prl_version() const { return PRL_version_; }
  const std::string& home_provider() const { return home_provider_; }
  const std::string& home_provider_code() const { return home_provider_code_; }
  const std::string& home_provider_country() const {
    return home_provider_country_;
  }
  const std::string& home_provider_id() const { return home_provider_id_; }
  const std::string& home_provider_name() const { return home_provider_name_; }
  const std::string& selected_cellular_network() const {
    return selected_cellular_network_;
  }
  const CellularNetworkList& found_cellular_networks() const {
    return found_cellular_networks_;
  }
  bool data_roaming_allowed() const { return data_roaming_allowed_; }
  bool support_network_scan() const { return support_network_scan_; }

 private:
  bool ParseValue(int index, const Value* value);
  void ParseInfo(const DictionaryValue* info);

  
  std::string device_path_;
  std::string name_;
  ConnectionType type_;
  bool scanning_;
  
  std::string carrier_;
  std::string home_provider_;
  std::string home_provider_code_;
  std::string home_provider_country_;
  std::string home_provider_id_;
  std::string home_provider_name_;
  std::string MEID_;
  std::string IMEI_;
  std::string IMSI_;
  std::string ESN_;
  std::string MDN_;
  std::string MIN_;
  std::string model_id_;
  std::string manufacturer_;
  SIMLockState sim_lock_state_;
  int sim_retries_left_;
  SIMPinRequire sim_pin_required_;
  std::string firmware_revision_;
  std::string hardware_revision_;
  int PRL_version_;
  std::string selected_cellular_network_;
  CellularNetworkList found_cellular_networks_;
  bool data_roaming_allowed_;
  bool support_network_scan_;

  friend class NetworkLibraryImpl;
  DISALLOW_COPY_AND_ASSIGN(NetworkDevice);
};

class Network {
 public:
  virtual ~Network() {}

  const std::string& service_path() const { return service_path_; }
  const std::string& name() const { return name_; }
  const std::string& device_path() const { return device_path_; }
  const std::string& ip_address() const { return ip_address_; }
  ConnectionType type() const { return type_; }
  ConnectionMode mode() const { return mode_; }
  ConnectionState connection_state() const { return state_; }
  bool connecting() const { return IsConnectingState(state_); }
  bool configuring() const { return state_ == STATE_CONFIGURATION; }
  bool connected() const { return state_ == STATE_READY; }
  bool connecting_or_connected() const { return connecting() || connected(); }
  bool failed() const { return state_ == STATE_FAILURE; }
  bool failed_or_disconnected() const {
    return failed() || state_ == STATE_IDLE;
  }
  ConnectionError error() const { return error_; }
  ConnectionState state() const { return state_; }
  
  
  bool connectable() const { return connectable_; }
  
  
  
  bool is_active() const { return is_active_; }
  bool favorite() const { return favorite_; }
  bool auto_connect() const { return auto_connect_; }
  ConnectivityState connectivity_state() const { return connectivity_state_; }
  bool added() const { return added_; }

  const std::string& unique_id() const { return unique_id_; }

  
  
  
  void SetAutoConnect(bool auto_connect);

  
  void SetName(const std::string& name);

  
  std::string GetStateString() const;

  
  std::string GetErrorString() const;

  static bool IsConnectingState(ConnectionState state) {
    return (state == STATE_ASSOCIATION ||
            state == STATE_CONFIGURATION ||
            state == STATE_CARRIER);
  }

 protected:
  Network(const std::string& service_path, ConnectionType type)
      : state_(STATE_UNKNOWN),
        error_(ERROR_NO_ERROR),
        connectable_(true),
        is_active_(false),
        favorite_(false),
        auto_connect_(false),
        connectivity_state_(CONN_STATE_UNKNOWN),
        priority_order_(0),
        added_(false),
        service_path_(service_path),
        type_(type) {}

  
  virtual bool ParseValue(int index, const Value* value);
  virtual void ParseInfo(const DictionaryValue* info);

  
  virtual void SetStringProperty(const char* prop, const std::string& str,
                                 std::string* dest);
  virtual void SetBooleanProperty(const char* prop, bool b, bool* dest);
  virtual void SetIntegerProperty(const char* prop, int i, int* dest);
  virtual void SetValueProperty(const char* prop, Value* val);
  virtual void ClearProperty(const char* prop);

  
  virtual void SetOrClearStringProperty(const char* prop,
                                        const std::string& str,
                                        std::string* dest);

  std::string device_path_;
  std::string name_;
  std::string ip_address_;
  ConnectionMode mode_;
  ConnectionState state_;
  ConnectionError error_;
  bool connectable_;
  bool is_active_;
  bool favorite_;
  bool auto_connect_;
  ConnectivityState connectivity_state_;

  
  std::string unique_id_;

 private:
  void set_name(const std::string& name) { name_ = name; }
  void set_connecting(bool connecting) {
    state_ = (connecting ? STATE_ASSOCIATION : STATE_IDLE);
  }
  void set_connected(bool connected) {
    state_ = (connected ? STATE_READY : STATE_IDLE);
  }
  void set_state(ConnectionState state) { state_ = state; }
  void set_connectable(bool connectable) { connectable_ = connectable; }
  void set_active(bool is_active) { is_active_ = is_active; }
  void set_error(ConnectionError error) { error_ = error; }
  void set_connectivity_state(ConnectivityState connectivity_state) {
    connectivity_state_ = connectivity_state;
  }
  void set_added(bool added) { added_ = added; }

  
  void InitIPAddress();

  
  int priority_order_;

  
  bool added_;

  
  std::string service_path_;
  ConnectionType type_;

  friend class NetworkLibraryImpl;
  friend class NetworkLibraryStubImpl;
  DISALLOW_COPY_AND_ASSIGN(Network);
  
  FRIEND_TEST_ALL_PREFIXES(WifiConfigViewTest, ChangeAutoConnectSaveTest);
};

class EthernetNetwork : public Network {
 public:
  explicit EthernetNetwork(const std::string& service_path) :
      Network(service_path, TYPE_ETHERNET) {
  }
 private:
  friend class NetworkLibraryImpl;
  DISALLOW_COPY_AND_ASSIGN(EthernetNetwork);
};

class VirtualNetwork : public Network {
 public:
  enum ProviderType {
    PROVIDER_TYPE_L2TP_IPSEC_PSK,
    PROVIDER_TYPE_L2TP_IPSEC_USER_CERT,
    PROVIDER_TYPE_OPEN_VPN,
    
    PROVIDER_TYPE_MAX,
  };

  explicit VirtualNetwork(const std::string& service_path) :
      Network(service_path, TYPE_VPN),
      provider_type_(PROVIDER_TYPE_L2TP_IPSEC_PSK) {
  }

  const std::string& server_hostname() const { return server_hostname_; }
  ProviderType provider_type() const { return provider_type_; }
  const std::string& ca_cert() const { return ca_cert_; }
  const std::string& psk_passphrase() const { return psk_passphrase_; }
  const std::string& user_cert() const { return user_cert_; }
  const std::string& user_cert_key() const { return user_cert_key_; }
  const std::string& username() const { return username_; }
  const std::string& user_passphrase() const { return user_passphrase_; }

  bool NeedMoreInfoToConnect() const;

  
  void SetCACert(const std::string& ca_cert);
  void SetPSKPassphrase(const std::string& psk_passphrase);
  void SetUserCert(const std::string& user_cert);
  void SetUserCertKey(const std::string& key);
  void SetUsername(const std::string& username);
  void SetUserPassphrase(const std::string& user_passphrase);

  std::string GetProviderTypeString() const;

 private:
  
  virtual bool ParseValue(int index, const Value* value);
  virtual void ParseInfo(const DictionaryValue* info);

  
  bool ParseProviderValue(int index, const Value* value);

  void set_server_hostname(const std::string& server_hostname) {
    server_hostname_ = server_hostname;
  }
  void set_provider_type(ProviderType provider_type) {
    provider_type_ = provider_type;
  }
  void set_ca_cert(const std::string& ca_cert) {
    ca_cert_ = ca_cert;
  }
  void set_psk_passphrase(const std::string& psk_passphrase) {
    psk_passphrase_ = psk_passphrase;
  }
  void set_user_cert(const std::string& user_cert) {
    user_cert_ = user_cert;
  }
  void set_user_cert_key(const std::string& key) {
    user_cert_key_ = key;
  }
  void set_username(const std::string& username) {
    username_ = username;
  }
  void set_user_passphrase(const std::string& user_passphrase) {
    user_passphrase_ = user_passphrase;
  }

  std::string server_hostname_;
  ProviderType provider_type_;
  std::string ca_cert_;
  std::string psk_passphrase_;
  std::string user_cert_;
  std::string user_cert_key_;
  std::string username_;
  std::string user_passphrase_;

  friend class NetworkLibraryImpl;
  DISALLOW_COPY_AND_ASSIGN(VirtualNetwork);
};
typedef std::vector<VirtualNetwork*> VirtualNetworkVector;

class WirelessNetwork : public Network {
 public:
  int strength() const { return strength_; }

 protected:
  WirelessNetwork(const std::string& service_path, ConnectionType type)
      : Network(service_path, type),
        strength_(0) {}
  int strength_;

  
  virtual bool ParseValue(int index, const Value* value);

 private:
  void set_strength(int strength) { strength_ = strength; }

  friend class NetworkLibraryImpl;
  friend class NetworkLibraryStubImpl;
  DISALLOW_COPY_AND_ASSIGN(WirelessNetwork);
};

class CellularDataPlan;

class CellularNetwork : public WirelessNetwork {
 public:
  enum DataLeft {
    DATA_UNKNOWN,
    DATA_NORMAL,
    DATA_LOW,
    DATA_VERY_LOW,
    DATA_NONE
  };

  struct Apn {
    std::string apn;
    std::string network_id;
    std::string username;
    std::string password;

    Apn() {}
    Apn(const std::string& apn, const std::string& network_id,
        const std::string& username, const std::string& password)
        : apn(apn), network_id(network_id),
          username(username), password(password) {
    }
    void Set(const DictionaryValue& dict);
  };

  virtual ~CellularNetwork();

  explicit CellularNetwork(const std::string& service_path)
      : WirelessNetwork(service_path, TYPE_CELLULAR),
        activation_state_(ACTIVATION_STATE_UNKNOWN),
        network_technology_(NETWORK_TECHNOLOGY_UNKNOWN),
        roaming_state_(ROAMING_STATE_UNKNOWN),
        data_left_(DATA_UNKNOWN) {
  }
  
  
  bool StartActivation() const;
  
  
  
  void RefreshDataPlansIfNeeded() const;

  const ActivationState activation_state() const { return activation_state_; }
  bool activated() const {
    return activation_state() == ACTIVATION_STATE_ACTIVATED;
  }
  const NetworkTechnology network_technology() const {
    return network_technology_;
  }
  const NetworkRoamingState roaming_state() const { return roaming_state_; }
  bool restricted_pool() const {
    return connectivity_state() == CONN_STATE_RESTRICTED;
  }
  bool needs_new_plan() const {
    return restricted_pool() && connected() && activated();
  }
  const std::string& operator_name() const { return operator_name_; }
  const std::string& operator_code() const { return operator_code_; }
  const std::string& operator_country() const { return operator_country_; }
  const std::string& payment_url() const { return payment_url_; }
  const std::string& usage_url() const { return usage_url_; }
  DataLeft data_left() const { return data_left_; }
  const Apn& apn() const { return apn_; }
  const Apn& last_good_apn() const { return last_good_apn_; }
  void SetApn(const Apn& apn);
  bool SupportsDataPlan() const;

  
  bool is_gsm() const {
    return network_technology_ != NETWORK_TECHNOLOGY_EVDO &&
        network_technology_ != NETWORK_TECHNOLOGY_1XRTT &&
        network_technology_ != NETWORK_TECHNOLOGY_UNKNOWN;
  }

  
  std::string GetNetworkTechnologyString() const;
  
  std::string GetConnectivityStateString() const;
  
  std::string GetActivationStateString() const;
  
  std::string GetRoamingStateString() const;

  
  static std::string ActivationStateToString(ActivationState activation_state);

 protected:
  
  virtual bool ParseValue(int index, const Value* value);

  ActivationState activation_state_;
  NetworkTechnology network_technology_;
  NetworkRoamingState roaming_state_;
  
  std::string operator_name_;
  std::string operator_code_;
  std::string operator_country_;
  std::string payment_url_;
  std::string usage_url_;
  
  DataLeft data_left_;  
  Apn apn_;
  Apn last_good_apn_;

 private:
  void set_activation_state(ActivationState state) {
    activation_state_ = state;
  }
  void set_payment_url(const std::string& url) { payment_url_ = url; }
  void set_usage_url(const std::string& url) { usage_url_ = url; }
  void set_network_technology(NetworkTechnology technology) {
    network_technology_ = technology;
  }
  void set_roaming_state(NetworkRoamingState state) { roaming_state_ = state; }
  void set_data_left(DataLeft data_left) { data_left_ = data_left; }
  void set_apn(const Apn& apn) { apn_ = apn; }
  void set_last_good_apn(const Apn& apn) { last_good_apn_ = apn; }

  friend class NetworkLibraryImpl;
  friend class NetworkLibraryStubImpl;
  DISALLOW_COPY_AND_ASSIGN(CellularNetwork);
};
typedef std::vector<CellularNetwork*> CellularNetworkVector;

class WifiNetwork : public WirelessNetwork {
 public:
  explicit WifiNetwork(const std::string& service_path)
      : WirelessNetwork(service_path, TYPE_WIFI),
        encryption_(SECURITY_NONE),
        passphrase_required_(false),
        eap_method_(EAP_METHOD_UNKNOWN),
        eap_phase_2_auth_(EAP_PHASE_2_AUTH_AUTO),
        eap_use_system_cas_(true),
        save_credentials_(false) {
  }

  bool encrypted() const { return encryption_ != SECURITY_NONE; }
  ConnectionSecurity encryption() const { return encryption_; }
  const std::string& passphrase() const { return passphrase_; }
  const std::string& identity() const { return identity_; }
  const std::string& cert_path() const { return cert_path_; }
  bool passphrase_required() const { return passphrase_required_; }

  EAPMethod eap_method() const { return eap_method_; }
  EAPPhase2Auth eap_phase_2_auth() const { return eap_phase_2_auth_; }
  const std::string& eap_server_ca_cert_nss_nickname() const {
    return eap_server_ca_cert_nss_nickname_; }
  const std::string& eap_client_cert_pkcs11_id() const {
    return eap_client_cert_pkcs11_id_; }
  const bool eap_use_system_cas() const { return eap_use_system_cas_; }
  const std::string& eap_identity() const { return eap_identity_; }
  const std::string& eap_anonymous_identity() const {
    return eap_anonymous_identity_; }
  const std::string& eap_passphrase() const { return eap_passphrase_; }
  bool save_credentials() const { return save_credentials_; }

  const std::string& GetPassphrase() const;

  bool SetSsid(const std::string& ssid);
  bool SetHexSsid(const std::string& ssid_hex);
  void SetPassphrase(const std::string& passphrase);
  void SetIdentity(const std::string& identity);
  void SetCertPath(const std::string& cert_path);

  
  void SetEAPMethod(EAPMethod method);
  void SetEAPPhase2Auth(EAPPhase2Auth auth);
  void SetEAPServerCaCertNssNickname(const std::string& nss_nickname);
  void SetEAPClientCertPkcs11Id(const std::string& pkcs11_id);
  void SetEAPUseSystemCAs(bool use_system_cas);
  void SetEAPIdentity(const std::string& identity);
  void SetEAPAnonymousIdentity(const std::string& identity);
  void SetEAPPassphrase(const std::string& passphrase);
  void SetSaveCredentials(bool save_credentials);

  
  void EraseCredentials();

  
  
  std::string GetEncryptionString() const;

  
  bool IsPassphraseRequired() const;

  
  bool IsCertificateLoaded() const;

 private:
  
  virtual bool ParseValue(int index, const Value* value);
  virtual void ParseInfo(const DictionaryValue* info);

  void CalculateUniqueId();

  void set_encryption(ConnectionSecurity encryption) {
    encryption_ = encryption;
  }
  void set_passphrase(const std::string& passphrase) {
    passphrase_ = passphrase;
  }
  void set_passphrase_required(bool passphrase_required) {
    passphrase_required_ = passphrase_required;
  }
  void set_identity(const std::string& identity) {
    identity_ = identity;
  }
  void set_cert_path(const std::string& cert_path) {
    cert_path_ = cert_path;
  }

  ConnectionSecurity encryption_;
  std::string passphrase_;
  bool passphrase_required_;
  std::string identity_;
  std::string cert_path_;

  EAPMethod eap_method_;
  EAPPhase2Auth eap_phase_2_auth_;
  std::string eap_server_ca_cert_nss_nickname_;
  std::string eap_client_cert_pkcs11_id_;
  bool eap_use_system_cas_;
  std::string eap_identity_;
  std::string eap_anonymous_identity_;
  std::string eap_passphrase_;
  
  bool save_credentials_;

  
  
  std::string user_passphrase_;

  friend class NetworkLibraryImpl;
  DISALLOW_COPY_AND_ASSIGN(WifiNetwork);
};
typedef std::vector<WifiNetwork*> WifiNetworkVector;

class CellularDataPlan {
 public:
  CellularDataPlan()
      : plan_name("Unknown"),
        plan_type(CELLULAR_DATA_PLAN_UNLIMITED),
        plan_data_bytes(0),
        data_bytes_used(0) { }
  explicit CellularDataPlan(const CellularDataPlanInfo &plan)
      : plan_name(plan.plan_name ? plan.plan_name : ""),
        plan_type(plan.plan_type),
        update_time(base::Time::FromInternalValue(plan.update_time)),
        plan_start_time(base::Time::FromInternalValue(plan.plan_start_time)),
        plan_end_time(base::Time::FromInternalValue(plan.plan_end_time)),
        plan_data_bytes(plan.plan_data_bytes),
        data_bytes_used(plan.data_bytes_used) { }
  
  string16 GetPlanDesciption() const;
  
  
  string16 GetRemainingWarning() const;
  
  string16 GetDataRemainingDesciption() const;
  
  string16 GetPlanExpiration() const;
  
  string16 GetUsageInfo() const;
  
  std::string GetUniqueIdentifier() const;
  base::TimeDelta remaining_time() const;
  int64 remaining_minutes() const;
  
  int64 remaining_data() const;
  
  std::string plan_name;
  CellularDataPlanType plan_type;
  base::Time update_time;
  base::Time plan_start_time;
  base::Time plan_end_time;
  int64 plan_data_bytes;
  int64 data_bytes_used;
};
typedef ScopedVector<CellularDataPlan> CellularDataPlanVector;

struct CellTower {
  enum RadioType {
    RADIOTYPE_GSM,
    RADIOTYPE_CDMA,
    RADIOTYPE_WCDMA,
  } radio_type;                   
  int mobile_country_code;        
  int mobile_network_code;        
  int location_area_code;         
  int cell_id;                    
  base::Time timestamp;  
  int signal_strength;   
  int timing_advance;    
                         
};

struct WifiAccessPoint {
  std::string mac_address;  
  std::string name;         
  base::Time timestamp;     
  int signal_strength;      
  int signal_to_noise;      
  int channel;              
};

typedef std::vector<CellTower> CellTowerVector;
typedef std::vector<WifiAccessPoint> WifiAccessPointVector;

struct NetworkIPConfig {
  NetworkIPConfig(const std::string& device_path, IPConfigType type,
                  const std::string& address, const std::string& netmask,
                  const std::string& gateway, const std::string& name_servers)
      : device_path(device_path),
        type(type),
        address(address),
        netmask(netmask),
        gateway(gateway),
        name_servers(name_servers) {}

  
  bool operator< (const NetworkIPConfig& other) const {
    return type < other.type;
  }

  std::string device_path;
  IPConfigType type;
  std::string address;  
  std::string netmask;
  std::string gateway;
  std::string name_servers;
};
typedef std::vector<NetworkIPConfig> NetworkIPConfigVector;

class NetworkLibrary {
 public:
  enum HardwareAddressFormat {
    FORMAT_RAW_HEX,
    FORMAT_COLON_SEPARATED_HEX
  };

  class NetworkManagerObserver {
   public:
    
    
    virtual void OnNetworkManagerChanged(NetworkLibrary* obj) = 0;
  };

  class NetworkObserver {
   public:
    
    
    virtual void OnNetworkChanged(NetworkLibrary* cros,
                                  const Network* network) = 0;
  };

  class NetworkDeviceObserver {
   public:
    
    
    virtual void OnNetworkDeviceChanged(NetworkLibrary* cros,
                                        const NetworkDevice* device) = 0;
  };

  class CellularDataPlanObserver {
   public:
    
    virtual void OnCellularDataPlanChanged(NetworkLibrary* obj) = 0;
  };

  class PinOperationObserver {
   public:
    
    
    virtual void OnPinOperationCompleted(NetworkLibrary* cros,
                                         PinOperationError error) = 0;
  };

  class UserActionObserver {
   public:
    
    
    virtual void OnConnectionInitiated(NetworkLibrary* cros,
                                       const Network* network) = 0;
  };

  virtual ~NetworkLibrary() {}

  virtual void AddNetworkManagerObserver(NetworkManagerObserver* observer) = 0;
  virtual void RemoveNetworkManagerObserver(
      NetworkManagerObserver* observer) = 0;

  
  
  virtual void AddNetworkObserver(const std::string& service_path,
                                  NetworkObserver* observer) = 0;
  
  virtual void RemoveNetworkObserver(const std::string& service_path,
                                     NetworkObserver* observer) = 0;
  
  virtual void RemoveObserverForAllNetworks(NetworkObserver* observer) = 0;

  
  virtual void AddNetworkDeviceObserver(const std::string& device_path,
                                        NetworkDeviceObserver* observer) = 0;
  
  virtual void RemoveNetworkDeviceObserver(const std::string& device_path,
                                           NetworkDeviceObserver* observer) = 0;

  
  
  
  virtual void Lock() = 0;
  
  virtual void Unlock() = 0;
  
  virtual bool IsLocked() = 0;

  virtual void AddCellularDataPlanObserver(
      CellularDataPlanObserver* observer) = 0;
  virtual void RemoveCellularDataPlanObserver(
      CellularDataPlanObserver* observer) = 0;

  virtual void AddPinOperationObserver(PinOperationObserver* observer) = 0;
  virtual void RemovePinOperationObserver(PinOperationObserver* observer) = 0;

  virtual void AddUserActionObserver(UserActionObserver* observer) = 0;
  virtual void RemoveUserActionObserver(UserActionObserver* observer) = 0;

  
  virtual const EthernetNetwork* ethernet_network() const = 0;
  virtual bool ethernet_connecting() const = 0;
  virtual bool ethernet_connected() const = 0;

  
  virtual const WifiNetwork* wifi_network() const = 0;
  virtual bool wifi_connecting() const = 0;
  virtual bool wifi_connected() const = 0;

  
  virtual const CellularNetwork* cellular_network() const = 0;
  virtual bool cellular_connecting() const = 0;
  virtual bool cellular_connected() const = 0;

  
  virtual const VirtualNetwork* virtual_network() const = 0;
  virtual bool virtual_network_connecting() const = 0;
  virtual bool virtual_network_connected() const = 0;

  
  virtual bool Connected() const = 0;

  
  virtual bool Connecting() const = 0;

  
  virtual const std::string& IPAddress() const = 0;

  
  virtual const WifiNetworkVector& wifi_networks() const = 0;

  
  virtual const WifiNetworkVector& remembered_wifi_networks() const = 0;

  
  virtual const CellularNetworkVector& cellular_networks() const = 0;

  
  virtual const VirtualNetworkVector& virtual_networks() const = 0;

  
  virtual const NetworkDevice* FindNetworkDeviceByPath(
      const std::string& path) const = 0;

  
  virtual const NetworkDevice* FindCellularDevice() const = 0;

  
  virtual const NetworkDevice* FindEthernetDevice() const = 0;

  
  virtual const NetworkDevice* FindWifiDevice() const = 0;

  
  
  
  
  
  
  
  
  virtual Network* FindNetworkByPath(const std::string& path) const = 0;
  virtual WifiNetwork* FindWifiNetworkByPath(const std::string& path) const = 0;
  virtual CellularNetwork* FindCellularNetworkByPath(
      const std::string& path) const = 0;
  virtual VirtualNetwork* FindVirtualNetworkByPath(
      const std::string& path) const = 0;

  
  
  virtual Network* FindNetworkFromRemembered(
      const Network* remembered) const = 0;

  
  
  virtual const CellularDataPlanVector* GetDataPlans(
      const std::string& path) const = 0;

  
  
  
  
  
  
  virtual const CellularDataPlan* GetSignificantDataPlan(
      const std::string& path) const = 0;

  
  virtual void ChangePin(const std::string& old_pin,
                         const std::string& new_pin) = 0;

  
  virtual void ChangeRequirePin(bool require_pin,
                                const std::string& pin) = 0;

  
  virtual void EnterPin(const std::string& pin) = 0;

  
  virtual void UnblockPin(const std::string& puk,
                          const std::string& new_pin) = 0;

  
  virtual void RequestCellularScan() = 0;

  
  virtual void RequestCellularRegister(const std::string& network_id) = 0;

  
  virtual void SetCellularDataRoamingAllowed(bool new_value) = 0;

  
  virtual void RequestNetworkScan() = 0;

  
  
  
  
  
  
  virtual bool GetWifiAccessPoints(WifiAccessPointVector* result) = 0;

  

  
  virtual void ConnectToWifiNetwork(WifiNetwork* network) = 0;

  
  virtual void ConnectToWifiNetwork(const std::string& service_path) = 0;

  
  
  virtual void ConnectToWifiNetwork(ConnectionSecurity security,
                                    const std::string& ssid,
                                    const std::string& passphrase,
                                    const std::string& identity,
                                    const std::string& certpath) = 0;

  
  virtual void ConnectToCellularNetwork(CellularNetwork* network) = 0;

  
  virtual void SignalCellularPlanPayment() = 0;

  
  virtual bool HasRecentCellularPlanPayment() = 0;

  
  virtual void ConnectToVirtualNetwork(VirtualNetwork* network) = 0;

  
  
  virtual void ConnectToVirtualNetworkPSK(
      const std::string& service_name,
      const std::string& server_hostname,
      const std::string& psk,
      const std::string& username,
      const std::string& user_passphrase) = 0;

  
  virtual void DisconnectFromNetwork(const Network* network) = 0;

  
  virtual void ForgetWifiNetwork(const std::string& service_path) = 0;

  
  
  virtual std::string GetCellularHomeCarrierId() const = 0;

  virtual bool ethernet_available() const = 0;
  virtual bool wifi_available() const = 0;
  virtual bool cellular_available() const = 0;

  virtual bool ethernet_enabled() const = 0;
  virtual bool wifi_enabled() const = 0;
  virtual bool cellular_enabled() const = 0;

  virtual bool wifi_scanning() const = 0;

  virtual const Network* active_network() const = 0;
  virtual const Network* connected_network() const = 0;

  virtual bool offline_mode() const = 0;

  
  virtual void EnableEthernetNetworkDevice(bool enable) = 0;

  
  virtual void EnableWifiNetworkDevice(bool enable) = 0;

  
  virtual void EnableCellularNetworkDevice(bool enable) = 0;

  
  virtual void EnableOfflineMode(bool enable) = 0;

  
  
  
  
  virtual NetworkIPConfigVector GetIPConfigs(
      const std::string& device_path,
      std::string* hardware_address,
      HardwareAddressFormat) = 0;

  
  
  static NetworkLibrary* GetImpl(bool stub);
};

}  

#endif  
