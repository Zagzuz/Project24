#pragma once

#include "melee_creature.hpp"
#include "demonology.hpp"
#include "animalism.hpp"
#include "summoner.hpp"

namespace unit_library
{
	// creatures
	inline melee_creature<necromancy> skeleton = { "skeleton", 1, 2, 3, 3 };
	inline melee_creature<demonology> demon = { "demon", 10, 20, 30, 5 };
	inline melee_creature<animalism> tiger = { "tiger", 5, 10, 20, 4 };
	inline melee_creature<animalism> penguin = { "penguin", 1, 1, 0, 0 };
	// summoners
	inline summoner<necromancy> arantir = { "Arantir", 200, 100, 15, { 2.0, 3.0, 4.0 } };
	inline summoner<demonology> agrail = { "Agrail", 150, 300, 20, { 0.0, 10.0, 2.0 } };
}