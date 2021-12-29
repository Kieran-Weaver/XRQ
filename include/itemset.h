#ifndef ITEMSET_H
#define ITEMSET_H

#include <map>
#include <string>
#include <string_view>

#define HIDDENFRIEND(X, RHS, OP, OP2) friend X OP(X lhs, RHS rhs) { \
		lhs OP2 rhs; \
		return lhs; \
	}

namespace xrq {
	class id_of_item;
	class id_of_player;
	class id_of_room;
	class Generic_File_Database;
};

using ISMap = std::map<std::string, unsigned int, std::less<>>;

struct ItemSet {
	ItemSet(unsigned int capacity__ = 0) : capacity_(capacity__) {}
	unsigned int size() const { return this->size_; }
	unsigned int capacity() const { return this->capacity_; }
	bool deserialize(xrq::Generic_File_Database& db, const xrq::id_of_item& item, size_t size);
	void serialize(xrq::Generic_File_Database& db, const xrq::id_of_item& item, size_t size) const;
	unsigned int operator[](std::string_view name) const;
	ItemSet& operator|=(const ItemSet& rhs);
	ItemSet& operator+=(const std::string& item);
	ItemSet& operator-=(std::string_view item);
	HIDDENFRIEND(ItemSet, const ItemSet&, operator|, |=)
	HIDDENFRIEND(ItemSet, const std::string&, operator+, +=)
	HIDDENFRIEND(ItemSet, std::string_view, operator-, -=)
	explicit operator bool() const { return this->error; }
	ISMap::const_iterator lower_bound(std::string_view item) const {
		return items.lower_bound(item);
	}
	ISMap::const_iterator cbegin() const {
		return items.cbegin();
	}
	ISMap::const_iterator cend() const {
		return items.cend();
	}
private:
	ISMap items = {};
	unsigned int size_ = 0;
	unsigned int capacity_ = 0;
	bool error = false;
};

#endif
