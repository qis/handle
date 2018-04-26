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

#ifndef PHD_HANDLE_EBCO_HPP
#define PHD_HANDLE_EBCO_HPP

#include <phd/meta.hpp>

#include <utility>

namespace phd {

	template <typename T, typename = void>
	struct ebco {
		T value;

		ebco() = default;
		ebco(const ebco&) = default;
		ebco(ebco&&) = default;
		ebco& operator=(const ebco&) = default;
		ebco& operator=(ebco&&) = default;
		ebco(const T& v)
		: value(v){};
		ebco(T&& v)
		: value(std::move(v)){};
		ebco& operator=(const T& v) {
			value = v;
		}
		ebco& operator=(T&& v) {
			value = std::move(v);
		};
		template <typename Arg, typename... Args, typename = std::enable_if_t<!std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, ebco> && !std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, T>>>
		ebco(Arg&& arg, Args&&... args)
		: T(std::forward<Arg>(arg), std::forward<Args>(args)...){};

		T& get_value() {
			return value;
		}

		T const& get_value() const {
			return value;
		}
	};

	template <typename T>
	struct ebco<T, std::enable_if_t<std::is_class_v<T>>> : T {
		ebco() = default;
		ebco(const ebco&) = default;
		ebco(ebco&&) = default;
		ebco(const T& v)
		: T(v){};
		ebco(T&& v)
		: T(std::move(v)){};
		template <typename Arg, typename... Args, typename = std::enable_if_t<!std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, ebco> && !std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, T>>>
		ebco(Arg&& arg, Args&&... args)
		: T(std::forward<Arg>(arg), std::forward<Args>(args)...){};

		ebco& operator=(const ebco&) = default;
		ebco& operator=(ebco&&) = default;
		ebco& operator=(const T& v) {
			static_cast<T&>(*this) = v;
		}
		ebco& operator=(T&& v) {
			static_cast<T&>(*this) = std::move(v);
		};

		T& get_value() {
			return static_cast<T&>(*this);
		}

		T const& get_value() const {
			return static_cast<T const&>(*this);
		}
	};

} // namespace phd

#endif // PHD_HANDLE_EBCO_HPP
