#include <memory>
#include <utility>
#include "bencode.hpp"

struct torrent {
	std::unique_ptr<ben_map> meta_info;

	explicit torrent(std::unique_ptr<ben_map> meta_info): meta_info(std::move(meta_info)) {}
	explicit torrent(std::string file){
		std::unique_ptr<ben_map> ben_map = parse_file(std::move(file));
		if (ben_map != nullptr) {
			meta_info = std::move(ben_map);
		} //throw or somethin 
	}
};
