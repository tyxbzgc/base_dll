// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.

#ifndef BASE_COMMAND_LINE_H_
#define BASE_COMMAND_LINE_H_

#include <map>
#include <vector>

#include "base/base_export.h"
#include "base/basic_types.h"
#include "base/platform.h"
#include "base/macros.h"

namespace base {
class BASE_EXPORT CommandLine {
  public:
    explicit CommandLine(const string16 & program = string16(L""));

  public:
    void ParseFromString(const string16 & command_line);

    const string16& GetCommandLine();
    string16 GetProgram() const {
        return program_;
    }
    const std::vector<string16> & GetNoneSwitchArgs() const {
        return nonswitch_args_;
    }

    string16 GetSwitchValue(const string16 & switch_name);

    void AppendSwitchArg(const string16 & switch_name
                         , const string16 & switch_value);

    bool HasExternalLink();

  public:
    // Define const switch names, just instead of macros
    static const wchar_t * kPrefix;
    static const wchar_t * kDelimeter;

    static const wchar_t * kProcessType;
    static const wchar_t * kPipeName;
    static const wchar_t * kBreakpad;
    static const wchar_t * kOuter;

  private:
    string16 argv_;
    wchar_t path_[MAX_PATH];
    bool changed_;

    string16 program_;
    std::map<string16, string16> switch_values_;
    // Non-switch command-line arguments.
    std::vector<string16> nonswitch_args_;

    DISALLOW_COPY_AND_ASSIGN(CommandLine);
};
}  // namespace base

#endif  // BASE_COMMAND_LINE_H_
