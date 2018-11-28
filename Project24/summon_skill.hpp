#pragma once

#include "typed_skill.hpp"
#include "creature_squad.hpp"

template <template <class> class UnitType, class SchoolType>
class summon_skill : typed_skill<SchoolType>
{
public:
	summon_skill
	(
		std::string name,
		const ChType& required_level,
		const ChType& mana_cost
	) :
		typed_skill(std::move(name), required_level, mana_cost)
	{}

	//creature_squad<UnitType, SchoolType> operator() ()
	//{
	//	return creature_squad<UnitType, SchoolType>();
	//}
};
