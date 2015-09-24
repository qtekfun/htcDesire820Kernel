// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_UTIL_H_
#define CHROME_BROWSER_UI_GTK_GTK_UTIL_H_
#pragma once

#include <gtk/gtk.h>
#include <string>
#include <vector>

#include "base/string16.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebDragOperation.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "webkit/glue/window_open_disposition.h"

typedef struct _cairo cairo_t;
typedef struct _GdkColor GdkColor;
typedef struct _GtkWidget GtkWidget;

class BrowserWindow;
class GtkThemeService;
class GURL;
class Profile;
struct RendererPreferences;  

const int kSkiaToGDKMultiplier = 257;

#define GDK_COLOR_RGB(r, g, b) {0, r * kSkiaToGDKMultiplier, \
        g * kSkiaToGDKMultiplier, b * kSkiaToGDKMultiplier}

namespace event_utils {

WindowOpenDisposition DispositionFromEventFlags(guint state);

}  

namespace gtk_util {

extern const GdkColor kGdkWhite;
extern const GdkColor kGdkGray;
extern const GdkColor kGdkBlack;
extern const GdkColor kGdkGreen;


const int kControlSpacing = 6;

const int kLabelSpacing = 12;

const int kGroupIndent = 12;

const int kContentAreaBorder = 12;

const int kContentAreaSpacing = 18;

const int kFormControlSpacing = 10;

GtkWidget* CreateLabeledControlsGroup(
    std::vector<GtkWidget*>* labels,
    const char* text, ...);

GtkWidget* CreateGtkBorderBin(GtkWidget* child, const GdkColor* color,
                              int top, int bottom, int left, int right);

GtkWidget* LeftAlignMisc(GtkWidget* misc);

GtkWidget* CreateBoldLabel(const std::string& text);

void GetWidgetSizeFromCharacters(GtkWidget* widget,
                                 double width_chars, double height_lines,
                                 int* width, int* height);

void GetWidgetSizeFromResources(GtkWidget* widget,
                                int width_chars, int height_lines,
                                int* width, int* height);

void SetWindowSizeFromResources(GtkWindow* window,
                                int width_id, int height_id, bool resizable);

void CenterOverWindow(GtkWindow* window, GtkWindow* parent);

void MakeAppModalWindowGroup();

void AppModalDismissedUngroupWindows();

void RemoveAllChildren(GtkWidget* container);

void ForceFontSizePixels(GtkWidget* widget, double size_pixels);

void UndoForceFontSize(GtkWidget* widget);

gfx::Point GetWidgetScreenPosition(GtkWidget* widget);

gfx::Rect GetWidgetScreenBounds(GtkWidget* widget);

gfx::Size GetWidgetSize(GtkWidget* widget);

void ConvertWidgetPointToScreen(GtkWidget* widget, gfx::Point* p);

void InitRCStyles();

GtkWidget* CenterWidgetInHBox(GtkWidget* hbox, GtkWidget* widget,
                              bool pack_at_end, int padding);

bool IsScreenComposited();

void EnumerateTopLevelWindows(ui::EnumerateWindowsDelegate* delegate);

void SetButtonClickableByMouseButtons(GtkWidget* button,
                                      bool left, bool middle, bool right);

void SetButtonTriggersNavigation(GtkWidget* button);

int MirroredLeftPointForRect(GtkWidget* widget, const gfx::Rect& bounds);

int MirroredXCoordinate(GtkWidget* widget, int x);

bool WidgetContainsCursor(GtkWidget* widget);

void SetWindowIcon(GtkWindow* window);

void SetDefaultWindowIcon(GtkWindow* window);

GtkWidget* AddButtonToDialog(GtkWidget* dialog, const gchar* text,
                             const gchar* stock_id, gint response_id);

GtkWidget* BuildDialogButton(GtkWidget* dialog, int ids_id,
                             const gchar* stock_id);

GtkWidget* CreateEntryImageHBox(GtkWidget* entry, GtkWidget* image);

void SetLabelColor(GtkWidget* label, const GdkColor* color);

GtkWidget* IndentWidget(GtkWidget* content);

void UpdateGtkFontSettings(RendererPreferences* prefs);

gfx::Point ScreenPoint(GtkWidget* widget);

gfx::Point ClientPoint(GtkWidget* widget);

GdkPoint MakeBidiGdkPoint(gint x, gint y, gint width, bool ltr);

void DrawTextEntryBackground(GtkWidget* offscreen_entry,
                             GtkWidget* widget_to_draw_on,
                             GdkRectangle* dirty_rec,
                             GdkRectangle* rec);

void DrawThemedToolbarBackground(GtkWidget* widget,
                                 cairo_t* cr,
                                 GdkEventExpose* event,
                                 const gfx::Point& tabstrip_origin,
                                 GtkThemeService* provider);

GdkColor AverageColors(GdkColor color_one, GdkColor color_two);

void SetAlwaysShowImage(GtkWidget* image_menu_item);

gfx::Rect GetWidgetRectRelativeToToplevel(GtkWidget* widget);

void SuppressDefaultPainting(GtkWidget* container);

WindowOpenDisposition DispositionForCurrentButtonPressEvent();

bool GrabAllInput(GtkWidget* widget);

gfx::Rect WidgetBounds(GtkWidget* widget);

void SetWMLastUserActionTime(GtkWindow* window);

guint32 XTimeNow();

bool URLFromPrimarySelection(Profile* profile, GURL* url);

bool AddWindowAlphaChannel(GtkWidget* window);

void GetTextColors(GdkColor* normal_base,
                   GdkColor* selected_base,
                   GdkColor* normal_text,
                   GdkColor* selected_text);

void ShowDialog(GtkWidget* dialog);
void ShowDialogWithLocalizedSize(GtkWidget* dialog,
                                 int width_id,
                                 int height_id,
                                 bool resizeable);
void ShowDialogWithMinLocalizedWidth(GtkWidget* dialog,
                                     int width_id);

void PresentWindow(GtkWidget* window, int timestamp);

GtkWindow* GetDialogWindow(GtkWidget* dialog);

gfx::Rect GetDialogBounds(GtkWidget* dialog);

string16 GetStockPreferencesMenuLabel();

bool IsWidgetAncestryVisible(GtkWidget* widget);

void SetLabelWidth(GtkWidget* label, int pixel_width);

void InitLabelSizeRequestAndEllipsizeMode(GtkWidget* label);

GdkDragAction WebDragOpToGdkDragAction(WebKit::WebDragOperationsMask op);
WebKit::WebDragOperationsMask GdkDragActionToWebDragOp(GdkDragAction action);

void ApplyMessageDialogQuirks(GtkWidget* dialog);

void DoCut(BrowserWindow* window);
void DoCopy(BrowserWindow* window);
void DoPaste(BrowserWindow* window);

}  

#endif  
