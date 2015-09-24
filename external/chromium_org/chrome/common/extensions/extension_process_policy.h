// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_PROCESS_POLICY_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_PROCESS_POLICY_H_

class ExtensionSet;
class GURL;

namespace extensions {

class Extension;

const Extension* GetNonBookmarkAppExtension(const ExtensionSet& extensions,
                                            const GURL& url);

bool CrossesExtensionProcessBoundary(
    const ExtensionSet& extensions,
    const GURL& old_url,
    const GURL& new_url,
    bool should_consider_workaround);

}  

#endif  
