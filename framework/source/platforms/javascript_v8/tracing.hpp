#pragma once

#include <logger.hpp>

#include <v8.h>

namespace interop {

inline std::string to_string(v8::GCType type)
{
    std::string ret;

    if (type & v8::GCType::kGCTypeIncrementalMarking) {
        ret += "GCTypeIncrementalMarking;";
    }
    if (type & v8::GCType::kGCTypeMarkSweepCompact) {
        ret += "GCTypeMarkSweepCompact;";
    }
    if (type & v8::GCType::kGCTypeProcessWeakCallbacks) {
        ret += "GCTypeProcessWeakCallbacks;";
    }
    if (type & v8::GCType::kGCTypeScavenge) {
        ret += "GCTypeScavenge;";
    }

    if (ret.empty()) {
        return "unknown";
    }

    return ret;

} // namespace

inline std::string to_string(v8::GCCallbackFlags flags)
{
    std::string ret;

    if (flags == v8::GCCallbackFlags::kNoGCCallbackFlags) {
        return "NoGCCallbackFlags;";
    }
    if (flags & v8::GCCallbackFlags::kGCCallbackFlagConstructRetainedObjectInfos) {
        ret += "GCCallbackFlagConstructRetainedObjectInfos;";
    }
    if (flags & v8::GCCallbackFlags::kGCCallbackFlagForced) {
        ret += "GCCallbackFlagForced;";
    }
    if (flags & v8::GCCallbackFlags::kGCCallbackFlagSynchronousPhantomCallbackProcessing) {
        ret += "GCCallbackFlagSynchronousPhantomCallbackProcessing;";
    }
    if (flags & v8::GCCallbackFlags::kGCCallbackFlagCollectAllAvailableGarbage) {
        ret += "GCCallbackFlagCollectAllAvailableGarbage;";
    }
    if (flags & v8::GCCallbackFlags::kGCCallbackFlagCollectAllExternalMemory) {
        ret += "GCCallbackFlagCollectAllExternalMemory;";
    }
    if (flags & v8::GCCallbackFlags::kGCCallbackScheduleIdleGarbageCollection) {
        ret += "GCCallbackScheduleIdleGarbageCollection;";
    }

    if (ret.empty()) {
        return "no match";
    }

    return ret;
}

void log_gc_start(v8::Isolate *, v8::GCType type, v8::GCCallbackFlags flags)
{
    interop_logger(debug, "type: " + to_string(type) + " flags: " + to_string(flags));
}
void log_gc_end(v8::Isolate *, v8::GCType type, v8::GCCallbackFlags flags)
{
    interop_logger(debug, "type: " + to_string(type) + " flags: " + to_string(flags));
}

void start_gc_tracing(v8::Isolate * isolate)
{
    isolate->AddGCPrologueCallback(log_gc_start);
    isolate->AddGCEpilogueCallback(log_gc_end);
}

} // namespace interop
