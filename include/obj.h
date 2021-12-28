#ifndef OBJ_H
#define OBJ_H

#include "data.h"
#include <string_view>

namespace xrq {
	class Generic_File_Database;
};

Player make_player(xrq::Generic_File_Database& db, std::string_view name, std::string_view room);

#endif
