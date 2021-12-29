#include "include/data.h"
#include "include/util.h"
#include "include/gen/xrq.h"
#include "include/parse.h"
#include "include/commands/general.h"
#include "include/rqstate.h"
#include <iostream>

#define INV_SIZE 10

int main(int argc, char **argv) {
	xrq::File_Database db = xrq::File_Database("xrq.joedb");
	RQState state = state_init(db, "assets/map.json", INV_SIZE);
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
	auto generic_move = rq_move(db, rq_move::GENERIC);

	if (state.error) {
		std::cerr << state.error_string << std::endl;
		return -1;
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
	db.checkpoint_full_commit();

	return 0;
}
