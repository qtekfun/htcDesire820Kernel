// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_ABOUT_CHROME_DIALOG_H_
#define CHROME_BROWSER_UI_GTK_ABOUT_CHROME_DIALOG_H_
#pragma once

class Profile;
typedef struct _GtkWindow GtkWindow;

void ShowAboutDialogForProfile(GtkWindow* parent, Profile* profile);

#endif  
