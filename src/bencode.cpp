#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include "bencode.hpp"


std::string parse_next_string(std::string::iterator& cur){

	auto len_begin = cur;
    while (*cur != ':') {++cur;}
    int64_t length = std::stol(std::string{len_begin, cur});
    ++cur;
    auto str_begin = cur;
    cur += length;
    return std::string{str_begin, cur};
}


std::unique_ptr<ben_item> decode(std::string::iterator& cur){
	switch (fromChar(*cur)){
		case ben_type_begin::integer: {
			return parse<ben_int>(cur, process_int);
		}
		case ben_type_begin::dict: {
			return parse<ben_map>(cur, process_map);
		}
		case ben_type_begin::list: {
			return parse<ben_list>(cur, process_list);
		}
		case ben_type_begin::string: {
			return std::make_unique<ben_string>(parse_next_string(cur));
		}
		case ben_type_begin::UNKNOWN: {
			throw std::runtime_error("Parser Error");
		}
	}
}


template <typename Ben_Type, typename Parser>
std::unique_ptr<Ben_Type> parse(std::string::iterator& cur, Parser& p){
	auto res = p(cur);
	if (!res) {throw std::runtime_error("Parser Error");}
	return res;
}


std::unique_ptr<ben_int> process_int(std::string::iterator& cur){
	cur++;
	auto before = cur;
	while (*cur != ben_type_end){cur++;}
	auto b_int = std::make_unique<ben_int>(ben_int{std::stol(std::string{before, cur})});
	cur++;
	return b_int;
}


std::unique_ptr<ben_list> process_list(std::string::iterator& cur){
	cur++;
	auto list = std::make_unique<ben_list>(std::vector<std::unique_ptr<ben_item>>{});
	while (*cur != ben_type_end){
		auto item = decode(cur);
		list->val.push_back(std::move(item));
	}
	cur++;
	return list;
}


std::pair<std::string, std::unique_ptr<ben_item>> decode_pair(std::string::iterator& cur){
	std::string first = parse_next_string(cur);
	auto second = decode(cur);
	return {first, std::move(second)};
}


std::unique_ptr<ben_map> process_map(std::string::iterator& cur){
	cur++;
	auto map = std::make_unique<ben_map>(std::map<std::string, std::unique_ptr<ben_item>>{});
	while (*cur != ben_type_end){
		auto pair = decode_pair(cur);
		map->val[pair.first] = std::move(pair.second);
	}
	cur++;
	return map;
}


std::unique_ptr<ben_map> parse_file(std::string file){
	auto cur = file.begin();
	switch (fromChar(*cur)) {
		case ben_type_begin::dict: {
			return process_map(cur);
		}
		default:{
			std::cout << "No Dictionary at Top Level of File" << std::endl;
			throw std::runtime_error("Parser Error");
		}
	}
}


