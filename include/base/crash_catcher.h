// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.
#ifndef BASE_CRASHCATCHER_H_
#define BASE_CRASHCATCHER_H_

#include "base/basic_types.h"

namespace base {
// custom the internal crash behavior, and pass it to WebRender::Create
struct CrashCatcherSetting {
    class WaitableEvent {
      public:
        virtual void Notify() = 0;
    };

    struct CrashCatchInfo {
        unsigned long pid;
        unsigned long tid;

        ////////////////////////////////////
        // in memory space of |pid_| process
        void* peb_address;
        void* teb_address;
        void* exception_pointers; /*EXCEPTION_POINTERS* under windows*/
        ////////////////////////////////////

        uint64 living_time;
        uint64 avail_virtualmem;
        string16 dump_path_;
        string16 command_line_;

        CrashCatchInfo() : pid(0)
            , tid(0)
            , peb_address(NULL)
            , teb_address(NULL)
            , living_time(0)
            , avail_virtualmem(0)
            , exception_pointers(NULL) {}
    };

    // crashed process will wait to exit until |event|->Notify
    // was called or 60s timeout
    typedef void(*OnCrashedCallback)(const CrashCatchInfo& _info
                                     , WaitableEvent* _event);

    // be sure you have the write access to |auto_dump_dir_|,
    // dump will be auto-generated under this directory
    // if |generate_dump_| is true
    string16 auto_dump_dir_;
    bool generate_dump_;
    OnCrashedCallback on_crashed_callback_func_;

    CrashCatcherSetting() : on_crashed_callback_func_(nullptr)
        , generate_dump_(true) {}
};
}  // namespace base
#endif  // BASE_CRASHCATCHER_H_
