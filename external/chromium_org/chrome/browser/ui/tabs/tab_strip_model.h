// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_H_
#define CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/models/list_selection_model.h"

class Profile;
class TabStripModelDelegate;
class TabStripModelOrderController;

namespace content {
class WebContents;
}

class TabStripModel {
 public:
  
  enum CloseTypes {
    CLOSE_NONE                     = 0,

    
    
    CLOSE_USER_GESTURE             = 1 << 0,

    
    
    CLOSE_CREATE_HISTORICAL_TAB    = 1 << 1,
  };

  
  enum AddTabTypes {
    
    
    ADD_NONE          = 0,

    
    ADD_ACTIVE        = 1 << 0,

    
    ADD_PINNED        = 1 << 1,

    
    
    
    ADD_FORCE_INDEX   = 1 << 2,

    
    
    
    ADD_INHERIT_GROUP = 1 << 3,

    
    
    
    ADD_INHERIT_OPENER = 1 << 4,
  };

  
  
  enum NewTab {
    
    NEW_TAB_BUTTON,

    
    
    
    
    NEW_TAB_COMMAND,

    
    NEW_TAB_CONTEXT_MENU,

    
    NEW_TAB_ENUM_COUNT,
  };

  static const int kNoTab = -1;

  
  
  TabStripModel(TabStripModelDelegate* delegate, Profile* profile);
  virtual ~TabStripModel();

  
  TabStripModelDelegate* delegate() const { return delegate_; }

  
  void AddObserver(TabStripModelObserver* observer);
  void RemoveObserver(TabStripModelObserver* observer);

  
  int count() const { return static_cast<int>(contents_data_.size()); }
  bool empty() const { return contents_data_.empty(); }

  
  Profile* profile() const { return profile_; }

  
  int active_index() const { return selection_model_.active(); }

  
  
  
  
  bool closing_all() const { return closing_all_; }

  
  TabStripModelOrderController* order_controller() const {
    return order_controller_.get();
  }

  

  
  bool ContainsIndex(int index) const;

  
  
  void AppendWebContents(content::WebContents* contents, bool foreground);

  
  
  
  
  
  
  
  
  
  
  void InsertWebContentsAt(int index,
                           content::WebContents* contents,
                           int add_types);

  
  
  
  
  
  
  bool CloseWebContentsAt(int index, uint32 close_types);

  
  
  
  content::WebContents* ReplaceWebContentsAt(
      int index,
      content::WebContents* new_contents);

  
  
  
  
  content::WebContents* DiscardWebContentsAt(int index);

  
  
  
  
  content::WebContents* DetachWebContentsAt(int index);

  
  
  
  
  void ActivateTabAt(int index, bool user_gesture);

  
  
  void AddTabAtToSelection(int index);

  
  
  
  
  
  
  
  
  void MoveWebContentsAt(int index, int to_position, bool select_after_move);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void MoveSelectedTabsTo(int index);

  
  content::WebContents* GetActiveWebContents() const;

  
  
  content::WebContents* GetWebContentsAt(int index) const;

  
  
  int GetIndexOfWebContents(const content::WebContents* contents) const;

  
  
  void UpdateWebContentsStateAt(
      int index,
      TabStripModelObserver::TabChangeType change_type);

  
  
  
  void CloseAllTabs();

  
  bool TabsAreLoading() const;

  
  
  content::WebContents* GetOpenerOfWebContentsAt(int index);

  
  
  void SetOpenerOfWebContentsAt(int index, content::WebContents* opener);

  
  
  
  
  
  int GetIndexOfNextWebContentsOpenedBy(const content::WebContents* opener,
                                        int start_index,
                                        bool use_group) const;

  
  
  int GetIndexOfLastWebContentsOpenedBy(const content::WebContents* opener,
                                        int start_index) const;

  
  
  
  
  void TabNavigating(content::WebContents* contents,
                     content::PageTransition transition);

  
  
  
  
  
  void ForgetAllOpeners();

  
  
  
  
  
  void ForgetGroup(content::WebContents* contents);

  
  
  bool ShouldResetGroupOnSelect(content::WebContents* contents) const;

  
  void SetTabBlocked(int index, bool blocked);

  
  
  void SetTabPinned(int index, bool pinned);

  
  
  bool IsTabPinned(int index) const;

  
  
  bool IsMiniTab(int index) const;

  
  
  bool IsAppTab(int index) const;

  
  bool IsTabBlocked(int index) const;

  
  
  bool IsTabDiscarded(int index) const;

  
  
  
  int IndexOfFirstNonMiniTab() const;

  
  
  
  
  
  int ConstrainInsertionIndex(int index, bool mini_tab);

  
  void ExtendSelectionTo(int index);

  
  
  void ToggleSelectionAt(int index);

  
  
  void AddSelectionFromAnchorTo(int index);

  
  bool IsTabSelected(int index) const;

  
  void SetSelectionFromModel(const ui::ListSelectionModel& source);

  const ui::ListSelectionModel& selection_model() const {
    return selection_model_;
  }

  

  
  
  
  
  
  void AddWebContents(content::WebContents* contents,
                      int index,
                      content::PageTransition transition,
                      int add_types);

  
  void CloseSelectedTabs();

  
  void SelectNextTab();
  void SelectPreviousTab();

  
  void SelectLastTab();

  
  void MoveTabNext();
  void MoveTabPrevious();

  

  
  enum ContextMenuCommand {
    CommandFirst = 0,
    CommandNewTab,
    CommandReload,
    CommandDuplicate,
    CommandCloseTab,
    CommandCloseOtherTabs,
    CommandCloseTabsToRight,
    CommandRestoreTab,
    CommandTogglePinned,
    CommandBookmarkAllTabs,
    CommandSelectByDomain,
    CommandSelectByOpener,
    CommandLast
  };

  
  
  bool IsContextMenuCommandEnabled(int context_index,
                                   ContextMenuCommand command_id) const;

  
  
  
  void ExecuteContextMenuCommand(int context_index,
                                 ContextMenuCommand command_id);

  
  
  
  std::vector<int> GetIndicesClosedByCommand(int index,
                                             ContextMenuCommand id) const;

  
  
  bool WillContextMenuPin(int index);

  
  
  static bool ContextMenuCommandToBrowserCommand(int cmd_id, int* browser_cmd);

 private:
  class WebContentsData;

  
  enum NotifyTypes {
    NOTIFY_DEFAULT,

    
    NOTIFY_USER_GESTURE,
  };

  
  
  std::vector<content::WebContents*> GetWebContentsFromIndices(
      const std::vector<int>& indices) const;

  
  void GetIndicesWithSameDomain(int index, std::vector<int>* indices);

  
  
  void GetIndicesWithSameOpener(int index, std::vector<int>* indices);

  
  
  
  std::vector<int> GetIndicesForCommand(int index) const;

  
  
  
  
  
  bool IsNewTabAtEndOfTabStrip(content::WebContents* contents) const;

  
  
  
  
  
  
  
  
  bool InternalCloseTabs(const std::vector<int>& indices,
                         uint32 close_types);

  
  
  
  
  
  
  
  
  void InternalCloseTab(content::WebContents* contents,
                        int index,
                        bool create_historical_tabs);

  
  content::WebContents* GetWebContentsAtImpl(int index) const;

  
  void NotifyIfTabDeactivated(content::WebContents* contents);

  
  void NotifyIfActiveTabChanged(content::WebContents* old_contents,
                                NotifyTypes notify_types);

  
  
  
  
  void NotifyIfActiveOrSelectionChanged(
      content::WebContents* old_contents,
      NotifyTypes notify_types,
      const ui::ListSelectionModel& old_model);

  
  
  
  void SetSelection(const ui::ListSelectionModel& new_model,
                    NotifyTypes notify_types);

  
  
  void SelectRelativeTab(bool forward);

  
  
  void MoveWebContentsAtImpl(int index,
                             int to_position,
                             bool select_after_move);

  
  
  void MoveSelectedTabsToImpl(int index, size_t start, size_t length);

  
  
  
  static bool OpenerMatches(const WebContentsData* data,
                            const content::WebContents* opener,
                            bool use_group);

  
  void ForgetOpenersAndGroupsReferencing(const content::WebContents* tab);

  
  TabStripModelDelegate* delegate_;

  
  typedef std::vector<WebContentsData*> WebContentsDataVector;
  WebContentsDataVector contents_data_;

  
  Profile* profile_;

  
  bool closing_all_;

  
  
  scoped_ptr<TabStripModelOrderController> order_controller_;

  
  typedef ObserverList<TabStripModelObserver> TabStripModelObservers;
  TabStripModelObservers observers_;

  ui::ListSelectionModel selection_model_;

  
  bool in_notify_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(TabStripModel);
};

#endif  
