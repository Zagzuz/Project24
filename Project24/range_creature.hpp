#pragma once

#include "creature.hpp"

template <class SchoolType>
class range_creature : creature<SchoolType>
{
protected:
	ChType attack_range_;
public:
	range_creature() : 
		creature<SchoolType>(),
		attack_range_(0)
	{}

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
