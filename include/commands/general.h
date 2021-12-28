#ifndef GENERAL_COMMANDS_H
#define GENERAL_COMMANDS_H

#include <string>
#include <string_view>

class RQState;

class rq_start {
public:
	rq_start(std::string_view room_, unsigned int inv_size_) : \
		room(room_), inv_size(inv_size_) {}
	int operator()(RQState& state, std::string_view input);
private:
	std::string room;
	unsigned int inv_size;
};

int rq_exit(RQState& state, std::string_view input);

#endif
