#ifndef PARSE_H
#define PARSE_H

#include <functional>
#include <map>
#include <string>
#include <string_view>

template<typename T>
using cmdset = std::map<std::string, std::function<int(T&, std::string_view)>, std::less<>>;

template<typename T>
std::string prefix_search(const T& cmds, std::string_view input) {
	const auto it = cmds.lower_bound(input);
	if (it == cmds.cend()) return "";
	
	const std::string& key = it->first;
	if (key.compare(0, input.size(), input) == 0) { // Prefix test
		return key;
	}
	
	return "";
}

template<typename T>
int prefix_cmd(const cmdset<T>& cmds, T& object, std::string_view cmd, std::string_view arg) {
	auto key = prefix_search(cmds, cmd);
	
	if (key.empty()) return -1;
	
	return cmds.find(key)->second(object, arg);
}

#endif
