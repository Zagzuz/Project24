#pragma once

#include <map>
#include <string>
#include "squad.hpp"
#include "squad_library.hpp"

struct squad_saver
{
	std::map<std::string, squad*(*)(size_t, std::string)> creatures_map =
	{
		{"skeleton", squad_library::skeleton_squad},
		{"tiger", squad_library::tiger_squad},
		{"penguin", squad_library::penguin_squad},
		{"demon", squad_library::demon_squad}
	};
	std::map<std::string, squad*(*)()> summoners_map =
	{
		{"Agrail", squad_library::agrail_squad},
		{"Arantir", squad_library::arantir_squad}
	};
};
