// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_VERSION_INFO_WIN_H_
#define BASE_FILES_FILE_VERSION_INFO_WIN_H_

#include <string>

#include "base/base_export.h"
#include "base/basic_types.h"
#include "base/files/file_version_info.h"

struct tagVS_FIXEDFILEINFO;
typedef tagVS_FIXEDFILEINFO VS_FIXEDFILEINFO;

namespace base {
namespace filemisc {
class BASE_EXPORT FileVersionInfoWin : public FileVersionInfo {
public:
  FileVersionInfoWin(void* data, WORD language, WORD code_page);
  ~FileVersionInfoWin() override;

  // Accessors to the different version properties.
  // Returns an empty string if the property is not found.
  string16 company_name() override;
  string16 company_short_name() override;
  string16 product_name() override;
  string16 product_short_name() override;
  string16 internal_name() override;
  string16 product_version() override;
  string16 private_build() override;
  string16 special_build() override;
  string16 comments() override;
  string16 original_filename() override;
  string16 file_description() override;
  string16 file_version() override;
  string16 legal_copyright() override;
  string16 legal_trademarks() override;
  string16 last_change() override;
  bool is_official_build() override;

  // Lets you access other properties not covered above.
  bool GetValue(const wchar_t* name, std::wstring* value);

  // Similar to GetValue but returns a wstring (empty string if the property
  // does not exist).
  std::wstring GetStringValue(const wchar_t* name);

  // Get the fixed file info if it exists. Otherwise NULL
  VS_FIXEDFILEINFO* fixed_file_info() { return fixed_file_info_; }

private:
  char* data_;
  WORD language_;
  WORD code_page_;
  // This is a pointer into the data_ if it exists. Otherwise NULL.
  VS_FIXEDFILEINFO* fixed_file_info_;

  DISALLOW_COPY_AND_ASSIGN(FileVersionInfoWin);
};
}  // namespace filemisc
}  // namespace base
#endif  // BASE_FILES_FILE_VERSION_INFO_WIN_H_
