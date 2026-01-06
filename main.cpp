#include <iostream>
#include <sstream>
#include <fstream>
#include "torrent.hpp"
#include "udp_tracker.hpp"
#include "http_tracker.hpp"
#include "influx.hpp"

int main(){
	std::ifstream meta_file("test.torrent");
	std::stringstream buffer;
	buffer << meta_file.rdbuf();
	torrent torrent{buffer.str()};
	std::cout << torrent.to_string() << std::endl;
	return 0;
}

