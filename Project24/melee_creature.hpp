#pragma once

#include "creature.hpp"
#include <type_traits>

template <class SchoolType>
class melee_creature : public creature<SchoolType>
{
protected:
	using buff = void(*)(melee_creature<SchoolType>&);
public:
	melee_creature
	(
		std::string name,
		const ChType& xp_for_the_kill,
		const ChType& health,
		const ChType& attack,
		const ChType& defense
	) :
		creature<SchoolType>(
		std::move(name), 
		xp_for_the_kill, 
		health, attack,
		defense)
	{}

	void apply_buff(const std::any& b) override
	{
		std::any_cast<buff>(b)(*this);
	}
	unit<SchoolType>* get_copy() override
	{
		return new melee_creature<SchoolType>(*this);
	}

	friend struct buff_library;
};
