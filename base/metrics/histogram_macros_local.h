// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_HISTOGRAM_MACROS_LOCAL_H_
#define BASE_METRICS_HISTOGRAM_MACROS_LOCAL_H_

#include "base/logging.h"
#include "base/metrics/histogram.h"
#include "base/metrics/histogram_macros_internal.h"
#include "base/time/time.h"

// TODO(rkaplow): Migrate all LOCAL_* usage within Chromium to include this
// file instead of the histogram_macros.h file.

//------------------------------------------------------------------------------
// Enumeration histograms.
//
// For usage details, see the equivalents in histogram_macros.h.
#if 0
#define LOCAL_HISTOGRAM_ENUMERATION(name, sample, enum_max)                    \
   INTERNAL_HISTOGRAM_ENUMERATION_WITH_FLAG(                                   \
        name, sample, enum_max,                                                \
        base::HistogramBase::kNoFlags)

#define LOCAL_HISTOGRAM_BOOLEAN(name, sample)                                  \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, AddBoolean(sample),                   \
        base::BooleanHistogram::FactoryGet(name, base::Histogram::kNoFlags))

//------------------------------------------------------------------------------
// Percentage histograms.
//
// For usage details, see the equivalents in histogram_macros.h

#define LOCAL_HISTOGRAM_PERCENTAGE(name, under_one_hundred)                    \
    LOCAL_HISTOGRAM_ENUMERATION(name, under_one_hundred, 101)

//------------------------------------------------------------------------------
// Count histograms. These are used for collecting numeric data. Note that we
// have macros for more specialized use cases below (memory, time, percentages).
// For usage details, see the equivalents in histogram_macros.h.

#define LOCAL_HISTOGRAM_COUNTS_100(name, sample)                               \
    LOCAL_HISTOGRAM_CUSTOM_COUNTS(name, sample, 1, 100, 50)

#define LOCAL_HISTOGRAM_COUNTS_10000(name, sample)                             \
    LOCAL_HISTOGRAM_CUSTOM_COUNTS(name, sample, 1, 10000, 50)

#define LOCAL_HISTOGRAM_COUNTS_1000000(name, sample)                           \
    LOCAL_HISTOGRAM_CUSTOM_COUNTS(name, sample, 1, 1000000, 50)

#define LOCAL_HISTOGRAM_CUSTOM_COUNTS(name, sample, min, max, bucket_count)    \
    INTERNAL_HISTOGRAM_CUSTOM_COUNTS_WITH_FLAG(                                \
        name, sample, min, max, bucket_count, base::HistogramBase::kNoFlags)

//------------------------------------------------------------------------------
// Timing histograms. These are used for collecting timing data (generally
// latencies).
//
// For usage details, see the equivalents in histogram_macros.h.

#define LOCAL_HISTOGRAM_TIMES(name, sample) LOCAL_HISTOGRAM_CUSTOM_TIMES(      \
    name, sample, base::TimeDelta::FromMilliseconds(1),                        \
    base::TimeDelta::FromSeconds(10), 50)

#define LOCAL_HISTOGRAM_CUSTOM_TIMES(name, sample, min, max, bucket_count)     \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, AddTime(sample),                      \
        base::Histogram::FactoryTimeGet(name, min, max, bucket_count,          \
                                        base::HistogramBase::kNoFlags))

//------------------------------------------------------------------------------
// Memory histograms.
//
// For usage details, see the equivalents in histogram_macros.h.

#define LOCAL_HISTOGRAM_MEMORY_KB(name, sample) LOCAL_HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1000, 500000, 50)

//------------------------------------------------------------------------------
// Deprecated histograms. Not recommended for current use.

// TODO(rkaplow): See if we can clean up this macro and usage.
// Legacy non-explicit version. We suggest using LOCAL_HISTOGRAM_COUNTS_1000000
// instead.
#define LOCAL_HISTOGRAM_COUNTS(name, sample)                                   \
    LOCAL_HISTOGRAM_CUSTOM_COUNTS(name, sample, 1, 1000000, 50)
#else
#endif

#define LOCAL_HISTOGRAM_TIMES(name, sample) void(0)
#define LOCAL_HISTOGRAM_CUSTOM_TIMES(name, sample, min, max, bucket_count) void(0)
#define LOCAL_HISTOGRAM_COUNTS(name, sample) void(0)
#define LOCAL_HISTOGRAM_COUNTS_100(name, sample) void(0)
#define LOCAL_HISTOGRAM_COUNTS_10000(name, sample) void(0)
#define LOCAL_HISTOGRAM_COUNTS_1000000(name, sample) void(0)
#define LOCAL_HISTOGRAM_CUSTOM_COUNTS(name, sample, min, max, bucket_count) void(0)
#define HISTOGRAM_ENUMERATION_WITH_FLAG(name, sample, boundary, flag) void(0)
#define LOCAL_HISTOGRAM_PERCENTAGE(name, under_one_hundred) void(0)
#define LOCAL_HISTOGRAM_BOOLEAN(name, sample) void(0)
#define LOCAL_HISTOGRAM_ENUMERATION(name, sample, boundary_value) void(0)
#define LOCAL_HISTOGRAM_CUSTOM_ENUMERATION(name, sample, custom_ranges) void(0)
#define LOCAL_HISTOGRAM_MEMORY_KB(name, sample) void(0)
#endif  // BASE_METRICS_HISTOGRAM_MACROS_LOCAL_H_
