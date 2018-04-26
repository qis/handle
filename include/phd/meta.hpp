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

#ifndef PHD_HANDLE_META_HPP
#define PHD_HANDLE_META_HPP

#include <type_traits>

namespace phd {

	template <typename T>
	struct remove_type {
		typedef void type;
	};

	template <typename T>
	struct remove_type<T*> {
		typedef typename remove_type<T>::type* type;
	};

	namespace detail {
		template <typename T>
		struct has_typedef_pointer {
			typedef char yes[1];
			typedef char no[2];

			template <typename C>
			static yes& test(typename C::pointer*);

			template <typename>
			static no& test(...);

			static const bool value = sizeof(test<T>(0)) == sizeof(yes);
		};

		template <bool b, typename T, typename Fallback>
		struct pointer_typedef_enable_if {};

		template <typename T, typename Fallback>
		struct pointer_typedef_enable_if<false, T, Fallback> {
			typedef Fallback type;
		};

		template <typename T, typename Fallback>
		struct pointer_typedef_enable_if<true, T, Fallback> {
			typedef typename T::pointer type;
		};
	} // namespace detail

	template <typename T, typename Dx>
	struct pointer_type {
		typedef typename detail::pointer_typedef_enable_if<detail::has_typedef_pointer<Dx>::value, Dx, T>::type type;
	};

	template <typename T, typename D>
	using pointer_type_t = typename pointer_type<T, D>::type;
} // namespace phd

#endif // PHD_HANDLE_META_HPP
