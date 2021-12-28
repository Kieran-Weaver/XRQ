#include "../include/util.h"
#include "../include/gen/xrq.h"
#include "../include/data.h"
#include "../submodules/json_struct/include/json_struct.h"
#include <fstream>

std::string readWholeFile(const char* filename) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	std::string contents;
	std::size_t size;

	in.seekg(0, std::ios::end);
	size = in.tellg();
	contents.resize(size);
	in.seekg(0, std::ios::beg);
	in.read(contents.data(), size);
	in.close();

	return contents;
}

JsonData db_init(xrq::Generic_File_Database& db, std::string_view filename, uint32_t item_size) {
	std::string json = readWholeFile("assets/map.json");
	JS::ParseContext context(json);
	JsonData data{};

	data.error = 0;
	if (context.parseTo(data) != JS::Error::NoError) {
		data.error_string = context.makeErrorString();
		data.error = 1;
		return data;
	}
	
	for (auto& room : data.rooms) {
		auto items = db.new_vector_of_item(item_size);
		room.joedb_ptr = db.new_room(room.name, items);
	}
	
	for (auto& npc : data.npcs) {
		npc.joedb_ptr = db.new_npc(npc.name, db.find_room_by_name(npc.room));
	}
	
	return data;
}
