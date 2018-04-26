# handle v1.0

ᴮᵉᶜᵃᵘˢᵉ ˢᵒᵐᵉᵇᵒᵈʸ ˢᵃᶦᵈ ᵗʰᶦˢ ʷᵒᵘᶫᵈ ᵇᵉ ᵘˢᵉᶠᵘᶫ﹗

This library is for allowing a person to easily use `handle` types without the assumption that the stored type is a pointer. It extends the concept of a regular deleter. A basic example:

````cpp
#include <phd/handle.hpp>

int main (int, char*[]) {
	phd::handle<int> my_handle(24);
	assert(my_handle != nullptr);
	int released_handle = my_handle.release();
	assert(released_handle == 24);
	assert(my_handle == nullptr);
}
````

It extends beyond that: it allows for a deleter, similar to `std::unique_pointer`:

````cpp
#include <phd/handle.hpp>

struct my_stateful_int_deleter {
	int the_null_value;

	my_stateful_int_deleter(int stateful_null_value = 24) : the_null_value(stateful_null_value) {}

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
		// nothing
	}
};

int main (int, char*[]) {
	phd::handle<int, my_stateful_int_deleter> my_handle(1, 24);
	assert(my_handle != nullptr);
	assert(my_handle.get() == 1);
	my_handle.reset(50);
	assert(my_handle.get() == 50);
}
````

## Build

There is no build to just use the library! This is a header-only, linker-hassle-free™ library. Enjoy! 🎉