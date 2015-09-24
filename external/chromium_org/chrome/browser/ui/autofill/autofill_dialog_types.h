// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_TYPES_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_TYPES_H_

#include <map>
#include <vector>

#include "base/callback_forward.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "components/autofill/core/browser/autofill_metrics.h"
#include "components/autofill/core/browser/field_types.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/text_constants.h"
#include "url/gurl.h"

namespace autofill {

class AutofillField;

struct DetailInput {
  enum Length {
    SHORT,  
    LONG,   
    NONE,   
  };

  
  Length length;

  ServerFieldType type;

  
  int placeholder_text_rid;

  
  
  float expand_weight;

  
  
  base::string16 initial_value;
};

typedef base::Callback<bool(const DetailInput& input,
                            const AutofillField& field)>
    InputFieldComparator;

enum DialogSection {
  
  SECTION_MIN,

  
  SECTION_CC = SECTION_MIN,
  SECTION_BILLING,
  
  SECTION_CC_BILLING,
  SECTION_SHIPPING,

  
  SECTION_MAX = SECTION_SHIPPING
};

class DialogNotification {
 public:
  enum Type {
    NONE,
    DEVELOPER_WARNING,
    REQUIRED_ACTION,
    SECURITY_WARNING,
    WALLET_ERROR,
    WALLET_USAGE_CONFIRMATION,
  };

  DialogNotification();
  DialogNotification(Type type, const base::string16& display_text);
  ~DialogNotification();

  
  
  SkColor GetBackgroundColor() const;
  SkColor GetBorderColor() const;
  SkColor GetTextColor() const;

  
  bool HasArrow() const;

  
  bool HasCheckbox() const;

  Type type() const { return type_; }
  const base::string16& display_text() const { return display_text_; }

  void set_link_url(const GURL& link_url) { link_url_ = link_url; }
  const GURL& link_url() const { return link_url_; }

  const gfx::Range& link_range() const { return link_range_; }

  void set_tooltip_text(const base::string16& tooltip_text) {
    tooltip_text_ = tooltip_text;
  }
  const base::string16& tooltip_text() const { return tooltip_text_; }

  void set_checked(bool checked) { checked_ = checked; }
  bool checked() const { return checked_; }

 private:
  Type type_;
  base::string16 display_text_;

  
  
  GURL link_url_;
  gfx::Range link_range_;

  
  
  base::string16 tooltip_text_;

  
  
  bool checked_;
};

extern SkColor const kWarningColor;

struct SuggestionState {
  SuggestionState();
  SuggestionState(bool visible,
                  const base::string16& vertically_compact_text,
                  const base::string16& horizontally_compact_text,
                  const gfx::Image& icon,
                  const base::string16& extra_text,
                  const gfx::Image& extra_icon);
  ~SuggestionState();

  
  bool visible;

  
  
  
  
  base::string16 vertically_compact_text;
  base::string16 horizontally_compact_text;

  gfx::Image icon;
  base::string16 extra_text;
  gfx::Image extra_icon;
};

struct DialogOverlayString {
  DialogOverlayString();
  ~DialogOverlayString();

  
  base::string16 text;

  
  SkColor text_color;

  
  gfx::Font font;
};

struct DialogOverlayState {
  DialogOverlayState();
  ~DialogOverlayState();

  
  
  gfx::Image image;

  
  DialogOverlayString string;
};

enum ValidationType {
  VALIDATE_EDIT,   
  VALIDATE_FINAL,  
};

typedef std::vector<DetailInput> DetailInputs;
typedef std::map<ServerFieldType, base::string16> FieldValueMap;

struct ValidityMessage {
  ValidityMessage(const base::string16& text, bool sure);
  ~ValidityMessage();

  
  base::string16 text;

  
  
  
  bool sure;
};

class ValidityMessages {
 public:
  ValidityMessages();
  ~ValidityMessages();

  void Set(ServerFieldType field, const ValidityMessage& message);
  const ValidityMessage& GetMessageOrDefault(ServerFieldType field) const;

  bool HasSureError(ServerFieldType field) const;
  bool HasErrors() const;
  bool HasSureErrors() const;

 private:
  typedef std::map<ServerFieldType, ValidityMessage> MessageMap;
  MessageMap messages_;
  ValidityMessage default_message_;
};

}  

#endif  
