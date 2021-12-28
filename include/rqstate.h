#ifndef RQSTATE_H
#define RQSTATE_H

#include <string>
#include <vector>
#include <map>
#include "gen/xrq_wrapper.h"

struct Obj {
	std::string name;
	std::string room;
	enum { PLAYER, NPC_, ITEM } type;
	enum { MAP, BATTLE } state;
};

struct Room {
	xrq_id_of_room joedb_ptr;
	std::string name;
	std::string info;
	std::vector<std::string> exits;
	std::unordered_map<std::string, Obj> objs;
};

struct Player : public Obj {
	xrq_id_of_player joedb_ptr;
	uint32_t hp;
	uint32_t sp;
//	TODO: add buffs
};

struct NPC : public Obj {
	xrq_id_of_npc joedb_ptr;
	uint32_t hp;
	std::string entry;
	uint32_t maxhp;
	std::vector<std::string> moves;
	std::unordered_map<std::string, uint32_t> drops;
};

struct Item : public Obj {
	std::string name;
	int32_t atk;
	uint32_t cost;
	int32_t hp;
	int32_t sp;
};

struct RQState {
	Player player;
	std::map<std::string, Room, std::less<>> rooms;
	std::map<std::string, NPC, std::less<>> npcs;
	std::map<std::string, Item, std::less<>> items;
	bool error;
	bool initialized;
	bool done;
	std::string error_string;
};

#endif
