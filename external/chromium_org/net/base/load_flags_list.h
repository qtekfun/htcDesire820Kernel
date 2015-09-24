// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


LOAD_FLAG(NORMAL, 0)

LOAD_FLAG(VALIDATE_CACHE, 1 << 0)

LOAD_FLAG(BYPASS_CACHE, 1 << 1)

LOAD_FLAG(PREFERRING_CACHE, 1 << 2)

LOAD_FLAG(ONLY_FROM_CACHE, 1 << 3)

LOAD_FLAG(FROM_CACHE_IF_OFFLINE, 1 << 4)

LOAD_FLAG(DISABLE_CACHE, 1 << 5)

LOAD_FLAG(DISABLE_INTERCEPT, 1 << 6)

LOAD_FLAG(ENABLE_UPLOAD_PROGRESS, 1 << 7)

LOAD_FLAG(ENABLE_LOAD_TIMING, 1 << 8)

LOAD_FLAG(IGNORE_CERT_COMMON_NAME_INVALID, 1 << 9)

LOAD_FLAG(IGNORE_CERT_DATE_INVALID, 1 << 10)

LOAD_FLAG(IGNORE_CERT_AUTHORITY_INVALID, 1 << 11)

LOAD_FLAG(DISABLE_CERT_REVOCATION_CHECKING, 1 << 12)

LOAD_FLAG(IGNORE_CERT_WRONG_USAGE, 1 << 13)

LOAD_FLAG(DO_NOT_SAVE_COOKIES, 1 << 14)

LOAD_FLAG(BYPASS_PROXY, 1 << 15)

LOAD_FLAG(IS_DOWNLOAD, 1 << 16)

LOAD_FLAG(VERIFY_EV_CERT, 1 << 17)

LOAD_FLAG(DO_NOT_SEND_COOKIES, 1 << 18)

LOAD_FLAG(DO_NOT_SEND_AUTH_DATA, 1 << 19)

LOAD_FLAG(IGNORE_ALL_CERT_ERRORS, 1 << 20)

LOAD_FLAG(MAIN_FRAME, 1 << 21)

LOAD_FLAG(SUB_FRAME, 1 << 22)

LOAD_FLAG(REPORT_RAW_HEADERS, 1 << 23)

LOAD_FLAG(PREFETCH, 1 << 24)

LOAD_FLAG(IGNORE_LIMITS, 1 << 25)

LOAD_FLAG(DO_NOT_PROMPT_FOR_LOGIN, 1 << 26)

LOAD_FLAG(MAYBE_USER_GESTURE, 1 << 27)

LOAD_FLAG(DO_NOT_USE_EMBEDDED_IDENTITY, 1 << 28)
