#pragma once

#include "melee_creature.hpp"
#include "unit _library.hpp"
#include "typed_skill.hpp"

struct skill_library
{
private:
	static creature<animalism>* summon_tiger_func() { return new melee_creature<animalism>(unit_library::tiger); }
	static creature<animalism>* summon_penguin_func() { return new melee_creature<animalism>(unit_library::penguin); }
	static creature<demonology>* summon_demon_func() { return new melee_creature<demonology>(unit_library::demon); }
	static creature<necromancy>* summon_skeleton_func() { return new melee_creature<necromancy>(unit_library::skeleton); }
public:
	static skill* summon_tiger() { return new typed_skill<animalism>("Tiger summon", summon_tiger_func, 1, 5, 12, "Images/summon_tiger.png"); }
	static skill* summon_penguin() { return new typed_skill<animalism>("Penguin summon", summon_penguin_func, 1, 5, 12, "Images/summon_penguin.png"); }
	static skill* summon_demon() { return new typed_skill<demonology>("Demon summon", summon_demon_func, 1, 5, 12, "Images/summon_demon.png"); }
	static skill* summon_skeleton() { return new typed_skill<necromancy>("Skeleton summon", summon_skeleton_func, 1, 5, 12, "Images/summon_skeleton.png"); }
};