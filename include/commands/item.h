#ifndef ITEM_COMMANDS_H
#define ITEM_COMMANDS_H

#include <string_view>
class RQState;
namespace xrq {
	class Generic_File_Database;
};

class rq_item {
public:
	enum item_cmd { GET, DROP, DESTROY };
	rq_item(xrq::Generic_File_Database& db_, item_cmd task_) : \
		db(db_), task(task_) {}
	int operator()(RQState& state, std::string_view input);
private:
	xrq::Generic_File_Database& db;
	item_cmd task;
};

int rq_inventory(RQState& state, std::string_view input);

#endif
