// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_JSON_ASSERTIONS_H_
#define BASE_JSON_ASSERTIONS_H_

#include <stdlib.h>
#include <sstream>

#include "base/base_export.h"

/** It should not be possible for a maliciously designed file to
 *  cause an abort() or seg-fault, so these macros are used only
 *  for pre-condition violations and internal logic errors.
 */
#if JSON_USE_EXCEPTION

// @todo <= add detail about condition in exception
# define JSON_ASSERT(condition)                                                \
  {if (!(condition)) {Json::throwLogicError( "assert json failed" );}}

# define JSON_FAIL_MESSAGE(message)                                            \
  {                                                                            \
    JSONCPP_OSTRINGSTREAM oss; oss << message;                                    \
    Json::throwLogicError(oss.str());                                          \
    abort();                                                                   \
  }

#else // JSON_USE_EXCEPTION

# define JSON_ASSERT(condition) assert(condition)

// The call to assert() will show the failure message in debug builds. In
// release builds we abort, for a core-dump or debugger.
# define JSON_FAIL_MESSAGE(message)                                            \
  {                                                                            \
    JSONCPP_OSTRINGSTREAM oss; oss << message;                                    \
    assert(false && oss.str().c_str());                                        \
    abort();                                                                   \
  }


#endif

#define JSON_ASSERT_MESSAGE(condition, message)                                \
  if (!(condition)) {                                                          \
    JSON_FAIL_MESSAGE(message);                                                \
  }

#endif  // BASE_JSON_ASSERTIONS_H_
