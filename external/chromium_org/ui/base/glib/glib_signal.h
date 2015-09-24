// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GLIB_GLIB_SIGNAL_H_
#define UI_BASE_GLIB_GLIB_SIGNAL_H_

typedef void* gpointer;



#define CHROMEG_CALLBACK_0(CLASS, RETURN, METHOD, SENDER)           \
  static RETURN METHOD ## Thunk(SENDER sender, gpointer userdata) { \
    return reinterpret_cast<CLASS*>(userdata)->METHOD(sender);      \
  }                                                                 \
                                                                    \
  RETURN METHOD(SENDER);

#define CHROMEG_CALLBACK_1(CLASS, RETURN, METHOD, SENDER, ARG1)     \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one,            \
                                gpointer userdata) {                \
    return reinterpret_cast<CLASS*>(userdata)->METHOD(sender, one); \
  }                                                                 \
                                                                    \
  RETURN METHOD(SENDER, ARG1);

#define CHROMEG_CALLBACK_2(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2)    \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,       \
                                gpointer userdata) {                     \
    return reinterpret_cast<CLASS*>(userdata)->METHOD(sender, one, two); \
  }                                                                      \
                                                                         \
  RETURN METHOD(SENDER, ARG1, ARG2);

#define CHROMEG_CALLBACK_3(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, ARG3) \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,          \
                                ARG3 three, gpointer userdata) {            \
    return reinterpret_cast<CLASS*>(userdata)->                             \
        METHOD(sender, one, two, three);                                    \
  }                                                                         \
                                                                            \
  RETURN METHOD(SENDER, ARG1, ARG2, ARG3);

#define CHROMEG_CALLBACK_4(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, ARG3, \
                           ARG4)                                            \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,          \
                                ARG3 three, ARG4 four,                      \
                                gpointer userdata) {                        \
    return reinterpret_cast<CLASS*>(userdata)->                             \
        METHOD(sender, one, two, three, four);                              \
  }                                                                         \
                                                                            \
  RETURN METHOD(SENDER, ARG1, ARG2, ARG3, ARG4);

#define CHROMEG_CALLBACK_5(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, ARG3, \
                           ARG4, ARG5)                                      \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,          \
                                ARG3 three, ARG4 four, ARG5 five,           \
                                gpointer userdata) {                        \
    return reinterpret_cast<CLASS*>(userdata)->                             \
        METHOD(sender, one, two, three, four, five);                        \
  }                                                                         \
                                                                            \
  RETURN METHOD(SENDER, ARG1, ARG2, ARG3, ARG4, ARG5);

#define CHROMEG_CALLBACK_6(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, ARG3, \
                           ARG4, ARG5, ARG6)                                \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,          \
                                ARG3 three, ARG4 four, ARG5 five,           \
                                ARG6 six, gpointer userdata) {              \
    return reinterpret_cast<CLASS*>(userdata)->                             \
        METHOD(sender, one, two, three, four, five, six);                   \
  }                                                                         \
                                                                            \
  RETURN METHOD(SENDER, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6);

#define CHROMEG_VIRTUAL_CALLBACK_0(CLASS, RETURN, METHOD, SENDER)   \
  static RETURN METHOD ## Thunk(SENDER sender, gpointer userdata) { \
    return reinterpret_cast<CLASS*>(userdata)->METHOD(sender);      \
  }                                                                 \
                                                                    \
  virtual RETURN METHOD(SENDER);

#define CHROMEG_VIRTUAL_CALLBACK_1(CLASS, RETURN, METHOD, SENDER, ARG1) \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one,                \
                                gpointer userdata) {                    \
    return reinterpret_cast<CLASS*>(userdata)->METHOD(sender, one);     \
  }                                                                     \
                                                                        \
  virtual RETURN METHOD(SENDER, ARG1);

#define CHROMEG_VIRTUAL_CALLBACK_2(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2) \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,            \
                                gpointer userdata) {                          \
    return reinterpret_cast<CLASS*>(userdata)->METHOD(sender, one, two);      \
  }                                                                           \
                                                                              \
  virtual RETURN METHOD(SENDER, ARG1, ARG2);

#define CHROMEG_VIRTUAL_CALLBACK_3(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, \
                                   ARG3)                                      \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,            \
                                ARG3 three, gpointer userdata) {              \
    return reinterpret_cast<CLASS*>(userdata)->                               \
        METHOD(sender, one, two, three);                                      \
  }                                                                           \
                                                                              \
  virtual RETURN METHOD(SENDER, ARG1, ARG2, ARG3);

#define CHROMEG_VIRTUAL_CALLBACK_4(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, \
                                   ARG3, ARG4)                                \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,            \
                                ARG3 three, ARG4 four,                        \
                                gpointer userdata) {                          \
    return reinterpret_cast<CLASS*>(userdata)->                               \
        METHOD(sender, one, two, three, four);                                \
  }                                                                           \
                                                                              \
  virtual RETURN METHOD(SENDER, ARG1, ARG2, ARG3, ARG4);

#define CHROMEG_VIRTUAL_CALLBACK_5(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, \
                                   ARG3,  ARG4, ARG5)                         \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,            \
                                ARG3 three, ARG4 four, ARG5 five,             \
                                gpointer userdata) {                          \
    return reinterpret_cast<CLASS*>(userdata)->                               \
        METHOD(sender, one, two, three, four, five);                          \
  }                                                                           \
                                                                              \
  virtual RETURN METHOD(SENDER, ARG1, ARG2, ARG3, ARG4, ARG5);

#define CHROMEG_VIRTUAL_CALLBACK_6(CLASS, RETURN, METHOD, SENDER, ARG1, ARG2, \
                                   ARG3, ARG4, ARG5, ARG6)                    \
  static RETURN METHOD ## Thunk(SENDER sender, ARG1 one, ARG2 two,            \
                                ARG3 three, ARG4 four, ARG5 five,             \
                                ARG6 six, gpointer userdata) {                \
    return reinterpret_cast<CLASS*>(userdata)->                               \
        METHOD(sender, one, two, three, four, five, six);                     \
  }                                                                           \
                                                                              \
  virtual RETURN METHOD(SENDER, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6);

#endif
