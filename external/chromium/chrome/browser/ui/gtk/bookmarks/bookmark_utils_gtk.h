// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_UTILS_GTK_H_
#define CHROME_BROWSER_UI_GTK_BOOKMARKS_BOOKMARK_UTILS_GTK_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "ui/base/gtk/gtk_integers.h"

class BookmarkModel;
class BookmarkNode;
class GtkThemeService;
class Profile;

typedef struct _GdkDragContext GdkDragContext;
typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GtkSelectionData GtkSelectionData;
typedef struct _GtkWidget GtkWidget;

namespace bookmark_utils {

extern const char kBookmarkNode[];

GdkPixbuf* GetPixbufForNode(const BookmarkNode* node, BookmarkModel* model,
                            bool native);

GtkWidget* GetDragRepresentation(GdkPixbuf* pixbuf,
                                 const string16& title,
                                 GtkThemeService* provider);
GtkWidget* GetDragRepresentationForNode(const BookmarkNode* node,
                                        BookmarkModel* model,
                                        GtkThemeService* provider);

void ConfigureButtonForNode(const BookmarkNode* node, BookmarkModel* model,
                            GtkWidget* button, GtkThemeService* provider);

std::string BuildTooltipFor(const BookmarkNode* node);

const BookmarkNode* BookmarkNodeForWidget(GtkWidget* widget);

void SetButtonTextColors(GtkWidget* label, GtkThemeService* provider);


int GetCodeMask(bool folder);

void WriteBookmarkToSelection(const BookmarkNode* node,
                              GtkSelectionData* selection_data,
                              guint target_type,
                              Profile* profile);

void WriteBookmarksToSelection(const std::vector<const BookmarkNode*>& nodes,
                               GtkSelectionData* selection_data,
                               guint target_type,
                               Profile* profile);

std::vector<const BookmarkNode*> GetNodesFromSelection(
    GdkDragContext* context,
    GtkSelectionData* selection_data,
    guint target_type,
    Profile* profile,
    gboolean* delete_selection_data,
    gboolean* dnd_success);

bool CreateNewBookmarkFromNamedUrl(
    GtkSelectionData* selection_data,
    BookmarkModel* model,
    const BookmarkNode* parent,
    int idx);

bool CreateNewBookmarksFromURIList(
    GtkSelectionData* selection_data,
    BookmarkModel* model,
    const BookmarkNode* parent,
    int idx);

bool CreateNewBookmarkFromNetscapeURL(
    GtkSelectionData* selection_data,
    BookmarkModel* model,
    const BookmarkNode* parent,
    int idx);

}  

#endif  
