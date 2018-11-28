#pragma once

#include <string>
#include <any>

using ChType = double;

template <class SchoolType>
class unit
{
protected:
	std::string name_;
public:
	unit() : name_("The Chosen One") {}
 	explicit unit(std::string name) : name_(std::move(name)) {}
	std::string get_name() const { return name_; }
	virtual void apply_buff(const std::any&) = 0;
	virtual unit<SchoolType>* get_copy() = 0;
};