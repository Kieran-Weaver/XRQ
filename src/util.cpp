#include "../include/util.h"
#include "../include/gen/xrq.h"
#include "../include/data.h"
#include "../include/rqstate.h"
#include "../submodules/json_struct/include/json_struct.h"
#include <fstream>

std::string readWholeFile(std::string_view filename) {
	std::ifstream in(filename.data(), std::ios::in | std::ios::binary);
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

RQState state_init(xrq::Generic_File_Database& db, bool load, \
	std::string_view filename, uint32_t item_size) {
	
	std::string json = readWholeFile(filename);
	JS::ParseContext context(json);
	JsonData data{};
	RQState state{};

	state.error = false;
	if (context.parseTo(data) != JS::Error::NoError) {
		state.error_string = context.makeErrorString();
		state.error = true;
		return state;
	}
	
	for (auto& jroom : data.rooms) {
		auto& room = state.rooms[jroom.name];
		room.name = jroom.name;
		room.info = jroom.info;
		room.exits = jroom.exits;
		room.objs = {};

		if (load) {
			room.joedb_ptr = db.find_room_by_name(jroom.name);
		} else {
			auto items = db.new_vector_of_item(item_size);
			room.joedb_ptr = db.new_room(jroom.name, items);
		}
	}
	
	for (auto& jnpc : data.npcs) {
		auto& npc = state.npcs[jnpc.name];
		npc.name = jnpc.name;
		npc.room = jnpc.room;
		npc.maxhp = jnpc.maxhp;
		npc.hp = npc.maxhp;
		npc.moves = jnpc.moves;
		npc.drops = jnpc.drops;
		state.rooms[npc.room].objs[npc.name] = {
			npc.name,
			Obj::NPC_,
			Obj::MAP
		};
		
		if (load) {
			npc.joedb_ptr = db.find_npc_by_name(jnpc.name);
		} else {
			npc.joedb_ptr = db.new_npc(jnpc.name, db.find_room_by_name(jnpc.room));
		}
	}
	
	for (auto& jitem : data.items) {
		auto& item = state.items[jitem.name];
		item.name = jitem.name;
		item.cost = jitem.cost;
		item.hp = jitem.hp;
		item.sp = jitem.sp;
	}
	
	state.initialized = false;
	state.done = false;
	state.player = {};
	
	return state;
}

std::pair<std::string, std::string> split_cmd(const std::string& cmd) {
	std::pair<std::string, std::string> parts;

	auto first_space = cmd.find_first_of(' ');
	if (first_space == std::string::npos) {
		parts.first = cmd;
		parts.second = "";
	} else {
		parts.first = cmd.substr(0, first_space);
		parts.second = cmd.substr(first_space + 1);
	}
	
	return parts;
}
