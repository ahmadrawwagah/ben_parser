#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum ben_type_begin : char {
	dict = 'd',
	integer = 'i',
	list = 'l',
	STRING

};

inline ben_type_begin fromChar(char e){
	switch (e){
		case 'd':{
			return ben_type_begin::dict;
		}
		case 'i':{
			return ben_type_begin::integer;
		}
		case 'l':{
			return ben_type_begin::list;
		}
		default:{
			return ben_type_begin::STRING;
		}
	}
}

const char ben_type_end = 'e';


struct ben_item {
    virtual ~ben_item() = default;
    virtual std::string toString() {
        return "";
    }

};


struct ben_int : ben_item {
	ben_int(int64_t val): val(std::move(val)) {}
	int64_t val;
 	std::string toString() {
    	return std::to_string(val);
    }   
    virtual ~ben_int() = default;
};

struct ben_string : ben_item {
	std::string val;

	ben_string(std::string val): val(std::move(val)) {}
	
	std::string toString() {
    	return val;
    }
    virtual ~ben_string() = default;
};

struct ben_list : ben_item {
	std::vector<std::unique_ptr<ben_item>> val;
	ben_list(std::vector<std::unique_ptr<ben_item>> val): val(std::move(val)) {}

	std::string toString(){
		std::stringstream ss;
		ss << "[ ";
		for (const auto& val : val) {
        	ss << val->toString() << ", ";
    	}
    	ss << "]" << std::endl;
    	return ss.str();
	}

    virtual ~ben_list() = default;
};

struct ben_map : ben_item {
	std::unordered_map<std::string, std::unique_ptr<ben_item>> val;
	ben_map(std::unordered_map<std::string, std::unique_ptr<ben_item>> val): val(std::move(val)) {}

	std::string toString(){
		std::stringstream ss;
		ss << "{";
		for (const auto& pair: val){
			ss << "Key: " << pair.first << " Value: " << pair.second->toString() << std::endl;
		}
		ss << "}" << std::endl;
		return ss.str();
	}

    virtual ~ben_map() = default;

};






