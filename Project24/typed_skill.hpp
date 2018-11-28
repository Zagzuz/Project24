#pragma once

#include "unit.hpp"
#include "creature.hpp"
#include "skill.hpp"

template <class SchoolType>
class typed_skill : public skill
{
protected:
	using func = creature<SchoolType>*(*)();
	func f_;
public:
	typed_skill
	(
		std::string name,
		const func f,
		const ChType& required_level,
		const ChType& mana_cost,
		const ChType& number_coefficient,
		std::string path
	) :
		skill
		(
			std::move(name),
			required_level,
			mana_cost,
			number_coefficient,
			std::move(path)
		),
		f_(f)
	{}
};
