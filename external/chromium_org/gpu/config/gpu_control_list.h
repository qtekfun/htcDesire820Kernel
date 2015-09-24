// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_CONFIG_GPU_CONTROL_LIST_H_
#define GPU_CONFIG_GPU_CONTROL_LIST_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "build/build_config.h"
#include "gpu/gpu_export.h"

namespace gpu {
struct GPUInfo;

class GPU_EXPORT GpuControlList {
 public:
  enum OsType {
    kOsLinux,
    kOsMacosx,
    kOsWin,
    kOsChromeOS,
    kOsAndroid,
    kOsAny,
    kOsUnknown
  };

  enum OsFilter {
    
    kCurrentOsOnly,
    
    kAllOs
  };

  GpuControlList();
  virtual ~GpuControlList();

  
  
  bool LoadList(const std::string& json_context, OsFilter os_filter);

  
  
  
  
  
  std::set<int> MakeDecision(
      OsType os, std::string os_version, const GPUInfo& gpu_info);

  
  
  
  void GetDecisionEntries(std::vector<uint32>* entry_ids,
                          bool disabled) const;

  
  
  
  
  
  
  
  
  
  void GetReasons(base::ListValue* problem_list) const;

  
  uint32 max_entry_id() const;

  
  std::string version() const;

  
  
  
  bool needs_more_info() const { return needs_more_info_; }

  
  size_t num_entries() const;

  
  void AddSupportedFeature(const std::string& feature_name, int feature_id);
  
  void set_supports_feature_type_all(bool supported);

  
  void enable_control_list_logging(
      const std::string& control_list_logging_name) {
    control_list_logging_enabled_ = true;
    control_list_logging_name_ = control_list_logging_name;
  }

 private:
  friend class GpuControlListEntryTest;
  friend class MachineModelInfoTest;
  friend class NumberInfoTest;
  friend class OsInfoTest;
  friend class StringInfoTest;
  friend class VersionInfoTest;

  enum NumericOp {
    kBetween,  
    kEQ,  
    kLT,  
    kLE,  
    kGT,  
    kGE,  
    kAny,
    kUnknown  
  };

  class GPU_EXPORT VersionInfo {
   public:
    
    VersionInfo(const std::string& version_op,
                const std::string& version_style,
                const std::string& version_string,
                const std::string& version_string2);
    ~VersionInfo();

    
    
    bool Contains(const std::string& version, char splitter) const;
    
    bool Contains(const std::string& version) const;

    
    bool IsLexical() const;

    
    bool IsValid() const;

   private:
    enum VersionStyle {
      kVersionStyleNumerical,
      kVersionStyleLexical,
      kVersionStyleUnknown
    };

    static VersionStyle StringToVersionStyle(const std::string& version_style);

    
    
    
    
    
    
    
    
    
    
    static int Compare(const std::vector<std::string>& version,
                       const std::vector<std::string>& version_ref,
                       VersionStyle version_style);

    NumericOp op_;
    VersionStyle version_style_;
    std::vector<std::string> version_;
    std::vector<std::string> version2_;
  };

  class GPU_EXPORT OsInfo {
   public:
    OsInfo(const std::string& os,
           const std::string& version_op,
           const std::string& version_string,
           const std::string& version_string2);
    ~OsInfo();

    
    bool Contains(OsType type, const std::string& version) const;

    
    bool IsValid() const;

    OsType type() const;

    
    static OsType StringToOsType(const std::string& os);

   private:
    OsType type_;
    scoped_ptr<VersionInfo> version_info_;
  };

  class GPU_EXPORT StringInfo {
   public:
    StringInfo(const std::string& string_op, const std::string& string_value);

    
    bool Contains(const std::string& value) const;

    
    bool IsValid() const;

   private:
    enum Op {
      kContains,
      kBeginWith,
      kEndWith,
      kEQ,  
      kUnknown  
    };

    
    static Op StringToOp(const std::string& string_op);

    Op op_;
    std::string value_;
  };

  class GPU_EXPORT FloatInfo {
   public:
    FloatInfo(const std::string& float_op,
              const std::string& float_value,
              const std::string& float_value2);

    
    bool Contains(float value) const;

    
    bool IsValid() const;

   private:
    NumericOp op_;
    float value_;
    float value2_;
  };

  class GPU_EXPORT IntInfo {
   public:
    IntInfo(const std::string& int_op,
            const std::string& int_value,
            const std::string& int_value2);

    
    bool Contains(int value) const;

    
    bool IsValid() const;

   private:
    NumericOp op_;
    int value_;
    int value2_;
  };

  class GPU_EXPORT MachineModelInfo {
   public:
    MachineModelInfo(const std::string& name_op,
                     const std::string& name_value,
                     const std::string& version_op,
                     const std::string& version_string,
                     const std::string& version_string2);
    ~MachineModelInfo();

    
    bool Contains(const std::string& name, const std::string& version) const;

    
    bool IsValid() const;

   private:
    scoped_ptr<StringInfo> name_info_;
    scoped_ptr<VersionInfo> version_info_;
  };

  class GpuControlListEntry;
  typedef scoped_refptr<GpuControlListEntry> ScopedGpuControlListEntry;

  typedef base::hash_map<std::string, int> FeatureMap;

  class GPU_EXPORT GpuControlListEntry
      : public base::RefCounted<GpuControlListEntry> {
   public:
    
    
    static ScopedGpuControlListEntry GetEntryFromValue(
        const base::DictionaryValue* value, bool top_level,
        const FeatureMap& feature_map,
        bool supports_feature_type_all);

    
    
    void LogControlListMatch(
        const std::string& control_list_logging_name) const;

    
    
    bool Contains(OsType os_type, const std::string& os_version,
                  const GPUInfo& gpu_info) const;

    
    
    bool NeedsMoreInfo(const GPUInfo& gpu_info) const;

    
    OsType GetOsType() const;

    
    uint32 id() const;

    
    bool disabled() const;

    
    const std::string& description() const { return description_; }

    
    const std::vector<int>& cr_bugs() const { return cr_bugs_; }
    const std::vector<int>& webkit_bugs() const { return webkit_bugs_; }

    
    const std::set<int>& features() const;

   private:
    friend class base::RefCounted<GpuControlListEntry>;

    enum MultiGpuStyle {
      kMultiGpuStyleOptimus,
      kMultiGpuStyleAMDSwitchable,
      kMultiGpuStyleNone
    };

    enum MultiGpuCategory {
      kMultiGpuCategoryPrimary,
      kMultiGpuCategorySecondary,
      kMultiGpuCategoryAny,
      kMultiGpuCategoryNone
    };

    GpuControlListEntry();
    ~GpuControlListEntry();

    bool SetId(uint32 id);

    void SetDisabled(bool disabled);

    bool SetOsInfo(const std::string& os,
                   const std::string& version_op,
                   const std::string& version_string,
                   const std::string& version_string2);

    bool SetVendorId(const std::string& vendor_id_string);

    bool AddDeviceId(const std::string& device_id_string);

    bool SetMultiGpuStyle(const std::string& multi_gpu_style_string);

    bool SetMultiGpuCategory(const std::string& multi_gpu_category_string);

    bool SetDriverVendorInfo(const std::string& vendor_op,
                             const std::string& vendor_value);

    bool SetDriverVersionInfo(const std::string& version_op,
                              const std::string& version_style,
                              const std::string& version_string,
                              const std::string& version_string2);

    bool SetDriverDateInfo(const std::string& date_op,
                           const std::string& date_string,
                           const std::string& date_string2);

    bool SetGLVendorInfo(const std::string& vendor_op,
                         const std::string& vendor_value);

    bool SetGLRendererInfo(const std::string& renderer_op,
                           const std::string& renderer_value);

    bool SetGLExtensionsInfo(const std::string& extensions_op,
                             const std::string& extensions_value);

    bool SetGLResetNotificationStrategyInfo(const std::string& op,
                                            const std::string& int_string,
                                            const std::string& int_string2);

    bool SetCpuBrand(const std::string& cpu_op,
                     const std::string& cpu_value);

    bool SetPerfGraphicsInfo(const std::string& op,
                             const std::string& float_string,
                             const std::string& float_string2);

    bool SetPerfGamingInfo(const std::string& op,
                           const std::string& float_string,
                           const std::string& float_string2);

    bool SetPerfOverallInfo(const std::string& op,
                            const std::string& float_string,
                            const std::string& float_string2);

    bool SetMachineModelInfo(const std::string& name_op,
                             const std::string& name_value,
                             const std::string& version_op,
                             const std::string& version_string,
                             const std::string& version_string2);

    bool SetGpuCountInfo(const std::string& op,
                         const std::string& int_string,
                         const std::string& int_string2);

    bool SetFeatures(const std::vector<std::string>& features,
                     const FeatureMap& feature_map,
                     bool supports_feature_type_all);

    void AddException(ScopedGpuControlListEntry exception);

    static MultiGpuStyle StringToMultiGpuStyle(const std::string& style);

    static MultiGpuCategory StringToMultiGpuCategory(
        const std::string& category);

    
    
    static bool StringToFeature(const std::string& feature_name,
                                int* feature_id,
                                const FeatureMap& feature_map);

    uint32 id_;
    bool disabled_;
    std::string description_;
    std::vector<int> cr_bugs_;
    std::vector<int> webkit_bugs_;
    scoped_ptr<OsInfo> os_info_;
    uint32 vendor_id_;
    std::vector<uint32> device_id_list_;
    MultiGpuStyle multi_gpu_style_;
    MultiGpuCategory multi_gpu_category_;
    scoped_ptr<StringInfo> driver_vendor_info_;
    scoped_ptr<VersionInfo> driver_version_info_;
    scoped_ptr<VersionInfo> driver_date_info_;
    scoped_ptr<StringInfo> gl_vendor_info_;
    scoped_ptr<StringInfo> gl_renderer_info_;
    scoped_ptr<StringInfo> gl_extensions_info_;
    scoped_ptr<IntInfo> gl_reset_notification_strategy_info_;
    scoped_ptr<StringInfo> cpu_brand_;
    scoped_ptr<FloatInfo> perf_graphics_info_;
    scoped_ptr<FloatInfo> perf_gaming_info_;
    scoped_ptr<FloatInfo> perf_overall_info_;
    scoped_ptr<MachineModelInfo> machine_model_info_;
    scoped_ptr<IntInfo> gpu_count_info_;
    std::set<int> features_;
    std::vector<ScopedGpuControlListEntry> exceptions_;
  };

  
  static OsType GetOsType();

  bool LoadList(const base::DictionaryValue& parsed_json, OsFilter os_filter);

  void Clear();

  static NumericOp StringToNumericOp(const std::string& op);

  std::string version_;
  std::vector<ScopedGpuControlListEntry> entries_;

  
  
  
  std::vector<ScopedGpuControlListEntry> active_entries_;

  uint32 max_entry_id_;

  bool needs_more_info_;

  
  FeatureMap feature_map_;
  bool supports_feature_type_all_;

  bool control_list_logging_enabled_;
  std::string control_list_logging_name_;
};

}  

#endif  

