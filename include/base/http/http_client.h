// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_HTTP_HTTP_CLIENT_H_
#define BASE_HTTP_HTTP_CLIENT_H_

#include "base/base_export.h"

namespace base {
namespace http {
class BASE_EXPORT HttpClient {
 public:
  HttpClient(void);
  ~HttpClient(void);
 public:
  /**
  * @brief HTTP POST����
  * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
  * @param strPost �������,ʹ�����¸�ʽpara1=val1?2=val2&��
  * @param strResponse �������,���ص�����
  * @return �����Ƿ�Post�ɹ�
  */
  int Post(const std::string & strUrl,
           const std::string & strPost,
           std::string & strResponse);

  /**
  * @brief HTTP GET����
  * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com
  * @param strResponse �������,���ص�����
  * @return �����Ƿ�Post�ɹ�
  */
  int Get(const std::string & strUrl, std::string & strResponse);

  /**
  * @brief HTTPS POST����,��֤��汾
  * @param strUrl �������,�����Url��ַ,��:https://www.alipay.com
  * @param strPost �������,ʹ�����¸�ʽpara1=val1?2=val2&��
  * @param strResponse �������,���ص�����
  * @param pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��.
  * @return �����Ƿ�Post�ɹ�
  */
  int Posts(const std::string & strUrl,
            const std::string & strPost,
            std::string & strResponse,
            const char * pCaPath = NULL);

  /**
  * @brief HTTPS GET����,��֤��汾
  * @param strUrl �������,�����Url��ַ,��:https://www.alipay.com
  * @param strResponse �������,���ص�����
  * @param pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��.
  * @return �����Ƿ�Post�ɹ�
  */
  int Gets(const std::string & strUrl,
           std::string & strResponse,
           const char * pCaPath = NULL);

 public:
  void SetDebug(bool bDebug);

 private:
  bool m_bDebug;
};
}  // namespace http
}  // namespace base
#endif  // BASE_HTTP_HTTP_CLIENT_H_
