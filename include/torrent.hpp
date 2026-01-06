#include <exception>
#include <memory>
#include <iostream>
#include <cstdint>
#include <utility>
#include "bencode.hpp"

struct torrent {
	std::unique_ptr<ben_map> meta_info_;
	std::vector<std::string> pieces_; // maybe change piece to a struct containing downloaded amount, or make that a bitset somewhere else
	int64_t piece_length_{};
	std::string name_;
	int64_t length_{};

	explicit torrent(std::unique_ptr<ben_map> meta_info) { init(std::move(meta_info)); }
	explicit torrent(std::string file){
		try {
			std::unique_ptr<ben_map> ben_map = parse_file(std::move(file));
			init(std::move(ben_map));
		} catch (std::exception &e) {
			std::cout << "Caught Exception: " << e.what() << std::endl;
		
		}
	}
	
	std::string to_string(){
		return std::format("name: {}, size: {}, num_pieces: {}, piece_length: {}", name_, length_, pieces_.size(), piece_length_);
	}

	private:
	
	void init(std::unique_ptr<ben_map> meta_info){
		meta_info_ = std::move(meta_info);
		
		auto& info_map = meta_info_->get<ben_map>("info");
		name_ = info_map.get<ben_string>("name").val; 	
		length_ = info_map.get<ben_int>("length").val;	
		piece_length_ = info_map.get<ben_int>("piece length").val;	

		std::string pieces = info_map.get<ben_string>("pieces").val;
		auto it = pieces.begin();

		while (it != pieces.end())
		{
			pieces_.emplace_back(it, it + PIECE_HASH_LENGTH);
			it += PIECE_HASH_LENGTH;
		}
	}

};
