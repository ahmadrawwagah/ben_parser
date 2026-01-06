static_assert(true);
#pragma pack(push, 1)
#include <cstdint>
#include <memory>
#include <format>
#include <string>
#include <map>
#include <vector>

const int PIECE_HASH_LENGTH = 20;

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

template <typename Derived>
struct ben_node : ben_item {
	 std::string to_string() final {
        return static_cast<const Derived*>(this)->to_string_impl();
    }

    std::string to_ben_string() final {
        return static_cast<const Derived*>(this)->to_ben_string_impl();
    }
};

template <typename T>
T& expect(ben_item& item) {
	if (auto* p = dynamic_cast<T*>(&item)) {
		return *p;
	}
	throw std::runtime_error("Type Mismatch");
}
//this is awful but i cant think of another way. i wish torrent meta files were stronger typed

struct ben_int : ben_node<ben_int> {
	int64_t val;
 	
	explicit ben_int(int64_t val): val(val) {}
 	
 	[[nodiscard]] std::string to_string_impl() const {
    	return std::to_string(val);
    }
	
	[[nodiscard]] std::string to_ben_string_impl() const {
		return std::format("i{}e", val);
	}
    
};

struct  ben_string : ben_node<ben_string> {
	std::string val;

	explicit ben_string(std::string val): val(std::move(val)) {}
	
	[[nodiscard]] std::string to_string_impl() const {
    	return val;
    }
	
	[[nodiscard]] std::string to_ben_string_impl() const{
		return std::format("{}:{}", val.length(), val);
	}
    
};

struct  ben_list : ben_node<ben_list> {
	std::vector<std::unique_ptr<ben_item>> val;
	
	explicit ben_list(std::vector<std::unique_ptr<ben_item>> val): val(std::move(val)) {}

	[[nodiscard]] std::string to_string_impl() const{
		std::string s;
		s += "[ ";
		for (const auto& val : val) {
        	s += val->to_string() + ", ";
    	}
    	s += "]\n";
    	return s;
	}
	
	[[nodiscard]] std::string to_ben_string_impl() const{
		std::string s;
		s += "l";
		for (const auto& val : val) {
        	s += val->to_ben_string();
    	}
    	s += "e";
    	return s;
	}
    
};

struct  ben_map : ben_node<ben_map> {
	std::map<std::string, std::unique_ptr<ben_item>> val;
	
	explicit ben_map(std::map<std::string, std::unique_ptr<ben_item>> val): val(std::move(val))  {}



	[[nodiscard]] std::string to_string_impl() const{
		std::string s;
		s += "{";
		for (const auto& pair: val){
			s += std::format("Key: {} Value: {}\n", pair.first, pair.second->to_string());
		}
		s += "}";
		return s;
	}

	[[nodiscard]] std::string to_ben_string_impl() const{
		std::string s;
		s += "d";
		for (const auto& pair: val){
			s += std::format("{}:{}{}", pair.first.length(), pair.first, pair.second->to_ben_string());
		}
		s += "e";
		return s;
	}


	template <typename T>
	T& get(const std::string& key) {
		return expect<T>(*val.at(key));
	}

};
#pragma pack(pop)

std::unique_ptr<ben_list> process_list(std::string::iterator& cur);
std::unique_ptr<ben_int> process_int(std::string::iterator& cur);
std::unique_ptr<ben_map> process_map(std::string::iterator& cur);
std::unique_ptr<ben_item> decode(std::string::iterator& cur);
std::pair<std::string, std::unique_ptr<ben_item>> decode_pair(std::string::iterator& cur);
std::string parse_next_string(std::string::iterator& cur);
std::unique_ptr<ben_map> parse_file(std::string file);

template <typename Ben_Type, typename Parser>
std::unique_ptr<Ben_Type> parse(std::string::iterator& cur, Parser& p);

