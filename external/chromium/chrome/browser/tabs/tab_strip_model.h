// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_TAB_STRIP_MODEL_H_
#define CHROME_BROWSER_TABS_TAB_STRIP_MODEL_H_
#pragma once

#include <vector>

#include "base/observer_list.h"
#include "chrome/browser/tabs/tab_strip_model_observer.h"
#include "chrome/browser/tabs/tab_strip_selection_model.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/page_transition_types.h"

class NavigationController;
class Profile;
class TabContents;
class TabContentsWrapper;
class TabStripModelDelegate;
class TabStripModelOrderController;

class TabStripModel : public NotificationObserver {
 public:
  
  enum InsertionPolicy {
    
    INSERT_AFTER,

    
    INSERT_BEFORE,
  };

  
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
    return order_controller_;
  }

  
  void SetInsertionPolicy(InsertionPolicy policy);
  InsertionPolicy insertion_policy() const;

  
  
  bool HasObserver(TabStripModelObserver* observer);

  

  
  bool ContainsIndex(int index) const;

  
  
  void AppendTabContents(TabContentsWrapper* contents, bool foreground);

  
  
  
  
  
  
  
  
  
  
  void InsertTabContentsAt(int index,
                           TabContentsWrapper* contents,
                           int add_types);

  
  
  
  
  
  
  bool CloseTabContentsAt(int index, uint32 close_types);

  
  
  
  
  
  
  
  
  void ReplaceNavigationControllerAt(int index,
                                     TabContentsWrapper* contents);

  
  
  
  TabContentsWrapper* ReplaceTabContentsAt(int index,
                                           TabContentsWrapper* new_contents);

  
  
  
  
  TabContentsWrapper* DetachTabContentsAt(int index);

  
  
  
  
  void ActivateTabAt(int index, bool user_gesture);

  
  
  
  
  
  
  
  
  void MoveTabContentsAt(int index, int to_position, bool select_after_move);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void MoveSelectedTabsTo(int index);

  
  
  TabContentsWrapper* GetSelectedTabContents() const;

  
  
  TabContentsWrapper* GetTabContentsAt(int index) const;

  
  
  int GetIndexOfTabContents(const TabContentsWrapper* contents) const;

  
  
  
  
  
  
  int GetWrapperIndex(const TabContents* contents) const;

  
  
  int GetIndexOfController(const NavigationController* controller) const;

  
  
  void UpdateTabContentsStateAt(
      int index,
      TabStripModelObserver::TabChangeType change_type);

  
  
  
  void EnsureNewTabVisible(bool force_create);

  
  
  
  void CloseAllTabs();

  
  bool TabsAreLoading() const;

  
  NavigationController* GetOpenerOfTabContentsAt(int index);

  
  
  
  
  
  int GetIndexOfNextTabContentsOpenedBy(const NavigationController* opener,
                                        int start_index,
                                        bool use_group) const;

  
  
  int GetIndexOfFirstTabContentsOpenedBy(const NavigationController* opener,
                                         int start_index) const;

  
  
  int GetIndexOfLastTabContentsOpenedBy(const NavigationController* opener,
                                        int start_index) const;

  
  
  
  
  void TabNavigating(TabContentsWrapper* contents,
                     PageTransition::Type transition);

  
  
  
  
  
  void ForgetAllOpeners();

  
  
  
  
  
  void ForgetGroup(TabContentsWrapper* contents);

  
  
  bool ShouldResetGroupOnSelect(TabContentsWrapper* contents) const;

  
  void SetTabBlocked(int index, bool blocked);

  
  
  void SetTabPinned(int index, bool pinned);

  
  
  bool IsTabPinned(int index) const;

  
  
  bool IsMiniTab(int index) const;

  
  
  bool IsAppTab(int index) const;

  
  bool IsTabBlocked(int index) const;

  
  
  
  int IndexOfFirstNonMiniTab() const;

  
  
  
  
  
  int ConstrainInsertionIndex(int index, bool mini_tab);

  
  void ExtendSelectionTo(int index);

  
  
  void ToggleSelectionAt(int index);

  
  
  void AddSelectionFromAnchorTo(int index);

  
  bool IsTabSelected(int index) const;

  
  void SetSelectionFromModel(const TabStripSelectionModel& source);

  const TabStripSelectionModel& selection_model() const {
    return selection_model_;
  }

  

  
  
  
  
  void AddTabContents(TabContentsWrapper* contents,
                      int index,
                      PageTransition::Type transition,
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
    CommandUseVerticalTabs,
    CommandSelectByDomain,
    CommandSelectByOpener,
    CommandLast
  };

  
  
  bool IsContextMenuCommandEnabled(int context_index,
                                   ContextMenuCommand command_id) const;

  
  bool IsContextMenuCommandChecked(int context_index,
                                   ContextMenuCommand command_id) const;

  
  
  
  void ExecuteContextMenuCommand(int context_index,
                                 ContextMenuCommand command_id);

  
  
  
  std::vector<int> GetIndicesClosedByCommand(int index,
                                             ContextMenuCommand id) const;

  
  
  bool WillContextMenuPin(int index);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  static bool ContextMenuCommandToBrowserCommand(int cmd_id, int* browser_cmd);

 private:
  
  void GetIndicesWithSameDomain(int index, std::vector<int>* indices);

  
  
  void GetIndicesWithSameOpener(int index, std::vector<int>* indices);

  
  
  
  std::vector<int> GetIndicesForCommand(int index) const;

  
  
  
  
  
  bool IsNewTabAtEndOfTabStrip(TabContentsWrapper* contents) const;

  
  
  
  
  
  
  
  
  bool InternalCloseTabs(const std::vector<int>& indices, uint32 close_types);

  
  
  
  
  
  
  
  
  void InternalCloseTab(TabContentsWrapper* contents,
                        int index,
                        bool create_historical_tabs);

  TabContentsWrapper* GetContentsAt(int index) const;

  
  
  void NotifyTabSelectedIfChanged(TabContentsWrapper* old_contents,
                                  int to_index,
                                  bool user_gesture);

  
  
  void NotifySelectionChanged(int old_selected_index);

  
  int GetNewTabCount() const;

  
  
  void SelectRelativeTab(bool forward);

  
  
  void MoveTabContentsAtImpl(int index,
                             int to_position,
                             bool select_after_move);

  
  
  void MoveSelectedTabsToImpl(int index, size_t start, size_t length);

  
  
  
  struct TabContentsData;
  static bool OpenerMatches(const TabContentsData* data,
                            const NavigationController* opener,
                            bool use_group);

  
  void ForgetOpenersAndGroupsReferencing(const NavigationController* tab);

  
  TabStripModelDelegate* delegate_;

  
  
  
  
  struct TabContentsData {
    explicit TabContentsData(TabContentsWrapper* a_contents)
        : contents(a_contents),
          reset_group_on_select(false),
          pinned(false),
          blocked(false) {
      SetGroup(NULL);
    }

    
    
    void SetGroup(NavigationController* a_group) {
      group = a_group;
      opener = a_group;
    }

    
    
    void ForgetOpener() {
      opener = NULL;
    }

    TabContentsWrapper* contents;
    
    
    
    
    
    
    
    
    
    
    NavigationController* group;
    
    
    
    
    NavigationController* opener;
    
    
    
    
    
    
    bool reset_group_on_select;

    
    bool pinned;

    
    bool blocked;
  };

  
  typedef std::vector<TabContentsData*> TabContentsDataVector;
  TabContentsDataVector contents_data_;

  
  Profile* profile_;

  
  bool closing_all_;

  
  
  TabStripModelOrderController* order_controller_;

  
  typedef ObserverList<TabStripModelObserver> TabStripModelObservers;
  TabStripModelObservers observers_;

  
  NotificationRegistrar registrar_;

  TabStripSelectionModel selection_model_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(TabStripModel);
};

#endif  
