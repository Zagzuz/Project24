#pragma once
#include "squad.hpp"
#include "melee_creature_squad.hpp"
#include "unit _library.hpp"

namespace squad_library
{
	// creatures
	inline squad* tiger_squad(const std::size_t size) { return new melee_creature_squad<animalism>(1, "Images/tiger_squad.png", unit_library::tiger, size); }
	inline squad* penguin_squad(const std::size_t size) { return new melee_creature_squad<animalism>(1, "Images/penguin_squad.png", unit_library::penguin, size); }
	inline squad* demon_squad(const std::size_t size) { return new melee_creature_squad<demonology>(1, "Images/demon_squad.png", unit_library::demon, size); }
	inline squad* skeleton_squad(const std::size_t size) { return new melee_creature_squad<necromancy>(1, "Images/skeleton_squad.png", unit_library::skeleton, size); }
	// summoners
	inline squad* arantir_squad() { return new summoner_squad<necromancy>(3, "Images/arantir.png", unit_library::arantir); }
	inline squad* agrail_squad() { return new summoner_squad<demonology>(4, "Images/agrail.png", unit_library::agrail); }
}
