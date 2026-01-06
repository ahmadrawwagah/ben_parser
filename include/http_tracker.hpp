#include "tracker.hpp"

struct http_tracker : tracker_node<http_tracker> {
	

	[[nodiscard]] std::string to_string_impl() const {
    	return "";
    }
};
