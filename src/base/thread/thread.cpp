// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.


#include "base/thread/thread.h"

#include <assert.h>

#include <list>
#include <map>
#include <string>

#include "base/log.h"

namespace base {
unsigned int __stdcall ThreadFunc(void* closure) {
  Thread * thread = static_cast<Thread *>(closure);

  thread->ThreadMain();

  return 0;
}

Thread::Thread(LPCSTR default_name)
  : stopping_(NULL)
  , thread_id_(0)
  , thread_(HANDLE(INVALID_HANDLE_VALUE))
  , message_loop_(NULL)
  , startup_data_(NULL) {
  if (default_name) {
    strncpy_s(name_, default_name, _TRUNCATE);
  } else {
    name_[0] = '\0';
  }
}

Thread::~Thread(void) {
  Stop();
}

bool Thread::Start() {
  return StartWithOptions(Options());
}

bool Thread::StartWithOptions(const Options& options) {
  // the startup_data_ is pointing to a local variable, valid only
  // before the function exit.
  StartupData startup_data(options);
  startup_data_ = &startup_data;

  if (options.thread_name_) {
    strncpy_s(name_, options.thread_name_, _TRUNCATE);
  }

  uint32 flags = 0;

  uint32 threadId = 0;
  thread_ = (HANDLE)_beginthreadex(NULL
    , options.stack_size_
    , ThreadFunc
    , this
    , flags
    , &threadId);

  thread_id_ = threadId;
  if (0 == thread_) {
    DWORD error = GetLastError();
    startup_data_ = NULL;
    return false;
  }

  // Wait for the thread to start and initialize message_loop_
  startup_data_->started_event_.Wait();

  const char* name = name_;
  if (name[0]) {
    DebuggerThreadNameMgr::SetThreadName(thread_id_, name);
  }

  return true;
}

bool Thread::Stop() {
  if (!ThreadWasStarted())
    return false;

  if (!StopSoon())
    return false;

  WaitForSingleObject(thread_, INFINITE);

  CloseHandle(thread_);
  thread_ = HANDLE(INVALID_HANDLE_VALUE);

  // The thread no longer needs to be joined.
  startup_data_ = NULL;

  stopping_ = false;

  return true;
}

bool Thread::StopSoon() {
  if (stopping_ || !message_loop_)
    return false;

  stopping_ = true;
  message_loop_->PostTask(new ThreadQuitTask());

  return true;
}

void Thread::Run(MessageLoop * message_loop) {
  message_loop->Run();
}

void Thread::ThreadMain() {
  // The message loop for this thread.
  if (MessageLoop::TYPE_UI ==
    startup_data_->options_.type_) {
    message_loop_ = new MessageLoopForUI(name_);
  } else if (MessageLoop::TYPE_IO ==
    startup_data_->options_.type_) {
    message_loop_ = new MessageLoopForIO(name_);
  } else {
    message_loop_ =
      new MessageLoop(MessageLoop::TYPE_DEFAULT, name_);
  }

  message_loop_->GetMessageLoopProxy(&message_loop_proxy_);

  // Complete the initialization of our base object.
  thread_id_ = GetCurrentThreadId();

  bool use_com = startup_data_->options_.use_com_;
  DWORD coinitex_flag = startup_data_->options_.coinitex_flag_;
  bool couninitex_hack =
    startup_data_->options_.couninitex_hack_;

  // the startup_data_ is guaranteed to be valid at this point because the
  // function StartWithOptions has not exit, waiting for this event
  // But the pointer is INVALID after this point because StartWithOptions
  // exit.
  startup_data_->started_event_.Signal();

  if (use_com) {
    if (-1 != coinitex_flag) {
      ::CoInitializeEx(NULL, coinitex_flag);
    } else {
      ::OleInitialize(NULL);
    }
  }

  Run(message_loop_);

  if (use_com) {
    if (-1 != coinitex_flag) {
      ::CoUninitialize();
    } else {
      ::OleUninitialize();
    }

    if (couninitex_hack) {
      int i = 0;
      while (++i <= 2) {
        ::CoUninitialize();
      }
    }
  }

  // We can't receive messages anymore.
  delete message_loop_;
  message_loop_ = NULL;
}

base::MessageLoop::Type Thread::GetType() {
  if (message_loop_)
    return message_loop_->GetType();
  else
    return base::MessageLoop::TYPE_DEFAULT;
}
}  // namespace base

namespace base {
ThreadLocalPointer<MessageLoop> g_tls_message_loop;
void MessageLoop::AddDestructionObserver(
  base::DestructionObserver * destruction_observer) {
  base::AutoLock lock(destruction_observers_lock_);

  std::list<base::DestructionObserver *>::const_iterator it =
    find(destruction_observers_.begin()
    , destruction_observers_.end()
    , destruction_observer);

  if (it != destruction_observers_.end())
    return;

  destruction_observers_.push_back(destruction_observer);
}

void MessageLoop::RemoveDestructionObserver(
  base::DestructionObserver * destruction_observer) {
  base::AutoLock lock(destruction_observers_lock_);

  std::list<base::DestructionObserver *>::iterator it =
    find(destruction_observers_.begin()
    , destruction_observers_.end()
    , destruction_observer);

  if (it != destruction_observers_.end())
    destruction_observers_.erase(it);
}

MessageLoop * MessageLoop::Current() {
  return g_tls_message_loop.Get();
}

MessageLoop::MessageLoop(Type type
  , const string8& thread_name)
  : type_(type)
  , name_(thread_name)
  , nestable_task_allowed_(true)
  , state_(NULL)
  , next_sequence_number_(0)
  , process_task_(true)
  , quit_all_(false) {
  // store this pointer in thread local storage
  g_tls_message_loop.Set(this);
  if (NULL == (message_loop_proxy_ = new MessageLoopProxyInternal(this))) {
    assert(false);
  }

  // create pump
  if (TYPE_UI == type_) {
    pump_ = new(std::nothrow) MessagePumpForUI();
  } else if (TYPE_IO == type_) {
    pump_ = new(std::nothrow) MessagePumpForIO;
  } else {
    pump_ = new(std::nothrow) MessagePumpDefault();
  }
}

MessageLoop::~MessageLoop(void) {
  if (message_loop_proxy_)
    message_loop_proxy_->WillDestroyCurrentMessageLoop();

  while (true) {
    base::AutoLock lock(destruction_observers_lock_);

    if (destruction_observers_.empty())
      break;

    base::DestructionObserver * current_observer =
      *destruction_observers_.begin();
    destruction_observers_.pop_front();

    // the function will erase from destruction_observers_.
    if (current_observer)
      current_observer->WillDestroyCurrentMessageLoop();
  }

  // Clean up any unprocessed tasks, but take care: deleting a task could
  // result in the addition of more tasks (e.g., via DeleteSoon).  We set a
  // limit on the number of times we will allow a deleted task to generate
  // more tasks.  Normally, we should only pass through this loop once or
  // twice.If we end up hitting the loop limit, then it is probably due to one
  // task that is being stubborn.  Inspect the queues to see who is left.
  bool did_work = false;
  int i = 0;
  for (; i < 100; ++i) {
    DeletePendingTasks();
    ReloadWorkQueue();
    // If we end up with empty queues, then break out of the loop.
    did_work = DeletePendingTasks();
    if (!did_work)
      break;
  }

  assert(i < 100);

  // OK, now make it so that no one can find us.
  g_tls_message_loop.Set(NULL);
}

bool MessageLoop::GetMessageLoopProxy(
  base::ScopedRefPtr<base::MessageLoopProxyInternal>* message_loop_proxy) {
  if (message_loop_proxy && message_loop_proxy_) {
    *message_loop_proxy = message_loop_proxy_;
    return true;
  }

  return false;
}

void MessageLoop::Run() {
  AutoRunState save_state(this);
  RunHandler();
}

void MessageLoop::RunAllPending() {
  AutoRunState save_state(this);

  // Means run until we would otherwise block.
  state_->quit_received = true;

  RunHandler();
}

void MessageLoop::RunHandler() {
  RunInternal();
}

void MessageLoop::RunInternal() {
  if (state_->dispatcher_ && GetType() == TYPE_UI)
    static_cast<MessagePumpForUI *>(pump_.get())->
      RunWithDispatcher(this, state_->dispatcher_, process_task_);
  else
    pump_->Run(this);

  if (quit_all_) {
    if (state_->run_depth_ > 1)
      pump_->Quit();
    else
      quit_all_ = false;
  }
}

bool MessageLoop::ProcessNextDelayedNonNestableTask() {
  if (state_->run_depth_ != 1)
    return false;

  if (deferred_non_nestable_work_queue_.empty())
    return false;

  Task * task = deferred_non_nestable_work_queue_.front().task_;
  deferred_non_nestable_work_queue_.pop();

  RunTask(task);

  return true;
}

void MessageLoop::Quit()  {
  Log(LL_DEBUG, TAG(L"Base"), L"FUNC:%s", __FUNCTIONW__);
  if (state_) {
    // assert(state_->run_depth_ > 1);
    state_->quit_received = true;
  }
}

void MessageLoop::QuitNow() {
  if (state_) {
    // assert(state_->run_depth_ > 1);
    pump_->Quit();
  }
}

void MessageLoop::QuitAll() {
  quit_all_ = true;
}

void MessageLoop::PostTask(Task * task) {
  PostTask_Helper(task, 0, true);
}

void MessageLoop::PostDelayedTask(Task * task
  , int64 delay_milli_second) {
  PostTask_Helper(task, delay_milli_second, true);
}

void MessageLoop::PostNonNestableTask(Task * task) {
  PostTask_Helper(task, 0, false);
}

void MessageLoop::PostNonNestableDelayedTask(Task * task
  , int64 delay_milli_second) {
  PostTask_Helper(task, delay_milli_second, false);
}

// Possibly called on a background thread!
void MessageLoop::PostTask_Helper(Task * task, int64 delay_milli_second
  , bool nestable) {
  PendingTask pending_task(task, nestable);

  if (delay_milli_second > 0) {
    pending_task.delayed_run_time_ = base::Time::Now() +
      base::TimeDelta::FromMilliseconds(delay_milli_second);
  }

  // Warning: Don't try to short-circuit, and handle this thread's tasks
  // more directly, as it could starve handling of foreign threads.Put
  // every task into this queue.
  base::ScopedRefPtr<MessagePump> pump;
  {
    base::AutoLock locked(incoming_queue_lock_);

    // bool empty = incoming_queue_.empty();
    int32 task_number = static_cast<int32>(incoming_queue_.size());
    incoming_queue_.push(pending_task);
    if (/*!empty*/task_number > 0) {
      return;  // Someone else should have started the sub-pump.
    }
    pump = pump_;
  }
  // Since the incoming_queue_ may contain a task that destroys this message
  // loop, we cannot exit incoming_queue_lock_ until we are done with |this|.
  // We use a stack-Based reference to the message pump so that we can call
  // ScheduleWork outside of incoming_queue_lock_.

  // trigger start work
  if (pump)
    pump->ScheduleWork();
}

void MessageLoop::SetNestableTasksAllowed(bool allowed) {
  if (nestable_task_allowed_ != allowed) {
    nestable_task_allowed_ = allowed;
    if (!nestable_task_allowed_)
      return;

    // Start the native pump if we are not already pumping.
    pump_->ScheduleWork();
  }
}

bool MessageLoop::NestableTasksAllowed() const {
  return nestable_task_allowed_;
}

bool MessageLoop::IsNested() {
  return state_->run_depth_ > 1;
}

int32 MessageLoop::RunDepth() {
  return state_->run_depth_;
}

int32 MessageLoop::WaitingTaskNumber() {
  base::AutoLock lock(incoming_queue_lock_);
  return static_cast<int32>(incoming_queue_.size());
}

void MessageLoop::RunTask(Task * task) {
  nestable_task_allowed_ = false;

  LARGE_INTEGER li1;
  ::QueryPerformanceCounter(&li1);

  task->Run();
  task->Destroy();
  task = NULL;

  LARGE_INTEGER li2;
  ::QueryPerformanceCounter(&li2);

  nestable_task_allowed_ = true;
}

bool MessageLoop::DeferOrRunPendingTask(
  const PendingTask & pending_task) {
  // NOTE:(xiongzefa) to keep the deferred task's run-sequence
  // we only allow to run deferred task popped from
  // deferred_non_nestable_work_queue_!
  if (pending_task.nestable_/* || state_->run_depth_ == 1*/) {
    RunTask(pending_task.task_);

    return true;
  }

  // We couldn't run the task now because we're in a nested message loop
  // and the task isn't nestable.
  deferred_non_nestable_work_queue_.push(pending_task);
  return false;
}

void MessageLoop::AddToDelayedWorkQueue(
  const PendingTask & pending_task) {
  PendingTask new_pending_task(pending_task);
  new_pending_task.sequence_number = next_sequence_number_++;
  delay_work_queue_.push(new_pending_task);
}

void MessageLoop::ReloadWorkQueue() {
  if (!work_queue_.empty())
    return;  // Wait till we *really* need to lock and load.

  // Acquire all we can from the inter-thread queue with one lock acquisition.
  {
    base::AutoLock lock(incoming_queue_lock_);
    if (incoming_queue_.empty())
      return;

    incoming_queue_.Swap(&work_queue_);  // Constant time
  }
}

bool MessageLoop::DeletePendingTasks() {
  // clear work queue
  bool did_work = !work_queue_.empty();
  while (!work_queue_.empty()) {
    PendingTask pending_task = work_queue_.front();
    work_queue_.pop();

    if (!pending_task.delayed_run_time_.IsNull()) {
      // We want to delete delayed tasks in the same order in which
      // they would normally be deleted in case of any funny
      // dependencies between delayed tasks.
      AddToDelayedWorkQueue(pending_task);
    } else {
      pending_task.task_->Destroy();
      pending_task.task_ = NULL;
    }
  }

  // clear deferred non nestable work queue
  did_work |= !deferred_non_nestable_work_queue_.empty();
  while (!deferred_non_nestable_work_queue_.empty()) {
    // TODO(darin): Delete all tasks once it is safe to do so.
    // Until it is totaly safe, only delete them under Purify and
    // Valgrind.
    Task * task = NULL;
    task = deferred_non_nestable_work_queue_.front().task_;
    deferred_non_nestable_work_queue_.pop();
    if (task) {
      task->Destroy();
      task = NULL;
    }
  }

  // clear delayed work queue
  did_work |= !delay_work_queue_.empty();
  while (!delay_work_queue_.empty()) {
    Task * task = delay_work_queue_.top().task_;
    delay_work_queue_.pop();
    if (task) {
      task->Destroy();
      task = NULL;
    }
  }

  return did_work;
}

bool MessageLoop::DoWork() {
  if (!nestable_task_allowed_) {
    // Task can't be executed right now.
    return false;
  }

  for (;;) {
    ReloadWorkQueue();
    if (work_queue_.empty())
      break;

    // Execute oldest task.
    do {
      PendingTask pending_task = work_queue_.front();
      work_queue_.pop();

      if (!pending_task.delayed_run_time_.IsNull()) {
        AddToDelayedWorkQueue(pending_task);

        if (delay_work_queue_.top().task_ == pending_task.task_)
          pump_->ScheduleDelayedWork(
            pending_task.delayed_run_time_);
      } else {
        // if one task run, return TRUE
        if (DeferOrRunPendingTask(pending_task))
          return true;
      }
    } while (!work_queue_.empty());
  }

  return false;
}

bool MessageLoop::DoDelayedWork(base::Time * next_delayed_work_time) {
  if (!nestable_task_allowed_ || delay_work_queue_.empty()) {
    *next_delayed_work_time = base::Time();
    return false;
  }

  if (delay_work_queue_.top().delayed_run_time_ > base::Time::Now()) {
    *next_delayed_work_time = delay_work_queue_.top().delayed_run_time_;
    return false;
  }

  PendingTask pending_task = delay_work_queue_.top();
  delay_work_queue_.pop();

  if (!delay_work_queue_.empty())
    *next_delayed_work_time = delay_work_queue_.top().delayed_run_time_;

  return DeferOrRunPendingTask(pending_task);
}

bool MessageLoop::DoIdleWork() {
  std::list<IdleHandler*>::iterator it = idle_handlers_.begin();
  for (; it != idle_handlers_.end(); ++it) {
    if (*it != NULL)
      (*it)->OnIdle();
  }

  if (ProcessNextDelayedNonNestableTask())
    return true;

  if (state_->quit_received)
    pump_->Quit();

  return false;
}

bool MessageLoop::AddIdleHandler(IdleHandler* idle_handler) {
  if (!idle_handler)
    return false;
#ifdef _DEBUG
  if (type_ != TYPE_UI)
    assert(!L"目前AddIdlerHandler只是引用在UI线程，用作新的UI更新机制");
  // 检测是否重复添加
  std::list<IdleHandler*>::iterator it = idle_handlers_.begin();
  for (; it != idle_handlers_.end(); ++it) {
    if (idle_handler == *it) {
      assert(!"重复添加IdleHandler");
      return false;
    }
  }
#endif
  idle_handlers_.push_back(idle_handler);

  return true;
}

bool MessageLoop::RemoveIdleHandler(IdleHandler* idle_handler) {
#ifdef _DEBUG
  if (type_ != TYPE_UI)
    assert(!L"目前AddIdlerHandler只是引用在UI线程，用作新的UI更新机制");
#endif
  if (!idle_handler)
    return false;

  std::list<IdleHandler*>::iterator it = idle_handlers_.begin();
  for (; it != idle_handlers_.end(); ++it) {
    if (idle_handler == *it) {
      idle_handlers_.erase(it);
      return true;
    }
  }

  assert(L"没有找到IdleHandler");
  return false;
}

void MessageLoop::DoIdleHandler() {
  std::list<IdleHandler*>::iterator it = idle_handlers_.begin();
  for (; it != idle_handlers_.end(); ++it) {
    if (*it != NULL)
      (*it)->OnIdle();
  }
}

// store the run state of message_loop
MessageLoop::AutoRunState::AutoRunState(
  MessageLoop * message_loop)
  : message_loop_(message_loop) {
  previous_state_ = message_loop->state_;
  if (previous_state_) {
    run_depth_ = previous_state_->run_depth_ + 1;
  } else {
    run_depth_ = 1;
  }

  message_loop->state_ = this;

  // Initialize the other fields:
  quit_received = false;

  dispatcher_ = NULL;
}

MessageLoop::AutoRunState::~AutoRunState() {
  message_loop_->state_ = previous_state_;
}

bool MessageLoop::PendingTask::operator< (
  const PendingTask& other) const {
  if (delayed_run_time_ < other.delayed_run_time_)
    return false;

  if (delayed_run_time_ > other.delayed_run_time_)
    return true;

  // If the times happen to match, then we use the sequence number to decide.
  // Compare the difference to support integer roll-over.
  return (sequence_number - other.sequence_number) > 0;
}

//------------------------------------------------------------------------------
// MessageLoopForUI
void MessageLoopForUI::PumpOutPendingPaintMessages() {
  PumpUI()->PumpOutPendingPaintMessages();
}

void MessageLoopForUI::Run(Dispatcher* dispatcher) {
  AutoRunState save_state(this);
  state_->dispatcher_ = dispatcher;

  RunHandler();
}

//------------------------------------------------------------------------------
// MessageLoopForIO
void MessageLoopForIO::RegisterIOHandler(HANDLE pipe
  , IOHandler* handler) {
  PumpIO()->RegisterIOHandler(pipe, handler);
}

bool MessageLoopForIO::WaitForIOCompletion(DWORD timeout
  , IOHandler* filter) {
  return PumpIO()->WaitForIOCompletion(timeout, filter);
}
}  // namespace base

namespace base {
MessageLoopProxyInternal::MessageLoopProxyInternal(
  MessageLoop* target_message_loop_proxy)
  : base::MessageLoopProxy(::GetCurrentThreadId())
  , target_message_loop_proxy_(target_message_loop_proxy) {
}

MessageLoopProxyInternal::~MessageLoopProxyInternal() {}

void MessageLoopProxyInternal::WillDestroyCurrentMessageLoop() {
  base::AutoLock lock(message_loop_proxy_lock_);
  target_message_loop_proxy_ = NULL;
}

void MessageLoopProxyInternal::PostTask_Helper(Task* task
  , int64 delay_milli_second
  , bool nestable) {
  base::AutoLock lock(message_loop_proxy_lock_);
  if (target_message_loop_proxy_ && task) {
    target_message_loop_proxy_->
      PostTask_Helper(task, delay_milli_second, nestable);
  } else if (task) {
    task->Destroy();
    task = NULL;
  }
}

void MessageLoopProxyInternal::PostDelayedTask(Task* task
  , INT64 delay_milli_second) {
  PostTask_Helper(task, delay_milli_second, true);
}

void MessageLoopProxyInternal::PostNonNestableTask(Task* task) {
  PostTask_Helper(task, 0, false);
}

void MessageLoopProxyInternal::PostNonNestableDelayedTask(Task* task
  , int64 delay_milli_second) {
  PostTask_Helper(task, delay_milli_second, false);
}

void MessageLoopProxyInternal::SetRunNestMessageLoopAllowed(bool allowed) {
  if (!target_message_loop_proxy_) {
    return;
  } else {
    target_message_loop_proxy_->SetNestableTasksAllowed(allowed);
  }
}

bool MessageLoopProxyInternal::IsRunNestMessageLoopAllowed() {
  if (!target_message_loop_proxy_) {
    return false;
  } else {
    return target_message_loop_proxy_->NestableTasksAllowed();
  }
}

int32 MessageLoopProxyInternal::RunDepth() {
  if (!target_message_loop_proxy_) {
    return FALSE;
  }

  return target_message_loop_proxy_->RunDepth();
}

void MessageLoopProxyInternal::PostTask(base::Task * task, int64 delay) {
  PostTask_Helper(task, delay, true);
}

bool MessageLoopProxyInternal::RunNestMessageLoop() {
  if (!target_message_loop_proxy_) {
    return false;
  }

  bool old_state = target_message_loop_proxy_->NestableTasksAllowed();
  target_message_loop_proxy_->SetNestableTasksAllowed(true);
  target_message_loop_proxy_->Run();
  target_message_loop_proxy_->SetNestableTasksAllowed(old_state);

  return true;
}

void MessageLoopProxyInternal::Quit() {
  base::AutoLock lock(message_loop_proxy_lock_);
  if (target_message_loop_proxy_) {
    target_message_loop_proxy_->Quit();
  }
}

void MessageLoopProxyInternal::QuitNow() {
  base::AutoLock lock(message_loop_proxy_lock_);
  if (target_message_loop_proxy_) {
    target_message_loop_proxy_->QuitNow();
  }
}

void MessageLoopProxyInternal::Run() {
  assert(false);
}

void MessageLoopProxyInternal::RegisterIOHandler(HANDLE pipe
  , base::IOHandler* handler) {
  base::AutoLock lock(message_loop_proxy_lock_);
  if (target_message_loop_proxy_) {
    static_cast<MessageLoopForIO*>(
      target_message_loop_proxy_)->RegisterIOHandler((HANDLE)pipe, handler);
  }
}
}  // namespace base

namespace base {
const wchar_t kWndClass[] = L"WRSMessagePumpWindow";

// Message sent to get an additional time slice for pumping (processing) another
// task (a series of such messages creates a continuous task pump).
const int32 kMsgHaveWork = WM_USER + 1;

MessagePump::MessagePump(void)
  : state_(NULL)
  , have_work_(0) {}

MessagePump::~MessagePump(void) {}

void MessagePump::RunWithDispatcher(
  Delegate * delegate, Dispatcher * dispatcher, bool /*process_task*/) {
  RunState state;
  state.delegate_ = delegate;
  state.dispatcher_ = dispatcher;
  state.should_quit_ = false;
  state.run_depth_ = state_ ? state_->run_depth_ + 1 : 1;
  // state.process_task_ = process_task;

  RunState * previous_state = state_;
  state_ = &state;

  DoRunLoop();

  state_ = previous_state;
}

void MessagePump::Quit() {
  state_->should_quit_ = true;
}

int32 MessagePump::GetCurrentDelay() const {
  if (delayed_work_time_.IsNull())
    return -1;

  // Be careful here.  TimeDelta has a precision of microseconds, but we want
  // a value in milliseconds.  If there are 5.5ms left, should the delay be 5
  // or 6?  It should be 6 to avoid executing delayed work too early.
  double timeout =
    ceil((delayed_work_time_ - base::Time::Now()).InMillisecondsF());

  // If this value is negative, then we need to run delayed work soon.
  int32 delay = static_cast<int32>(timeout);
  if (delay < 0)
    delay = 0;

  return delay;
}

//-----------------------------------------------------------------------------
// MessagePumpForUI public:
MessagePumpForUI::MessagePumpForUI() {
  InitMessageWnd();
}

MessagePumpForUI::~MessagePumpForUI() {
  DestroyWindow(message_wnd_);
  UnregisterClass(kWndClass, GetModuleHandle(NULL));
}

void MessagePumpForUI::ScheduleWork() {
  if (InterlockedExchange(&have_work_, 1))
    return;  // Someone else continued the pumping.

  // Make sure the MessagePump does some work for us.
  PostMessage(message_wnd_, kMsgHaveWork, reinterpret_cast<WPARAM>(this), 0);
}

void MessagePumpForUI::ScheduleDelayedWork(
  const base::Time& delayed_work_time) {
  //
  // We would *like* to provide high resolution timers.  Windows timers using
  // SetTimer() have a 10ms granularity.  We have to use WM_TIMER as a wakeup
  // mechanism because the application can enter modal windows loops where it
  // is not running our MessageLoop; the only way to have our timers fire in
  // these cases is to post messages there.
  //
  // To provide sub-10ms timers, we process timers directly from our run loop.
  // For the base case, timers will be processed there as the run loop does
  // its normal work.  However, we *also* set the system timer so that
  // WM_TIMER  events fire. This mops up the case of timers not being able to
  // work in modal message loops.  It is possible for the SetTimer to pop and
  // have no pending timers, because they could have already been processed by
  // the run loop itself.
  //
  // We use a single SetTimer corresponding to the timer that will expire
  // soonest.  As new timers are created and destroyed, we update SetTimer.
  // Getting a spurrious SetTimer event firing is benign, as we'll just be
  // processing an empty timer queue.
  //
  delayed_work_time_ = delayed_work_time;

  int delay_milli_sec = GetCurrentDelay();

  if (delay_milli_sec < USER_TIMER_MINIMUM)
    delay_milli_sec = USER_TIMER_MINIMUM;

  // Create a WM_TIMER event that will wake us up to check for any pending
  // timers (in case we are running within a nested, external sub-pump).
  SetTimer(message_wnd_
    , reinterpret_cast<UINT_PTR>(this)
    , static_cast<uint32>(delay_milli_sec)
    , NULL);
}

void MessagePumpForUI::PumpOutPendingPaintMessages() {
  // If we are being called outside of the context of Run, then don't try to
  // do any work.
  if (!state_)
    return;

  // Create a mini-message-pump to force immediate processing of only Windows
  // WM_PAINT messages.  Don't provide an infinite loop, but do enough peeking
  // to get the job done.  Actual base max is 4 peeks, but we'll be a little
  // safe here.
  const int32 kMaxPeekCount = 20;
  int32 peek_count;
  for (peek_count = 0; peek_count < kMaxPeekCount; ++peek_count) {
    MSG msg;
    if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE | PM_QS_PAINT))
      break;

    ProcessMessageHelper(msg);
    if (state_->should_quit_)  // Handle WM_QUIT.
      break;
  }
}

bool MessagePumpForUI::AddMsgFilter(
  base::MessageFilter * filter) {
  filters_.push_back(filter);
  return true;
}

bool MessagePumpForUI::RemoveMsgFilter(
  base::MessageFilter * filter) {
  std::list<base::MessageFilter *>::iterator it =
    std::find(filters_.begin(), filters_.end(), filter);
  if (it != filters_.end()) {
    filters_.erase(it);
    return true;
  } else {
    return false;
  }
}

bool MessagePumpForUI::ProcessMessage(const MSG& msg) {
  if (filters_.size() > 0) {
    std::list<base::MessageFilter *>::iterator it =
      --filters_.end();

    for (;; --it) {
      MessageFilter* filter = *it;
      // 这里包含了几个窗口类的预处理，例如canvas的PreTranslateMessage（）
      if (filter != NULL
        && filter->PreTranslateMessage(const_cast<MSG*>(&msg)))
        return true;

      if (it == filters_.begin())
        break;
    }
  }

  return false;   // not translated
}

bool MessagePumpForUI::AddShortCutKeyMsgFilter(
  base::MessageFilter * filter) {
  if (filter) {
    shortcut_filters_.push_back(filter);
  }

  return true;
}

bool MessagePumpForUI::AddAcceleratorHanderMsgFilter(
  base::MessageFilter * filter) {
  if (filter) {
    accelerator_filters_.push_back(filter);
  }

  return true;
}


bool MessagePumpForUI::RemoveShortCutKeyMsgFilter(
  base::MessageFilter * filter) {
  std::list<base::MessageFilter *>::iterator it =
    std::find(shortcut_filters_.begin()
    , shortcut_filters_.end()
    , filter);

  if (it != shortcut_filters_.end())  {
    shortcut_filters_.erase(it);
    return true;
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------------
// MessagePumpForUI private:

// static
LRESULT CALLBACK MessagePumpForUI::WndProcThunk(
  HWND hwnd,
  UINT message,
  WPARAM wparam,
  LPARAM lparam) {
  switch (message) {
    case kMsgHaveWork:
      reinterpret_cast<MessagePumpForUI*>(wparam)->HandleWorkMessage();
      break;
    case WM_TIMER:
      reinterpret_cast<MessagePumpForUI*>(wparam)->HandleTimerMessage();
      break;
  }

  return DefWindowProc(hwnd, message, wparam, lparam);
}

void MessagePumpForUI::DoRunLoop() {
  // IF this was just a simple PeekMessage() loop (servicing all possible work
  // queues), then Windows would try to achieve the following order according
  // to MSDN documentation about PeekMessage with no filter):
  //  * Sent messages
  //  * Posted messages
  //  * Sent messages (again)
  //  * WM_PAINT messages
  //  * WM_TIMER messages
  //
  // Summary: none of the above classes is starved, and sent messages has
  // twice the chance of being processed (i.e., reduced service time).

  for (;;) {
    // If we do any work, we may create more messages etc., and more work
    // may possibly be waiting in another task group.  When we (for example)
    // ProcessNextWindowsMessage(), there is a good chance there are still
    // more messages waiting.  On the other hand, when any of these methods
    // return having done no work, then it is pretty unlikely that calling
    // them again quickly will find any work to do.  Finally, if they all
    // say they had no work, then it is a good time to consider sleeping
    // (waiting) for more work.

    bool more_work_is_plausible = ProcessNextWindowsMessage();
    if (state_->should_quit_) {
      Log(LL_DEBUG
        , TAG(L"Base")
        , L"After ProcessNextWindowsMessage,MoreWork:%d"
        , static_cast<int32>(more_work_is_plausible));
      break;
    }

    // if (state_->process_task_)
    {
      more_work_is_plausible |= state_->delegate_->DoWork();
    }

    if (state_->should_quit_) {
      Log(LL_DEBUG
        , TAG(L"Base")
        , L"After DoWork,MoreWork:%d"
        , static_cast<int32>(more_work_is_plausible));
      break;
    }

    // if (state_->process_task_)
    {
      more_work_is_plausible |=
        state_->delegate_->DoDelayedWork(&delayed_work_time_);
    }

    // If we did not process any delayed work, then we can assume that our
    // existing WM_TIMER if any will fire when delayed work should run.  We
    // don't want to disturb that timer if it is already in flight. However,
    // if we did do all remaining delayed work, then lets kill the WM_TIMER.
    if (more_work_is_plausible && delayed_work_time_.IsNull())
      KillTimer(message_wnd_, reinterpret_cast<UINT_PTR>(this));

    if (state_->should_quit_) {
      Log(LL_DEBUG
        , TAG(L"Base")
        , L"After DoDelayedWork,MoreWork:%d"
        , static_cast<int32>(more_work_is_plausible));
      break;
    }

    if (more_work_is_plausible)
      continue;

    // if (state_->process_task_)
    {
      more_work_is_plausible = state_->delegate_->DoIdleWork();
    }

    if (state_->should_quit_) {
      Log(LL_DEBUG
        , TAG(L"Base")
        , L"After DoIdleWork,MoreWork:%d"
        , static_cast<int32>(more_work_is_plausible));
      break;
    }

    if (more_work_is_plausible)
      continue;

    WaitForWork();  // Wait (sleep) until we have work to do again.
  }
}

void MessagePumpForUI::InitMessageWnd() {
  HINSTANCE hinst = GetModuleHandle(NULL);

  WNDCLASSEX wc = { 0 };
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = WndProcThunk;
  wc.hInstance = hinst;
  wc.lpszClassName = kWndClass;
  RegisterClassEx(&wc);

  message_wnd_ = CreateWindow(kWndClass
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , HWND_MESSAGE
    , 0
    , hinst
    , 0);
}

void MessagePumpForUI::WaitForWork() {
  // Wait until a message is available, up to the time needed by the timer
  // manager to fire the next set of timers.
  int32 delay = GetCurrentDelay();
  if (delay < 0)  // Negative value means no timers waiting.
    delay = INFINITE;

  // if (!state_->process_task_)
  // {
  //    delay = INFINITE;
  // }

  DWORD result = MsgWaitForMultipleObjectsEx(
    0, NULL, static_cast<DWORD>(delay), QS_ALLINPUT,
    MWMO_INPUTAVAILABLE);

  if (WAIT_OBJECT_0 == result) {
    // A WM_* message is available.
    // If a parent child relationship exists between windows across threads
    // then their thread inputs are implicitly attached. This causes the
    // MsgWaitForMultipleObjectsEx API to return indicating that messages
    // are ready for processing (specifically mouse messages intended for
    // the child window. Occurs if the child window has capture) The
    // subsequent PeekMessages call fails to return any messages thus
    // causing us to enter a tight loop at times.
    // The WaitMessage call below is a workaround to give the child window
    // sometime to process its input messages.
    MSG msg = { 0 };
    DWORD queue_status = GetQueueStatus(QS_MOUSE);
    if (HIWORD(queue_status) & QS_MOUSE &&
      !PeekMessage(&msg
        , NULL
        , WM_MOUSEFIRST
        , WM_MOUSELAST
        , PM_NOREMOVE)) {
      WaitMessage();
    }

    return;
  }
}

void MessagePumpForUI::HandleWorkMessage() {
  // If we are being called outside of the context of Run, then don't try to
  // do any work.  This could correspond to a MessageBox call or something of
  // that sort.
  if (!state_) {
    // Since we handled a kMsgHaveWork message, we must still update this
    // flag.
    InterlockedExchange(&have_work_, 0);
    return;
  }

  // Let whatever would have run had we not been putting messages in the queue
  // run now.  This is an attempt to make our dummy message not starve other
  // messages that may be in the Windows message queue.
  ProcessPumpReplacementMessage();

  // Now give the delegate a chance to do some work.  He'll let us know if he
  // needs to do more work.
  if (state_->delegate_->DoWork())
    ScheduleWork();
}

void MessagePumpForUI::HandleTimerMessage() {
  KillTimer(message_wnd_, reinterpret_cast<UINT_PTR>(this));

  // If we are being called outside of the context of Run, then don't do
  // anything.  This could correspond to a MessageBox call or something of
  // that sort.
  if (!state_)
    return;

  state_->delegate_->DoDelayedWork(&delayed_work_time_);
  if (!delayed_work_time_.IsNull()) {
    // A bit gratuitous to set delayed_work_time_ again, but oh well.
    ScheduleDelayedWork(delayed_work_time_);
  }
}

bool MessagePumpForUI::ProcessNextWindowsMessage() {
  // If there are sent messages in the queue then PeekMessage internally
  // dispatches the message and returns false. We return true in this
  // case to ensure that the message loop peeks again instead of calling
  // MsgWaitForMultipleObjectsEx again.
  bool send_messages_in_queue = false;
  DWORD queue_status = GetQueueStatus(QS_SENDMESSAGE);
  if (HIWORD(queue_status) & QS_SENDMESSAGE)
    send_messages_in_queue = true;

  MSG msg;
  if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    return ProcessMessageHelper(msg);

  return send_messages_in_queue;
}

bool MessagePumpForUI::HandleAccelerator(const MSG& msg) {
  if (state_->dispatcher_)
    return state_->dispatcher_->Dispatch(msg);

  return false;
}

bool MessagePumpForUI::ProcessMessageHelper(const MSG& msg) {
  if (WM_QUIT == msg.message) {
    // Repost the QUIT message so that it will be retrieved by the primary
    // GetMessage() loop.
    state_->should_quit_ = true;
    PostQuitMessage(static_cast<int>(msg.wParam));
    return false;
  }

  // While running our main message pump, we discard kMsgHaveWork messages.
  if (msg.message == kMsgHaveWork && msg.hwnd == message_wnd_)
    return ProcessPumpReplacementMessage();

  if (CallMsgFilter(const_cast<MSG*>(&msg), kMessageFilterCode))
    return true;

  if (!PreTranslateMessage(&msg)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return true;
}

bool MessagePumpForUI::ProcessPumpReplacementMessage() {
  // When we encounter a kMsgHaveWork message, this method is called to peek
  // and process a replacement message, such as a WM_PAINT or WM_TIMER.  The
  // goal is to make the kMsgHaveWork as non-intrusive as possible, even
  // though a continuous stream of such messages are posted.  This method
  // carefully peeks a message while there is no chance for a kMsgHaveWork to
  // be pending, then resets the have_work_ flag (allowing a replacement
  // kMsgHaveWork to possibly be posted), and finally dispatches that peeked
  // replacement. Note that the re-post of kMsgHaveWork may be asynchronous to
  // this thread!!

  MSG msg;
  bool have_message = (0 != PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));

  // Since we discarded a kMsgHaveWork message, we must update the flag.
  InterlockedExchange(&have_work_, 0);

  // We don't need a special time slice if we didn't have_message to process.
  if (!have_message)
    return false;

  // Guarantee we'll get another time slice in the case where we go into
  // native windows code.This ScheduleWork() may hurt performance a tiny bit
  // when tasks appear very infrequently, but when the event queue is busy,the
  // kMsgHaveWork events get (percentage wise) rarer and rarer.
  ScheduleWork();

  return ProcessMessageHelper(msg);
}

bool MessagePumpForUI::PreTranslateMessage(const MSG* msg) {
  MSG * non_const_msg = const_cast<MSG *>(msg);
  if (HandleAccelerator(*msg)) {
    return true;
  }

  if (filters_.size() > 0) {
    std::list<base::MessageFilter *>::iterator it =
      --filters_.end();

    for (;; --it) {
      MessageFilter* filter = *it;
      // 这里包含了几个窗口类的预处理，例如canvas的PreTranslateMessage（）
      if (filter != NULL
        && filter->PreTranslateMessage(non_const_msg))
        return true;

      if (it == filters_.begin())
        break;
    }
  }

  return false;   // not translated
}

//-----------------------------------------------------------------------------
// MessagePumpForIO public:
MessagePumpForIO::MessagePumpForIO() {
  completion_port_ = (HANDLE)CreateIoCompletionPort(
    HANDLE(INVALID_HANDLE_VALUE), NULL, NULL, 1);
}

void MessagePumpForIO::ScheduleWork() {
  if (InterlockedExchange(&have_work_, 1))
    return;  // Someone else continued the pumping.

  // Make sure the MessagePump does some work for us.
  PostQueuedCompletionStatus(completion_port_, 0,
    reinterpret_cast<ULONG_PTR>(this),
    reinterpret_cast<OVERLAPPED*>(this));
}

void MessagePumpForIO::ScheduleDelayedWork(
  const base::Time& delayed_work_time) {
  // We know that we can't be blocked right now since this method can only be
  // called on the same thread as Run, so we only need to update our record of
  // how long to sleep when we do sleep.
  delayed_work_time_ = delayed_work_time;
}

void MessagePumpForIO::RegisterIOHandler(
  HANDLE pipe,
  IOHandler* handler) {
  ULONG_PTR key = reinterpret_cast<ULONG_PTR>(handler);
  CreateIoCompletionPort(pipe, completion_port_, key, 1);
}

//-----------------------------------------------------------------------------
// MessagePumpForIO private:
void MessagePumpForIO::DoRunLoop() {
  for (;;) {
    // If we do any work, we may create more messages etc.,and more work may
    // possibly be waiting in another task group.  When we (for example)
    // WaitForIOCompletion(), there is a good chance there are still more
    // messages waiting. On the other hand, when any of these methods return
    // having done no work, then it is pretty unlikely that calling them
    // again quickly will find any work to do. Finally, if they all say they
    // had no work, then it is a good time to consider sleeping (waiting)
    // for more work.

    bool more_work_is_plausible = state_->delegate_->DoWork();
    if (state_->should_quit_)
      break;

    more_work_is_plausible |= WaitForIOCompletion(0, NULL);
    if (state_->should_quit_)
      break;

    more_work_is_plausible |=
      state_->delegate_->DoDelayedWork(&delayed_work_time_);
    if (state_->should_quit_)
      break;

    if (more_work_is_plausible)
      continue;

    more_work_is_plausible = state_->delegate_->DoIdleWork();
    if (state_->should_quit_)
      break;

    if (more_work_is_plausible)
      continue;

    WaitForWork();  // Wait (sleep) until we have work to do again.
  }
}

// Wait until IO completes, up to the time needed by the timer manager to fire
// the next set of timers.
void MessagePumpForIO::WaitForWork() {
  // We do not support nested IO message loops. This is to avoid messy
  // recursion problems.;

  int32 timeout = GetCurrentDelay();
  if (timeout < 0)  // Negative value means no timers waiting.
    timeout = INFINITE;

  WaitForIOCompletion(static_cast<DWORD>(timeout), NULL);
}

bool MessagePumpForIO::WaitForIOCompletion(DWORD timeout
  , IOHandler * filter) {
  IOItem item;

  if (completed_items_.empty()
    || !MatchCompletedIOItem(filter, &item)) {
    // We have to ask the system for another IO completion.
    if (!GetIOItem(timeout, &item))
      return false;

    if (ProcessInternalIOItem(item))
      return true;
  }

  // process the io item
  // liuyaoming : when the Context.handler_ is initialized? should be
  // item.handler_?
  if (item.context_->handler_) {
    if (filter && item.handler_ != filter) {
      // Save this item for later
      completed_items_.push_back(item);
    } else {
      item.handler_->OnIOCompleted(item.context_
        , item.bytes_transfered_
        , item.error_);
    }
  } else {
    // The handler must be gone by now, just cleanup the mess.
    delete item.context_;
  }

  return true;
}

// Asks the OS for another IO completion result.
bool MessagePumpForIO::GetIOItem(DWORD timeout, IOItem * item) {
  memset(item, 0, sizeof(*item));

  ULONG_PTR key = NULL;
  OVERLAPPED * overlapped = NULL;
  if (!GetQueuedCompletionStatus(completion_port_,
    &item->bytes_transfered_, &key,
    &overlapped, timeout)) {
    if (!overlapped)
      return false;  // Nothing in the queue.

    item->error_ = GetLastError();
    item->bytes_transfered_ = 0;
  }

  item->handler_ = reinterpret_cast<IOHandler*>(key);
  item->context_ = reinterpret_cast<IOContext*>(overlapped);

  return true;
}

bool MessagePumpForIO::ProcessInternalIOItem(const IOItem & item) {
  if (this == reinterpret_cast<MessagePumpForIO *>(item.context_) &&
    this == reinterpret_cast<MessagePumpForIO *>(item.handler_)) {
    // this item is get from our ScheduleWork
    InterlockedExchange(&have_work_, 0);
    return true;
  }

  return false;
}

// Returns a completion item that was previously received, matching the filter
bool MessagePumpForIO::MatchCompletedIOItem(IOHandler * filter, IOItem * item) {
  for (std::list<IOItem>::iterator it = completed_items_.begin();
    it != completed_items_.end(); ++it) {
    if (!filter || it->handler_ == filter) {
      *item = *it;
      completed_items_.erase(it);
      return true;
    }
  }

  return false;
}

MessagePumpDefault::MessagePumpDefault()
  : event_(false, false) {}

void MessagePumpDefault::DoRunLoop() {
  for (;;) {
    bool did_work = state_->delegate_->DoWork();
    if (state_->should_quit_)
      break;

    did_work |= state_->delegate_->DoDelayedWork(&delayed_work_time_);
    if (state_->should_quit_)
      break;

    if (did_work)
      continue;

    did_work = state_->delegate_->DoIdleWork();
    if (state_->should_quit_)
      break;

    // if has done any task, continue
    if (did_work)
      continue;

    // no task done, sleep until delayed_work_time
    if (delayed_work_time_.IsNull()) {
      event_.Wait();
    } else {
      base::TimeDelta delay = delayed_work_time_ - base::Time::Now();
      if (delay > base::TimeDelta()) {
        event_.Wait(static_cast<uint32>(delay.InMilliseconds()));
      } else {
        // It looks like delayed_work_time_ indicates a time in the
        // past, so we need to call DoDelayedWork now.
        delayed_work_time_ = base::Time();
      }
    }
    // Since event_ is auto-reset, we don't need to do anything special here
    // other than service each delegate method.
  }
}

void MessagePumpDefault::ScheduleWork() {
  // Since this can be called on any thread, we need to ensure that our Run
  // loop wakes up.
  event_.Signal();
}

void MessagePumpDefault::ScheduleDelayedWork(
  const base::Time& delayed_work_time) {
  // We know that we can't be blocked on Wait right now since this method can
  // only be called on the same thread as Run, so we only need to update our
  // record of how long to sleep when we do sleep.
  delayed_work_time_ = delayed_work_time;
}
}  // namespace base

namespace base {
static base::Lock g_lock;
//////////////////////////////////////////////////////////////////////////
// static
MessageLoopProxyManager* MessageLoopProxyManager::Instance() {
  base::AutoLock lock(g_lock);
  static MessageLoopProxyManager* s_instance = NULL;
  if (s_instance == NULL)
    s_instance = new MessageLoopProxyManager();

  return s_instance;
}

//////////////////////////////////////////////////////////////////////////
MessageLoopProxyManager::MessageLoopProxyManager() {}

MessageLoopProxyManager::~MessageLoopProxyManager() {}

bool MessageLoopProxyManager::RegisterMessageLoopProxy(
  MessageLoopProxy * target_message_loop_proxy) {
  if (!target_message_loop_proxy
    || GetMessageLoopProxy(target_message_loop_proxy->GetThreadID()))
    return false;

  base::AutoLock lock(callers_lock_);
  thread_id_2_message_loop_[target_message_loop_proxy->GetThreadID()]
    = target_message_loop_proxy;

  return true;
}

bool MessageLoopProxyManager::PostTaskByThreadID(DWORD thread_id
  , base::Task* task
  , INT64 delay) {
  base::ScopedRefPtr<MessageLoopProxy> target_message_loop_proxy
    = GetMessageLoopProxy(thread_id);

  if (!target_message_loop_proxy)
    return false;

  target_message_loop_proxy->PostTask(task, delay);

  return true;
}

bool MessageLoopProxyManager::RunNestMessageLoop(DWORD thread_id) {
  base::ScopedRefPtr<MessageLoopProxy> target_message_loop_proxy
    = GetMessageLoopProxy(thread_id);

  if (!target_message_loop_proxy)
    return false;

  target_message_loop_proxy->RunNestMessageLoop();

  return true;
}

bool MessageLoopProxyManager::QuitMessageLoop(ThreadId thread_id, bool now) {
  base::ScopedRefPtr<MessageLoopProxy> target_message_loop_proxy
    = GetMessageLoopProxy(thread_id);

  if (!target_message_loop_proxy)
    return false;

  if (now)
    target_message_loop_proxy->QuitNow();
  else
    target_message_loop_proxy->Quit();

  return true;
}

base::ScopedRefPtr<MessageLoopProxy> MessageLoopProxyManager::
  GetMessageLoopProxy(ThreadId thread_id) {
  base::AutoLock lock(callers_lock_);
  std::map<ThreadId, base::ScopedRefPtr<MessageLoopProxy>>::iterator
    iter = thread_id_2_message_loop_.end();

  iter = thread_id_2_message_loop_.find(thread_id);
  if (iter != thread_id_2_message_loop_.end())
    return iter->second;

  return base::ScopedRefPtr<MessageLoopProxy>();
}

void MessageLoopProxyManager::Clear() {
  base::AutoLock lock(callers_lock_);
  thread_id_2_message_loop_.clear();
}
}  // namespace base

namespace base {
const int WATCHING_INTERVAL_MS = 20;

static double S_PERFORMANCE_FREQUENCY = 0.;

struct ProfileInit {
  ProfileInit() {
    static LARGE_INTEGER li = {0};
    QueryPerformanceFrequency(&li);
    S_PERFORMANCE_FREQUENCY = static_cast<double>(li.QuadPart);
  }

  ~ProfileInit() {}
};

static ProfileInit s_init;
//////////////////////////////////////////////////////////////////////////
InspectPackage::InspectPackage(uint64 inspect_id)
  : inspect_id_(inspect_id)
  , send_stamp_(0)
  , receive_stamp_(0) {}

void InspectPackage::Mark(eInspectMark mark) {
  LARGE_INTEGER li = {0};
  ::QueryPerformanceCounter(&li);

  switch (mark) {
    case eIM_SEND: {
      send_stamp_ = li.QuadPart;
    }
    break;
    case eIM_INVOKE: {
      invoke_stamp_ = li.QuadPart;
    }
    break;
    case eIM_RECEIVE: {
      receive_stamp_ = li.QuadPart;
    }
    break;
  }
}

double InspectPackage::Stamp(int64 datum) {
  return (send_stamp_-datum)/S_PERFORMANCE_FREQUENCY;
}

double InspectPackage::Lag(eInspectMark mark) {
  int64 lLag = 0;
  switch (mark) {
    case eIM_INVOKE: {
      lLag = invoke_stamp_ - send_stamp_;
    }
    break;
    case eIM_RECEIVE: {
      lLag = receive_stamp_ - invoke_stamp_;
    }
    break;
  }

  return lLag/S_PERFORMANCE_FREQUENCY;
}

//////////////////////////////////////////////////////////////////////////
MessageLoopProfile::MessageLoopProfile(ThreadId watch_thread_id
  , ThreadId target_thread_id)
  : watch_thread_id_(watch_thread_id)
  , target_thread_id_(target_thread_id)
  , inspect_seed_(0)
  , switch_(false) {}

MessageLoopProfile::~MessageLoopProfile() {
  watch_thread_id_ = 0;
  target_thread_id_ = 0;
}

//////////////////////////////////////////////////////////////////////////
void MessageLoopProfile::Start() {
  switch_ = true;

  // Log(LL_DEBUG
  //  ,  L"Base"
  //  , L"[Profile] %-12lu %-12s %-12s %-12s %-12s"
  //  , target_thread_id_
  //  , L"Stamp"
  //  , L"ID"
  //  , L"Invoke Lag"
  //  , L"ReciveLag");

  LARGE_INTEGER li = {0};
  ::QueryPerformanceCounter(&li);
  start_stamp_ = li.QuadPart;

  Inspecting();
}

void MessageLoopProfile::Stop() {
  switch_ = false;

  watch_thread_id_ = 0;
  target_thread_id_ = 0;
}

void MessageLoopProfile::Inspecting() {
  if (!watch_thread_id_ || !target_thread_id_ || !switch_)
    return;

  // 1.Generate package
  InspectPackage package(++inspect_seed_);
  package.Mark(InspectPackage::eIM_SEND);

  MessageLoopProxyManager::Instance()->
    PostTaskByThreadID(target_thread_id_
      , base::CreatePtrTask(this
        , &MessageLoopProfile::InvokeOnTarget
        , package));

  // 2.Continue inspecting
  MessageLoopProxyManager::Instance()->
    PostTaskByThreadID(watch_thread_id_
      , base::CreatePtrTask(this
        , &MessageLoopProfile::Inspecting)
      , WATCHING_INTERVAL_MS);
}

void MessageLoopProfile::InvokeOnTarget(InspectPackage package) {
  package.Mark(InspectPackage::eIM_INVOKE);

  if (!watch_thread_id_ || !target_thread_id_)
    return;

  base::MessageLoopProxyManager::Instance()->
    PostTaskByThreadID(watch_thread_id_
      , base::CreatePtrTask(this
        , &MessageLoopProfile::ReciveOnWatch
        , package));
}

void MessageLoopProfile::ReciveOnWatch(InspectPackage package) {
  package.Mark(InspectPackage::eIM_RECEIVE);

  // Log(LL_DEBUG
  //  , TAG(L"Base")
  //  , L"[Profile] %-12lu %-12lf %-12llu %-12lf %-12lf"
  //  , target_thread_id_
  //  , package.Stamp(start_stamp_)
  //  , package.ID()
  //  , package.Lag(InspectPackage::eIM_INVOKE)
  //  , package.Lag(InspectPackage::eIM_RECEIVE));
}
}  // namespace base
