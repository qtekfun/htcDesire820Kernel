// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_MENU_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_MENU_MANAGER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/values.h"
#include "chrome/browser/extensions/extension_icon_manager.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/url_pattern_set.h"


class Profile;
class SkBitmap;

namespace content {
class WebContents;
struct ContextMenuParams;
}

namespace extensions {
class Extension;
class StateStore;

class MenuItem {
 public:
  
  typedef std::vector<MenuItem*> List;

  
  struct Id {
    Id();
    
    
    
    Id(bool incognito, const std::string& extension_id);
    ~Id();

    bool operator==(const Id& other) const;
    bool operator!=(const Id& other) const;
    bool operator<(const Id& other) const;

    bool incognito;
    std::string extension_id;
    
    int uid;
    std::string string_uid;
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
    LAUNCHER = 512
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

    scoped_ptr<base::Value> ToValue() const {
      return scoped_ptr<base::Value>(
          new base::FundamentalValue(static_cast<int>(value_)));
    }

    bool Populate(const base::Value& value) {
      int int_value;
      if (!value.GetAsInteger(&int_value) || int_value < 0)
        return false;
      value_ = int_value;
      return true;
    }

   private:
    uint32 value_;  
  };

  MenuItem(const Id& id,
           const std::string& title,
           bool checked,
           bool enabled,
           Type type,
           const ContextList& contexts);
  virtual ~MenuItem();

  
  bool incognito() const { return id_.incognito; }
  const std::string& extension_id() const { return id_.extension_id; }
  const std::string& title() const { return title_; }
  const List& children() { return children_; }
  const Id& id() const { return id_; }
  Id* parent_id() const { return parent_id_.get(); }
  int child_count() const { return children_.size(); }
  ContextList contexts() const { return contexts_; }
  Type type() const { return type_; }
  bool checked() const { return checked_; }
  bool enabled() const { return enabled_; }
  const URLPatternSet& document_url_patterns() const {
    return document_url_patterns_;
  }
  const URLPatternSet& target_url_patterns() const {
    return target_url_patterns_;
  }

  
  void set_title(const std::string& new_title) { title_ = new_title; }
  void set_contexts(ContextList contexts) { contexts_ = contexts; }
  void set_type(Type type) { type_ = type; }
  void set_enabled(bool enabled) { enabled_ = enabled; }
  void set_document_url_patterns(const URLPatternSet& patterns) {
    document_url_patterns_ = patterns;
  }
  void set_target_url_patterns(const URLPatternSet& patterns) {
    target_url_patterns_ = patterns;
  }

  
  
  base::string16 TitleWithReplacement(const base::string16& selection,
                                size_t max_length) const;

  
  bool SetChecked(bool checked);

  
  scoped_ptr<base::DictionaryValue> ToValue() const;

  
  
  static MenuItem* Populate(const std::string& extension_id,
                            const base::DictionaryValue& value,
                            std::string* error);

  
  bool PopulateURLPatterns(std::vector<std::string>* document_url_patterns,
                           std::vector<std::string>* target_url_patterns,
                           std::string* error);

 protected:
  friend class MenuManager;

  
  void AddChild(MenuItem* item);

  
  
  MenuItem* ReleaseChild(const Id& child_id, bool recursive);

  
  
  void GetFlattenedSubtree(MenuItem::List* list);

  
  
  std::set<Id> RemoveAllDescendants();

 private:
  
  Id id_;

  
  std::string title_;

  Type type_;

  
  bool checked_;

  
  bool enabled_;

  
  ContextList contexts_;

  
  
  scoped_ptr<Id> parent_id_;

  
  
  URLPatternSet document_url_patterns_;

  
  
  URLPatternSet target_url_patterns_;

  
  List children_;

  DISALLOW_COPY_AND_ASSIGN(MenuItem);
};

class MenuManager : public content::NotificationObserver,
                    public base::SupportsWeakPtr<MenuManager>,
                    public BrowserContextKeyedService {
 public:
  MenuManager(Profile* profile, StateStore* store_);
  virtual ~MenuManager();

  
  static MenuManager* Get(Profile* profile);

  
  std::set<std::string> ExtensionIds();

  
  
  
  
  
  const MenuItem::List* MenuItems(const std::string& extension_id);

  
  
  
  bool AddContextItem(const Extension* extension, MenuItem* item);

  
  
  bool AddChildItem(const MenuItem::Id& parent_id,
                    MenuItem* child);

  
  
  
  
  
  bool ChangeParent(const MenuItem::Id& child_id,
                    const MenuItem::Id* parent_id);

  
  
  
  bool RemoveContextMenuItem(const MenuItem::Id& id);

  
  void RemoveAllContextItems(const std::string& extension_id);

  
  MenuItem* GetItemById(const MenuItem::Id& id) const;

  
  
  
  
  bool ItemUpdated(const MenuItem::Id& id);

  
  void ExecuteCommand(Profile* profile,
                      content::WebContents* web_contents,
                      const content::ContextMenuParams& params,
                      const MenuItem::Id& menu_item_id);

  
  
  
  const SkBitmap& GetIconForExtension(const std::string& extension_id);

  
  virtual void Observe(int type, const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void WriteToStorage(const Extension* extension);

  
  
  void ReadFromStorage(const std::string& extension_id,
                       scoped_ptr<base::Value> value);

  
  void RemoveAllIncognitoContextItems();

 private:
  FRIEND_TEST_ALL_PREFIXES(MenuManagerTest, DeleteParent);
  FRIEND_TEST_ALL_PREFIXES(MenuManagerTest, RemoveOneByOne);

  
  
  void RadioItemSelected(MenuItem* item);

  
  
  
  
  void SanitizeRadioList(const MenuItem::List& item_list);

  
  bool DescendantOf(MenuItem* item, const MenuItem::Id& ancestor_id);

  
  typedef std::map<std::string, MenuItem::List> MenuItemMap;
  MenuItemMap context_items_;

  
  
  
  std::map<MenuItem::Id, MenuItem*> items_by_id_;

  content::NotificationRegistrar registrar_;

  ExtensionIconManager icon_manager_;

  Profile* profile_;

  
  StateStore* store_;

  DISALLOW_COPY_AND_ASSIGN(MenuManager);
};

}  

#endif  
