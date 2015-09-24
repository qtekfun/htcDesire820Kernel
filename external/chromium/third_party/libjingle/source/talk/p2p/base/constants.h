/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_P2P_BASE_CONSTANTS_H_
#define TALK_P2P_BASE_CONSTANTS_H_

#include <string>
#include "talk/xmllite/qname.h"


namespace cricket {


extern const std::string NS_EMPTY;
extern const std::string NS_JINGLE;
extern const std::string NS_GINGLE;

enum SignalingProtocol {
  PROTOCOL_JINGLE,
  PROTOCOL_GINGLE,
  PROTOCOL_HYBRID,
};

extern const buzz::QName QN_ACTION;
extern const std::string LN_INITIATOR;
extern const buzz::QName QN_INITIATOR;
extern const buzz::QName QN_CREATOR;

extern const buzz::QName QN_JINGLE;
extern const buzz::QName QN_JINGLE_CONTENT;
extern const buzz::QName QN_JINGLE_CONTENT_NAME;
extern const buzz::QName QN_JINGLE_CONTENT_MEDIA;
extern const buzz::QName QN_JINGLE_REASON;
extern const std::string JINGLE_CONTENT_MEDIA_AUDIO;
extern const std::string JINGLE_CONTENT_MEDIA_VIDEO;
extern const std::string JINGLE_ACTION_SESSION_INITIATE;
extern const std::string JINGLE_ACTION_SESSION_INFO;
extern const std::string JINGLE_ACTION_SESSION_ACCEPT;
extern const std::string JINGLE_ACTION_SESSION_TERMINATE;
extern const std::string JINGLE_ACTION_TRANSPORT_INFO;
extern const std::string JINGLE_ACTION_TRANSPORT_ACCEPT;

extern const buzz::QName QN_GINGLE_SESSION;
extern const std::string GINGLE_ACTION_INITIATE;
extern const std::string GINGLE_ACTION_INFO;
extern const std::string GINGLE_ACTION_ACCEPT;
extern const std::string GINGLE_ACTION_REJECT;
extern const std::string GINGLE_ACTION_TERMINATE;
extern const std::string GINGLE_ACTION_CANDIDATES;
extern const std::string GINGLE_ACTION_NOTIFY;
extern const std::string GINGLE_ACTION_UPDATE;
extern const std::string GINGLE_ACTION_VIEW;

extern const std::string LN_ERROR;
extern const buzz::QName QN_GINGLE_REDIRECT;
extern const std::string STR_REDIRECT_PREFIX;

extern const std::string LN_DESCRIPTION;
extern const std::string LN_PAYLOADTYPE;
extern const buzz::QName QN_ID;
extern const buzz::QName QN_SID;
extern const buzz::QName QN_NAME;
extern const buzz::QName QN_CLOCKRATE;
extern const buzz::QName QN_BITRATE;
extern const buzz::QName QN_CHANNELS;
extern const buzz::QName QN_WIDTH;
extern const buzz::QName QN_HEIGHT;
extern const buzz::QName QN_FRAMERATE;
extern const buzz::QName QN_PARAMETER;
extern const std::string LN_NAME;
extern const std::string LN_VALUE;
extern const buzz::QName QN_PAYLOADTYPE_PARAMETER_NAME;
extern const buzz::QName QN_PAYLOADTYPE_PARAMETER_VALUE;
extern const std::string PAYLOADTYPE_PARAMETER_BITRATE;
extern const std::string PAYLOADTYPE_PARAMETER_HEIGHT;
extern const std::string PAYLOADTYPE_PARAMETER_WIDTH;
extern const std::string PAYLOADTYPE_PARAMETER_FRAMERATE;
extern const std::string LN_BANDWIDTH;

extern const std::string CN_AUDIO;
extern const std::string CN_VIDEO;
extern const std::string CN_OTHER;

extern const std::string NS_JINGLE_RTP;
extern const buzz::QName QN_JINGLE_RTP_CONTENT;
extern const buzz::QName QN_JINGLE_RTP_PAYLOADTYPE;
extern const buzz::QName QN_JINGLE_RTP_BANDWIDTH;

extern const std::string NS_GINGLE_AUDIO;
extern const buzz::QName QN_GINGLE_AUDIO_CONTENT;
extern const buzz::QName QN_GINGLE_AUDIO_PAYLOADTYPE;
extern const buzz::QName QN_GINGLE_AUDIO_SRCID;
extern const std::string NS_GINGLE_VIDEO;
extern const buzz::QName QN_GINGLE_VIDEO_CONTENT;
extern const buzz::QName QN_GINGLE_VIDEO_PAYLOADTYPE;
extern const buzz::QName QN_GINGLE_VIDEO_SRCID;
extern const buzz::QName QN_GINGLE_VIDEO_BANDWIDTH;

extern const buzz::QName QN_ENCRYPTION;
extern const buzz::QName QN_ENCRYPTION_REQUIRED;
extern const buzz::QName QN_CRYPTO;
extern const buzz::QName QN_GINGLE_AUDIO_CRYPTO_USAGE;
extern const buzz::QName QN_GINGLE_VIDEO_CRYPTO_USAGE;
extern const buzz::QName QN_CRYPTO_SUITE;
extern const buzz::QName QN_CRYPTO_KEY_PARAMS;
extern const buzz::QName QN_CRYPTO_TAG;
extern const buzz::QName QN_CRYPTO_SESSION_PARAMS;

extern const std::string LN_TRANSPORT;
extern const std::string LN_CANDIDATE;
extern const buzz::QName QN_JINGLE_P2P_TRANSPORT;
extern const buzz::QName QN_JINGLE_P2P_CANDIDATE;
extern const buzz::QName QN_UFRAG;
extern const buzz::QName QN_COMPONENT;
extern const buzz::QName QN_PWD;
extern const buzz::QName QN_IP;
extern const buzz::QName QN_PORT;
extern const buzz::QName QN_NETWORK;
extern const buzz::QName QN_GENERATION;
extern const buzz::QName QN_PRIORITY;
extern const buzz::QName QN_PROTOCOL;
extern const std::string JINGLE_CANDIDATE_TYPE_PEER_STUN;
extern const std::string JINGLE_CANDIDATE_TYPE_SERVER_STUN;
extern const std::string JINGLE_CANDIDATE_NAME_RTP;
extern const std::string JINGLE_CANDIDATE_NAME_RTCP;

extern const std::string NS_GINGLE_P2P;
extern const buzz::QName QN_GINGLE_P2P_TRANSPORT;
extern const buzz::QName QN_GINGLE_P2P_CANDIDATE;
extern const buzz::QName QN_GINGLE_P2P_UNKNOWN_CHANNEL_NAME;
extern const buzz::QName QN_GINGLE_CANDIDATE;
extern const buzz::QName QN_ADDRESS;
extern const buzz::QName QN_USERNAME;
extern const buzz::QName QN_PASSWORD;
extern const buzz::QName QN_PREFERENCE;
extern const std::string GINGLE_CANDIDATE_TYPE_STUN;
extern const std::string GINGLE_CANDIDATE_NAME_RTP;
extern const std::string GINGLE_CANDIDATE_NAME_RTCP;
extern const std::string GINGLE_CANDIDATE_NAME_VIDEO_RTP;
extern const std::string GINGLE_CANDIDATE_NAME_VIDEO_RTCP;

extern const std::string NS_GINGLE_RAW;
extern const buzz::QName QN_GINGLE_RAW_TRANSPORT;
extern const buzz::QName QN_GINGLE_RAW_CHANNEL;

extern const std::string JINGLE_ERROR_BAD_REQUEST;  
extern const std::string JINGLE_ERROR_OUT_OF_ORDER;
extern const std::string JINGLE_ERROR_UNKNOWN_SESSION;

extern const std::string STR_TERMINATE_DECLINE;  
extern const std::string STR_TERMINATE_SUCCESS;  
extern const std::string STR_TERMINATE_ERROR;  
extern const std::string STR_TERMINATE_INCOMPATIBLE_PARAMETERS;  

extern const std::string STR_TERMINATE_CALL_ENDED;
extern const std::string STR_TERMINATE_RECIPIENT_UNAVAILABLE;
extern const std::string STR_TERMINATE_RECIPIENT_BUSY;
extern const std::string STR_TERMINATE_INSUFFICIENT_FUNDS;
extern const std::string STR_TERMINATE_NUMBER_MALFORMED;
extern const std::string STR_TERMINATE_NUMBER_DISALLOWED;
extern const std::string STR_TERMINATE_PROTOCOL_ERROR;
extern const std::string STR_TERMINATE_INTERNAL_SERVER_ERROR;
extern const std::string STR_TERMINATE_UNKNOWN_ERROR;

extern const buzz::QName QN_GINGLE_NOTIFY;
extern const buzz::QName QN_GINGLE_NOTIFY_NICK;
extern const buzz::QName QN_GINGLE_NOTIFY_SOURCE;
extern const buzz::QName QN_GINGLE_NOTIFY_SOURCE_MTYPE;
extern const buzz::QName QN_GINGLE_NOTIFY_SOURCE_SSRC;
extern const std::string GINGLE_NOTIFY_SOURCE_MTYPE_AUDIO;
extern const std::string GINGLE_NOTIFY_SOURCE_MTYPE_VIDEO;

extern const buzz::QName QN_GINGLE_VIEW;
extern const buzz::QName QN_GINGLE_VIEW_TYPE;
extern const buzz::QName QN_GINGLE_VIEW_NICK;
extern const buzz::QName QN_GINGLE_VIEW_MEDIA_TYPE;
extern const buzz::QName QN_GINGLE_VIEW_SSRC;
extern const std::string GINGLE_VIEW_TYPE_STATIC;
extern const std::string GINGLE_VIEW_TYPE_DYNAMIC;
extern const std::string GINGLE_VIEW_MEDIA_TYPE_AUDIO;
extern const std::string GINGLE_VIEW_MEDIA_TYPE_VIDEO;
extern const buzz::QName QN_GINGLE_VIEW_PARAMS;
extern const buzz::QName QN_GINGLE_VIEW_PARAMS_WIDTH;
extern const buzz::QName QN_GINGLE_VIEW_PARAMS_HEIGHT;
extern const buzz::QName QN_GINGLE_VIEW_PARAMS_FRAMERATE;

#ifdef FEATURE_ENABLE_VOICEMAIL
extern const std::string NS_VOICEMAIL;
extern const buzz::QName QN_VOICEMAIL_REGARDING;
#endif

}  

#endif  
