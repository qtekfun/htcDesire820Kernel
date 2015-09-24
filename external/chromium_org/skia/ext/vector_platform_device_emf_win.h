// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_VECTOR_PLATFORM_DEVICE_EMF_WIN_H_
#define SKIA_EXT_VECTOR_PLATFORM_DEVICE_EMF_WIN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "skia/ext/platform_device.h"
#include "third_party/skia/include/core/SkMatrix.h"
#include "third_party/skia/include/core/SkRegion.h"

namespace skia {

class VectorPlatformDeviceEmf : public SkBitmapDevice, public PlatformDevice {
 public:
  SK_API static SkBaseDevice* CreateDevice(int width, int height, bool isOpaque,
                                           HANDLE shared_section);

  
  static SkBaseDevice* create(HDC dc, int width, int height);

  VectorPlatformDeviceEmf(HDC dc, const SkBitmap& bitmap);
  virtual ~VectorPlatformDeviceEmf();

  
  virtual PlatformSurface BeginPlatformPaint() OVERRIDE;
  virtual void DrawToNativeContext(HDC dc, int x, int y,
                                   const RECT* src_rect) OVERRIDE;
  
  virtual uint32_t getDeviceCapabilities();
  virtual void drawPaint(const SkDraw& draw, const SkPaint& paint) OVERRIDE;
  virtual void drawPoints(const SkDraw& draw, SkCanvas::PointMode mode,
                          size_t count, const SkPoint[],
                          const SkPaint& paint) OVERRIDE;
  virtual void drawRect(const SkDraw& draw, const SkRect& r,
                        const SkPaint& paint) OVERRIDE;
  virtual void drawRRect(const SkDraw&, const SkRRect& rr,
                         const SkPaint& paint) OVERRIDE;
  virtual void drawPath(const SkDraw& draw, const SkPath& path,
                        const SkPaint& paint,
                        const SkMatrix* prePathMatrix = NULL,
                        bool pathIsMutable = false) OVERRIDE;
  virtual void drawBitmapRect(const SkDraw& draw, const SkBitmap& bitmap,
                              const SkRect* src, const SkRect& dst,
                              const SkPaint& paint,
                              SkCanvas::DrawBitmapRectFlags flags) SK_OVERRIDE;
  virtual void drawBitmap(const SkDraw& draw, const SkBitmap& bitmap,
                          const SkMatrix& matrix,
                          const SkPaint& paint) OVERRIDE;
  virtual void drawSprite(const SkDraw& draw, const SkBitmap& bitmap,
                          int x, int y, const SkPaint& paint) OVERRIDE;
  virtual void drawText(const SkDraw& draw, const void* text, size_t len,
                        SkScalar x, SkScalar y, const SkPaint& paint) OVERRIDE;
  virtual void drawPosText(const SkDraw& draw, const void* text, size_t len,
                           const SkScalar pos[], SkScalar constY,
                           int scalarsPerPos, const SkPaint& paint) OVERRIDE;
  virtual void drawTextOnPath(const SkDraw& draw, const void* text, size_t len,
                              const SkPath& path, const SkMatrix* matrix,
                              const SkPaint& paint) OVERRIDE;
  virtual void drawVertices(const SkDraw& draw, SkCanvas::VertexMode,
                            int vertexCount,
                            const SkPoint verts[], const SkPoint texs[],
                            const SkColor colors[], SkXfermode* xmode,
                            const uint16_t indices[], int indexCount,
                            const SkPaint& paint) OVERRIDE;
  virtual void drawDevice(const SkDraw& draw, SkBaseDevice*, int x, int y,
                          const SkPaint&) OVERRIDE;

  virtual void setMatrixClip(const SkMatrix& transform, const SkRegion& region,
                             const SkClipStack&) OVERRIDE;

  void LoadClipRegion();

 protected:
  virtual SkBaseDevice* onCreateCompatibleDevice(SkBitmap::Config, int width,
                                                 int height, bool isOpaque,
                                                 Usage usage) OVERRIDE;

 private:
  
  
  
  bool ApplyPaint(const SkPaint& paint);

  
  
  HGDIOBJ SelectObject(HGDIOBJ object);

  
  bool CreateBrush(bool use_brush, const SkPaint& paint);

  
  bool CreatePen(bool use_pen, const SkPaint& paint);

  
  void Cleanup();

  
  bool CreateBrush(bool use_brush, COLORREF color);

  
  bool CreatePen(bool use_pen, COLORREF color, int stroke_width,
                 float stroke_miter, DWORD pen_style);

  
  void InternalDrawBitmap(const SkBitmap& bitmap, int x, int y,
                          const SkPaint& paint);

  
  
  HDC hdc_;

  
  
  SkMatrix transform_;

  
  SkRegion clip_region_;

  
  HGDIOBJ previous_brush_;

  
  HGDIOBJ previous_pen_;

  DISALLOW_COPY_AND_ASSIGN(VectorPlatformDeviceEmf);
};

typedef void (*SkiaEnsureTypefaceCharactersAccessible)
    (const LOGFONT& font, const wchar_t* text, unsigned int text_length);

SK_API void SetSkiaEnsureTypefaceCharactersAccessible(
    SkiaEnsureTypefaceCharactersAccessible func);

}  

#endif  
