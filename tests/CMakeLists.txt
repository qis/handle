# # handle tests
# The MIT License (MIT)
#
# Copyright � 2018 ThePhD
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


find_package(Catch REQUIRED)

set(handle_test_sources
	"source/handle_tests.cpp"
	"source/handle_tests_main.cpp"
)

prepend(prefixed_handle_test_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${handle_test_sources})

add_executable(handle_test ${prefixed_handle_test_sources})
if (MSVC)
	target_compile_options(handle_test
		PRIVATE /std:c++latest)
else()
	target_compile_options(handle_test
		PRIVATE -std=c++1z)
endif()
target_include_directories(handle_test
	PRIVATE "include")
target_link_libraries(handle_test 
	PRIVATE phd::handle ${Catch_Libraries} ${CMAKE_DL_LIBS}
)
add_test(NAME handle_test COMMAND handle_test)
