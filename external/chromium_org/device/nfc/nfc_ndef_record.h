// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_NFC_NFC_NDEF_RECORD_H_
#define DEVICE_NFC_NFC_NDEF_RECORD_H_

#include <string>
#include <vector>

#include "base/values.h"

namespace device {

class NfcNdefRecord {
 public:
  
  enum Type {
    kTypeHandoverCarrier,
    kTypeHandoverRequest,
    kTypeHandoverSelect,
    kTypeSmartPoster,
    kTypeText,
    kTypeURI,
    kTypeUnknown
  };

  
  
  
  
  
  

  

  
  
  static const char kFieldEncoding[];

  
  static const char kFieldLanguageCode[];

  
  static const char kFieldText[];

  

  
  
  static const char kFieldURI[];

  
  
  static const char kFieldMimeType[];

  
  
  
  static const char kFieldTargetSize[];

  
  

  
  
  
  
  
  static const char kFieldTitles[];

  
  
  static const char kFieldAction[];

  
  static const char kEncodingUtf8[];
  static const char kEncodingUtf16[];

  
  
  

  
  static const char kSmartPosterActionDo[];

  
  static const char kSmartPosterActionSave[];

  
  static const char kSmartPosterActionOpen[];

  NfcNdefRecord();
  virtual ~NfcNdefRecord();

  
  Type type() const { return type_; }

  
  
  const base::DictionaryValue& data() const { return data_; }

  
  bool IsPopulated() const;

  
  
  
  
  
  
  
  
  bool Populate(Type type, const base::DictionaryValue* data);

 private:
  
  Type type_;

  
  base::DictionaryValue data_;

  DISALLOW_COPY_AND_ASSIGN(NfcNdefRecord);
};

class NfcNdefMessage {
 public:
  
  typedef std::vector<NfcNdefRecord*> RecordList;

  NfcNdefMessage();
  virtual ~NfcNdefMessage();

  
  const RecordList& records() const { return records_; }

  
  
  void AddRecord(NfcNdefRecord* record);

 private:
  
  RecordList records_;

  DISALLOW_COPY_AND_ASSIGN(NfcNdefMessage);
};

}  

#endif  
