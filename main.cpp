#include "include/data.h"
#include "include/util.h"
#include "include/parse.h"
#include "include/gen/xrq.h"
#include "include/rqstate.h"
#include "include/commands.h"
#include <iostream>
#include <filesystem>

#define INV_SIZE 10

constexpr const char* db_file = "xrq.joedb";
namespace fs = std::filesystem;

int main(int argc, char **argv) {
	bool load = fs::exists(db_file);
	xrq::File_Database db = xrq::File_Database(db_file);
	RQState state = state_init(db, load, "assets/map.json", INV_SIZE);
	cmdset<RQState> uninit_commands = {};
	cmdset<RQState> map_commands = {};
	std::string command;
	
	uninit_commands["join"] = rq_start(db, "Test Room 1", INV_SIZE);
	uninit_commands["exit"] = rq_exit;
	map_commands["exit"] = rq_exit;
	map_commands["north"] = rq_move(db, rq_move::NORTH);
	map_commands["south"] = rq_move(db, rq_move::SOUTH);
	map_commands["east"] = rq_move(db, rq_move::EAST);
	map_commands["west"] = rq_move(db, rq_move::WEST);
	map_commands["get"] = rq_item(db, rq_item::GET);
	map_commands["drop"] = rq_item(db, rq_item::DROP);
	map_commands["destroy"] = rq_item(db, rq_item::DESTROY);
	map_commands["inventory"] = rq_inventory;
	auto generic_move = rq_move(db, rq_move::GENERIC);

	if (state.error) {
		std::cerr << state.error_string << std::endl;
		return -1;
	}

	if (load) {
		for (auto player : db.get_player_table()) {
			auto name = db.get_name(player);
			auto& splayer = state.player;
			splayer.joedb_ptr = player;
			splayer.name = name;
			splayer.room = db.get_name(db.get_room(player));
			splayer.hp = db.get_maxhp(player);
			splayer.sp = db.get_maxsp(player);
			state.player.items = ItemSet(INV_SIZE);
			state.player.items.deserialize(db, db.get_items(state.player.joedb_ptr), INV_SIZE);
			splayer.msgq = {};
		}
		state.initialized = true;
	}
	
	db.checkpoint_full_commit();
	
	while (!state.done) {
		for (auto& msg : state.player.msgq) {
			std::cout << msg << "\n";
		}
		state.player.msgq.clear();
		std::cout << "> " << std::flush;
		std::getline(std::cin, command);

		const auto& [cmd, arg] = split_cmd(command);
		
		cmdset<RQState>& commands = state.initialized ? map_commands : uninit_commands;
		int errcode = prefix_cmd(commands, state, cmd, arg);
		if (errcode == -1) {
			errcode = generic_move(state, command);
		}
		
		switch (errcode) {
			case -2: // No command
				break;
			case -1:
				state.player.msgq.push_back("Invalid command: " + cmd);
				break;
			case 0:
				state.player.msgq.push_back("Command failed");
				break;
			case 1: // Successful command
				break;
			default:
				state.player.msgq.push_back("Unknown error");
				break;
		}
	}

	std::cout << "Exited successfully" << std::endl;
	state.player.items.serialize(db, db.get_items(state.player.joedb_ptr), INV_SIZE);
	db.checkpoint_full_commit();

	return 0;
}
