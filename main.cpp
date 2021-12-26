#include "include/data.h"
#include "include/gen/xrq.h"
#include <iostream>
#include <fstream>
#define INITIAL_ITEM_SIZE 4

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

int main(int argc, char **argv) {
	xrq::File_Database db("xrq.joedb");
	std::string json = readWholeFile("assets/map.json");
	JS::ParseContext context(json);
	
	JsonData data;
	if (context.parseTo(data) != JS::Error::NoError) {
		std::cerr << context.makeErrorString() << std::endl;
		return -1;
	}
	
	for (auto& room : data.rooms) {
		auto items = db.new_vector_of_item(INITIAL_ITEM_SIZE);
		room.joedb_ptr = db.new_room(room.name, items);
		std::cout << "name: " << room.name << std::endl;
		std::cout << "info: " << room.info << std::endl;
		std::cout << "exits: " << std::endl;
		for (const auto& exit: room.exits) {
			std::cout << exit << std::endl;
		}
	}
	
	for (auto& npc : data.npcs) {
		npc.joedb_ptr = db.new_npc(npc.name, db.find_room_by_name(npc.room));
	}
	
	db.checkpoint_full_commit();
	return 0;
}
