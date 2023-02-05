#ifndef raytracer_atomics_h
#define raytracer_atomics_h

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
typedef volatile LONG AtomicInt32;
#else
typedef volatile int32_t AtomicInt32;
#endif

/**
 * If theVal == oldVal, sets theVal = newVal and returns true.
 * Otherwise returns false.
 */
inline bool AtomicCompareAndSwap(int32_t oldVal, int32_t newVal, AtomicInt32* theVal) {
#ifdef __APPLE__
   return OSAtomicCompareAndSwap32Barrier(oldVal, newVal, theVal);
#elif defined(_WIN32) || defined(_WIN64)
   return InterlockedCompareExchange(theVal, newVal, oldVal) == oldVal;
#else
   return __sync_bool_compare_and_swap(theVal, oldVal, newVal);
#endif
}

inline int AtomicAdd(AtomicInt32* val, int32_t delta) {
#ifdef __APPLE__
   return OSAtomicAdd32Barrier(delta, val);
#elif defined(_WIN32) || defined(_WIN64)
   return InterlockedExchangeAdd (val, delta);
#else
   return __sync_fetch_and_and(val, delta);
#endif
}

inline float AtomicAdd(volatile float* val, float delta) {
   union bits { float f; int32_t i; };
   bits oldVal, newVal;

   do {
      oldVal.f = *val;
      newVal.f = oldVal.f + delta;
   } while(AtomicCompareAndSwap(oldVal.i, newVal.i, (AtomicInt32*) val) == false);

   return newVal.f;
}

#endif
