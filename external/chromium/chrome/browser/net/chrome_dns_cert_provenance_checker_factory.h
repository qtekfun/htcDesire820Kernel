// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CHROME_DNS_CERT_PROVENANCE_CHECKER_FACTORY
#define CHROME_BROWSER_NET_CHROME_DNS_CERT_PROVENANCE_CHECKER_FACTORY
#pragma once

#include "net/socket/dns_cert_provenance_checker.h"


namespace net {
class DnsRRResolver;
}

class ChromeURLRequestContext;

typedef net::DnsCertProvenanceChecker* (*DnsCertProvenanceCheckerFactory) (
    net::DnsRRResolver* dnsrr_resolver,
    ChromeURLRequestContext* url_req_context);

net::DnsCertProvenanceChecker* CreateDnsCertProvenanceChecker(
    net::DnsRRResolver* dnsrr_resolver,
    ChromeURLRequestContext* url_req_context);

void SetDnsCertProvenanceCheckerFactory(DnsCertProvenanceCheckerFactory);

#endif  
