// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_TEST_TEST_LAYERS_H_
#define UI_COMPOSITOR_TEST_TEST_LAYERS_H_

#include <string>

namespace ui {
class Layer;

namespace test {

std::string ChildLayerNamesAsString(const ui::Layer& parent);

}  
}  

#endif  
