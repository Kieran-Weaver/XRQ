#ifndef PARSE_H
#define PARSE_H

#include <functional>
#include <map>
#include <string>
#include <string_view>

template<typename T>
using cmdset = std::map<std::string, std::function<int(T&, std::string_view)>, std::less<>>;

template<typename T>
int prefix_cmd(const cmdset<T>& cmds, T& object, std::string_view cmd, std::string_view arg) {
	auto it = cmds.lower_bound(cmd);
	if (it == cmds.end()) return -1;
	
	const std::string& key = it->first;
	if (key.compare(0, cmd.size(), cmd) == 0) { // Prefix test
		return it->second(object, arg);
	}
	
	return -1;
}

#endif
