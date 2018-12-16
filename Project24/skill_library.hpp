#pragma once

#include "skill.hpp"

struct skill_library
{
	static skill* summon_tiger() { return new skill("Tiger summon", 1, 5, 12, squad_library::tiger_squad, "Images/tiger_squad.png", "animalism"); }
	static skill* summon_penguin() { return new skill("Penguin summon", 1, 5, 12, squad_library::penguin_squad, "Images/penguin_squad.png", "animalism"); }
	static skill* summon_demon() { return new skill("Demon summon", 1, 5, 12, squad_library::demon_squad, "Images/demon_squad.png", "demonology"); }
	static skill* summon_skeleton() { return new skill("Skeleton summon", 1, 5, 12, squad_library::skeleton_squad, "Images/skeleton_squad.png", "necromancy"); }
};