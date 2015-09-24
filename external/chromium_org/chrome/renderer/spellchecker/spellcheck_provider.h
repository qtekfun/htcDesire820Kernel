// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_PROVIDER_H_
#define CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_PROVIDER_H_

#include <vector>

#include "base/id_map.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/public/renderer/render_view_observer_tracker.h"
#include "third_party/WebKit/public/web/WebSpellCheckClient.h"

class RenderView;
class SpellCheck;
class SpellCheckMarker;
struct SpellCheckResult;

namespace blink {
class WebString;
class WebTextCheckingCompletion;
struct WebTextCheckingResult;
}

class SpellCheckProvider
    : public content::RenderViewObserver,
      public content::RenderViewObserverTracker<SpellCheckProvider>,
      public blink::WebSpellCheckClient {
 public:
  typedef IDMap<blink::WebTextCheckingCompletion> WebTextCheckCompletions;

  SpellCheckProvider(content::RenderView* render_view,
                     SpellCheck* spellcheck);
  virtual ~SpellCheckProvider();

  
  
  void RequestTextChecking(
      const base::string16& text,
      blink::WebTextCheckingCompletion* completion,
      const std::vector<SpellCheckMarker>& markers);

  
  size_t pending_text_request_size() const {
    return text_check_completions_.size();
  }

  
  void set_spellcheck(SpellCheck* spellcheck) { spellcheck_ = spellcheck; }

  
  void EnableSpellcheck(bool enabled);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void FocusedNodeChanged(const blink::WebNode& node) OVERRIDE;

 private:
  friend class TestingSpellCheckProvider;

  
  
  
  bool SatisfyRequestFromCache(const base::string16& text,
                               blink::WebTextCheckingCompletion* completion);

  
  virtual void spellCheck(
      const blink::WebString& text,
      int& offset,
      int& length,
      blink::WebVector<blink::WebString>* optional_suggestions) OVERRIDE;
  virtual void checkTextOfParagraph(
      const blink::WebString& text,
      blink::WebTextCheckingTypeMask mask,
      blink::WebVector<blink::WebTextCheckingResult>* results) OVERRIDE;

  virtual void requestCheckingOfText(
      const blink::WebString& text,
      const blink::WebVector<uint32>& markers,
      const blink::WebVector<unsigned>& marker_offsets,
      blink::WebTextCheckingCompletion* completion) OVERRIDE;

  virtual blink::WebString autoCorrectWord(
      const blink::WebString& misspelled_word) OVERRIDE;
  virtual void showSpellingUI(bool show) OVERRIDE;
  virtual bool isShowingSpellingUI() OVERRIDE;
  virtual void updateSpellingUIWithMisspelledWord(
      const blink::WebString& word) OVERRIDE;

#if !defined(OS_MACOSX)
  void OnRespondSpellingService(
      int identifier,
      bool succeeded,
      const base::string16& text,
      const std::vector<SpellCheckResult>& results);
#endif

  
  
  bool HasWordCharacters(const base::string16& text, int index) const;

#if defined(OS_MACOSX)
  void OnAdvanceToNextMisspelling();
  void OnRespondTextCheck(
      int identifier,
      const std::vector<SpellCheckResult>& results);
  void OnToggleSpellPanel(bool is_currently_visible);
#endif

  
  WebTextCheckCompletions text_check_completions_;

  
  
  base::string16 last_request_;
  blink::WebVector<blink::WebTextCheckingResult> last_results_;

  
  bool spelling_panel_visible_;

  
  SpellCheck* spellcheck_;

  DISALLOW_COPY_AND_ASSIGN(SpellCheckProvider);
};

#endif  
