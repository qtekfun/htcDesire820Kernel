// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_GDATA_WAPI_PARSER_H_
#define GOOGLE_APIS_DRIVE_GDATA_WAPI_PARSER_H_

#include <string>
#include <utility>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string_piece.h"
#include "base/time/time.h"
#include "google_apis/drive/drive_entry_kinds.h"
#include "url/gurl.h"

namespace base {
class FilePath;
class DictionaryValue;
class Value;

template <class StructType>
class JSONValueConverter;

namespace internal {
template <class NestedType>
class RepeatedMessageConverter;
}  

}  

namespace google_apis {

class Link {
 public:
  enum LinkType {
    LINK_UNKNOWN,
    LINK_SELF,
    LINK_NEXT,
    LINK_PARENT,
    LINK_ALTERNATE,
    LINK_EDIT,
    LINK_EDIT_MEDIA,
    LINK_ALT_EDIT_MEDIA,
    LINK_ALT_POST,
    LINK_FEED,
    LINK_POST,
    LINK_BATCH,
    LINK_RESUMABLE_EDIT_MEDIA,
    LINK_RESUMABLE_CREATE_MEDIA,
    LINK_TABLES_FEED,
    LINK_WORKSHEET_FEED,
    LINK_THUMBNAIL,
    LINK_EMBED,
    LINK_PRODUCT,
    LINK_ICON,
    LINK_OPEN_WITH,
    LINK_SHARE,
  };
  Link();
  ~Link();

  
  
  static void RegisterJSONConverter(base::JSONValueConverter<Link>* converter);

  
  LinkType type() const { return type_; }

  
  const GURL& href() const { return href_; }

  
  const std::string& title() const { return title_; }

  
  
  const std::string& app_id() const { return app_id_; }

  
  const std::string& mime_type() const { return mime_type_; }

  void set_type(LinkType type) { type_ = type; }
  void set_href(const GURL& href) { href_ = href; }
  void set_title(const std::string& title) { title_ = title; }
  void set_app_id(const std::string& app_id) { app_id_ = app_id; }
  void set_mime_type(const std::string& mime_type) { mime_type_ = mime_type; }

 private:
  friend class ResourceEntry;
  
  
  
  static bool GetLinkType(const base::StringPiece& rel, LinkType* type);

  
  
  
  static bool GetAppID(const base::StringPiece& rel, std::string* app_id);

  LinkType type_;
  GURL href_;
  std::string title_;
  std::string app_id_;
  std::string mime_type_;

  DISALLOW_COPY_AND_ASSIGN(Link);
};

class ResourceLink {
 public:
  enum ResourceLinkType {
    FEED_LINK_UNKNOWN,
    FEED_LINK_ACL,
    FEED_LINK_REVISIONS,
  };
  ResourceLink();

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<ResourceLink>* converter);

  
  ResourceLinkType type() const { return type_; }

  
  const GURL& href() const { return href_; }

  void set_type(ResourceLinkType type) { type_ = type; }
  void set_href(const GURL& href) { href_ = href; }

 private:
  friend class ResourceEntry;
  
  
  
  static bool GetFeedLinkType(
      const base::StringPiece& rel, ResourceLinkType* result);

  ResourceLinkType type_;
  GURL href_;

  DISALLOW_COPY_AND_ASSIGN(ResourceLink);
};

class Author {
 public:
  Author();

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<Author>* converter);

  
  const std::string& name() const { return name_; }
  const std::string& email() const { return email_; }

  void set_name(const std::string& name) { name_ = name; }
  void set_email(const std::string& email) { email_ = email; }

 private:
  friend class ResourceEntry;

  std::string name_;
  std::string email_;

  DISALLOW_COPY_AND_ASSIGN(Author);
};

class Category {
 public:
  enum CategoryType {
    CATEGORY_UNKNOWN,
    CATEGORY_ITEM,
    CATEGORY_KIND,
    CATEGORY_LABEL,
  };

  Category();

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<Category>* converter);

  
  const std::string& label() const { return label_; }

  
  CategoryType type() const { return type_; }

  
  const std::string& term() const { return term_; }

  void set_label(const std::string& label) { label_ = label; }
  void set_type(CategoryType type) { type_ = type; }
  void set_term(const std::string& term) { term_ = term; }

 private:
  friend class ResourceEntry;
  
  
  
  
  static bool GetCategoryTypeFromScheme(
      const base::StringPiece& scheme, CategoryType* result);

  std::string label_;
  CategoryType type_;
  std::string term_;

  DISALLOW_COPY_AND_ASSIGN(Category);
};

class Content {
 public:
  Content();

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<Content>* converter);

  
  
  
  const GURL& url() const { return url_; }
  const std::string& mime_type() const { return mime_type_; }

  void set_url(const GURL& url) { url_ = url; }
  void set_mime_type(const std::string& mime_type) { mime_type_ = mime_type; }

 private:
  friend class ResourceEntry;

  GURL url_;
  std::string mime_type_;
};

class AppIcon {
 public:
  enum IconCategory {
    ICON_UNKNOWN,          
    ICON_DOCUMENT,         
    ICON_APPLICATION,      
    ICON_SHARED_DOCUMENT,  
                           
  };

  AppIcon();
  ~AppIcon();

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<AppIcon>* converter);

  
  IconCategory category() const { return category_; }

  
  int icon_side_length() const { return icon_side_length_; }

  
  const ScopedVector<Link>& links() const { return links_; }

  
  
  GURL GetIconURL() const;

  void set_category(IconCategory category) { category_ = category; }
  void set_icon_side_length(int icon_side_length) {
    icon_side_length_ = icon_side_length;
  }
  void set_links(ScopedVector<Link> links) { links_ = links.Pass(); }

 private:
  
  
  static bool GetIconCategory(const base::StringPiece& category,
                              IconCategory* result);

  IconCategory category_;
  int icon_side_length_;
  ScopedVector<Link> links_;

  DISALLOW_COPY_AND_ASSIGN(AppIcon);
};

class CommonMetadata {
 public:
  CommonMetadata();
  virtual ~CommonMetadata();

  
  
  const Link* GetLinkByType(Link::LinkType type) const;

  
  base::Time updated_time() const { return updated_time_; }

  
  const std::string& etag() const { return etag_; }

  
  const ScopedVector<Author>& authors() const { return authors_; }

  
  const ScopedVector<Link>& links() const { return links_; }
  ScopedVector<Link>* mutable_links() { return &links_; }

  
  const ScopedVector<Category>& categories() const { return categories_; }

  void set_etag(const std::string& etag) { etag_ = etag; }
  void set_authors(ScopedVector<Author> authors) {
    authors_ = authors.Pass();
  }
  void set_links(ScopedVector<Link> links) {
    links_ = links.Pass();
  }
  void set_categories(ScopedVector<Category> categories) {
    categories_ = categories.Pass();
  }
  void set_updated_time(const base::Time& updated_time) {
    updated_time_ = updated_time;
  }

 protected:
  
  
  template<typename CommonMetadataDescendant>
  static void RegisterJSONConverter(
      base::JSONValueConverter<CommonMetadataDescendant>* converter);

  std::string etag_;
  ScopedVector<Author> authors_;
  ScopedVector<Link> links_;
  ScopedVector<Category> categories_;
  base::Time updated_time_;

  DISALLOW_COPY_AND_ASSIGN(CommonMetadata);
};

class ResourceEntry : public CommonMetadata {
 public:
  ResourceEntry();
  virtual ~ResourceEntry();

  
  
  
  
  
  
  
  
  
  
  
  static scoped_ptr<ResourceEntry> ExtractAndParse(const base::Value& value);

  
  
  
  
  static scoped_ptr<ResourceEntry> CreateFrom(const base::Value& value);

  
  static std::string GetEntryNodeName();

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<ResourceEntry>* converter);

  
  
  static bool HasFieldPresent(const base::Value* value, bool* result);

  
  
  
  static bool ParseChangestamp(const base::Value* value, int64* result);

  
  
  const std::string& resource_id() const { return resource_id_; }

  
  
  
  const std::string& id() const { return id_; }

  DriveEntryKind kind() const { return kind_; }
  const std::string& title() const { return title_; }
  base::Time published_time() const { return published_time_; }
  base::Time last_viewed_time() const { return last_viewed_time_; }
  const std::vector<std::string>& labels() const { return labels_; }

  
  
  const GURL& download_url() const { return content_.url(); }

  const std::string& content_mime_type() const { return content_.mime_type(); }

  
  
  
  const ScopedVector<ResourceLink>& resource_links() const {
    return resource_links_;
  }

  
  const std::string& filename() const { return filename_; }

  
  const std::string& suggested_filename() const { return suggested_filename_; }

  
  const std::string& file_md5() const { return file_md5_; }

  
  int64 file_size() const { return file_size_; }

  
  bool deleted() const { return deleted_ || removed_; }

  
  
  int64 changestamp() const { return changestamp_; }

  
  
  int64 image_width() const { return image_width_; }

  
  
  int64 image_height() const { return image_height_; }

  
  
  int64 image_rotation() const { return image_rotation_; }

  
  
  std::string GetEntryKindText() const;

  
  
  std::string GetHostedDocumentExtension() const;

  
  bool is_hosted_document() const {
    return (ClassifyEntryKind(kind_) & KIND_OF_HOSTED_DOCUMENT) > 0;
  }
  
  bool is_google_document() const {
    return (ClassifyEntryKind(kind_) & KIND_OF_GOOGLE_DOCUMENT) > 0;
  }
  
  bool is_external_document() const {
    return (ClassifyEntryKind(kind_) & KIND_OF_EXTERNAL_DOCUMENT) > 0;
  }
  
  bool is_folder() const {
    return (ClassifyEntryKind(kind_) & KIND_OF_FOLDER) > 0;
  }
  
  bool is_file() const {
    return (ClassifyEntryKind(kind_) & KIND_OF_FILE) > 0;
  }
  
  bool is_special() const {
    return !is_file() && !is_folder() && !is_hosted_document();
  }

  

  
  enum EntryKindClass {
    KIND_OF_NONE = 0,
    KIND_OF_HOSTED_DOCUMENT = 1,
    KIND_OF_GOOGLE_DOCUMENT = 1 << 1,
    KIND_OF_EXTERNAL_DOCUMENT = 1 << 2,
    KIND_OF_FOLDER = 1 << 3,
    KIND_OF_FILE = 1 << 4,
  };

  
  
  
  
  static int ClassifyEntryKind(DriveEntryKind kind);

  
  
  static int ClassifyEntryKindByFileExtension(const base::FilePath& file);

  void set_resource_id(const std::string& resource_id) {
    resource_id_ = resource_id;
  }
  void set_id(const std::string& id) { id_ = id; }
  void set_kind(DriveEntryKind kind) { kind_ = kind; }
  void set_title(const std::string& title) { title_ = title; }
  void set_published_time(const base::Time& published_time) {
    published_time_ = published_time;
  }
  void set_last_viewed_time(const base::Time& last_viewed_time) {
    last_viewed_time_ = last_viewed_time;
  }
  void set_labels(const std::vector<std::string>& labels) {
    labels_ = labels;
  }
  void set_content(const Content& content) {
    content_ = content;
  }
  void set_resource_links(ScopedVector<ResourceLink> resource_links) {
    resource_links_ = resource_links.Pass();
  }
  void set_filename(const std::string& filename) { filename_ = filename; }
  void set_suggested_filename(const std::string& suggested_filename) {
    suggested_filename_ = suggested_filename;
  }
  void set_file_md5(const std::string& file_md5) { file_md5_ = file_md5; }
  void set_file_size(int64 file_size) { file_size_ = file_size; }
  void set_deleted(bool deleted) { deleted_ = deleted; }
  void set_removed(bool removed) { removed_ = removed; }
  void set_changestamp(int64 changestamp) { changestamp_ = changestamp; }
  void set_image_width(int64 image_width) { image_width_ = image_width; }
  void set_image_height(int64 image_height) { image_height_ = image_height; }
  void set_image_rotation(int64 image_rotation) {
    image_rotation_ = image_rotation;
  }

  
  
  
  void FillRemainingFields();

 private:
  friend class base::internal::RepeatedMessageConverter<ResourceEntry>;
  friend class ResourceList;
  friend class ResumeUploadRequest;

  
  static DriveEntryKind GetEntryKindFromTerm(const std::string& term);
  
  static const char* GetEntryKindDescription(DriveEntryKind kind);

  std::string resource_id_;
  std::string id_;
  DriveEntryKind kind_;
  std::string title_;
  base::Time published_time_;
  
  base::Time last_viewed_time_;
  std::vector<std::string> labels_;
  Content content_;
  ScopedVector<ResourceLink> resource_links_;
  
  std::string filename_;
  std::string suggested_filename_;
  std::string file_md5_;
  int64 file_size_;
  bool deleted_;
  bool removed_;
  int64 changestamp_;
  int64 image_width_;
  int64 image_height_;
  int64 image_rotation_;

  DISALLOW_COPY_AND_ASSIGN(ResourceEntry);
};

class ResourceList : public CommonMetadata {
 public:
  ResourceList();
  virtual ~ResourceList();

  
  
  
  
  
  
  
  
  
  static scoped_ptr<ResourceList> ExtractAndParse(const base::Value& value);

  
  
  
  
  
  static scoped_ptr<ResourceList> CreateFrom(const base::Value& value);

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<ResourceList>* converter);

  
  
  bool GetNextFeedURL(GURL* url) const;

  
  const ScopedVector<ResourceEntry>& entries() const { return entries_; }
  ScopedVector<ResourceEntry>* mutable_entries() { return &entries_; }

  
  
  void ReleaseEntries(std::vector<ResourceEntry*>* entries);

  
  int start_index() const { return start_index_; }

  
  int items_per_page() const { return items_per_page_; }

  
  
  int64 largest_changestamp() const { return largest_changestamp_; }

  
  const std::string& title() { return title_; }

  void set_entries(ScopedVector<ResourceEntry> entries) {
    entries_ = entries.Pass();
  }
  void set_start_index(int start_index) {
    start_index_ = start_index;
  }
  void set_items_per_page(int items_per_page) {
    items_per_page_ = items_per_page;
  }
  void set_title(const std::string& title) {
    title_ = title;
  }
  void set_largest_changestamp(int64 largest_changestamp) {
    largest_changestamp_ = largest_changestamp;
  }

 private:
  
  
  bool Parse(const base::Value& value);

  ScopedVector<ResourceEntry> entries_;
  int start_index_;
  int items_per_page_;
  std::string title_;
  int64 largest_changestamp_;

  DISALLOW_COPY_AND_ASSIGN(ResourceList);
};

class InstalledApp {
 public:
  typedef std::vector<std::pair<int, GURL> > IconList;

  InstalledApp();
  virtual ~InstalledApp();

  
  const std::string& app_name() const { return app_name_; }

  
  const std::string& app_id() const { return app_id_; }

  
  const std::string& object_type() const { return object_type_; }

  
  bool supports_create() const { return supports_create_; }

  
  
  
  const ScopedVector<std::string>& primary_mimetypes() const {
    return primary_mimetypes_;
  }

  
  
  
  const ScopedVector<std::string>& secondary_mimetypes() const {
    return secondary_mimetypes_;
  }

  
  
  
  const ScopedVector<std::string>& primary_extensions() const {
    return primary_extensions_;
  }

  
  
  
  const ScopedVector<std::string>& secondary_extensions() const {
    return secondary_extensions_;
  }

  
  const ScopedVector<Link>& links() const { return links_; }

  
  const ScopedVector<AppIcon>& app_icons() const {
    return app_icons_;
  }

  
  
  IconList GetIconsForCategory(AppIcon::IconCategory category) const;

  
  GURL GetProductUrl() const;

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<InstalledApp>* converter);

  void set_app_id(const std::string& app_id) { app_id_ = app_id; }
  void set_app_name(const std::string& app_name) { app_name_ = app_name; }
  void set_object_type(const std::string& object_type) {
    object_type_ = object_type;
  }
  void set_supports_create(bool supports_create) {
    supports_create_ = supports_create;
  }
  void set_primary_mimetypes(
      ScopedVector<std::string> primary_mimetypes) {
    primary_mimetypes_ = primary_mimetypes.Pass();
  }
  void set_secondary_mimetypes(
      ScopedVector<std::string> secondary_mimetypes) {
    secondary_mimetypes_ = secondary_mimetypes.Pass();
  }
  void set_primary_extensions(
      ScopedVector<std::string> primary_extensions) {
    primary_extensions_ = primary_extensions.Pass();
  }
  void set_secondary_extensions(
      ScopedVector<std::string> secondary_extensions) {
    secondary_extensions_ = secondary_extensions.Pass();
  }
  void set_links(ScopedVector<Link> links) {
    links_ = links.Pass();
  }
  void set_app_icons(ScopedVector<AppIcon> app_icons) {
    app_icons_ = app_icons.Pass();
  }

 private:
  
  
  static bool GetValueString(const base::Value* value,
                             std::string* result);

  std::string app_id_;
  std::string app_name_;
  std::string object_type_;
  bool supports_create_;
  ScopedVector<std::string> primary_mimetypes_;
  ScopedVector<std::string> secondary_mimetypes_;
  ScopedVector<std::string> primary_extensions_;
  ScopedVector<std::string> secondary_extensions_;
  ScopedVector<Link> links_;
  ScopedVector<AppIcon> app_icons_;
};

class AccountMetadata {
 public:
  AccountMetadata();
  virtual ~AccountMetadata();

  
  
  
  
  
  static scoped_ptr<AccountMetadata> CreateFrom(const base::Value& value);

  int64 quota_bytes_total() const {
    return quota_bytes_total_;
  }

  int64 quota_bytes_used() const {
    return quota_bytes_used_;
  }

  int64 largest_changestamp() const {
    return largest_changestamp_;
  }

  const ScopedVector<InstalledApp>& installed_apps() const {
    return installed_apps_;
  }

  void set_quota_bytes_total(int64 quota_bytes_total) {
    quota_bytes_total_ = quota_bytes_total;
  }
  void set_quota_bytes_used(int64 quota_bytes_used) {
    quota_bytes_used_ = quota_bytes_used;
  }
  void set_largest_changestamp(int64 largest_changestamp) {
    largest_changestamp_ = largest_changestamp;
  }
  void set_installed_apps(ScopedVector<InstalledApp> installed_apps) {
    installed_apps_ = installed_apps.Pass();
  }

  
  
  static void RegisterJSONConverter(
      base::JSONValueConverter<AccountMetadata>* converter);

 private:
  
  
  bool Parse(const base::Value& value);

  int64 quota_bytes_total_;
  int64 quota_bytes_used_;
  int64 largest_changestamp_;
  ScopedVector<InstalledApp> installed_apps_;

  DISALLOW_COPY_AND_ASSIGN(AccountMetadata);
};


}  

#endif  
