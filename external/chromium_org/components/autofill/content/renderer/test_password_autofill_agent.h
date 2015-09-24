// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_RENDERER_TEST_PASSWORD_AUTOFILL_AGENT_H_
#define COMPONENTS_AUTOFILL_CONTENT_RENDERER_TEST_PASSWORD_AUTOFILL_AGENT_H_

#include "components/autofill/content/renderer/password_autofill_agent.h"

namespace autofill {

class TestPasswordAutofillAgent : public PasswordAutofillAgent {
 public:
  explicit TestPasswordAutofillAgent(content::RenderView* render_view);
  virtual ~TestPasswordAutofillAgent();

 private:
  
  
  
  virtual bool OriginCanAccessPasswordManager(
      const blink::WebSecurityOrigin& origin) OVERRIDE;
};

}  

#endif  