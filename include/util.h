#ifndef UTIL_H
#define UTIL_H

#include <string_view>
#include <utility>
#include "data.h"

namespace xrq {
	class Generic_File_Database;
};


std::string readWholeFile(std::string_view filename);
JsonData db_init(xrq::Generic_File_Database& db, std::string_view filename, uint32_t item_size);
std::pair<std::string, std::string> split_cmd(const std::string& cmd);

#endif
