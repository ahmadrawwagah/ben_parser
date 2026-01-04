#include <iostream>
#include <sstream>
#include <fstream>
#include "torrent.hpp"

int main(){
	std::ifstream meta_file("test.torrent");
	std::stringstream buffer;
	buffer << meta_file.rdbuf();
	torrent torrent{buffer.str()};
	std::cout << torrent.meta_info->to_string() << std::endl;
	return 1;
}

