#include "include/data.h"
#include "include/util.h"
#include "include/gen/xrq.h"
#include "include/parse.h"
#include "include/commands/general.h"
#include "include/rqstate.h"
#include <iostream>

#define INV_SIZE 10

int main(int argc, char **argv) {
	RQState state = {xrq::File_Database("xrq.joedb"), {}, {}, false, false};
	state.data = db_init(state.db, "assets/map.json", INV_SIZE);
	cmdset<RQState> uninit_commands = {};
	cmdset<RQState> map_commands = {};
	std::string command;
	
	uninit_commands["join"] = rq_start("Test Room 1", INV_SIZE);
	uninit_commands["exit"] = rq_exit;
	map_commands["exit"] = rq_exit;

	if (state.data.error) {
		std::cerr << state.data.error_string << std::endl;
		return -1;
	}
	
	state.db.checkpoint_full_commit();
	
	while (!state.done) {
		std::cout << "> " << std::flush;
		std::getline(std::cin, command);

		const auto& [cmd, arg] = split_cmd(command);
		
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
