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


#ifndef TALK_EXAMPLES_CALL_VOICEMAILJIDREQUESTER_H_
#define TALK_EXAMPLES_CALL_VOICEMAILJIDREQUESTER_H_

#include "talk/xmpp/xmpptask.h"

namespace buzz {

class Task;

class VoicemailJidRequester : public sigslot::has_slots<>,
                              public talk_base::Task {
 public:
  VoicemailJidRequester(talk_base::Task* parent, const Jid& their_jid, const Jid& my_jid);

  
  sigslot::signal2<const Jid&, const Jid&> SignalGotVoicemailJid;
  sigslot::signal1<const Jid&> SignalVoicemailJidError;

  virtual int ProcessStart();
 protected:

  virtual int Process(int state);

 private:
  
  
  void OnFirstVoicemailJidError(buzz::Jid jid, const XmlElement* xml_element);

  
  void OnFirstVoicemailJidSuccess(buzz::Jid jid, const XmlElement* xml_element);

  
  
  void OnSecondVoicemailJidError(buzz::Jid jid, const XmlElement* xml_element);

  
  void OnSecondVoicemailJidSuccess(buzz::Jid jid,
                                   const XmlElement* xml_element);

  
  
  bool ProcessVoicemailXml(const XmlElement* xml_element);

  
  
  void StartSecondQuery();

  talk_base::Task* parent_;

  Jid their_jid_;

  
  Jid my_jid_;

  
  
  bool done_with_query_;
};
}

#endif  
