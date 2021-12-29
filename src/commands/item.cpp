#include "../../include/commands/item.h"
#include "../../include/gen/xrq.h"
#include "../../include/rqstate.h"
#include "../../include/parse.h"
#include <string>

int rq_item::operator()(RQState& state, std::string_view input) {
	auto& room = state.rooms[state.player.room];
	ItemSet* toSet = nullptr;
	ItemSet* fromSet = nullptr;

	switch (task) {
		case GET:
			toSet = &state.player.items;
			fromSet = &room.items;
			break;
		case DROP:
			fromSet = &state.player.items;
			toSet = &room.items;
			break;
		case DESTROY:
			fromSet = &state.player.items;
			break;
		default:
			return -1;
			break;
	}
	
	auto key = prefix_search(*fromSet, input);
	if (key.empty()) return 0;

	(*fromSet) -= key;
	if (static_cast<bool>(*fromSet)) return 0;

	if (toSet) {
		(*toSet) += key;
		if (static_cast<bool>(*toSet)) {
			(*fromSet) += key;
			return 0;
		} else {
			return 1;
		}
	}
	
	return 1;
}

int rq_inventory(RQState& state, std::string_view input) {
	auto& player = state.player;
	
	player.msgq.push_back("Items: ");
	for (auto it = player.items.cbegin(); it != player.items.cend(); it++) {
		player.msgq.push_back(it->first + " x " + std::to_string(it->second));
	}
	
	return 1;
}
