// handle tests
// The MIT License (MIT)

// Copyright � 2018 ThePhD

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

#include <catch.hpp>

#include <phd/handle.hpp>

#include <iostream>

struct my_stateful_int_deleter {
	int the_null_value;

	my_stateful_int_deleter(int stateful_null_value = 24)
	: the_null_value(stateful_null_value) {
	}

	void write_null(int& h) const {
		h = the_null_value;
	}

	bool is_null(const int& h) const {
		return h == the_null_value;
	}

	void operator()(const int& h) const {
		if (h < the_null_value) {
			std::cout << "Less than the null value on deletion" << std::endl;
		}
		else {
			std::cout << "More than or equal to the null value on deletion" << std::endl;
		}
	}
};

TEST_CASE("handle/basic", "handle has basic fancy pointer amenities") {
	phd::handle<int> my_handle(24);
	REQUIRE(my_handle != nullptr);
	int released_handle = my_handle.release();
	REQUIRE(released_handle == 24);
	REQUIRE(my_handle == nullptr);
}

TEST_CASE("handle/stateful", "handle works with stateful deleters") {
	phd::handle<int, my_stateful_int_deleter> my_handle(1, 25);
	REQUIRE(my_handle != nullptr);
	REQUIRE(my_handle.get() == 1);
	my_handle.reset(50);
	REQUIRE(my_handle.get() == 50);
}
