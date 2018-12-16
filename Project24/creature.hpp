#pragma once

#include "unit.hpp"

template <class SchoolType>
class creature : public unit<SchoolType>
{
protected:
	ChType xp_for_the_kill_;
	ChType health_;
	ChType attack_;
	ChType defense_;
	using buff = void(*)(creature<SchoolType>&);
public:
	creature() {}
	creature
	(
		std::string name,
		const ChType& xp_for_the_kill,
		const ChType& health,
		const ChType& attack,
		const ChType& defense
	);
	creature(const creature&) = default;
	creature(creature&&) = default;
	creature& operator=(const creature& rhs);
	ChType get_health() const { return health_; }
	ChType get_attack() const { return attack_; }
	ChType get_defense() const { return defense_; }
	ChType get_xp() const { return xp_for_the_kill_; }
	void apply_buff(const std::any& b) override;
	/*virtual unit<SchoolType>* get_copy() override
	{
		return new creature<SchoolType>(*this);
	}*/

	friend struct buff_library;
};

template <class SchoolType>
creature<SchoolType>::creature
(
	std::string name, 
	const ChType& xp_for_the_kill, 
	const ChType& health,
	const ChType& attack, 
	const ChType& defense
) :
	unit<SchoolType>(std::move(name)),
	xp_for_the_kill_(xp_for_the_kill),
	health_(health),
	attack_(attack),
	defense_(defense)
{}

template <class SchoolType>
creature<SchoolType>& creature<SchoolType>::operator=(const creature& rhs)
{
	this->name_ = rhs.name_;
	this->health_ = rhs.health_;
	this->attack_ = rhs.attack_;
	this->defense_ = rhs.defense_;
	return *this;
}

template <class SchoolType>
void creature<SchoolType>::apply_buff(const std::any& b)
{
	std::any_cast<buff>(b)(*this);
}
