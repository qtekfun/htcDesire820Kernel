// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_DEPENDENCY_GRAPH_H_
#define COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_DEPENDENCY_GRAPH_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_export.h"

class DependencyNode;

class BROWSER_CONTEXT_KEYED_SERVICE_EXPORT DependencyGraph {
 public:
  DependencyGraph();
  ~DependencyGraph();

  
  
  void AddNode(DependencyNode* node);
  void RemoveNode(DependencyNode* node);

  
  void AddEdge(DependencyNode* depended, DependencyNode* dependee);

  
  
  bool GetConstructionOrder(
      std::vector<DependencyNode*>* order) WARN_UNUSED_RESULT;

  
  
  bool GetDestructionOrder(
      std::vector<DependencyNode*>* order) WARN_UNUSED_RESULT;

  
  std::string DumpAsGraphviz(
      const std::string& toplevel_name,
      const base::Callback<std::string(DependencyNode*)>&
      node_name_callback) const;

 private:
  typedef std::multimap<DependencyNode*, DependencyNode*> EdgeMap;

  
  
  bool BuildConstructionOrder() WARN_UNUSED_RESULT;

  
  std::vector<DependencyNode*> all_nodes_;

  
  EdgeMap edges_;

  
  
  std::vector<DependencyNode*> construction_order_;

  DISALLOW_COPY_AND_ASSIGN(DependencyGraph);
};

#endif  
