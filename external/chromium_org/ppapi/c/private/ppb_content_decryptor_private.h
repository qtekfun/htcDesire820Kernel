/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_CONTENT_DECRYPTOR_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_CONTENT_DECRYPTOR_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/pp_content_decryptor.h"

#define PPB_CONTENTDECRYPTOR_PRIVATE_INTERFACE_0_9 \
    "PPB_ContentDecryptor_Private;0.9"
#define PPB_CONTENTDECRYPTOR_PRIVATE_INTERFACE \
    PPB_CONTENTDECRYPTOR_PRIVATE_INTERFACE_0_9



struct PPB_ContentDecryptor_Private_0_9 {
  void (*SessionCreated)(PP_Instance instance,
                         uint32_t session_id,
                         struct PP_Var web_session_id);
  /**
   * A message or request has been generated for key_system in the CDM, and
   * must be sent to the web application.
   *
   * For example, when the browser invokes <code>CreateSession()</code>
   * on the <code>PPP_ContentDecryptor_Private</code> interface, the plugin
   * must send a message containing the license request.
   *
   * Note that <code>SessionMessage()</code> can be used for purposes other than
   * responses to <code>CreateSession()</code> calls. See also the text
   * in the comment for <code>SessionReady()</code>, which describes a sequence
   * of <code>UpdateSession()</code> and <code>SessionMessage()</code> calls
   * required to prepare for decryption.
   *
   * @param[in] session_id Identifies the session for which the message
   * is intended.
   *
   * @param[in] message A <code>PP_Var</code> of type
   * <code>PP_VARTYPE_ARRAY_BUFFER</code> that contains the message.
   *
   * @param[in] destination_url A <code>PP_Var</code> of type
   * <code>PP_VARTYPE_STRING</code> containing the destination URL for the
   * message.
   */
  void (*SessionMessage)(PP_Instance instance,
                         uint32_t session_id,
                         struct PP_Var message,
                         struct PP_Var destination_url);
  void (*SessionReady)(PP_Instance instance, uint32_t session_id);
  void (*SessionClosed)(PP_Instance instance, uint32_t session_id);
  void (*SessionError)(PP_Instance instance,
                       uint32_t session_id,
                       int32_t media_error,
                       int32_t system_code);
  void (*DeliverBlock)(
      PP_Instance instance,
      PP_Resource decrypted_block,
      const struct PP_DecryptedBlockInfo* decrypted_block_info);
  void (*DecoderInitializeDone)(PP_Instance instance,
                                PP_DecryptorStreamType decoder_type,
                                uint32_t request_id,
                                PP_Bool success);
  void (*DecoderDeinitializeDone)(PP_Instance instance,
                                  PP_DecryptorStreamType decoder_type,
                                  uint32_t request_id);
  void (*DecoderResetDone)(PP_Instance instance,
                           PP_DecryptorStreamType decoder_type,
                           uint32_t request_id);
  void (*DeliverFrame)(
      PP_Instance instance,
      PP_Resource decrypted_frame,
      const struct PP_DecryptedFrameInfo* decrypted_frame_info);
  void (*DeliverSamples)(
      PP_Instance instance,
      PP_Resource audio_frames,
      const struct PP_DecryptedSampleInfo* decrypted_sample_info);
};

typedef struct PPB_ContentDecryptor_Private_0_9 PPB_ContentDecryptor_Private;

#endif  

