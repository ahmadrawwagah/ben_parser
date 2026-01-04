static_assert(true);
#pragma pack(push, 1)
#include <cstdint>
#include <memory>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

enum ben_type_begin : char {
	dict = 'd',
	integer = 'i',
	list = 'l',
	string = 109,
	UNKNOWN = 110

};

inline ben_type_begin fromChar(char begin){
	switch (begin){
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
			break;	
		}
	}
	return (std::isdigit(begin) != 0) ? ben_type_begin::string : ben_type_begin::UNKNOWN;
}

const char ben_type_end = 'e';


struct ben_item {
    virtual ~ben_item() = default;
    
    virtual std::string to_string() = 0;
	
	virtual std::string to_ben_string() = 0;

}; 


struct ben_int : ben_item {
	int64_t val;
 	
	explicit ben_int(int64_t val): val(val) {}
 	
 	std::string to_string() override {
    	return std::to_string(val);
    }
	
	std::string to_ben_string() override{
		return std::format("i{}e", val);
	}
    
    ~ben_int() override = default;
};

struct ben_string : ben_item {
	std::string val;

	explicit ben_string(std::string val): val(std::move(val)) {}
	
	std::string to_string() override {
    	return val;
    }
	
	std::string to_ben_string() override{
		return std::format("{}:{}", val.length(), val);
	}
    
    ~ben_string() override = default;
};

struct ben_list : ben_item {
	std::vector<std::unique_ptr<ben_item>> val;
	
	explicit ben_list(std::vector<std::unique_ptr<ben_item>> val): val(std::move(val)) {}

	std::string to_string() override{
		std::string s;
		s += "[ ";
		for (const auto& val : val) {
        	s += val->to_string() + ", ";
    	}
    	s += "]\n";
    	return s;
	}
	
	std::string to_ben_string() override{
		std::string s;
		s += "l";
		for (const auto& val : val) {
        	s += val->to_ben_string();
    	}
    	s += "e";
    	return s;
	}
    
    ~ben_list() override = default;
};

struct ben_map : ben_item {
	std::unordered_map<std::string, std::unique_ptr<ben_item>> val;
	std::vector<std::string> orig_key_order;
	
	explicit ben_map(std::unordered_map<std::string, std::unique_ptr<ben_item>> val): val(std::move(val))  {}



	std::string to_string() override{
		std::string s;
		s += "{";
		for (const auto& key: orig_key_order){
			auto& value = val[key];
			s += std::format("Key: {} Value: {}\n", key, value->to_string());
		}
		s += "}";
		return s;
	}

	std::string to_ben_string() override{
		std::string s;
		s += "d";
		for (const auto& key: orig_key_order){
			auto& value = val[key];
			s += std::format("{}:{}{}", key.length(), key, value->to_ben_string());
		}
		s += "e";
		return s;
	}
    ~ben_map() override = default;

};

#pragma pack(pop)
