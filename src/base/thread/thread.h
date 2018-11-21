// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.


#ifndef BASE_THREAD_THREAD_H_
#define BASE_THREAD_THREAD_H_

#include <math.h>

#include <list>
#include <map>
#include <queue>
#include <string>

#include "base/base_export.h"
#include "base/lock.h"
#include "base/platform.h"
#include "base/smart_ptr/ref_counted.h"
#include "base/smart_ptr/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/task.h"
#include "base/thread/thread_destruction_observer.h"
#include "base/thread/thread_local_pointer.h"
#include "base/thread/thread_name.h"
#include "base/time/time.h"

namespace base {
class BASE_EXPORT MessagePump
  : public base::RefCountedThreadSafe < MessagePump > {
 public:
  MessagePump();

 public:
  class BASE_EXPORT Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}

    // Called from within Run in response to ScheduleWork or when the
    // message pump would otherwise call DoDelayedWork.  Returns true to
    // indicate that work was done.  DoDelayedWork will not be called if
    // DoWork returns true.
    virtual bool DoWork() = 0;

    // Called from within Run in response to ScheduleDelayedWork or when
    // the message pump would otherwise sleep waiting for more work.
    // Returns true to indicate that delayed work was done.  DoIdleWork will
    // not be called if DoDelayedWork returns true.  Upon return
    // |next_delayed_work_time| indicates the time when DoDelayedWork should
    // be called again. If |next_delayed_work_time| is null (per Time::
    // is_null), then the queue of future delayed work (timer events) is
    // currently empty, and no additional calls to this function need to be
    // scheduled.
    virtual bool DoDelayedWork(base::Time * next_delayed_work_time) = 0;

    // Called from within Run just before the message pump goes to sleep.
    // Returns true to indicate that idle work was done.
    virtual bool DoIdleWork() = 0;
  };

  // DoRunLoop, store and restore state.
  void RunWithDispatcher(Delegate* delegate
    , Dispatcher * dispatcher
    , bool process_task);

  // MessagePump methods:
  virtual void Run(Delegate * delegate) {
    RunWithDispatcher(delegate, NULL, true);
  }

  virtual void Quit();

  // Schedule a DoWork callback to happen reasonably soon.  Does nothing if a
  // DoWork callback is already scheduled.  This method may be called from any
  // thread.  Once this call is made, DoWork should not be "starved" at least
  // until it returns a value of false.
  virtual void ScheduleWork() = 0;

  // Schedule a DoDelayedWork callback to happen at the specified time,
  // cancelling any pending DoDelayedWork callback.  This method may only be
  // used on the thread that called Run.
  virtual void ScheduleDelayedWork(const base::Time& delayed_work_time) = 0;

 protected:
  struct RunState {
    Delegate * delegate_;
    Dispatcher * dispatcher_;

    // Used to flag that the current Run() invocation should return ASAP.
    bool should_quit_;

    // Used to count how many Run() invocations are on the stack.
    int32 run_depth_;
  };

  // run loop
  virtual void DoRunLoop() = 0;

  // delta micro seconds from delayed_work_time to now
  int32 GetCurrentDelay() const;

 protected:
  virtual ~MessagePump();
  friend class base::RefCountedThreadSafe < MessagePump >;

  // The time at which delayed work should run.
  base::Time delayed_work_time_;

  // A boolean value used to indicate if there is a Task pending
  // There is at most one such message, and it
  // can drive execution of tasks when a native message pump is running.
  long have_work_;

  // State for the current invocation of Run.
  RunState * state_;
};

class BASE_EXPORT MessagePumpForUI : public MessagePump {
 public:
  // The application-defined code passed to the hook procedure.
  static const int kMessageFilterCode = 0x5001;

  MessagePumpForUI();
  virtual ~MessagePumpForUI();

  // post kMsgHaveWork message
  virtual void ScheduleWork();
  // set delayed_work_time, start a timer
  virtual void ScheduleDelayedWork(const base::Time& delayed_work_time);

  // Applications can call this to encourage us to process all pending
  // WM_PAINT messages.This method will process all paint messages the Windows
  // Message queue can provide, up to some fixed number (to avoid any infinite
  // loops). peek and process(ProcessMessageHelper) paint message
  void PumpOutPendingPaintMessages();

  bool AddMsgFilter(base::MessageFilter * filter);
  bool RemoveMsgFilter(base::MessageFilter * filter);
  bool ProcessMessage(const MSG& msg);

  bool AddShortCutKeyMsgFilter(base::MessageFilter * filter);
  bool RemoveShortCutKeyMsgFilter(base::MessageFilter * filter);

  bool MessagePumpForUI::AddAcceleratorHanderMsgFilter(
    base::MessageFilter * filter);

 private:
  // route to HandleWorkMessage or HandleTimerMessage
  static LRESULT CALLBACK WndProcThunk(HWND hwnd
    , uint32 message
    , WPARAM wparam
    , LPARAM lparam);

  virtual void DoRunLoop();

  // create the window for message loop
  void InitMessageWnd();

  // MsgWaitForMultipleObjectsEx
  void WaitForWork();

  // ProcessPumpReplacementMessage, DoWork, ScheduleWork
  void HandleWorkMessage();

  // DoDelayedWork, invoke ScheduleDelayedWork if needed
  void HandleTimerMessage();

  // Peek message and ProcessMessageHelper
  bool ProcessNextWindowsMessage();

  // call either PostQuitMessage, ProcessPumpReplacementMessage
  // , CallMsgFilter, dispatcher_->Dispatch
  bool ProcessMessageHelper(const MSG& msg);

  // PeekMessage, ScheduleWork, ProcessMessageHelper
  bool ProcessPumpReplacementMessage();

  bool PreTranslateMessage(const MSG* msg);
  bool HandleAccelerator(const MSG& msg);

  // A hidden message-only window.
  HWND message_wnd_;

  std::list<base::MessageFilter *> filters_;
  std::list<base::MessageFilter *> shortcut_filters_;

  std::list<base::MessageFilter *> accelerator_filters_;
};

//-----------------------------------------------------------------------------
// MessagePumpForIO extends MessagePumpWin with methods that are particular to a
// MessageLoop instantiated with TYPE_IO. This version of MessagePump does not
// deal with Windows mesagges, and instead has a Run loop Based on Completion
// Ports so it is better suited for IO operations.
//
class BASE_EXPORT MessagePumpForIO : public MessagePump {
 public:
  MessagePumpForIO();
  virtual ~MessagePumpForIO() {
    CloseHandle(completion_port_);
  }

  // MessagePump methods:
  virtual void ScheduleWork();
  virtual void ScheduleDelayedWork(const base::Time& delayed_work_time);

  // Register the handler to be used when asynchronous IO for the given file
  // completes. The registration persist as long as |file_handle| is valid,
  // so |handler| must be valid as long as there is pending IO for the given
  // file.
  void RegisterIOHandler(HANDLE pipe, IOHandler * handler);

  // Waits for the next IO completion that should be processed by |filter|,
  // for up to |timeout| milliseconds. Return true if any IO operation
  // completed,regardless of the involved handler, and false if the timeout
  // expired. If the completion port received any message and the involved IO
  // handler matches |filter|, the callback is called before returning from
  // this code;if the handler is not the one that we are looking for, the
  // callback will be postponed for another time, so reentrancy problems can
  // be avoided.External use of this method should be reserved for the rare
  // case when the caller is willing to allow pausing regular task dispatching
  // on this thread.
  bool WaitForIOCompletion(DWORD timeout, IOHandler * filter);

 private:
  struct IOItem {
    IOHandler * handler_;
    IOContext * context_;
    DWORD bytes_transfered_;
    DWORD error_;
  };

  virtual void DoRunLoop();

  // WaitForIOCompletion(CurrentDelay, NULL);
  void WaitForWork();

  // Returns a completion item that was previously received, matching the
  // filter
  bool MatchCompletedIOItem(IOHandler* filter, IOItem * item);

  // Asks the OS for another IO completion result
  bool GetIOItem(DWORD timeout, IOItem * item);

  bool ProcessInternalIOItem(const IOItem & item);

  // The completion port associated with this thread.
  HANDLE completion_port_;
  // This list will be empty almost always. It stores IO completions that have
  // not been delivered yet because somebody was doing cleanup.
  std::list<IOItem> completed_items_;
};

class BASE_EXPORT MessagePumpDefault : public MessagePump {
 public:
  MessagePumpDefault();
  ~MessagePumpDefault() {}

  // MessagePump methods:
  virtual void DoRunLoop();

  virtual void ScheduleWork();
  virtual void ScheduleDelayedWork(const base::Time& delayed_work_time);

 private:
  // Used to sleep until there is more work to do.
  base::WaitableEvent event_;

  // The time at which we should call DoDelayedWork.
  base::Time delayed_work_time_;
};
}  // namespace base

namespace base {
class BASE_EXPORT MessageLoop : public MessagePump::Delegate {
 public:
  friend class MessageLoopProxyInternal;

  enum Type {
    TYPE_DEFAULT, TYPE_UI, TYPE_IO
  };

  void AddDestructionObserver(base::DestructionObserver * destruction_observer);

  // Remove a DestructionObserver.  It is safe to call this method while a
  // DestructionObserver is receiving a notification callback.
  void RemoveDestructionObserver(
    base::DestructionObserver * destruction_observer);
  bool AddIdleHandler(IdleHandler* idle_handler);
  bool RemoveIdleHandler(IdleHandler* idle_handler);
  void DoIdleHandler();

 public:
  explicit MessageLoop(Type type = TYPE_DEFAULT
    , const string8 &thread_name = "TYPE_DEFAULT");
  virtual ~MessageLoop(void);

  void OnCrash() {
    state_ = NULL;
  }
  bool GetMessageLoopProxy(
    base::ScopedRefPtr<base::MessageLoopProxyInternal> * message_loop_proxy);

  // post task functions, call PostTask_Helper
  void PostTask(Task * task);
  void PostDelayedTask(Task * task, int64 delay_milli_second);
  void PostNonNestableTask(Task * task);
  void PostNonNestableDelayedTask(Task * task, int64 delay_milli_second);

  void Run();

  // Process all pending tasks, windows messages, etc., but don't wait/sleep.
  // Return as soon as all items that can be run are taken care of.
  void RunAllPending();

  // quit_received = TRUE
  void Quit();

  // This method is a variant of Quit, that does not wait for pending messages
  // to be processed before returning from Run.
  // pump->Quit
  void QuitNow();

  // This method is force Quit mutli-stack pump_->Run(this)
  void QuitAll();

  // Invokes Quit on the current MessageLoop when run.  Useful to schedule an
  // arbitrary MessageLoop to Quit.
  class QuitTask : public Task {
   public:
    virtual void Run() {
      MessageLoop * message_loop = MessageLoop::Current();
      if (message_loop) {
        message_loop->Quit();
      }
    }
  };

  // Returns the type passed to the constructor.
  Type GetType() const { return type_; }

  // Returns the MessageLoop object for the current thread, or null if none.
  static MessageLoop * Current();

  // Enables or disables the recursive task processing. This happens in the
  // case of recursive message loops. Some unwanted message loop may occurs
  // when using base controls or printer functions. By default, recursive task
  // processing is disabled.
  //
  // The specific case where tasks get queued is:
  // - The thread is running a message loop.
  // - It receives a task #1 and execute it.
  // - The task #1 implicitly start a message loop, like a MessageBox in the
  //   unit test. This can also be StartDoc or GetSaveFileName.
  // - The thread receives a task #2 before or while in this second message
  //   loop.
  // - With NestableTasksAllowed set to true, the task #2 will run right away.
  //   Otherwise, it will get executed right after task #1 completes at
  //   "thread message loop level".
  void SetNestableTasksAllowed(bool allowed);
  bool NestableTasksAllowed() const;

  // Enables nestable tasks on |loop| while in scope.
  class ScopedNestableTaskAllower {
   public:
    explicit ScopedNestableTaskAllower(MessageLoop* message_loop)
      : message_loop_(message_loop),
      original_allowed_(message_loop_->NestableTasksAllowed()) {
      message_loop_->SetNestableTasksAllowed(true);
    }

    ~ScopedNestableTaskAllower() {
      message_loop_->SetNestableTasksAllowed(original_allowed_);
    }

   private:
    MessageLoop * message_loop_;
    bool original_allowed_;
  };

  // Returns true if we are currently running a nested message loop.
  bool IsNested();

  int32 RunDepth();

  int32 WaitingTaskNumber();

  typedef base::Dispatcher Dispatcher;

  void RunWithDispatcher(Dispatcher *dispatcher) {
    AutoRunState save_state(this);
    state_->dispatcher_ = dispatcher;
    RunHandler();
  }

  //--------------------------------------------------------------------------
 protected:
  struct RunState {
    // Used to count how many Run() invocations are on the stack.
    int32 run_depth_;

    // Used to record that Quit() was called, or that we should quit the
    // pump once it becomes idle.
    bool quit_received;

    Dispatcher * dispatcher_;
  };

  // used for state save and restore
  class BASE_EXPORT AutoRunState : RunState {
   public:
    explicit AutoRunState(MessageLoop * message_loop);
    ~AutoRunState();
   private:
    MessageLoop* message_loop_;
    RunState * previous_state_;
  };

  // This structure is copied around by value.
  struct PendingTask {
    Task * task_;         // The task to run.
    base::Time delayed_run_time_;  // The time when the task should be run.
    int32 sequence_number;    // Used to facilitate sorting by run time.
    bool nestable_;      // True if OK to dispatch from a nested loop.

    PendingTask(Task * task, bool nestable)
      : task_(task)
      , sequence_number(0)
      , nestable_(nestable) {
    }

    // Used to support sorting.
    bool operator<(const PendingTask& other) const;
  };

  class TaskQueue : public std::queue < PendingTask > {
   public:
    void Swap(TaskQueue * queue) {
      c.swap(queue->c);  // Calls std::deque::swap
    }
  };

  typedef std::priority_queue<PendingTask> DelayedTaskQueue;

  MessagePump * pump() {
    return static_cast<MessagePump *>(pump_.get());
  }

  void RunHandler();

  // pump->Run();
  void RunInternal();

  // Called to process any delayed non-nestable tasks :
  // deferred_non_nestable_work_queue_.top();
  bool ProcessNextDelayedNonNestableTask();

  // Runs the specified task and deletes it.
  void RunTask(Task * task);

  // Calls RunTask or queues the pending_task on the deferred task list if it
  // cannot be run right now.  Returns true if the task was run.
  bool DeferOrRunPendingTask(const PendingTask& pending_task);

  // Adds the pending task to delayed_work_queue_.
  void AddToDelayedWorkQueue(const PendingTask& pending_task);

  // Load tasks from the incoming_queue_ into work_queue_ if the latter is
  // empty.
  void ReloadWorkQueue();

  // Delete tasks that haven't run yet without running them.  Used in the
  // destructor to make sure all the task's destructors get called.  Returns
  // true if some work was done.
  bool DeletePendingTasks();

  // Post a task to our incomming queue.
  void PostTask_Helper(Task * task
    , int64 delay_milli_second
    , bool nestable);

  // base::MessagePump::Delegate methods:
  virtual bool DoWork();
  virtual bool DoDelayedWork(base::Time * next_delayed_work_time);
  virtual bool DoIdleWork();

 public:
  bool process_task_;

 protected:
  Type type_;
  string8 name_;

  // A null terminated list which creates an incoming_queue of tasks that are
  // aquired under a mutex for processing on this instance's thread. These
  // tasks have not yet been sorted out into items for our work_queue_ vs
  // items that will be handled by the TimerManager.
  TaskQueue incoming_queue_;
  // Protect access to incoming_queue_.
  base::Lock incoming_queue_lock_;

  // A list of tasks that need to be processed by this instance.  Note that
  // this queue is only accessed (push/pop) by our current thread.
  TaskQueue work_queue_;

  // Contains delayed tasks, sorted by their 'delayed_work_time' property.
  DelayedTaskQueue delay_work_queue_;

  // A queue of non-nestable tasks that we had to defer because when it came
  // time to execute them we were in a nested message loop.  They will execute
  // once we're out of nested message loops.
  TaskQueue deferred_non_nestable_work_queue_;

  base::ScopedRefPtr<MessagePump> pump_;

  // A recursion block that prevents accidentally running additional tasks
  // when insider a (accidentally induced?) nested message pump.
  bool nestable_task_allowed_;

  RunState * state_;

  // The sequence number is used to facilitate FIFO sorting when two tasks
  // have the same delayed_run_time value.
  int32 next_sequence_number_;

  base::Lock destruction_observers_lock_;
  std::list<base::DestructionObserver *> destruction_observers_;
  std::list<IdleHandler*> idle_handlers_;

  base::ScopedRefPtr<MessageLoopProxyInternal> message_loop_proxy_;
  bool quit_all_;

 private:
  MessageLoop(const MessageLoop & right);
  MessageLoop & operator =(const MessageLoop & right);
};

class BASE_EXPORT MessageLoopForUI : public MessageLoop {
 public:
  MessageLoopForUI() : MessageLoop(TYPE_UI, "TYPE_UI") {
  }

  explicit MessageLoopForUI(const string8 &thread_name)
    : MessageLoop(TYPE_UI, thread_name) {
  }

  // Returns the MessageLoopForUI of the current thread.
  static MessageLoopForUI* Current() {
    MessageLoop* message_loop = MessageLoop::Current();
    return static_cast<MessageLoopForUI *>(message_loop);
  }

  void PumpOutPendingPaintMessages();

  void Run(Dispatcher * dispatcher);

  bool AddMsgFilter(base::MessageFilter * filter) {
    return PumpUI()->AddMsgFilter(filter);
  }

  bool RemoveMsgFilter(base::MessageFilter * filter) {
    return PumpUI()->RemoveMsgFilter(filter);
  }

  bool ProcessMessage(const MSG& msg) {
    return PumpUI()->ProcessMessage(msg);
  }

  bool AddShortCutKeyMsgFilter(base::MessageFilter * filter) {
    return PumpUI()->AddShortCutKeyMsgFilter(filter);
  }

  bool RemoveShortCutKeyMsgFilter(base::MessageFilter * filter) {
    return PumpUI()->RemoveShortCutKeyMsgFilter(filter);
  }

 protected:
  // TODO(rvargas): Make this platform independent.
  MessagePumpForUI * PumpUI() {
    return static_cast<MessagePumpForUI *>(pump_.get());
  }
};

class BASE_EXPORT MessageLoopForIO : public MessageLoop {
 public:
  explicit MessageLoopForIO(const string8 &thread_name)
    : MessageLoop(TYPE_IO, thread_name) {
  }

  // Returns the MessageLoopForIO of the current thread.
  static MessageLoopForIO * Current() {
    MessageLoop * message_loop = MessageLoop::Current();
    return static_cast<MessageLoopForIO *>(message_loop);
  }

  typedef base::IOHandler IOHandler;
  typedef base::IOContext IOContext;

  // Please see MessagePumpWin for definitions of these methods.
  void RegisterIOHandler(HANDLE pipe, IOHandler * handler);
  bool WaitForIOCompletion(DWORD timeout, IOHandler* filter);

 protected:
  // TODO(rvargas): Make this platform independent.
  MessagePumpForIO * PumpIO() {
    return static_cast<MessagePumpForIO *>(pump_.get());
  }
};
}  // namespace base

namespace base {
class Task;
class MessageLoop;
class MessageLoopProxy
  : public base::RefCountedThreadSafe<MessageLoopProxy> {
 public:
  explicit MessageLoopProxy(ThreadId thread_id) : thread_id_(thread_id) {}

 public:
  ThreadId GetThreadID() { return thread_id_; }

 public:
  virtual void PostTask(base::Task * task, int64 delay) = 0;

  virtual bool RunNestMessageLoop() = 0;
  virtual void QuitNow() = 0;
  virtual void Quit() = 0;

  virtual void Run() = 0;

  virtual void Replace(MessageLoopProxy * message_loop_proxy) {}

 protected:
  virtual ~MessageLoopProxy() {}
  friend class base::RefCountedThreadSafe<MessageLoopProxy>;

 private:
  ThreadId thread_id_;
};

class BASE_EXPORT MessageLoopProxyInternal
  : public MessageLoopProxy {
  friend class MessageLoop;

 public:
  ~MessageLoopProxyInternal();

  void PostDelayedTask(Task* task, int64 delay_milli_second);
  void PostNonNestableTask(Task* task);
  void PostNonNestableDelayedTask(Task* task, int64 delay_milli_second);
  void SetRunNestMessageLoopAllowed(bool allowed);
  bool IsRunNestMessageLoopAllowed();
  int32 RunDepth();

  void PostTask(base::Task * task, int64 delay = 0) override;
  bool RunNestMessageLoop() override;
  void Quit() override;
  void QuitNow() override;
  void Run() override;

  void RegisterIOHandler(HANDLE pipe, base::IOHandler* handler);

 private:
  // Can only be created by MessageLoop.
  explicit MessageLoopProxyInternal(
    MessageLoop* target_message_loop_proxy);

  void PostTask_Helper(Task* task, int64 delay_milli_second, bool nestable);

  // Called directly by MessageLoop::~MessageLoop.
  void WillDestroyCurrentMessageLoop();

 private:
  base::Lock  message_loop_proxy_lock_;
  MessageLoop* target_message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(MessageLoopProxyInternal);
};
}  // namespace base

namespace base {
class ThreadQuitTask : public Task {
 public:
  void Run() {
    MessageLoop * message_loop = MessageLoop::Current();
    if (message_loop) {
      message_loop->Quit();
    }
  }

  void Destroy() {
    delete this;
  }
};

class ThreadQuitNowTask : public Task {
 public:
  virtual void Run() {}

  virtual void Destroy() {
    delete this;
  }
};

class BASE_EXPORT Thread {
 public:
  struct Options {
   public:
    // Specifies the type of message loop that will be allocated on the
    // thread.
    MessageLoop::Type type_;

    // Specifies the maximum stack size that the thread is allowed to use
    // This does not necessarily correspond to the thread's initial stack
    // size. A value of 0 indicates that the default maximum should be used.
    uint32 stack_size_;

    bool use_com_;

    dword32 coinitex_flag_;

    // 线程名字，被用于逻辑可视化和调试模式下显示更可读的线程信息
    LPCSTR thread_name_;

    // BUGFIX: 10888 在某些情况下IE内核运行逻辑中，CoUninitialize与CoInitialize
    // 调用不对应导致在IE内核运行线程退出时，在线程ntdll!LdrShutdownThread退出函
    // 数内调用CoUnInitialize，由于LdrShutdownThread内有peb->loaderCriticalSe
    // -ction临界区，因此容易导致死锁，此标记当为TRUE时，表示在线程的Run函数退出之
    // 前会强制调用CoUnitialize，来在进入LdrShutdownThread之前，销毁OLE相关资源，
    // 防止死锁
    bool couninitex_hack_;

    Options()
      : type_(MessageLoop::TYPE_DEFAULT)
      , stack_size_(0)
      , use_com_(false)
      , coinitex_flag_((dword32)-1)
      , thread_name_(NULL)
      , couninitex_hack_(false) {}

    Options(MessageLoop::Type type
      , uint32 stack_size
      , bool use_com = false
      , dword32 coinitex_flag = -1
      , LPCSTR thread_name = NULL)
      : type_(type)
      , stack_size_(stack_size)
      , use_com_(use_com)
      , coinitex_flag_(coinitex_flag)
      , thread_name_(thread_name)
      , couninitex_hack_(false) {}
  };

  struct StartupData {
   public:
    // We get away with a const reference here because of how we are
    // allocated.
    const Thread::Options& options_;

    // Used to synchronize thread startup.
    base::WaitableEvent started_event_;

    explicit StartupData(const Options& opt)
      : options_(opt),
      started_event_(false, false) {}
  };

 public:
  explicit Thread(LPCSTR default_name = NULL);
  virtual ~Thread(void);

 public:
  // StartWithOptions(Options())
  bool Start();

  // begin thread and wait for start event.
  bool StartWithOptions(const Options& options);

  // The thread should only be called on the same thread that started us.
  // invoke StopSoon, wait thread to exit
  bool Stop();

  // post ThreadQuitTask task
  bool StopSoon();

  // member access function
  MessageLoop * GetMessageLoop() const { return message_loop_; }
  bool GetMessageLoopProxy(
    base::ScopedRefPtr<base::MessageLoopProxy> * message_loop_proxy) {
    if (message_loop_proxy_ && message_loop_proxy) {
       * message_loop_proxy = message_loop_proxy_;
      return true;
    }

    return false;
  }

  HANDLE GetThreadHandle() { return thread_; }

  ThreadId GetThreadID() const { return thread_id_; }

  bool IsRunning() const { return thread_id_ != 0; }

  // thread function
  void ThreadMain();

  base::MessageLoop::Type GetType();

 protected:
  virtual void Run(MessageLoop * message_loop);

  void SetMessageLoop(MessageLoop * message_loop) {
    message_loop_ = message_loop;
  }

 private:
  // We piggy-back on the startup_data_ member to know if we successfully
  // started the thread.  This way we know that we need to call Join.
  bool ThreadWasStarted() const { return NULL != startup_data_; }

 private:
  // If true, we're in the middle of stopping, and shouldn't access
  // |message_loop_|. It may non-NULL and invalid.
  bool stopping_;

  StartupData * startup_data_;

  // The thread's message loop.  Valid only while the thread is alive.
  MessageLoop * message_loop_;

  base::ScopedRefPtr<MessageLoopProxyInternal> message_loop_proxy_;

  ThreadId thread_id_;
  HANDLE thread_;

  char name_[64];
};
}  // namespace base

namespace base {
class BASE_EXPORT MessageLoopProxyManager {
 public:
  MessageLoopProxyManager();
  ~MessageLoopProxyManager();

 public:
  bool RegisterMessageLoopProxy(MessageLoopProxy * message_loop_proxy);

  bool PostTaskByThreadID(ThreadId thread_id, base::Task* task
    , int64 delay = 0);
  bool RunNestMessageLoop(ThreadId thread_id);
  bool QuitMessageLoop(ThreadId thread_id, bool now = true);

  void Clear();

 public:
  static MessageLoopProxyManager * Instance();

 protected:
  base::ScopedRefPtr<MessageLoopProxy> GetMessageLoopProxy(ThreadId thread_id);

 protected:
  base::Lock callers_lock_;
  std::map < ThreadId, base::ScopedRefPtr<MessageLoopProxy> >
    thread_id_2_message_loop_;
};
}  // namespace base

namespace base {
class InspectPackage {
 public:
  enum eInspectMark {
    eIM_SEND,
    eIM_INVOKE,
    eIM_RECEIVE,
  };
  explicit InspectPackage(uint64 inspect_id = 0);

 public:
  uint64 ID() {return inspect_id_;}
  double Stamp(int64 datum);

  void Mark(eInspectMark mark);
  double Lag(eInspectMark mark);

 private:
  uint64 inspect_id_;

  int64  send_stamp_;
  int64  invoke_stamp_;
  int64  receive_stamp_;
};

class BASE_EXPORT MessageLoopProfile
  : public base::RefCountedThreadSafe<MessageLoopProfile > {
 public:
  explicit MessageLoopProfile(ThreadId watch_thread_id
    , ThreadId target_thread_id);

 public:
  // Called On Watch Thread
  void Start();
  void Stop();

 private:
  void Inspecting();

  void InvokeOnTarget(InspectPackage package);
  void ReciveOnWatch(InspectPackage package);

 private:
  virtual ~MessageLoopProfile();
  friend class base::RefCountedThreadSafe<MessageLoopProfile>;

 private:
  bool switch_;
  int64 start_stamp_;

  uint64 inspect_seed_;

  ThreadId watch_thread_id_;
  ThreadId target_thread_id_;
};
}  // namespace base

#endif  // BASE_THREAD_THREAD_H_
