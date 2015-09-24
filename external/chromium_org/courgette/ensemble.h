// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef COURGETTE_ENSEMBLE_H_
#define COURGETTE_ENSEMBLE_H_

#include <vector>
#include <string>

#include "base/basictypes.h"

#include "courgette/courgette.h"
#include "courgette/region.h"
#include "courgette/streams.h"

namespace courgette {

class Ensemble;

class Element {
 public:
  Element(ExecutableType kind,
          Ensemble* ensemble,
          const Region& region);

  virtual ~Element();

  ExecutableType kind() const { return kind_; }
  const Region& region() const { return region_; }

  
  virtual std::string Name() const;

  
  
  size_t offset_in_ensemble() const;

 private:
  ExecutableType kind_;
  Ensemble* ensemble_;
  Region region_;

  DISALLOW_COPY_AND_ASSIGN(Element);
};


class Ensemble {
 public:
  Ensemble(const Region& region, const char* name)
      : region_(region), name_(name) {}
  ~Ensemble();

  const Region& region() const { return region_; }
  const std::string& name() const { return name_; }

  
  Status FindEmbeddedElements();

  
  const std::vector<Element*>& elements() const { return elements_; }


 private:
  Region region_;       
                        
  std::string name_;    

  std::vector<Element*> elements_;        
  std::vector<Element*> owned_elements_;  

  DISALLOW_COPY_AND_ASSIGN(Ensemble);
};

inline size_t Element::offset_in_ensemble() const {
  return region().start() - ensemble_->region().start();
}

struct CourgettePatchFile {
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  static const uint32 kMagic = 'C' | ('o' << 8) | ('u' << 16);

  static const uint32 kVersion = 20110216;
};

class TransformationPatcher {
 public:
  virtual ~TransformationPatcher() {}

  
  
  virtual Status Init(SourceStream* parameter_stream) = 0;

  
  virtual Status PredictTransformParameters(
      SinkStreamSet* predicted_parameters) = 0;

  
  
  virtual Status Transform(SourceStreamSet* corrected_parameters,
                           SinkStreamSet* transformed_element) = 0;

  
  
  virtual Status Reform(SourceStreamSet* transformed_element,
                        SinkStream* reformed_element) = 0;
};

class TransformationPatchGenerator {
 public:
  TransformationPatchGenerator(Element* old_element,
                               Element* new_element,
                               TransformationPatcher* patcher);

  virtual ~TransformationPatchGenerator();

  
  virtual ExecutableType Kind() = 0;

  
  virtual Status WriteInitialParameters(SinkStream* parameter_stream) = 0;

  
  
  
  
  
  virtual Status PredictTransformParameters(SinkStreamSet* prediction);

  
  
  virtual Status CorrectedTransformParameters(SinkStreamSet* parameters) = 0;

  
  
  
  
  
  // The output written to |old_transformed_element| must match exactly the
  // output written by the Transform method of the corresponding subclass of
  
  virtual Status Transform(SourceStreamSet* old_corrected_parameters,
                           SinkStreamSet* old_transformed_element,
                           SinkStreamSet* new_transformed_element) = 0;

  
  
  
  
  
  virtual Status Reform(SourceStreamSet* transformed_element,
                        SinkStream* reformed_element);

 protected:
  Element* old_element_;
  Element* new_element_;
  TransformationPatcher* patcher_;
};

}  
#endif  
