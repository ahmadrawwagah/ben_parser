#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include "bencode.hpp"


std::string parse_next_string(std::string::iterator& cur){
	auto before = cur;
	auto llen = 0;
	while (*cur++ != ':'){llen++;}
	int64_t length = std::stol(std::string{before, cur});
	cur += length;
	return {before + llen + 1, cur};
}


std::unique_ptr<ben_item> decode(std::string::iterator& cur){
	switch (fromChar(*cur)){
		case ben_type_begin::integer: {
			return parse<ben_int>(++cur, process_int);
		}
		case ben_type_begin::dict: {
			return parse<ben_map>(++cur, process_map);
		}
		case ben_type_begin::list: {
			return parse<ben_list>(++cur, process_list);
		}
		case ben_type_begin::string: {
			return std::make_unique<ben_string>(parse_next_string(cur));
		}
		case ben_type_begin::UNKNOWN:
		throw std::runtime_error("Parser Error");
	}
	return nullptr;
}

template <typename Ben_Type, typename Parser>
std::unique_ptr<Ben_Type> parse(std::string::iterator& cur, Parser& p){
	auto res = p(cur);
	if (!res) {throw std::runtime_error("Parser Error");}
	return res;
}


std::unique_ptr<ben_int> process_int(std::string::iterator& cur){
	auto b_int = std::make_unique<ben_int>(0);
	auto before = cur;
	while (*++cur != ben_type_end){}
	b_int->val = std::stol(std::string{before, cur});
	cur++;
	return b_int;
}


std::unique_ptr<ben_list> process_list(std::string::iterator& cur){
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
			return process_map(++cur);
		}
		default:{
			std::cout << "what the heck" << std::endl;
			return nullptr;
		}
	}
	return nullptr;
}
