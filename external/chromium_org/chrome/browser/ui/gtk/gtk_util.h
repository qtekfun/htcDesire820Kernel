// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_UTIL_H_
#define CHROME_BROWSER_UI_GTK_GTK_UTIL_H_

#include <gtk/gtk.h>
#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "ui/base/window_open_disposition.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

typedef struct _cairo cairo_t;
typedef struct _GtkWidget GtkWidget;

class BrowserWindow;
class GtkThemeService;
class GURL;
class Profile;

namespace gfx {
class Image;
}

namespace gtk_util {

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

void MakeAppModalWindowGroup();

void AppModalDismissedUngroupWindows();

void RemoveAllChildren(GtkWidget* container);

void ForceFontSizePixels(GtkWidget* widget, double size_pixels);

void UndoForceFontSize(GtkWidget* widget);

gfx::Size GetWidgetSize(GtkWidget* widget);

void ConvertWidgetPointToScreen(GtkWidget* widget, gfx::Point* p);

GtkWidget* CenterWidgetInHBox(GtkWidget* hbox, GtkWidget* widget,
                              bool pack_at_end, int padding);

void SetButtonClickableByMouseButtons(GtkWidget* button,
                                      bool left, bool middle, bool right);

void SetButtonTriggersNavigation(GtkWidget* button);

int MirroredLeftPointForRect(GtkWidget* widget, const gfx::Rect& bounds);

int MirroredRightPointForRect(GtkWidget* widget, const gfx::Rect& bounds);

int MirroredXCoordinate(GtkWidget* widget, int x);

bool WidgetContainsCursor(GtkWidget* widget);

void SetDefaultWindowIcon(GtkWindow* window);

void SetWindowIcon(GtkWindow* window, Profile* profile);

void SetWindowIcon(GtkWindow* window, Profile* profile, GdkPixbuf* icon);

GtkWidget* AddButtonToDialog(GtkWidget* dialog, const gchar* text,
                             const gchar* stock_id, gint response_id);

GtkWidget* BuildDialogButton(GtkWidget* dialog, int ids_id,
                             const gchar* stock_id);

GtkWidget* CreateEntryImageHBox(GtkWidget* entry, GtkWidget* image);

void SetLabelColor(GtkWidget* label, const GdkColor* color);

GtkWidget* IndentWidget(GtkWidget* content);

GdkPoint MakeBidiGdkPoint(gint x, gint y, gint width, bool ltr);

std::string BuildTooltipTitleFor(base::string16 title, const GURL& url);

void DrawTextEntryBackground(GtkWidget* offscreen_entry,
                             GtkWidget* widget_to_draw_on,
                             GdkRectangle* dirty_rec,
                             GdkRectangle* rec);

void SetLayoutText(PangoLayout* layout, const base::string16& text);

void DrawThemedToolbarBackground(GtkWidget* widget,
                                 cairo_t* cr,
                                 GdkEventExpose* event,
                                 const gfx::Point& tabstrip_origin,
                                 GtkThemeService* provider);

void DrawFullImage(cairo_t* cr,
                   GtkWidget* widget,
                   const gfx::Image& image,
                   gint dest_x,
                   gint dest_y);

GdkColor AverageColors(GdkColor color_one, GdkColor color_two);

void SetAlwaysShowImage(GtkWidget* image_menu_item);

gfx::Rect GetWidgetRectRelativeToToplevel(GtkWidget* widget);

void SuppressDefaultPainting(GtkWidget* container);

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

gfx::Rect GetDialogBounds(GtkWidget* dialog);

base::string16 GetStockPreferencesMenuLabel();

bool IsWidgetAncestryVisible(GtkWidget* widget);

void SetLabelWidth(GtkWidget* label, int pixel_width);

void InitLabelSizeRequestAndEllipsizeMode(GtkWidget* label);

void ApplyMessageDialogQuirks(GtkWidget* dialog);

}  

#endif  
