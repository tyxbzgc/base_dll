// Copyright (c) 2017 The TAL Authors.
// All rights reserved.


#ifndef BASE_TIME_TIME_H_
#define BASE_TIME_TIME_H_

#include <limits>
#include <time.h>
#include <windows.h>

#include "base/basic_types.h"

namespace base {
class TimeDelta;
class Time;
class TimeTicks;

// TimeDelta ------------------------------------------------------------------
class TimeDelta {
 public:
  TimeDelta() : delta_micro_secs_(0) {}

  // Converts units of time to CTimeDeltas.
  static TimeDelta FromDays(int64 days);
  static TimeDelta FromHours(int64 hours);
  static TimeDelta FromMinutes(int64 minutes);
  static TimeDelta FromSeconds(int64 secs);
  static TimeDelta FromMilliseconds(int64 ms);
  static TimeDelta FromMicroseconds(int64 us);

  // Returns the internal numeric value of the TimeDelta object. Please don't
  // use this and do arithmetic on it, as it is more error prone than using
  // the provided operators.
  int64 ToInternalValue() const {
    return delta_micro_secs_;
  }

  // Returns the time delta in some unit.
  int32 InDays() const;
  int32 InHours() const;
  int32 InMinutes() const;
  double InSecondsF() const;
  int64 InSeconds() const;
  double InMillisecondsF() const;
  int64 InMilliseconds() const;
  int64 InMillisecondsRoundedUp() const;
  int64 InMicroseconds() const;

  TimeDelta& operator=(TimeDelta other) {
    delta_micro_secs_ = other.delta_micro_secs_;
    return *this;
  }

  // Computations with other deltas.
  TimeDelta operator+(TimeDelta other) const {
    return TimeDelta(delta_micro_secs_ + other.delta_micro_secs_);
  }
  TimeDelta operator-(TimeDelta other) const {
    return TimeDelta(delta_micro_secs_ - other.delta_micro_secs_);
  }

  TimeDelta& operator+=(TimeDelta other) {
    delta_micro_secs_ += other.delta_micro_secs_;
    return *this;
  }
  TimeDelta& operator-=(TimeDelta other) {
    delta_micro_secs_ -= other.delta_micro_secs_;
    return *this;
  }
  TimeDelta operator-() const {
    return TimeDelta(-delta_micro_secs_);
  }

  // Computations with int
  TimeDelta operator*(int64 n) const {
    return TimeDelta(delta_micro_secs_ * n);
  }
  TimeDelta operator/(int64 n) const {
    return TimeDelta(delta_micro_secs_ / n);
  }
  TimeDelta& operator*=(int64 n) {
    delta_micro_secs_ *= n;
    return *this;
  }
  TimeDelta& operator/=(int64 n) {
    delta_micro_secs_ /= n;
    return *this;
  }
  int64 operator/(TimeDelta other) const {
    return delta_micro_secs_ / other.delta_micro_secs_;
  }

  // Defined below because it depends on the definition of the other classes.
  Time operator+(Time t) const;
  TimeTicks operator+(TimeTicks t) const;

  // Comparison operators.
  bool operator==(TimeDelta other) const {
    return delta_micro_secs_ == other.delta_micro_secs_;
  }
  bool operator!=(TimeDelta other) const {
    return delta_micro_secs_ != other.delta_micro_secs_;
  }
  bool operator<(TimeDelta other) const {
    return delta_micro_secs_ < other.delta_micro_secs_;
  }
  bool operator<=(TimeDelta other) const {
    return delta_micro_secs_ <= other.delta_micro_secs_;
  }
  bool operator>(TimeDelta other) const {
    return delta_micro_secs_ > other.delta_micro_secs_;
  }
  bool operator>=(TimeDelta other) const {
    return delta_micro_secs_ >= other.delta_micro_secs_;
  }

 private:
  friend class Time;
  friend class TimeTicks;
  friend TimeDelta operator*(int64 n, TimeDelta td);

  // Constructs a delta given the duration in microseconds. This is private
  // to avoid confusion by callers with an integer constructor. Use
  // FromSeconds, FromMilliseconds, etc. instead.
  explicit TimeDelta(int64 delta_us) : delta_micro_secs_(delta_us) {}

  // Delta in microseconds.
  int64 delta_micro_secs_;
};

inline TimeDelta operator*(int64 n, TimeDelta td) {
  return TimeDelta(n * td.delta_micro_secs_);
}

// Time -----------------------------------------------------------------------

// Represents a wall clock time.
class Time {
 public:
  static const int64 kMillisecondsPerSecond = 1000;
  static const int64 kMicrosecondsPerMillisecond = 1000;
  static const int64 kMicrosecondsPerSecond
    = kMicrosecondsPerMillisecond * kMillisecondsPerSecond;
  static const int64 kMicrosecondsPerMinute = kMicrosecondsPerSecond * 60;
  static const int64 kMicrosecondsPerHour = kMicrosecondsPerMinute * 60;
  static const int64 kMicrosecondsPerDay = kMicrosecondsPerHour * 24;
  static const int64 kMicrosecondsPerWeek = kMicrosecondsPerDay * 7;
  static const int64 kNanosecondsPerMicrosecond = 1000;
  static const int64 kNanosecondsPerSecond
    = kNanosecondsPerMicrosecond * kMicrosecondsPerSecond;

  // Represents an exploded time that can be formatted nicely.
  struct Exploded {
    int32 year;      // Four digit year "2007"
    int32 month;     // 1-based month (values 1 = January, etc.)
    int32 day_of_week;   // 0-based day of week (0 = Sunday, etc.)
    int32 day_of_month;  // 1-based day of month (1-31)
    int32 hour;      // Hour within the current day (0-23)
    int32 minute;    // Minute within the current hour (0-59)
    int32 second;    // Second within the current minute (0-59 plus leap
    //   seconds which may take it up to 60).
    int32 millisecond;   // Milliseconds within the current second (0-999)
  };

  // Contains the NULL time. Use Time::Now() to get the current time.
  Time() : utc_micro_secs_(0) {}

  // Returns true if the time object has not been initialized.
  bool IsNull() const {
    return utc_micro_secs_ == 0;
  }

  // Returns the current time. Watch out, the system might adjust its clock
  // in which case time will actually go backwards. We don't guarantee that
  // times are increasing, or that two calls to Now() won't be the same.
  static Time Now();

  // Returns the maximum time, which should be greater than any reasonable time
  // with which we might compare it.
  static Time Max();

  // Returns the current time. Same as Now() except that this function always
  // uses system time so that there are no discrepancies between the returned
  // time and system time even on virtual environments including our test bot.
  // For timing sensitive unittests, this function should be used.
  static Time NowFromSystemTime();

  // Converts to/from time_t in UTC and a Time class.
  // TODO(brettw) this should be removed once everybody starts using the
  // |Time| class.
  static Time FromTimeT(time_t tt);
  time_t ToTimeT() const;

  // Converts time to/from a double which is the number of seconds since epoch
  // (Jan 1, 1970).  Webkit uses this format to represent time.
  static Time FromDoubleT(double dt);
  double ToDoubleT() const;

  static Time FromFileTime(FILETIME ft);
  FILETIME ToFileTime() const;

  // Converts an exploded structure representing either the local time or UTC
  // into a Time class.
  static Time FromUTCExploded(const Exploded& exploded) {
    return FromExploded(false, exploded);
  }
  static Time FromLocalExploded(const Exploded& exploded) {
    return FromExploded(true, exploded);
  }

  // Converts an integer value representing Time to a class. This is used
  // when deserializing a |Time| structure, using a value known to be
  // compatible. It is not provided as a constructor because the integer type
  // may be unclear from the perspective of a caller.
  static Time FromInternalValue(int64 micro_secs) {
    return Time(micro_secs);
  }

  // For serializing, use FromInternalValue to reconstitute. Please don't use
  // this and do arithmetic on it, as it is more error prone than using the
  // provided operators.
  int64 ToInternalValue() const {
    return utc_micro_secs_;
  }

  // Fills the given exploded structure with either the local time or UTC from
  // this time structure (containing UTC).
  void UTCExplode(Exploded* exploded) const {
    return Explode(false, exploded);
  }
  void LocalExplode(Exploded* exploded) const {
    return Explode(true, exploded);
  }

  // Rounds this time down to the nearest day in local time. It will represent
  // midnight on that day.
  Time LocalMidnight() const;

  Time& operator=(Time other) {
    utc_micro_secs_ = other.utc_micro_secs_;
    return *this;
  }

  // Compute the difference between two times.
  TimeDelta operator-(Time other) const {
    return TimeDelta(utc_micro_secs_ - other.utc_micro_secs_);
  }

  // Modify by some time delta.
  Time& operator+=(TimeDelta delta) {
    utc_micro_secs_ += delta.delta_micro_secs_;
    return *this;
  }
  Time& operator-=(TimeDelta delta) {
    utc_micro_secs_ -= delta.delta_micro_secs_;
    return *this;
  }

  // Return a new time modified by some delta.
  Time operator+(TimeDelta delta) const {
    return Time(utc_micro_secs_ + delta.delta_micro_secs_);
  }
  Time operator-(TimeDelta delta) const {
    return Time(utc_micro_secs_ - delta.delta_micro_secs_);
  }

  // Comparison operators
  bool operator==(Time other) const {
    return utc_micro_secs_ == other.utc_micro_secs_;
  }
  bool operator!=(Time other) const {
    return utc_micro_secs_ != other.utc_micro_secs_;
  }
  bool operator<(Time other) const {
    return utc_micro_secs_ < other.utc_micro_secs_;
  }
  bool operator<=(Time other) const {
    return utc_micro_secs_ <= other.utc_micro_secs_;
  }
  bool operator>(Time other) const {
    return utc_micro_secs_ > other.utc_micro_secs_;
  }
  bool operator>=(Time other) const {
    return utc_micro_secs_ >= other.utc_micro_secs_;
  }

 private:
  friend class TimeDelta;

  // Explodes the given time to either local time |need_local = true| or UTC
  // |need_local = false|.
  void Explode(bool need_local, Exploded* exploded) const;

  // Unexplodes a given time assuming the source is either local time
  // |is_local = true| or UTC |is_local = false|.
  static Time FromExploded(bool local, const Exploded& exploded);

  explicit Time(int64 us) : utc_micro_secs_(us) {}

  // The representation of Jan 1, 1970 UTC in microseconds since the
  // platform-dependent epoch.
  static const int64 kTimeTToMicrosecondsOffset;

  // Time in microseconds in UTC.
  int64 utc_micro_secs_;
};

// TimeTicks ------------------------------------------------------------------
class TimeTicks {
 public:
  TimeTicks() : ticks_(0) {}

  // Platform-dependent tick count representing "right now."
  // The resolution of this clock is ~1-15ms.  Resolution varies depending
  // on hardware/operating system configuration.
  static TimeTicks Now();

  // Returns true if this object has not been initialized.
  bool IsNull() const {
    return ticks_ == 0;
  }

  // Returns the internal numeric value of the TimeTicks object.
  int64 ToInternalValue() const {
    return ticks_;
  }

  TimeTicks& operator=(TimeTicks other) {
    ticks_ = other.ticks_;
    return *this;
  }

  // Compute the difference between two times.
  TimeDelta operator-(TimeTicks other) const {
    return TimeDelta(ticks_ - other.ticks_);
  }

  // Modify by some time delta.
  TimeTicks& operator+=(TimeDelta delta) {
    ticks_ += delta.delta_micro_secs_;
    return *this;
  }
  TimeTicks& operator-=(TimeDelta delta) {
    ticks_ -= delta.delta_micro_secs_;
    return *this;
  }

  // Return a new TimeTicks modified by some delta.
  TimeTicks operator+(TimeDelta delta) const {
    return TimeTicks(ticks_ + delta.delta_micro_secs_);
  }
  TimeTicks operator-(TimeDelta delta) const {
    return TimeTicks(ticks_ - delta.delta_micro_secs_);
  }

  // Comparison operators
  bool operator==(TimeTicks other) const {
    return ticks_ == other.ticks_;
  }
  bool operator!=(TimeTicks other) const {
    return ticks_ != other.ticks_;
  }
  bool operator<(TimeTicks other) const {
    return ticks_ < other.ticks_;
  }
  bool operator<=(TimeTicks other) const {
    return ticks_ <= other.ticks_;
  }
  bool operator>(TimeTicks other) const {
    return ticks_ > other.ticks_;
  }
  bool operator>=(TimeTicks other) const {
    return ticks_ >= other.ticks_;
  }

 protected:
  friend class TimeDelta;

  // Please use Now() to create a new object. This is for internal use
  // and testing. Ticks is in microseconds.
  explicit TimeTicks(int64 ticks) : ticks_(ticks) {}

  // Tick count in microseconds.
  int64 ticks_;
};

inline Time TimeDelta::operator+(Time t) const {
  return Time(t.utc_micro_secs_ + delta_micro_secs_);
}

inline TimeTicks TimeDelta::operator+(TimeTicks t) const {
  return TimeTicks(t.ticks_ + delta_micro_secs_);
}

// Inline the TimeDelta factory methods, for fast TimeDelta construction.
inline TimeDelta TimeDelta::FromDays(int64 days) {
  return TimeDelta(days * Time::kMicrosecondsPerDay);
}

// static
inline TimeDelta TimeDelta::FromHours(int64 hours) {
  return TimeDelta(hours * Time::kMicrosecondsPerHour);
}

// static
inline TimeDelta TimeDelta::FromMinutes(int64 minutes) {
  return TimeDelta(minutes * Time::kMicrosecondsPerMinute);
}

// static
inline TimeDelta TimeDelta::FromSeconds(int64 secs) {
  return TimeDelta(secs * Time::kMicrosecondsPerSecond);
}

// static
inline TimeDelta TimeDelta::FromMilliseconds(int64 milli_secs) {
  return TimeDelta(milli_secs * Time::kMicrosecondsPerMillisecond);
}

// static
inline TimeDelta TimeDelta::FromMicroseconds(int64 micro_secs) {
  return TimeDelta(micro_secs);
}
}  // namespace base

namespace base {
// From MSDN, FILETIME "Contains a 64-bit value representing the number of
// 100-nanosecond intervals since January 1, 1601 (UTC)."
inline int64 FileTimeToMicroseconds(const FILETIME& ft) {
  // Need to bit_cast to fix alignment, then divide by 10 to convert
  // 100-nanoseconds to milliseconds. This only works on little-endian
  // machines.
  int64 micro_secs = ft.dwHighDateTime;
  micro_secs = micro_secs << 32;
  micro_secs += ft.dwLowDateTime;

  return micro_secs / 10;
  // return bit_cast<int64, FILETIME>(ft) / 10;
}

inline void MicrosecondsToFileTime(int64 micro_secs, FILETIME* ft) {
  // Multiply by 10 to convert milliseconds to 100-nanoseconds. Bit_cast will
  // handle alignment problems. This only works on little-endian machines.
  micro_secs *= 10;
  ft->dwLowDateTime = DWORD(micro_secs & 0xFFFFFFFF);
  ft->dwHighDateTime = DWORD((micro_secs >> 32) & 0xFFFFFFFF);
  // *ft = bit_cast<FILETIME, int64>(us * 10);
}

inline int64 CurrentWallclockMicroseconds() {
  FILETIME ft;
  ::GetSystemTimeAsFileTime(&ft);
  return FileTimeToMicroseconds(ft);
}

// Time between resampling the un-granular clock for this API.  60 seconds.
const int kMaxMillisecondsToAvoidDrift = 60 * Time::kMillisecondsPerSecond;

__declspec(selectany) int64 initial_time = 0;
__declspec(selectany) TimeTicks initial_ticks;

inline void InitializeClock() {
  initial_ticks = TimeTicks::Now();
  initial_time = CurrentWallclockMicroseconds();
}

// Convert current local time to time string, like 2016 09 22 23 59 59
inline uint64 CurrentTimeToTimeString() {
  SYSTEMTIME now = { 0 };
  GetLocalTime(&now);
  //2010 05 28 21 05 00
  uint64 now_time = now.wYear*(uint64)10000000000 + now.wMonth*(uint64)100000000
    + now.wDay*(uint64)1000000 + now.wHour * 10000 + now.wMinute * 100
    + now.wSecond;
  return now_time;
}
}  // namespace base

namespace base {
// The internal representation of Time uses FILETIME, whose epoch is 1601-01-01
// 00:00:00 UTC.  ((1970-1601)*365+89)*24*60*60*1000*1000, where 89 is the
// number of leap year days between 1601 and 1970: (1970-1601)/4 excluding
// 1700, 1800, and 1900.
// static
__declspec(selectany)
  const int64 Time::kTimeTToMicrosecondsOffset = 11644473600000000I64;

// TimeDelta ------------------------------------------------------------------

inline int32 TimeDelta::InDays() const {
  return static_cast<int32>(delta_micro_secs_ / Time::kMicrosecondsPerDay);
}

inline int32 TimeDelta::InHours() const {
  return static_cast<int32>(delta_micro_secs_ / Time::kMicrosecondsPerHour);
}

inline int32 TimeDelta::InMinutes() const {
  return static_cast<int32>(delta_micro_secs_ / Time::kMicrosecondsPerMinute);
}

inline double TimeDelta::InSecondsF() const {
  return static_cast<double>(delta_micro_secs_) /
    Time::kMicrosecondsPerSecond;
}

inline int64 TimeDelta::InSeconds() const {
  return delta_micro_secs_ / Time::kMicrosecondsPerSecond;
}

inline double TimeDelta::InMillisecondsF() const {
  return static_cast<double>(delta_micro_secs_) /
    Time::kMicrosecondsPerMillisecond;
}

inline int64 TimeDelta::InMilliseconds() const {
  return delta_micro_secs_ / Time::kMicrosecondsPerMillisecond;
}

inline int64 TimeDelta::InMillisecondsRoundedUp() const {
  return (delta_micro_secs_ + Time::kMicrosecondsPerMillisecond - 1) /
    Time::kMicrosecondsPerMillisecond;
}

inline int64 TimeDelta::InMicroseconds() const {
  return delta_micro_secs_;
}

// Time -----------------------------------------------------------------------

// static
inline Time Time::FromTimeT(time_t tt) {
  if (tt == 0)
    return Time();  // Preserve 0 so we can tell it doesn't exist.
  return Time((tt * kMicrosecondsPerSecond) + kTimeTToMicrosecondsOffset);
}

inline time_t Time::ToTimeT() const {
  if (utc_micro_secs_ == 0)
    return 0;  // Preserve 0 so we can tell it doesn't exist.

  return (utc_micro_secs_ - kTimeTToMicrosecondsOffset) /
    kMicrosecondsPerSecond;
}

// static
inline Time Time::FromDoubleT(double dt) {
  return Time(static_cast<int64>((dt *
    static_cast<double>(kMicrosecondsPerSecond)) + kTimeTToMicrosecondsOffset));
}

inline double Time::ToDoubleT() const {
  if (utc_micro_secs_ == 0)
    return 0;  // Preserve 0 so we can tell it doesn't exist.

  return (static_cast<double>(utc_micro_secs_ - kTimeTToMicrosecondsOffset) /
    static_cast<double>(kMicrosecondsPerSecond));
}

inline Time Time::LocalMidnight() const {
  Exploded exploded;
  LocalExplode(&exploded);
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  return FromLocalExploded(exploded);
}

// static
inline Time Time::Now() {
  if (initial_time == 0)
    InitializeClock();

  // We implement time using the high-resolution timers so that we can get
  // timeouts which are smaller than 10-15ms.  If we just used
  // CurrentWallclockMicroseconds(), we'd have the less-granular timer.
  //
  // To make this work, we initialize the clock (initial_time) and the
  // counter (initial_ctr).  To compute the initial time, we can check
  // the number of ticks that have elapsed, and compute the delta.
  //
  // To avoid any drift, we periodically resync the counters to the system
  // clock.
  while (true) {
    TimeTicks ticks = TimeTicks::Now();

    // Calculate the time elapsed since we started our timer
    TimeDelta elapsed = ticks - initial_ticks;

    // Check if enough time has elapsed that we need to resync the clock.
    if (elapsed.InMilliseconds() > kMaxMillisecondsToAvoidDrift) {
      InitializeClock();
      continue;
    }

    return Time(elapsed + Time(initial_time));
  }
}

// static
inline Time Time::Max() {
  return Time(std::numeric_limits<int64>::max());
}

// static
inline Time Time::NowFromSystemTime() {
  // Force resync.
  InitializeClock();
  return Time(initial_time);
}

// static
inline Time Time::FromFileTime(FILETIME ft) {
  return Time(FileTimeToMicroseconds(ft));
}

inline FILETIME Time::ToFileTime() const {
  FILETIME utc_ft;
  MicrosecondsToFileTime(utc_micro_secs_, &utc_ft);
  return utc_ft;
}

// static
inline Time Time::FromExploded(bool local, const Exploded& exploded) {
  // Create the system struct representing our exploded time. It will either
  // be in local time or UTC.
  SYSTEMTIME st;
  st.wYear = static_cast<WORD>(exploded.year);
  st.wMonth = static_cast<WORD>(exploded.month);
  st.wDayOfWeek = static_cast<WORD>(exploded.day_of_week);
  st.wDay = static_cast<WORD>(exploded.day_of_month);
  st.wHour = static_cast<WORD>(exploded.hour);
  st.wMinute = static_cast<WORD>(exploded.minute);
  st.wSecond = static_cast<WORD>(exploded.second);
  st.wMilliseconds = static_cast<WORD>(exploded.millisecond);

  // Convert to FILETIME.
  FILETIME ft;
  if (!SystemTimeToFileTime(&st, &ft)) {
    return Time(0);
  }

  // Ensure that it's in UTC.
  if (local) {
    FILETIME utc_ft;
    LocalFileTimeToFileTime(&ft, &utc_ft);
    return Time(FileTimeToMicroseconds(utc_ft));
  }

  return Time(FileTimeToMicroseconds(ft));
}

inline void Time::Explode(bool need_local, Exploded* exploded) const {
  // FILETIME in UTC.
  FILETIME utc_ft;
  MicrosecondsToFileTime(utc_micro_secs_, &utc_ft);

  // FILETIME in local time if necessary.
  bool success = TRUE;
  FILETIME ft;
  if (need_local)
    success = FileTimeToLocalFileTime(&utc_ft, &ft);
  else
    ft = utc_ft;

  // FILETIME in SYSTEMTIME (exploded).
  SYSTEMTIME st;
  if (!success || !FileTimeToSystemTime(&ft, &st)) {
    ZeroMemory(exploded, sizeof(exploded));
    return;
  }

  exploded->year = st.wYear;
  exploded->month = st.wMonth;
  exploded->day_of_week = st.wDayOfWeek;
  exploded->day_of_month = st.wDay;
  exploded->hour = st.wHour;
  exploded->minute = st.wMinute;
  exploded->second = st.wSecond;
  exploded->millisecond = st.wMilliseconds;
}

// TimeTicks ------------------------------------------------------------------
namespace {
  // The last timeGetTime value we saw, to detect rollover.
  LARGE_INTEGER last_seen_now = { 0 };
  LARGE_INTEGER roll_over_us = { 0 };
  LARGE_INTEGER frequency = { 0 };

  // Lock protecting rollover_ms and last_seen_now.
  // Note: this is a global object, and we usually avoid these. However, the
  // time code is low-level, and we don't want to use Singletons here (it
  // would be too easy to use a Singleton without even knowing it, and that
  // may lead to many gotchas). Its impact on startup time should be
  // negligible due to low-level nature of time code.
  //base::Lock rollover_lock;
}  // namespace

// static
inline TimeTicks TimeTicks::Now() {
  //base::AutoLock locked(rollover_lock);

  if (frequency.QuadPart == 0) {
    QueryPerformanceFrequency(&frequency);
  }
  LARGE_INTEGER time_couter;
  QueryPerformanceCounter(&time_couter);
  if (time_couter.QuadPart < last_seen_now.QuadPart) {
    roll_over_us.QuadPart += (0x100000000I64);
  }

  last_seen_now = time_couter;
  return (TimeTicks() +
    TimeDelta(static_cast<int64>((
    time_couter.QuadPart + roll_over_us.QuadPart) *
      Time::kMicrosecondsPerSecond /
      static_cast<double>(frequency.QuadPart))));
}
}  // namespace base
#endif  // BASE_TIME_TIME_H_
