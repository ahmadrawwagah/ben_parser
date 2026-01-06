#pragma once
#include <string>
struct tracker {
	
	virtual ~tracker() = default;
    
    virtual std::string to_string() = 0;
};


template <typename Derived>
struct tracker_node : tracker {
	
	std::string to_string() final {
        return static_cast<const Derived*>(this)->to_string_impl();
    }
};
