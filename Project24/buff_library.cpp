#include "buff_library.hpp"

void buff_library::necromancer_buff(summoner<necromancy>& u)
{
	u.mana_ *= 2;
}

void buff_library::skeleton_buff(creature<necromancy>& skeleton)
{
	skeleton.attack_ *= 100;
}