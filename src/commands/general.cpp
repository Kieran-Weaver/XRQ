#include "../../include/commands/general.h"
#include "../../include/gen/xrq.h"
#include "../../include/rqstate.h"

int rq_start::operator()(RQState& state, std::string_view input) {
	if (input.empty() || (input.find_first_of(' ') != std::string_view::npos)) return -1;
	if (state.initialized) return 0;

	state.player = {};

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
	state.player.type = Obj::PLAYER;
	state.player.state = Obj::MAP;

	return 1;
}

int rq_exit(RQState& state, std::string_view input) {
	state.done = true;
	(void)input;
	
	return 1;
}
