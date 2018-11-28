#pragma once

#include "summoner.hpp"
#include "creature.hpp"
#include "necromancy.hpp"

#include <string>

struct buff_library
{
	static void necromancer_buff(summoner<necromancy>& u);
	static void skeleton_buff(creature<necromancy>& skeleton);
};