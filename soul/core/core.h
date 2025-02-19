#pragma once

#include <atomic>
#include <type_traits>
#include <semaphore>

// Caught with -Wdangling 
// warning: temporary whose address is used as value of local variable ...
// string_view foo(const string& s LIFEIME_BOUND) ...
#if defined(__clang__)
#define LIFETIME_BOUND [[clang::lifetimebound]]
#else
#define LIFETIME_BOUND
#endif

#ifndef _ALWAYS_INLINE_
#define _ALWAYS_INLINE_ inline
#endif


template <class T>
class SafeNumeric {
	std::atomic<T> value;

	static_assert(std::atomic<T>::is_always_lock_free);

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		value.store(p_value, std::memory_order_release);
	}

	_ALWAYS_INLINE_ T get() const {
		return value.load(std::memory_order_acquire);
	}

	_ALWAYS_INLINE_ T increment() {
		return value.fetch_add(1, std::memory_order_acq_rel) + 1;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postincrement() {
		return value.fetch_add(1, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T decrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel) - 1;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postdecrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T add(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel) + p_value;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postadd(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T sub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel) - p_value;
	}

	_ALWAYS_INLINE_ T bit_or(T p_value) {
		return value.fetch_or(p_value, std::memory_order_acq_rel);
	}
	_ALWAYS_INLINE_ T bit_and(T p_value) {
		return value.fetch_and(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T bit_xor(T p_value) {
		return value.fetch_xor(p_value, std::memory_order_acq_rel);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postsub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ explicit SafeNumeric<T>(T p_value = static_cast<T>(0)) {
		set(p_value);
	}
};

class SafeFlag {
	std::atomic_bool flag;

	static_assert(std::atomic_bool::is_always_lock_free, "Atomic bool must be lock-free");

public:
	explicit SafeFlag(bool p_value = false) : flag(p_value) {}

	_ALWAYS_INLINE_ bool is_set() const {
		return flag.load(std::memory_order_acquire);
	}

	_ALWAYS_INLINE_ void set() {
		flag.store(true, std::memory_order_release);
	}

	_ALWAYS_INLINE_ void clear() {
		flag.store(false, std::memory_order_release);
	}

	_ALWAYS_INLINE_ void set_to(bool p_value) {
		flag.store(p_value, std::memory_order_release);
	}
};
