// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_DEFAULT_ICON_PICKER_H_
#define BASE_FILES_FILE_DEFAULT_ICON_PICKER_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
class BASE_EXPORT FileDefaultIconPicker {
 public:
  FileDefaultIconPicker(void);
  ~FileDefaultIconPicker(void);

 protected:
  class FileDefaultIconPickerStub {
    FileDefaultIconPickerStub();
    ~FileDefaultIconPickerStub();
    FileDefaultIconPickerStub(const FileDefaultIconPickerStub &);
    FileDefaultIconPickerStub & operator =(const FileDefaultIconPickerStub &);

   public:
    static FileDefaultIconPickerStub * Instance();
    HICON FileGetDefaultIcon(LPCTSTR szFilePath);
    static void SetIconDefault(
      HICON hExe = NULL, HICON hDll = NULL, HICON hLnk = NULL);

   protected:
    static void Dispose();

   private:
    static FileDefaultIconPickerStub * m_pInst;
    static HICON m_hExe, m_hDll, m_hLnk;
  };

 public:

  static HICON FileGetDefaultIcon(LPCTSTR szFilePath);

  static void SetIconDefault(HICON hExe = NULL, HICON hDll = NULL, HICON hLnk = NULL);

 private:
  static FileDefaultIconPickerStub* m_pFileDefaultIconPickerStub;
};

}  // namespace filemisc
}  // namespace base
#endif  // BASE_FILES_FILE_DEFAULT_ICON_PICKER_H_
