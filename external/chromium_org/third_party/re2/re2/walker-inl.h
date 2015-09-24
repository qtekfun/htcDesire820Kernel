// Copyright 2006 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.



#ifndef RE2_WALKER_INL_H__
#define RE2_WALKER_INL_H__

#include "re2/regexp.h"

namespace re2 {

template<typename T> struct WalkState;

template<typename T> class Regexp::Walker {
 public:
  Walker();
  virtual ~Walker();

  
  
  
  
  
  
  
  
  
  virtual T PreVisit(Regexp* re, T parent_arg, bool* stop);

  
  
  
  
  
  
  
  
  
  virtual T PostVisit(Regexp* re, T parent_arg, T pre_arg,
                      T* child_args, int nchild_args);

  
  
  virtual T Copy(T arg);

  
  
  
  
  
  
  
  
  virtual T ShortVisit(Regexp* re, T parent_arg) = 0;

  
  
  
  T Walk(Regexp* re, T top_arg);

  
  
  
  
  
  
  
  
  T WalkExponential(Regexp* re, T top_arg, int max_visits);

  
  
  
  void Reset();

  
  bool stopped_early() { return stopped_early_; }

 private:
  
  stack<WalkState<T> >* stack_;
  bool stopped_early_;
  int max_visits_;

  T WalkInternal(Regexp* re, T top_arg, bool use_copy);

  DISALLOW_EVIL_CONSTRUCTORS(Walker);
};

template<typename T> T Regexp::Walker<T>::PreVisit(Regexp* re,
                                                   T parent_arg,
                                                   bool* stop) {
  return parent_arg;
}

template<typename T> T Regexp::Walker<T>::PostVisit(Regexp* re,
                                                    T parent_arg,
                                                    T pre_arg,
                                                    T* child_args,
                                                    int nchild_args) {
  return pre_arg;
}

template<typename T> T Regexp::Walker<T>::Copy(T arg) {
  return arg;
}

template<typename T> struct WalkState {
  WalkState<T>(Regexp* re, T parent)
    : re(re),
      n(-1),
      parent_arg(parent),
      child_args(NULL) { }

  Regexp* re;  
  int n;  
  T parent_arg;  
  T pre_arg;
  T child_arg;  
  T* child_args;
};

template<typename T> Regexp::Walker<T>::Walker() {
  stack_ = new stack<WalkState<T> >;
  stopped_early_ = false;
}

template<typename T> Regexp::Walker<T>::~Walker() {
  Reset();
  delete stack_;
}

template<typename T> void Regexp::Walker<T>::Reset() {
  if (stack_ && stack_->size() > 0) {
    LOG(DFATAL) << "Stack not empty.";
    while (stack_->size() > 0) {
      delete stack_->top().child_args;
      stack_->pop();
    }
  }
}

template<typename T> T Regexp::Walker<T>::WalkInternal(Regexp* re, T top_arg,
                                                       bool use_copy) {
  Reset();

  if (re == NULL) {
    LOG(DFATAL) << "Walk NULL";
    return top_arg;
  }

  stack_->push(WalkState<T>(re, top_arg));

  WalkState<T>* s;
  for (;;) {
    T t;
    s = &stack_->top();
    Regexp* re = s->re;
    switch (s->n) {
      case -1: {
        if (--max_visits_ < 0) {
          stopped_early_ = true;
          t = ShortVisit(re, s->parent_arg);
          break;
        }
        bool stop = false;
        s->pre_arg = PreVisit(re, s->parent_arg, &stop);
        if (stop) {
          t = s->pre_arg;
          break;
        }
        s->n = 0;
        s->child_args = NULL;
        if (re->nsub_ == 1)
          s->child_args = &s->child_arg;
        else if (re->nsub_ > 1)
          s->child_args = new T[re->nsub_];
        
      }
      default: {
        if (re->nsub_ > 0) {
          Regexp** sub = re->sub();
          if (s->n < re->nsub_) {
            if (use_copy && s->n > 0 && sub[s->n - 1] == sub[s->n]) {
              s->child_args[s->n] = Copy(s->child_args[s->n - 1]);
              s->n++;
            } else {
              stack_->push(WalkState<T>(sub[s->n], s->pre_arg));
            }
            continue;
          }
        }

        t = PostVisit(re, s->parent_arg, s->pre_arg, s->child_args, s->n);
        if (re->nsub_ > 1)
          delete[] s->child_args;
        break;
      }
    }

    
    
    stack_->pop();
    if (stack_->size() == 0)
      return t;
    s = &stack_->top();
    if (s->child_args != NULL)
      s->child_args[s->n] = t;
    else
      s->child_arg = t;
    s->n++;
  }
}

template<typename T> T Regexp::Walker<T>::Walk(Regexp* re, T top_arg) {
  
  
  
  
  max_visits_ = 1000000;
  return WalkInternal(re, top_arg, true);
}

template<typename T> T Regexp::Walker<T>::WalkExponential(Regexp* re, T top_arg,
                                                          int max_visits) {
  max_visits_ = max_visits;
  return WalkInternal(re, top_arg, false);
}

}  

#endif  
