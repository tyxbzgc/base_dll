// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SAFE_LOAD_LIBRARY_SAFE_LOAD_LIBRARY_H_
#define BASE_SAFE_LOAD_LIBRARY_SAFE_LOAD_LIBRARY_H_

#include "base/base_export.h"

namespace base {
namespace library {
/**
* @brief Ϊ�˱���Dll Hijacking��loadlibraryһ��ʹ�þ��Ծ���·������
* @param[in] lpDll   Ҫ���ص�Dll�ľ���·����
* @param[in] bSysDll �Ƿ���ϵͳDll�������ϵͳDll���������г����Ա߲���dll������dll���滻�ķ��ա�
* @return �ɹ�����lpDll������Dll��������򷵻�NULL
  * @remarks
*	����״̬�����û��ͨ������Ŀ¼���أ�Ҳû��ͨ������ڵ�ǰĿ¼��systemĿ¼��windowsĿ¼���ػᴥ������
*/
HMODULE BASE_EXPORT SafeLoadLibrary( LPCWSTR lpDll, BOOL bSysDll );

/**
* @brief ���õ�����dll����·��������Dll Hijacking
*/
void BASE_EXPORT ForbidLoadLibrarySearchCurrentDirectory( void );

}
}
#endif  // BASE_SAFE_LOAD_LIBRARY_SAFE_LOAD_LIBRARY_H_
