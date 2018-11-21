// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_JSON_FORWARDS_H_
#define BASE_JSON_FORWARDS_H_

#include "base/base_export.h"
#include "base/json/json_config.h"

namespace base {
namespace json {

// writer.h
class FastWriter;
class StyledWriter;

// reader.h
class Reader;

// features.h
class Features;

// value.h
typedef unsigned int ArrayIndex;
class StaticString;
class Path;
class PathArgument;
class Value;
class ValueIteratorBase;
class ValueIterator;
class ValueConstIterator;

}  // namespace json
}  // namespace base
#endif  // BASE_JSON_FORWARDS_H_
