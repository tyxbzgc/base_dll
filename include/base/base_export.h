// Copyright (c) 2017 The TAL Authors.
// All rights reserved.


#ifndef BASE_BASE_EXPORT_H_
#define BASE_BASE_EXPORT_H_

#if defined(BASE_IMPL)
#define BASE_EXPORT __declspec(dllexport)
#else
#define BASE_EXPORT __declspec(dllimport)
#endif
#define CALLBACK __stdcall

#include <assert.h>
#define BASEASSERT assert

#include "base/basic_types.h"

#include "base/stldef.h"

#endif  // BASE_BASE_EXPORT_H_
