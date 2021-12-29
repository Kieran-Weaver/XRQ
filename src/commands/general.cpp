#include "../../include/commands/general.h"
#include "../../include/gen/xrq.h"
#include "../../include/rqstate.h"

int rq_start::operator()(RQState& state, std::string_view input) {
	if (input.empty() || (input.find_first_of(' ') != std::string_view::npos)) return -1;
	if (state.initialized) return 0;

	state.player = {};
	state.initialized = true;
	state.player.name = input;
	state.player.room = this->room;
	auto items = db.new_vector_of_item(this->inv_size);
	auto room_ref = db.find_room_by_name(this->room);

	state.player.joedb_ptr = this->db.new_player(
		state.player.name, // name
		items, // items
		room_ref, // room
		0,  // sequence
		10, // maxhp
		10, // maxsp
		1,  // atk
		0,  // xp
		0,  // level
		0   // money
	);
	
	state.player.hp = 10;
	state.player.sp = 10;

	state.rooms[state.player.room].objs[state.player.name] = {
		state.player.name,
		Obj::PLAYER,
		Obj::MAP
	};

	state.player.msgq.push_back("Welcome to XRQ!");
	state.player.msgq.push_back(state.rooms[state.player.room].info);
	return 1;
}

int rq_exit(RQState& state, std::string_view input) {
	state.done = true;
	(void)input;
	
	return 1;
}

int rq_move::operator()(RQState& state, std::string_view input) {
	auto& room = state.rooms[state.player.room];
	auto  obj = room.objs[state.player.name];

	std::string dest = "";
	
	switch (this->move) {
	case NORTH:
	case SOUTH:
	case EAST:
	case WEST:
		dest = room.exits[this->move];
		break;
	case GENERIC:
		if (input.empty()) return -2;
		for (const auto& name : room.exits) {
			if (name.compare(0, input.size(), input) == 0) {
				dest = name;
				break;
			}
		}
		break;
	default:
		return -1;
		break;
	}
	
	if (dest.empty()) return -1;
	if (dest == "null") return 0;
	
	room.objs.erase(state.player.name);
	state.player.room = dest;
	
	auto& new_room = state.rooms[state.player.room];
	new_room.objs[state.player.name] = obj;
	db.set_room(state.player.joedb_ptr, db.find_room_by_name(dest));
	state.player.msgq.push_back(new_room.info);
	
	return 1;
}
