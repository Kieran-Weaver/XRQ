#ifndef DATA_H
#define DATA_H
#include "gen/xrq_wrapper.h"
#include "../submodules/json_struct/include/json_struct.h"

struct Obj {
	std::string name;
	enum {
		PLAYER,
		NPC
	} type;
	enum {
		MAP,
		BATTLE
	} state;
};

using Exit = std::optional<std::string>;

struct Player : public Obj {
	xrq_id_of_player joedb_ptr;
	uint32_t hp;
	uint32_t sp;
//	buffs
};

struct NPC: public Obj {
	xrq_id_of_npc joedb_ptr;
	uint32_t hp;
	std::string entry;
	uint32_t maxhp;
	std::vector<std::string> moves;
	std::map<std::string, uint32_t> drops;

	JS_OBJ(name, entry, maxhp, moves, drops);
};

struct Room {
	xrq_id_of_room joedb_ptr;
	std::string name;
	std::string info;
	std::vector<Exit> exits;
	std::map<std::string, Obj> objs;
	
	JS_OBJ(name, info, exits);
};

struct Move {
	std::string name;
	uint32_t base;
	uint32_t charge;
	uint32_t weight;
	
	JS_OBJ(name, base, charge, weight);
};

struct Item {
	std::string name;
	int32_t atk;
	uint32_t cost;
	int32_t hp;
	int32_t sp;

	JS_OBJ(name, atk, cost, hp, sp);
};

#endif
