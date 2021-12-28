#ifndef RQSTATE_H
#define RQSTATE_H

#include "data.h"
#include "gen/xrq.h"

struct RQState {
	xrq::File_Database db;
	JsonData data;
	Player player;
	bool initialized;
	bool done;
};

#endif
