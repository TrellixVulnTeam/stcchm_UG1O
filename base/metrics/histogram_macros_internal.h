// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_HISTOGRAM_MACROS_INTERNAL_H_
#define BASE_METRICS_HISTOGRAM_MACROS_INTERNAL_H_

#include "base/atomicops.h"
#include "base/logging.h"
#include "base/metrics/histogram.h"
#include "base/time/time.h"

// This is for macros internal to base/metrics. They should not be used outside
// of this directory. For writing to UMA histograms, see histogram_macros.h.

// TODO(rkaplow): Improve commenting of these methods.

//------------------------------------------------------------------------------
// Histograms are often put in areas where they are called many many times, and
// performance is critical.  As a result, they are designed to have a very low
// recurring cost of executing (adding additional samples). Toward that end,
// the macros declare a static pointer to the histogram in question, and only
// take a "slow path" to construct (or find) the histogram on the first run
// through the macro. We leak the histograms at shutdown time so that we don't
// have to validate using the pointers at any time during the running of the
// process.


// In some cases (integration into 3rd party code), it's useful to separate the
// definition of |atomic_histogram_pointer| from its use. To achieve this we
// define HISTOGRAM_POINTER_USE, which uses an |atomic_histogram_pointer|, and
// STATIC_HISTOGRAM_POINTER_BLOCK, which defines an |atomic_histogram_pointer|
// and forwards to HISTOGRAM_POINTER_USE.
#if 0
#define HISTOGRAM_POINTER_USE(atomic_histogram_pointer,                        \
                              constant_histogram_name,                         \
                              histogram_add_method_invocation,                 \
                              histogram_factory_get_invocation)                \
  do {                                                                         \
    /*                                                                         \
     * Acquire_Load() ensures that we acquire visibility to the                \
     * pointed-to data in the histogram.                                       \
     */                                                                        \
    base::HistogramBase* histogram_pointer(                                    \
        reinterpret_cast<base::HistogramBase*>(                                \
            base::subtle::Acquire_Load(atomic_histogram_pointer)));            \
    if (!histogram_pointer) {                                                  \
      /*                                                                       \
       * This is the slow path, which will construct OR find the               \
       * matching histogram.  histogram_factory_get_invocation includes        \
       * locks on a global histogram name map and is completely thread         \
       * safe.                                                                 \
       */                                                                      \
      histogram_pointer = histogram_factory_get_invocation;                    \
                                                                               \
      /*                                                                       \
       * Use Release_Store to ensure that the histogram data is made           \
       * available globally before we make the pointer visible. Several        \
       * threads may perform this store, but the same value will be            \
       * stored in all cases (for a given named/spec'ed histogram).            \
       * We could do this without any barrier, since FactoryGet entered        \
       * and exited a lock after construction, but this barrier makes          \
       * things clear.                                                         \
       */                                                                      \
      base::subtle::Release_Store(                                             \
          atomic_histogram_pointer,                                            \
          reinterpret_cast<base::subtle::AtomicWord>(histogram_pointer));      \
    }                                                                          \
    if (DCHECK_IS_ON())                                                        \
      histogram_pointer->CheckName(constant_histogram_name);                   \
    histogram_pointer->histogram_add_method_invocation;                        \
  } while (0)

// This is a helper macro used by other macros and shouldn't be used directly.
// Defines the static |atomic_histogram_pointer| and forwards to
// HISTOGRAM_POINTER_USE.
#define STATIC_HISTOGRAM_POINTER_BLOCK(constant_histogram_name,                \
                                       histogram_add_method_invocation,        \
                                       histogram_factory_get_invocation)       \
  do {                                                                         \
    /*                                                                         \
     * The pointer's presence indicates that the initialization is complete.   \
     * Initialization is idempotent, so it can safely be atomically repeated.  \
     */                                                                        \
    static base::subtle::AtomicWord atomic_histogram_pointer = 0;              \
    HISTOGRAM_POINTER_USE(&atomic_histogram_pointer, constant_histogram_name,  \
                          histogram_add_method_invocation,                     \
                          histogram_factory_get_invocation);                   \
  } while (0)

// This is a helper macro used by other macros and shouldn't be used directly.
#define INTERNAL_HISTOGRAM_CUSTOM_COUNTS_WITH_FLAG(name, sample, min, max,     \
                                                   bucket_count, flag)         \
    STATIC_HISTOGRAM_POINTER_BLOCK(                                            \
        name, Add(sample),                                                     \
        base::Histogram::FactoryGet(name, min, max, bucket_count, flag))

// This is a helper macro used by other macros and shouldn't be used directly.
// One additional bucket is created in the LinearHistogram for the illegal
// values >= boundary_value so that mistakes in calling the UMA enumeration
// macros can be detected.
#define INTERNAL_HISTOGRAM_ENUMERATION_WITH_FLAG(name, sample, boundary, flag) \
    STATIC_HISTOGRAM_POINTER_BLOCK(                                            \
        name, Add(sample),                                                     \
        base::LinearHistogram::FactoryGet(                                     \
            name, 1, boundary, boundary + 1, flag))

// This is a helper macro used by other macros and shouldn't be used directly.
// This is necessary to expand __COUNTER__ to an actual value.
#define INTERNAL_SCOPED_UMA_HISTOGRAM_TIMER_EXPANDER(name, is_long, key)       \
  INTERNAL_SCOPED_UMA_HISTOGRAM_TIMER_UNIQUE(name, is_long, key)

// This is a helper macro used by other macros and shouldn't be used directly.
#define INTERNAL_SCOPED_UMA_HISTOGRAM_TIMER_UNIQUE(name, is_long, key)         \
  class ScopedHistogramTimer##key {                                            \
   public:                                                                     \
    ScopedHistogramTimer##key() : constructed_(base::TimeTicks::Now()) {}      \
    ~ScopedHistogramTimer##key() {                                             \
      base::TimeDelta elapsed = base::TimeTicks::Now() - constructed_;         \
      if (is_long) {                                                           \
        UMA_HISTOGRAM_LONG_TIMES_100(name, elapsed);                           \
      } else {                                                                 \
        UMA_HISTOGRAM_TIMES(name, elapsed);                                    \
      }                                                                        \
    }                                                                          \
   private:                                                                    \
    base::TimeTicks constructed_;                                              \
  } scoped_histogram_timer_##key
#else
#define HISTOGRAM_POINTER_USE(atomic_histogram_pointer,                   \
                              constant_histogram_name,                    \
                              histogram_add_method_invocation,            \
                              histogram_factory_get_invocation) void(0)
#define STATIC_HISTOGRAM_POINTER_BLOCK(constant_histogram_name,               \
                                       histogram_add_method_invocation,       \
                                       histogram_factory_get_invocation)      void(0)
#define INTERNAL_HISTOGRAM_CUSTOM_COUNTS_WITH_FLAG(name, sample, min, max,     \
                                                   bucket_count, flag) void(0)
#define INTERNAL_HISTOGRAM_ENUMERATION_WITH_FLAG(name, sample, boundary, flag) void(0)
#define INTERNAL_SCOPED_UMA_HISTOGRAM_TIMER_EXPANDER(name, is_long, key) void(0)
#define INTERNAL_SCOPED_UMA_HISTOGRAM_TIMER_UNIQUE(name, is_long, key) void(0)
#endif
#endif  // BASE_METRICS_HISTOGRAM_MACROS_INTERNAL_H_
