#include "include/data.h"
#include "include/util.h"
#include "include/gen/xrq.h"
#define INITIAL_ITEM_SIZE 4
#include <iostream>

int main(int argc, char **argv) {
	xrq::File_Database db("xrq.joedb");
	JsonData data = db_init(db, "assets/map.json", INITIAL_ITEM_SIZE);

	if (data.error) {
		std::cerr << data.error_string << std::endl;
		return -1;
	}
	
	db.checkpoint_full_commit();
	return 0;
}
