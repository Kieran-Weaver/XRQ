#include "../include/itemset.h"
#include "../include/gen/xrq.h"

bool ItemSet::deserialize(xrq::Generic_File_Database& db, const xrq::id_of_item& item, size_t size) {
	this->items = {};
	for (auto i = 0u; i < size; i++) {
		auto name = db.get_name(item[i]);
		auto num = db.get_num(item[i]);
		if (num != 0) {
			this->items[name] = num;
		}
	}
	
	return !items.empty();
}

void ItemSet::serialize(xrq::Generic_File_Database& db, const xrq::id_of_item& item, size_t size) const {

	size_t i = 0;

	for (const auto& it : items) {
		if (db.get_name(item[i]) != it.first)
			db.set_name(item[i], it.first);
		if (db.get_num(item[i]) != static_cast<int>(it.second))
			db.set_num(item[i], it.second);
		i++;
	}

	for (; i < size; i++) {
		if (db.get_num(item[i]) != 0) {
			db.set_num(item[i], 0);
		}
	}
}

unsigned int ItemSet::operator[](std::string_view name) const {
	auto it = this->items.find(name);
	if (it != this->items.end()) {
		return it->second;
	} else {
		return 0;
	}
}

ItemSet& ItemSet::operator|=(const ItemSet& rhs) {
	if ((this->size_ + rhs.size_) <= (this->capacity_)) {
		for (const auto& item : rhs.items) {
			this->items[item.first] += item.second;
		}
		this->error = false;
	} else {
		this->error = true;
	}
	
	return *this;
}

ItemSet& ItemSet::operator+=(const std::string& item) {
	if (this->size_ < this->capacity_) {
		this->items[item]++;
		this->size_++;
		this->error = false;
	} else {
		this->error = true;
	}
	
	return *this;
}

ItemSet& ItemSet::operator-=(std::string_view item) {
	auto it = this->items.find(item);
	if (it != this->items.end()) {
		if ((--(this->items[it->first])) == 0) {
			this->items.erase(it->first);
		}
		this->size_--;
		this->error = false;
	} else {
		this->error = true;
	}
	
	return *this;
}
