// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_HUNG_RENDERER_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_HUNG_RENDERER_VIEW_H_

#include "base/memory/scoped_vector.h"
#include "chrome/browser/favicon/favicon_tab_helper.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/base/models/table_model.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/table/table_grouper.h"
#include "ui/views/controls/table/table_view.h"
#include "ui/views/window/dialog_delegate.h"

namespace content {
class WebContents;
}

namespace views {
class LabelButton;
}

class HungPagesTableModel : public ui::TableModel, public views::TableGrouper {
 public:
  
  
  class Delegate {
   public:
    virtual void TabDestroyed() = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit HungPagesTableModel(Delegate* delegate);
  virtual ~HungPagesTableModel();

  void InitForWebContents(content::WebContents* hung_contents);

  
  
  content::RenderProcessHost* GetRenderProcessHost();

  
  content::RenderViewHost* GetRenderViewHost();

  
  virtual int RowCount() OVERRIDE;
  virtual base::string16 GetText(int row, int column_id) OVERRIDE;
  virtual gfx::ImageSkia GetIcon(int row) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;

  
  virtual void GetGroupRange(int model_index,
                             views::GroupRange* range) OVERRIDE;

 private:
  
  
  class WebContentsObserverImpl : public content::WebContentsObserver {
   public:
    WebContentsObserverImpl(HungPagesTableModel* model,
                            content::WebContents* tab);

    content::WebContents* web_contents() const {
      return content::WebContentsObserver::web_contents();
    }

    FaviconTabHelper* favicon_tab_helper() {
      return FaviconTabHelper::FromWebContents(web_contents());
    }

    
    virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
    virtual void WebContentsDestroyed(content::WebContents* tab) OVERRIDE;

   private:
    HungPagesTableModel* model_;

    DISALLOW_COPY_AND_ASSIGN(WebContentsObserverImpl);
  };

  
  
  void TabDestroyed(WebContentsObserverImpl* tab);

  typedef ScopedVector<WebContentsObserverImpl> TabObservers;
  TabObservers tab_observers_;

  ui::TableModelObserver* observer_;
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(HungPagesTableModel);
};

class HungRendererDialogView : public views::DialogDelegateView,
                               public views::ButtonListener,
                               public HungPagesTableModel::Delegate {
 public:
  
  
  static HungRendererDialogView* Create(gfx::NativeView context);

  
  static HungRendererDialogView* GetInstance();

  
  
  static void KillRendererProcess(base::ProcessHandle process_handle);

  
  static bool IsFrameActive(content::WebContents* contents);

  virtual void ShowForWebContents(content::WebContents* contents);
  virtual void EndForWebContents(content::WebContents* contents);

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual int GetDialogButtons() const OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual views::View* CreateExtraView() OVERRIDE;
  virtual bool Accept(bool window_closing)  OVERRIDE;
  virtual bool UseNewStyleForThisDialog() const OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void TabDestroyed() OVERRIDE;

 protected:
  HungRendererDialogView();
  virtual ~HungRendererDialogView();

  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  static HungRendererDialogView* g_instance_;

 private:
  
  void Init();

  static void InitClass();

  
  views::TableView* hung_pages_table_;

  
  views::LabelButton* kill_button_;

  
  
  scoped_ptr<HungPagesTableModel> hung_pages_table_model_;

  
  bool initialized_;

  
  static gfx::ImageSkia* frozen_icon_;

  DISALLOW_COPY_AND_ASSIGN(HungRendererDialogView);
};

#endif  
