#include "../include/obj.h"
#include "../include/gen/xrq.h"
#define INV_SIZE 10

Player make_player(xrq::Generic_File_Database& db, std::string_view name, std::string_view room) {
	Player player = {};
	
	player.name = name;
	player.room = room;
	auto items = db.new_vector_of_item(INV_SIZE);
	auto room_ref = db.find_room_by_name(player.room);

	player.joedb_ptr = db.new_player(
		player.name,
		items,
		room_ref,
		0,
		10,
		10,
		1,
		0,
		0,
		0
	);
	
	player.hp = 10;
	player.sp = 10;
	player.type = Obj::PLAYER;
	player.state = Obj::MAP;
	
	return player;
}
