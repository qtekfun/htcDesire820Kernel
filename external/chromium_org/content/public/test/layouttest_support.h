// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_LAYOUTTEST_SUPPORT_H_
#define CONTENT_PUBLIC_TEST_LAYOUTTEST_SUPPORT_H_

#include "base/callback_forward.h"

namespace blink {
class WebDeviceMotionData;
class WebDeviceOrientationData;
class WebGamepads;
struct WebSize;
}

namespace WebTestRunner {
class WebTestProxyBase;
}

namespace content {

class RenderView;

void EnableBrowserLayoutTestMode();


void EnableRendererLayoutTestMode();

void EnableWebTestProxyCreation(const base::Callback<
    void(RenderView*, WebTestRunner::WebTestProxyBase*)>& callback);

void SetMockGamepads(const blink::WebGamepads& pads);

void SetMockDeviceMotionData(const blink::WebDeviceMotionData& data);

void SetMockDeviceOrientationData(const blink::WebDeviceOrientationData& data);

int GetLocalSessionHistoryLength(RenderView* render_view);

void SyncNavigationState(RenderView* render_view);

void SetFocusAndActivate(RenderView* render_view, bool enable);

void ForceResizeRenderView(RenderView* render_view,
                           const blink::WebSize& new_size);

void SetDeviceScaleFactor(RenderView* render_view, float factor);

// some tests that were written before browsers had multi-process architecture
void UseSynchronousResizeMode(RenderView* render_view, bool enable);

void EnableAutoResizeMode(RenderView* render_view,
                          const blink::WebSize& min_size,
                          const blink::WebSize& max_size);
void DisableAutoResizeMode(RenderView* render_view,
                           const blink::WebSize& new_size);

void UseMockMediaStreams(RenderView* render_view);

}  

#endif  
