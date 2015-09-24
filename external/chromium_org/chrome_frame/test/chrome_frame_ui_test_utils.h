// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_CHROME_FRAME_UI_TEST_UTILS_H_
#define CHROME_FRAME_TEST_CHROME_FRAME_UI_TEST_UTILS_H_

#include <oleacc.h>

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_variant.h"
#include "chrome_frame/test/win_event_receiver.h"

namespace gfx {
class Rect;
}

namespace chrome_frame_test {

class AccObject : public base::RefCounted<AccObject> {
 public:
  typedef std::vector<scoped_refptr<AccObject> > RefCountedAccObjectVector;

  
  
  
  AccObject(IAccessible* accessible, int child_id = CHILDID_SELF);

  
  
  
  static AccObject* CreateFromWindow(HWND hwnd);

  
  
  static AccObject* CreateFromEvent(HWND hwnd, LONG object_id, LONG child_id);

  
  
  
  static AccObject* CreateFromDispatch(IDispatch* dispatch);

  
  
  
  
  static AccObject* CreateFromPoint(int x, int y);

  
  
  
  bool DoDefaultAction();

  
  
  bool LeftClick();

  
  
  bool RightClick();

  
  
  bool Focus();

  
  
  bool Select();

  
  bool SetValue(const std::wstring& value);

  
  bool GetName(std::wstring* name);

  
  bool GetRoleText(std::wstring* role_text);

  
  bool GetValue(std::wstring* value);

  
  bool GetState(int* state);

  
  
  bool GetLocation(gfx::Rect* location);

  
  
  bool GetLocationInClient(gfx::Rect* location);

  
  AccObject* GetParent();

  
  
  bool GetChildren(RefCountedAccObjectVector* objects);

  
  bool GetChildCount(int* child_count);

  
  
  
  
  
  bool GetFromNavigation(long navigation_type,
                         scoped_refptr<AccObject>* object);

  
  
  bool GetWindow(HWND* window);

  
  
  bool GetWindowClassName(std::wstring* class_name);

  
  
  
  
  bool GetSelectionRange(int* start_offset, int* end_offset);

  
  
  bool GetSelectedText(std::wstring* text);

  
  bool IsSimpleElement();

  
  
  bool Equals(AccObject* other);

  
  std::wstring GetDescription();

  
  
  std::wstring GetTree();

 private:
  friend class base::RefCounted<AccObject>;
  ~AccObject() {}

  
  
  
  
  static AccObject* CreateFromVariant(AccObject* object,
                                      const VARIANT& variant);

  
  
  bool PostMouseClickAtCenter(int button_down, int button_up);

  
  bool PostMouseButtonMessages(int button_up, int button_down, int x, int y);

  base::win::ScopedComPtr<IAccessible> accessible_;
  base::win::ScopedVariant child_id_;

  DISALLOW_COPY_AND_ASSIGN(AccObject);
};

class AccObjectMatcher {
 public:
  
  
  
  
  
  
  
  
  
  
  
  AccObjectMatcher(const std::wstring& name = L"",
                   const std::wstring& role_text = L"",
                   const std::wstring& value = L"");

  
  
  
  
  
  bool Find(AccObject* object, scoped_refptr<AccObject>* match) const;

  
  
  bool FindInWindow(HWND hwnd, scoped_refptr<AccObject>* match) const;

  
  bool DoesMatch(AccObject* object) const;

  
  std::wstring GetDescription() const;

 private:
  bool FindHelper(AccObject* object, scoped_refptr<AccObject>* match) const;

  std::wstring name_;
  std::wstring role_text_;
  std::wstring value_;
};

class AccEventObserver : public WinEventListener {
 public:
  AccEventObserver();
  virtual ~AccEventObserver();

  
  
  
  void WatchForOneValueChange(const AccObjectMatcher& matcher);

  
  virtual void OnAccDocLoad(HWND hwnd) = 0;

  
  
  virtual void OnAccValueChange(HWND hwnd, AccObject* object,
                                const std::wstring& new_value) = 0;

  
  
  virtual void OnTextCaretMoved(HWND hwnd, AccObject* object) = 0;

  
  virtual void OnMenuPopup(HWND hwnd) = 0;

 private:
  class EventHandler : public base::RefCounted<EventHandler> {
   public:
    explicit EventHandler(AccEventObserver* observer);

    
    
    void Handle(DWORD event, HWND hwnd, LONG object_id, LONG child_id);

    AccEventObserver* observer_;
  };

  
  virtual void OnEventReceived(DWORD event, HWND hwnd, LONG object_id,
                               LONG child_id);

  scoped_refptr<EventHandler> event_handler_;
  bool is_watching_;
  AccObjectMatcher watching_for_matcher_;
  WinEventReceiver event_receiver_;
};

bool FindAccObjectInWindow(HWND hwnd, const AccObjectMatcher& matcher,
                           scoped_refptr<AccObject>* object);

void DumpAccessibilityTreeForWindow(HWND hwnd);

bool IsDesktopUnlocked();

base::FilePath GetIAccessible2ProxyStubPath();

}  

#endif  
