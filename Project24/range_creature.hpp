#pragma once

#include "creature.hpp"
#include <sstream>

template <class SchoolType>
class range_creature : creature<SchoolType>
{
protected:
	ChType attack_range_;
public:
	range_creature() : creature<SchoolType>(), attack_range_(0) {}

	explicit range_creature
	(
		std::string name,
		const ChType& health,
		const ChType& attack,
		const ChType& defense,
		const ChType& attack_range
	) :
	creature<SchoolType>(std::move(name), health, attack, defense),
	attack_range_(attack_range)
	{}

	range_creature(const range_creature&) = default;
	range_creature(range_creature&&) = default;
	range_creature& operator=(const range_creature& rhs);

	std::string save_info() const override;
	void load_info(std::ifstream& file_stream) override;
};

template <class SchoolType>
range_creature<SchoolType>& range_creature<SchoolType>::operator=(const range_creature& rhs)
{
	this->name = rhs.name;
	this->health_ = rhs.health_;
	this->attack_ = rhs.attack_range_;
	this->defense_ = rhs.defense_;
	this->attack_range_ = rhs.attack_range_;
	return *this;
}

template <class SchoolType>
std::string range_creature<SchoolType>::save_info() const
{
	std::ostringstream oss;
	oss << "range_creature" << ' '
		<< this->name_ << ' '
		<< this->xp_for_the_kill_ << ' '
		<< this->health_ << ' '
		<< this->attack_ << ' '
		<< this->defense_ << ' '
		<< this->attack_range_ << ' ';
	return oss.str();
}

template <class SchoolType>
void range_creature<SchoolType>::load_info(std::ifstream& file_stream)
{
	file_stream >> this->name_;
	file_stream >> this->xp_for_the_kill_;
	file_stream >> this->health_;
	file_stream >> this->attack_;
	file_stream >> this->defense_;
	file_stream >> this->attack_range_;
}
