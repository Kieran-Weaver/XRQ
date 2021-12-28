#ifndef UTIL_H
#define UTIL_H

#include <string_view>
#include <utility>

namespace xrq {
	class Generic_File_Database;
};

class RQState;

std::string readWholeFile(std::string_view filename);
RQState state_init(xrq::Generic_File_Database& db, std::string_view filename, uint32_t item_size);
std::pair<std::string, std::string> split_cmd(const std::string& cmd);

#endif
