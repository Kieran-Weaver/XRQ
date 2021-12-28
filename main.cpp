#include "include/data.h"
#include "include/util.h"
#include "include/gen/xrq.h"
#include "include/parse.h"
#include "include/obj.h"
#define INITIAL_ITEM_SIZE 4
#include <iostream>

struct RQState {
	xrq::File_Database db;
	JsonData data;
	Player player;
	bool initialized;
	bool done;
};

int rq_start(RQState& state, std::string_view input) {
	if (input.empty() || (input.find(" ") != std::string_view::npos)) return -1;
	if (state.initialized) return 0;

	state.player = make_player(state.db, input, "Test Room 1");
	return 1;
}

int rq_exit(RQState& state, std::string_view input) {
	state.done = true;
	return 1;
}

int main(int argc, char **argv) {
	RQState state = {xrq::File_Database("xrq.joedb"), {}, {}, false, false};
	state.data = db_init(state.db, "assets/map.json", INITIAL_ITEM_SIZE);
	cmdset<RQState> uninit_commands = {};
	cmdset<RQState> map_commands = {};
	std::string command;
	std::string cmd, arg;
	
	uninit_commands["join"] = rq_start;
	uninit_commands["exit"] = rq_exit;
	map_commands["exit"] = rq_exit;

	if (state.data.error) {
		std::cerr << state.data.error_string << std::endl;
		return -1;
	}
	
	state.db.checkpoint_full_commit();
	
	while (!state.done) {
		std::cout << "> " << std::endl;
		std::getline(std::cin, command);

		auto first_space = command.find_first_of(' ');
		if (first_space == std::string::npos) {
			cmd = command;
			arg = "";
		} else {
			cmd = command.substr(0, first_space);
			arg = command.substr(first_space + 1);
		}
		
		cmdset<RQState>& commands = state.initialized ? map_commands : uninit_commands;
		int errcode = prefix_cmd(commands, state, cmd, arg);
			
		switch (errcode) {
			case -1:
				std::cout << "Invalid command: " << cmd << std::endl;
				break;
			case 0:
				std::cout << "Command failed" << std::endl;
				break;
			case 1:
				std::cout << "Success!" << std::endl;
				break;
			default:
				std::cout << "Unknown error" << std::endl;
				break;
		}
	}

	std::cout << "Exited successfully" << std::endl;
	state.db.checkpoint_full_commit();

	return 0;
}
