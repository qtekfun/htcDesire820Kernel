// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NET_X509_CERTIFICATE_MODEL_H_
#define CHROME_COMMON_NET_X509_CERTIFICATE_MODEL_H_

#include "net/cert/cert_type.h"
#include "net/cert/x509_certificate.h"

namespace x509_certificate_model {

std::string GetCertNameOrNickname(
    net::X509Certificate::OSCertHandle cert_handle);

std::string GetNickname(net::X509Certificate::OSCertHandle cert_handle);

std::string GetTokenName(net::X509Certificate::OSCertHandle cert_handle);

std::string GetVersion(net::X509Certificate::OSCertHandle cert_handle);

net::CertType GetType(net::X509Certificate::OSCertHandle cert_handle);

std::string GetEmailAddress(net::X509Certificate::OSCertHandle cert_handle);

void GetUsageStrings(
    net::X509Certificate::OSCertHandle cert_handle,
    std::vector<std::string>* usages);

std::string GetKeyUsageString(net::X509Certificate::OSCertHandle cert_handle);

std::string GetSerialNumberHexified(
    net::X509Certificate::OSCertHandle cert_handle,
    const std::string& alternative_text);

std::string GetIssuerCommonName(
    net::X509Certificate::OSCertHandle cert_handle,
    const std::string& alternative_text);

std::string GetIssuerOrgName(
    net::X509Certificate::OSCertHandle cert_handle,
    const std::string& alternative_text);

std::string GetIssuerOrgUnitName(
    net::X509Certificate::OSCertHandle cert_handle,
    const std::string& alternative_text);

std::string GetSubjectOrgName(
    net::X509Certificate::OSCertHandle cert_handle,
    const std::string& alternative_text);

std::string GetSubjectOrgUnitName(
    net::X509Certificate::OSCertHandle cert_handle,
    const std::string& alternative_text);

std::string GetSubjectCommonName(
    net::X509Certificate::OSCertHandle cert_handle,
    const std::string& alternative_text);

bool GetTimes(net::X509Certificate::OSCertHandle cert_handle,
              base::Time* issued, base::Time* expires);

std::string GetTitle(net::X509Certificate::OSCertHandle cert_handle);
std::string GetIssuerName(net::X509Certificate::OSCertHandle cert_handle);
std::string GetSubjectName(net::X509Certificate::OSCertHandle cert_handle);

void GetEmailAddresses(net::X509Certificate::OSCertHandle cert_handle,
                       std::vector<std::string>* email_addresses);

void GetNicknameStringsFromCertList(const net::CertificateList& certs,
                                    const std::string& cert_expired,
                                    const std::string& cert_not_yet_valid,
                                    std::vector<std::string>* nick_names);

std::string GetPkcs11Id(net::X509Certificate::OSCertHandle cert_handle);

struct Extension {
  std::string name;
  std::string value;
};

typedef std::vector<Extension> Extensions;

void GetExtensions(
    const std::string& critical_label,
    const std::string& non_critical_label,
    net::X509Certificate::OSCertHandle cert_handle,
    Extensions* extensions);

std::string HashCertSHA256(net::X509Certificate::OSCertHandle cert_handle);
std::string HashCertSHA1(net::X509Certificate::OSCertHandle cert_handle);

std::string ProcessIDN(const std::string& input);

void GetCertChainFromCert(net::X509Certificate::OSCertHandle cert_handle,
                          net::X509Certificate::OSCertHandles* cert_handles);
void DestroyCertChain(net::X509Certificate::OSCertHandles* cert_handles);

std::string GetDerString(net::X509Certificate::OSCertHandle cert_handle);
std::string GetCMSString(const net::X509Certificate::OSCertHandles& cert_chain,
                         size_t start, size_t end);

std::string ProcessSecAlgorithmSignature(
    net::X509Certificate::OSCertHandle cert_handle);
std::string ProcessSecAlgorithmSubjectPublicKey(
    net::X509Certificate::OSCertHandle cert_handle);
std::string ProcessSecAlgorithmSignatureWrap(
    net::X509Certificate::OSCertHandle cert_handle);

std::string ProcessSubjectPublicKeyInfo(
    net::X509Certificate::OSCertHandle cert_handle);

std::string ProcessRawBitsSignatureWrap(
    net::X509Certificate::OSCertHandle cert_handle);

void RegisterDynamicOids();

std::string ProcessRawBytesWithSeparators(const unsigned char* data,
                                          size_t data_length,
                                          char hex_separator,
                                          char line_separator);

std::string ProcessRawBytes(const unsigned char* data,
                            size_t data_length);

#if defined(USE_NSS)
std::string ProcessRawBits(const unsigned char* data,
                           size_t data_length);
#endif  

}  

#endif  
