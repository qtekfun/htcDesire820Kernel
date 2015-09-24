// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_ELEMENT_COMMANDS_H_
#define CHROME_TEST_CHROMEDRIVER_ELEMENT_COMMANDS_H_

#include <string>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class Value;
}

struct Session;
class Status;
class WebView;

typedef base::Callback<Status(
    Session* session,
    WebView* web_view,
    const std::string&,
    const base::DictionaryValue&,
    scoped_ptr<base::Value>*)> ElementCommand;

Status ExecuteElementCommand(
    const ElementCommand& command,
    Session* session,
    WebView* web_view,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteFindChildElement(
    int interval_ms,
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteFindChildElements(
    int interval_ms,
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteHoverOverElement(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteClickElement(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteTouchSingleTap(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteClearElement(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteSendKeysToElement(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteSubmitElement(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementText(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementValue(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementTagName(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteIsElementSelected(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteIsElementEnabled(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteIsElementDisplayed(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementLocation(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementLocationOnceScrolledIntoView(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementSize(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementAttribute(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteGetElementValueOfCSSProperty(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

Status ExecuteElementEquals(
    Session* session,
    WebView* web_view,
    const std::string& element_id,
    const base::DictionaryValue& params,
    scoped_ptr<base::Value>* value);

#endif  
