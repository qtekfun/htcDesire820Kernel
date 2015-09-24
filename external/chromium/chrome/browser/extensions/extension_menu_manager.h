// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_MENU_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_MENU_MANAGER_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/extensions/extension_icon_manager.h"
#include "chrome/common/extensions/extension_extent.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

struct ContextMenuParams;

class Extension;
class Profile;
class SkBitmap;
class TabContents;

class ExtensionMenuItem {
 public:
  
  typedef std::vector<ExtensionMenuItem*> List;

  
  struct Id {
    Id();
    Id(Profile* profile, const std::string& extension_id, int uid);
    ~Id();

    bool operator==(const Id& other) const;
    bool operator!=(const Id& other) const;
    bool operator<(const Id& other) const;

    Profile* profile;
    std::string extension_id;
    int uid;
  };

  
  enum Context {
    ALL = 1,
    PAGE = 2,
    SELECTION = 4,
    LINK = 8,
    EDITABLE = 16,
    IMAGE = 32,
    VIDEO = 64,
    AUDIO = 128,
    FRAME = 256,
  };

  
  enum Type {
    NORMAL,
    CHECKBOX,
    RADIO,
    SEPARATOR
  };

  
  class ContextList {
   public:
    ContextList() : value_(0) {}
    explicit ContextList(Context context) : value_(context) {}
    ContextList(const ContextList& other) : value_(other.value_) {}

    void operator=(const ContextList& other) {
      value_ = other.value_;
    }

    bool operator==(const ContextList& other) const {
      return value_ == other.value_;
    }

    bool operator!=(const ContextList& other) const {
      return !(*this == other);
    }

    bool Contains(Context context) const {
      return (value_ & context) > 0;
    }

    void Add(Context context) {
      value_ |= context;
    }

   private:
    uint32 value_;  
  };

  ExtensionMenuItem(const Id& id,
                    const std::string& title,
                    bool checked,
                    Type type,
                    const ContextList& contexts);
  virtual ~ExtensionMenuItem();

  
  const std::string& extension_id() const { return id_.extension_id; }
  const std::string& title() const { return title_; }
  const List& children() { return children_; }
  const Id& id() const { return id_; }
  Id* parent_id() const { return parent_id_.get(); }
  int child_count() const { return children_.size(); }
  ContextList contexts() const { return contexts_; }
  Type type() const { return type_; }
  bool checked() const { return checked_; }
  const ExtensionExtent& document_url_patterns() const {
    return document_url_patterns_;
  }
  const ExtensionExtent& target_url_patterns() const {
    return target_url_patterns_;
  }

  
  void set_title(const std::string& new_title) { title_ = new_title; }
  void set_contexts(ContextList contexts) { contexts_ = contexts; }
  void set_type(Type type) { type_ = type; }
  void set_document_url_patterns(const ExtensionExtent& patterns) {
    document_url_patterns_ = patterns;
  }
  void set_target_url_patterns(const ExtensionExtent& patterns) {
    target_url_patterns_ = patterns;
  }

  
  
  string16 TitleWithReplacement(const string16& selection,
                                size_t max_length) const;

  
  bool SetChecked(bool checked);

 protected:
  friend class ExtensionMenuManager;

  
  void AddChild(ExtensionMenuItem* item);

  
  
  ExtensionMenuItem* ReleaseChild(const Id& child_id, bool recursive);

  
  
  std::set<Id> RemoveAllDescendants();

 private:
  
  Id id_;

  
  std::string title_;

  Type type_;

  
  bool checked_;

  
  ContextList contexts_;

  
  
  scoped_ptr<Id> parent_id_;

  
  
  ExtensionExtent document_url_patterns_;

  
  
  ExtensionExtent target_url_patterns_;

  
  List children_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionMenuItem);
};

class ExtensionMenuManager : public NotificationObserver {
 public:
  
  
  static const int kAllowedSchemes;

  ExtensionMenuManager();
  virtual ~ExtensionMenuManager();

  
  std::set<std::string> ExtensionIds();

  
  
  
  
  
  const ExtensionMenuItem::List* MenuItems(const std::string& extension_id);

  
  
  
  bool AddContextItem(const Extension* extension, ExtensionMenuItem* item);

  
  
  bool AddChildItem(const ExtensionMenuItem::Id& parent_id,
                    ExtensionMenuItem* child);

  
  
  
  
  
  bool ChangeParent(const ExtensionMenuItem::Id& child_id,
                    const ExtensionMenuItem::Id* parent_id);

  
  
  
  bool RemoveContextMenuItem(const ExtensionMenuItem::Id& id);

  
  void RemoveAllContextItems(const std::string& extension_id);

  
  ExtensionMenuItem* GetItemById(const ExtensionMenuItem::Id& id) const;

  
  void ExecuteCommand(Profile* profile, TabContents* tab_contents,
                      const ContextMenuParams& params,
                      const ExtensionMenuItem::Id& menuItemId);

  
  
  
  const SkBitmap& GetIconForExtension(const std::string& extension_id);

  
  virtual void Observe(NotificationType type, const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  static bool HasAllowedScheme(const GURL& url);

 private:
  FRIEND_TEST_ALL_PREFIXES(ExtensionMenuManagerTest, DeleteParent);
  FRIEND_TEST_ALL_PREFIXES(ExtensionMenuManagerTest, RemoveOneByOne);

  
  
  void RadioItemSelected(ExtensionMenuItem* item);

  
  bool DescendantOf(ExtensionMenuItem* item,
                    const ExtensionMenuItem::Id& ancestor_id);

  
  typedef std::map<std::string, ExtensionMenuItem::List> MenuItemMap;
  MenuItemMap context_items_;

  
  
  
  std::map<ExtensionMenuItem::Id, ExtensionMenuItem*> items_by_id_;

  NotificationRegistrar registrar_;

  ExtensionIconManager icon_manager_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionMenuManager);
};

#endif  
