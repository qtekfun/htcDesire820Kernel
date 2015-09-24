// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ACTION_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ACTION_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/common/extensions/api/extension_action/action_info.h"
#include "chrome/common/extensions/extension_icon_set.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkDevice.h"
#include "ui/gfx/animation/linear_animation.h"

class GURL;
class SkBitmap;

namespace gfx {
class Canvas;
class Image;
class ImageSkia;
class Rect;
class Size;
}

class ExtensionAction {
 public:
  
  
  static const int kDefaultTabId;

  enum Appearance {
    
    INVISIBLE,
    
    
    WANTS_ATTENTION,
    
    ACTIVE,
  };

  
  class IconAnimation : public gfx::LinearAnimation {
   public:
    
    class Observer {
     public:
      virtual void OnIconChanged() = 0;

     protected:
      virtual ~Observer() {}
    };

    
    class ScopedObserver {
     public:
      ScopedObserver(const base::WeakPtr<IconAnimation>& icon_animation,
                     Observer* observer);
      ~ScopedObserver();

      
      const IconAnimation* icon_animation() const {
        return icon_animation_.get();
      }

     private:
      base::WeakPtr<IconAnimation> icon_animation_;
      Observer* observer_;

      DISALLOW_COPY_AND_ASSIGN(ScopedObserver);
    };

    virtual ~IconAnimation();

    
    
    const SkBitmap& Apply(const SkBitmap& icon) const;

    void AddObserver(Observer* observer);
    void RemoveObserver(Observer* observer);

   private:
    
    friend class ExtensionAction;
    IconAnimation();

    base::WeakPtr<IconAnimation> AsWeakPtr();

    
    virtual void AnimateToState(double state) OVERRIDE;

    
    mutable scoped_ptr<SkBaseDevice> device_;

    ObserverList<Observer> observers_;

    base::WeakPtrFactory<IconAnimation> weak_ptr_factory_;

    DISALLOW_COPY_AND_ASSIGN(IconAnimation);
  };

  ExtensionAction(const std::string& extension_id,
                  extensions::ActionInfo::Type action_type,
                  const extensions::ActionInfo& manifest_data);
  ~ExtensionAction();

  
  
  scoped_ptr<ExtensionAction> CopyForTest() const;

  
  
  
  static int GetIconSizeForType(extensions::ActionInfo::Type type);

  
  const std::string& extension_id() const { return extension_id_; }

  
  extensions::ActionInfo::Type action_type() const {
    return action_type_;
  }

  
  std::string id() const { return id_; }
  void set_id(const std::string& id) { id_ = id; }

  bool has_changed() const { return has_changed_; }
  void set_has_changed(bool value) { has_changed_ = value; }

  
  
  void SetPopupUrl(int tab_id, const GURL& url);

  
  bool HasPopup(int tab_id) const;

  
  GURL GetPopupUrl(int tab_id) const;

  
  void SetTitle(int tab_id, const std::string& title) {
    SetValue(&title_, tab_id, title);
  }

  
  
  std::string GetTitle(int tab_id) const { return GetValue(&title_, tab_id); }

  
  
  
  
  

  
  void SetIcon(int tab_id, const gfx::Image& image);

  
  
  gfx::Image ApplyAttentionAndAnimation(const gfx::ImageSkia& icon,
                                        int tab_id) const;

  
  gfx::ImageSkia GetExplicitlySetIcon(int tab_id) const;

  
  
  void set_default_icon(scoped_ptr<ExtensionIconSet> icon_set) {
     default_icon_ = icon_set.Pass();
  }

  const ExtensionIconSet* default_icon() const {
    return default_icon_.get();
  }

  
  void SetBadgeText(int tab_id, const std::string& text) {
    SetValue(&badge_text_, tab_id, text);
  }
  
  std::string GetBadgeText(int tab_id) const {
    return GetValue(&badge_text_, tab_id);
  }

  
  void SetBadgeTextColor(int tab_id, SkColor text_color) {
    SetValue(&badge_text_color_, tab_id, text_color);
  }
  
  
  SkColor GetBadgeTextColor(int tab_id) const {
    return GetValue(&badge_text_color_, tab_id);
  }

  
  void SetBadgeBackgroundColor(int tab_id, SkColor color) {
    SetValue(&badge_background_color_, tab_id, color);
  }
  
  
  SkColor GetBadgeBackgroundColor(int tab_id) const {
    return GetValue(&badge_background_color_, tab_id);
  }

  
  
  
  bool SetAppearance(int tab_id, Appearance value);
  
  
  void DeclarativeShow(int tab_id);
  void UndoDeclarativeShow(int tab_id);

  
  
  bool GetIsVisible(int tab_id) const {
    return GetAppearance(tab_id) != INVISIBLE;
  }

  
  bool WantsAttention(int tab_id) const {
    return GetAppearance(tab_id) == WANTS_ATTENTION;
  }

  
  void ClearAllValuesForTab(int tab_id);

  
  void PaintBadge(gfx::Canvas* canvas, const gfx::Rect& bounds, int tab_id);

  
  gfx::ImageSkia GetIconWithBadge(const gfx::ImageSkia& icon,
                                  int tab_id,
                                  const gfx::Size& spacing) const;

  
  
  base::WeakPtr<IconAnimation> GetIconAnimation(int tab_id) const;

 private:
  
  void RunIconAnimation(int tab_id);

  
  
  gfx::ImageSkia ApplyIconAnimation(int tab_id,
                                    const gfx::ImageSkia& orig) const;

  
  
  
  int GetIconWidth(int tab_id) const;

  template <class T>
  struct ValueTraits {
    static T CreateEmpty() {
      return T();
    }
  };

  template<class T>
  void SetValue(std::map<int, T>* map, int tab_id, const T& val) {
    (*map)[tab_id] = val;
  }

  template<class Map>
  static const typename Map::mapped_type* FindOrNull(
      const Map* map,
      const typename Map::key_type& key) {
    typename Map::const_iterator iter = map->find(key);
    if (iter == map->end())
      return NULL;
    return &iter->second;
  }

  template<class T>
  T GetValue(const std::map<int, T>* map, int tab_id) const {
    if (const T* tab_value = FindOrNull(map, tab_id)) {
      return *tab_value;
    } else if (const T* default_value = FindOrNull(map, kDefaultTabId)) {
      return *default_value;
    } else {
      return ValueTraits<T>::CreateEmpty();
    }
  }

  
  
  
  
  
  
  Appearance GetAppearance(int tab_id) const {
    if (const Appearance* tab_appearance = FindOrNull(&appearance_, tab_id))
      return *tab_appearance;

    if (ContainsKey(declarative_show_count_, tab_id))
      return ACTIVE;

    if (const Appearance* default_appearance =
        FindOrNull(&appearance_, kDefaultTabId))
      return *default_appearance;

    return INVISIBLE;
  }

  
  
  const std::string extension_id_;

  const extensions::ActionInfo::Type action_type_;

  
  
  std::map<int, GURL> popup_url_;
  std::map<int, std::string> title_;
  std::map<int, gfx::ImageSkia> icon_;
  std::map<int, std::string> badge_text_;
  std::map<int, SkColor> badge_background_color_;
  std::map<int, SkColor> badge_text_color_;
  std::map<int, Appearance> appearance_;

  
  
  
  
  
  

  
  
  std::map<int, int> declarative_show_count_;

  
  
  
  
  
  mutable std::map<int, base::WeakPtr<IconAnimation> > icon_animation_;

  
  
  scoped_ptr<const ExtensionIconSet> default_icon_;

  
  
  std::string id_;

  
  
  bool has_changed_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionAction);
};

template<>
struct ExtensionAction::ValueTraits<int> {
  static int CreateEmpty() {
    return -1;
  }
};

#endif  
