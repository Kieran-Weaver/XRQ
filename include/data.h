#ifndef DATA_H
#define DATA_H

#include "../submodules/json_struct/include/json_struct.h"
#include <unordered_map>
#include <vector>

struct JsonNPC {
	std::string name;
	std::string room;
	std::string entry;
	uint32_t maxhp;
	std::vector<std::string> moves;
	std::unordered_map<std::string, uint32_t> drops;
	
	JS_OBJ(name, room, entry, maxhp, moves, drops);
};

struct JsonRoom {
	std::string name;
	std::string info;
	std::string spawner;
	std::vector<std::string> exits;

	JS_OBJ(name, spawner, info, exits);
};

struct JsonMove {
	std::string name;
	uint32_t base;
	uint32_t charge;
	uint32_t weight;
	
	JS_OBJ(name, base, charge, weight);
};

struct JsonItem {
	std::string name;
	int32_t atk;
	uint32_t cost;
	int32_t hp;
	int32_t sp;

	JS_OBJ(name, atk, cost, hp, sp);
};

struct JsonData {
	std::vector<JsonRoom> rooms;
	std::vector<JsonNPC> npcs;
	std::vector<JsonMove> moves;
	std::vector<JsonItem> items;
	
	JS_OBJ(rooms, npcs, moves, items);
};
#endif
