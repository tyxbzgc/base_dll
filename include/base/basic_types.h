// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file contains definitions of our old basic integral types
// ((u)int{8,16,32,64}) and further includes. I recommend that you use the C99
// standard types instead, and include <stdint.h>/<stddef.h>/etc. as needed.
// Note that the macros and macro-like constructs that were formerly defined in
// this file are now available separately in base/macros.h.

#ifndef BASE_BASIC_TYPES_H_
#define BASE_BASIC_TYPES_H_

#include <limits.h>  // So we can set the bounds of our types.
#include <stddef.h>  // For size_t.
#include <stdint.h>  // For intptr_t.

#include <string>

#include "base/macros.h"

// DEPRECATED: Please use (u)int{8,16,32,64}_t instead (and include <stdint.h>).
typedef unsigned long dword_t;
typedef unsigned short word_t;

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef word_t word16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef dword_t dword32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef uint32 color_t;

typedef time_t time64;
typedef wchar_t char16;
typedef char    char8;

// DEPRECATED: Please use std::numeric_limits (from <limits>) instead.
const uint8  kUint8Max = 0xFF;
const uint16 kUint16Max = 0xFFFF;
const uint32 kUint32Max = 0xFFFFFFFF;
const uint64 kUint64Max = 0xFFFFFFFFFFFFFFFFULL;
const  int8  kInt8Min = -0x7F - 1;
const  int8  kInt8Max = 0x7F;
const  int16 kInt16Min = -0x7FFF - 1;
const  int16 kInt16Max = 0x7FFF;
const  int32 kInt32Min = -0x7FFFFFFF - 1;
const  int32 kInt32Max = 0x7FFFFFFF;
const  int64 kInt64Min = -0x7FFFFFFFFFFFFFFFLL - 1;
const  int64 kInt64Max = 0x7FFFFFFFFFFFFFFFLL;

// std string
typedef std::wstring string16;
typedef std::string string8;

#endif  // BASE_BASIC_TYPES_H_
