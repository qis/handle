// handle
// The MIT License (MIT)

// Copyright © 2018 ThePhD

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#ifndef PHD_HANDLE_HANDLE_HPP
#define PHD_HANDLE_HANDLE_HPP

#include "phd/meta.hpp"
#include "phd/ebco.hpp"

#include <type_traits>
#include <memory>

namespace phd {

	template <typename T>
	struct default_handle_deleter {
		using pointer = T;

		static void write_null(pointer& p) {
			p = T();
		}

		static bool is_null(const pointer& p) {
			return p == T();
		}

		void operator()(const pointer&) const {
			// no-op: override the default deleter
			// for handle-specific behavior
		}
	};

	template <typename T>
	struct default_handle_deleter<T*> : std::default_delete<T> {
		using pointer = T*;

		static void write_null(pointer& p) {
			p = T();
		}

		static bool is_null(const pointer& p) {
			return p == T();
		}
	};

	template <typename T,
		typename Dx = default_handle_deleter<T>>
	struct handle : ebco<Dx> {
	public:
		typedef pointer_type_t<T, Dx> pointer;
		typedef Dx deleter_type;

	private:
		typedef ebco<deleter_type> deleter_base;

		pointer res;

	public:
		handle() {
		}

		handle(pointer h)
		: res(h) {
		}

		handle(std::nullptr_t) {
			deleter_type& deleter = get_deleter();
			deleter.write_null(res);
		}

		handle(pointer h, deleter_type d)
		: deleter_base(std::move(d)), res(h) {
		}

		handle(std::nullptr_t, deleter_type d)
		: deleter_base(std::move(d)) {
			deleter_type& deleter = get_deleter();
			deleter.write_null(res);
		}

		template <typename... DxArgs>
		handle(pointer h, DxArgs&&... dx_args)
		: deleter_base(std::forward<DxArgs>(dx_args)...), res(h) {
		}

		template <typename... DxArgs>
		handle(std::nullptr_t, DxArgs&&... dx_args)
		: deleter_base(std::forward<Dx>(dx)...) {
			deleter_type& deleter = get_deleter();
			deleter.write_null(res);
		}

		handle(const handle& nocopy) = delete;
		handle(handle&& mov)
		: deleter_base(std::move(mov)), res(std::move(mov.res)) {
			deleter_type& deleter = get_deleter();
			mov.reset(mov.get_null());
		}

		handle& operator=(const handle&) = delete;
		handle& operator=(handle&& right) {
			reset(right.release());
			return *this;
		}
		handle& operator=(pointer right) {
			reset(right);
			return *this;
		}
		handle& operator=(std::nullptr_t) {
			reset(nullptr);
			return *this;
		}

		explicit operator bool() const {
			return is_null();
		}

		pointer get_null() const {
			pointer p;
			const deleter_type& deleter = get_deleter();
			deleter.write_null(p);
			return p;
		}

		bool is_null() const {
			const deleter_type& deleter = get_deleter();
			return deleter.is_null(res);
		}

		pointer get() const {
			return res;
		}

		void reset(pointer h) {
			deleter_type& deleter = get_deleter();
			if (!is_null())
				deleter(res);
			res = h;
		}

		void reset(std::nullptr_t) {
			deleter_type& deleter = get_deleter();
			if (!is_null())
				deleter(res);
			deleter.write_null(res);
		}

		pointer release() {
			pointer rel = std::move(res);
			deleter_type& deleter = get_deleter();
			deleter.write_null(res);
			return rel;
		}

		void swap(handle& other) {
			std::swap(other.get_deleter(), get_deleter());
			std::swap(other.res, res);
		}

		deleter_type& get_deleter() {
			deleter_type& deleter = deleter_base::get_value();
			return deleter;
		}

		const deleter_type& get_deleter() const {
			const deleter_type& deleter = deleter_base::get_value();
			return deleter;
		}

		pointer operator*() {
			return get();
		}

		pointer operator->() {
			return get();
		}

		~handle() {
			const deleter_type& deleter = get_deleter();
			if (!is_null())
				deleter(res);
			deleter.write_null(res);
		}
	};

	template <typename T, typename Dx>
	inline bool operator==(const handle<T, Dx>& left, const handle<T, Dx>& right) {
		return left.get() == right.get();
	}
	template <typename T, typename Dx>
	inline bool operator==(std::nullptr_t, const handle<T, Dx>& left) {
		return left.is_null();
	}
	template <typename T, typename Dx>
	inline bool operator==(const handle<T, Dx>& left, std::nullptr_t) {
		return left.is_null();
	}
	template <typename T, typename Dx>
	inline bool operator==(typename handle<T, Dx>::pointer right, const handle<T, Dx>& left) {
		return left.get() == right;
	}
	template <typename T, typename Dx>
	inline bool operator==(const handle<T, Dx>& left, typename handle<T, Dx>::pointer right) {
		return left.get() == right;
	}


	template <typename T, typename Dx>
	inline bool operator!=(const handle<T, Dx>& left, const handle<T, Dx>& right) {
		return left.get() != right.get();
	}
	template <typename T, typename Dx>
	inline bool operator!=(std::nullptr_t right, const handle<T, Dx>& left) {
		return left.get() != left.get_null();
	}
	template <typename T, typename Dx>
	inline bool operator!=(const handle<T, Dx>& left, std::nullptr_t right) {
		return left.get() != left.get_null();
	}
	template <typename T, typename Dx>
	inline bool operator!=(typename handle<T, Dx>::pointer right, const handle<T, Dx>& left) {
		return left.get() != right;
	}
	template <typename T, typename Dx>
	inline bool operator!=(const handle<T, Dx>& left, typename handle<T, Dx>::pointer right) {
		return left.get() != right;
	}
} // namespace phd

#endif // PHD_HANDLE_HANDLE_HPP
