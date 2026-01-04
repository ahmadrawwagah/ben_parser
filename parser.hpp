#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include "bencode.hpp"

std::unique_ptr<ben_list> process_list(std::string::iterator& cur);
std::unique_ptr<ben_int> process_int(std::string::iterator& cur);
std::unique_ptr<ben_map> process_map(std::string::iterator& cur);
std::unique_ptr<ben_item> decode(std::string::iterator& cur);
std::pair<std::string, std::unique_ptr<ben_item>> decode_pair(std::string::iterator& cur);
std::string parse_next_string(std::string::iterator& cur);
std::unique_ptr<ben_map> parse_file(std::string file);














