// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_CRYPTO_PROTOCOL_H_
#define NET_QUIC_CRYPTO_CRYPTO_PROTOCOL_H_

#include <map>
#include <string>
#include <vector>

#include "net/base/net_export.h"
#include "net/quic/quic_protocol.h"

// Version and Crypto tags are written to the wire with a big-endian
// the client hello tag (CHLO) will be written as the
#define TAG(a, b, c, d) ((d << 24) + (c << 16) + (b << 8) + a)

namespace net {

typedef std::string ServerConfigID;
typedef std::map<QuicTag, std::string> QuicTagValueMap;

const QuicTag kCHLO = TAG('C', 'H', 'L', 'O');  
const QuicTag kSHLO = TAG('S', 'H', 'L', 'O');  
const QuicTag kSCFG = TAG('S', 'C', 'F', 'G');  
const QuicTag kREJ  = TAG('R', 'E', 'J', '\0'); 
const QuicTag kCETV = TAG('C', 'E', 'T', 'V');  
                                                

const QuicTag kP256 = TAG('P', '2', '5', '6');  
const QuicTag kC255 = TAG('C', '2', '5', '5');  

const QuicTag kNULL = TAG('N', 'U', 'L', 'N');  
const QuicTag kAESG = TAG('A', 'E', 'S', 'G');  

const QuicTag kQBIC = TAG('Q', 'B', 'I', 'C');  
const QuicTag kPACE = TAG('P', 'A', 'C', 'E');  
const QuicTag kINAR = TAG('I', 'N', 'A', 'R');  

const QuicTag kX509 = TAG('X', '5', '0', '9');  
                                                
const QuicTag kX59R = TAG('X', '5', '9', 'R');  
                                                
const QuicTag kCHID = TAG('C', 'H', 'I', 'D');  

const QuicTag kVERS = TAG('V', 'E', 'R', 'S');  
const QuicTag kVER  = TAG('V', 'E', 'R', '\0'); 
const QuicTag kNONC = TAG('N', 'O', 'N', 'C');  
const QuicTag kKEXS = TAG('K', 'E', 'X', 'S');  
const QuicTag kAEAD = TAG('A', 'E', 'A', 'D');  
                                                
const QuicTag kCGST = TAG('C', 'G', 'S', 'T');  
                                                
const QuicTag kICSL = TAG('I', 'C', 'S', 'L');  
                                                
const QuicTag kKATO = TAG('K', 'A', 'T', 'O');  
const QuicTag kMSPC = TAG('M', 'S', 'P', 'C');  
const QuicTag kIRTT = TAG('I', 'R', 'T', 'T');  
const QuicTag kSWND = TAG('S', 'W', 'N', 'D');  
                                                
const QuicTag kSNI  = TAG('S', 'N', 'I', '\0'); 
                                                
const QuicTag kPUBS = TAG('P', 'U', 'B', 'S');  
const QuicTag kSCID = TAG('S', 'C', 'I', 'D');  
const QuicTag kORBT = TAG('O', 'B', 'I', 'T');  
const QuicTag kPDMD = TAG('P', 'D', 'M', 'D');  
const QuicTag kPROF = TAG('P', 'R', 'O', 'F');  
const QuicTag kCCS  = TAG('C', 'C', 'S', 0);    
const QuicTag kCCRT = TAG('C', 'C', 'R', 'T');  
const QuicTag kEXPY = TAG('E', 'X', 'P', 'Y');  

const QuicTag kCIDK = TAG('C', 'I', 'D', 'K');  
const QuicTag kCIDS = TAG('C', 'I', 'D', 'S');  

const QuicTag kPAD  = TAG('P', 'A', 'D', '\0'); 

const QuicTag kServerNonceTag =
    TAG('S', 'N', 'O', 0);  
const QuicTag kSourceAddressTokenTag =
    TAG('S', 'T', 'K', 0);  
const QuicTag kCertificateTag =
    TAG('C', 'R', 'T', 255);  

#undef TAG

const size_t kMaxEntries = 128;  

const size_t kNonceSize = 32;  

const size_t kOrbitSize = 8;  

const char kProofSignatureLabel[] = "QUIC server config signature";

const size_t kClientHelloMinimumSize = 1024;

const size_t kClientHelloMinimumSizeOld = 512;

}  

#endif  
