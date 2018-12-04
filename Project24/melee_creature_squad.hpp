#pragma once

#include <iostream>

#include "creature_squad.hpp"

template <class SchoolType>
class melee_creature_squad : public creature_squad<SchoolType>
{
public:
	explicit melee_creature_squad
	(
		const unsigned initiative,
		std::string way,
		unit<SchoolType>& unit_ref,
		const std::size_t size,
		std::string master_name = {}
	);
	melee_creature_squad(const melee_creature_squad&) = delete;
	melee_creature_squad(melee_creature_squad&&) = default;
	//void save(const std::string& filename) override;
};

template <class SchoolType>
melee_creature_squad<SchoolType>::melee_creature_squad
(
	const unsigned initiative,
	std::string way,
	unit<SchoolType>& unit_ref, 
	const std::size_t size,
	std::string master_name
) :
	creature_squad<SchoolType>
	(
		initiative,
		std::move(way),
		unit_ref,
		size,
		std::move(master_name)
	)
{}