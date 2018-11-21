// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/files/file_default_icon_picker.h"
#include <Shlwapi.h>
#include <ShellAPI.h>
#include <tchar.h>

namespace base {
namespace filemisc {

FileDefaultIconPicker::FileDefaultIconPickerStub::FileDefaultIconPickerStub() {
}

FileDefaultIconPicker::FileDefaultIconPickerStub::~FileDefaultIconPickerStub() {
  if(NULL != m_hExe) {
    ::DestroyIcon(m_hExe);
    m_hExe = NULL;
  }
  if(NULL != m_hDll) {
    ::DestroyIcon(m_hDll);
    m_hExe = NULL;
  }
  if(NULL != m_hLnk) {
    ::DestroyIcon(m_hLnk);
    m_hExe = NULL;
  }
}

HICON FileDefaultIconPicker::FileDefaultIconPickerStub::m_hExe = NULL;
HICON FileDefaultIconPicker::FileDefaultIconPickerStub::m_hDll = NULL;
HICON FileDefaultIconPicker::FileDefaultIconPickerStub::m_hLnk = NULL;
FileDefaultIconPicker::FileDefaultIconPickerStub* FileDefaultIconPicker::FileDefaultIconPickerStub::m_pInst = NULL;

FileDefaultIconPicker::FileDefaultIconPickerStub* FileDefaultIconPicker::FileDefaultIconPickerStub::Instance() {
  if(NULL == m_pInst) {
    m_pInst = new FileDefaultIconPickerStub();
    atexit(Dispose);
  }
  return m_pInst;
}

VOID FileDefaultIconPicker::FileDefaultIconPickerStub::Dispose() {
  if(NULL != m_pInst) {
    delete m_pInst;
    m_pInst = NULL;
  }
}

HICON FileDefaultIconPicker::FileDefaultIconPickerStub::FileGetDefaultIcon(LPCTSTR szFilePath) {
  HICON hIcon = NULL;
  LPTSTR szExt = NULL;

  if(NULL == szFilePath ||
      NULL == szFilePath[0] ||
      FALSE == ::PathFileExists(szFilePath))
    goto Exit0;

  szExt = ::PathFindExtension(szFilePath);
  if(NULL == szExt)
    goto Exit0;
  if(0 == ::_tcsicmp(szExt, _T(".exe")) && NULL != m_hExe)
    hIcon = ::DuplicateIcon(NULL, m_hExe);
  else if(0 == ::_tcsicmp(szExt, _T(".dll")) && NULL != m_hDll)
    hIcon = ::DuplicateIcon(NULL, m_hDll);
  else if(0 == ::_tcsicmp(szExt, _T(".lnk")) && NULL != m_hLnk)
    hIcon = ::DuplicateIcon(NULL, m_hLnk);
  else
    goto Exit0;
Exit0:
  return hIcon;
}

VOID FileDefaultIconPicker::FileDefaultIconPickerStub::SetIconDefault(
  HICON hExe /* = NULL */, HICON hDll /* = NULL */, HICON hLnk /* = NULL */) {
  m_hExe = hExe;
  m_hDll = hDll;
  m_hLnk = hLnk;
}

FileDefaultIconPicker::FileDefaultIconPickerStub *
  FileDefaultIconPicker::m_pFileDefaultIconPickerStub = NULL;

FileDefaultIconPicker::FileDefaultIconPicker(void) {
  m_pFileDefaultIconPickerStub = FileDefaultIconPicker::FileDefaultIconPickerStub::Instance();
}

FileDefaultIconPicker::~FileDefaultIconPicker(void) {
}

HICON FileDefaultIconPicker::FileGetDefaultIcon(LPCTSTR szFilePath) {
  if(NULL == m_pFileDefaultIconPickerStub) {
    m_pFileDefaultIconPickerStub =
      FileDefaultIconPicker::FileDefaultIconPickerStub::Instance();
  }
  return m_pFileDefaultIconPickerStub->FileGetDefaultIcon(szFilePath);
}

VOID FileDefaultIconPicker::SetIconDefault(
  HICON hExe /* = NULL */, HICON hDll /* = NULL */, HICON hLnk /* = NULL */) {
  if(NULL == m_pFileDefaultIconPickerStub) {
    m_pFileDefaultIconPickerStub =
      FileDefaultIconPicker::FileDefaultIconPickerStub::Instance();
  }
  m_pFileDefaultIconPickerStub->SetIconDefault(
    hExe, hDll, hLnk);
}
}  // namespace filemisc
}  // namespace base
