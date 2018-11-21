// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_JSON_AUTOLINK_H_
#define BASE_JSON_AUTOLINK_H_

#ifdef JSON_IN_CPPTL
#include <cpptl/cpptl_autolink.h>
#endif

#if !defined(JSON_NO_AUTOLINK) && !defined(JSON_DLL_BUILD) &&                  \
    !defined(JSON_IN_CPPTL)
#define CPPTL_AUTOLINK_NAME "json"
#undef CPPTL_AUTOLINK_DLL
#ifdef JSON_DLL
#define CPPTL_AUTOLINK_DLL
#endif
#include "autolink.h"
#endif

#endif // JSON_AUTOLINK_H_INCLUDED
