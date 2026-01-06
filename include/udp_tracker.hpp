#include "tracker.hpp"

struct udp_tracker : tracker_node<udp_tracker> {


	[[nodiscard]] std::string to_string_impl() const {
    	return "";
    }


};
