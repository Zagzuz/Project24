#include "school_of_magic.hpp"
#include "creature.hpp"
#include "necromancy.hpp"
#include "creature_squad.hpp"
#include "summoner.hpp"
#include "buff_library.hpp"
#include <string>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "animalism.hpp"
#include "summoner_squad.hpp"
#include "cell.hpp"
#include "gamemap.hpp"
#include "melee_creature.hpp"
#include "melee_creature_squad.hpp"
#include "unit _library.hpp"
#include "level.hpp"
#include "skill_library.hpp"
#include "squad_library.hpp"

constexpr pixels resolution_x = 1344, resolution_y = 756,
	aspect_ratio_x = 16, aspect_ratio_y = 9;

int main()
{
	sf::RenderWindow window
	(
		sf::VideoMode(resolution_x, resolution_y), 
		"Summoners", 
		sf::Style::Titlebar | sf::Style::Close
	);
	window.setFramerateLimit(60);

	sf::Image icon;
	if (icon.loadFromFile("Images/icon.png") == 0)
	{
		std::cerr << "Error loading icon";
		return 1;
	}
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	using mgamemap1 = gamemap<resolution_x, resolution_y, aspect_ratio_x, aspect_ratio_y>;
	using mlevel = level<resolution_x, resolution_y, aspect_ratio_x, aspect_ratio_y>;

	mgamemap1 g;
	//g.load();

	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());

	g.add_skill(0, 0, skill_library::summon_demon());
	g.add_skill(0, 0, skill_library::summon_demon());
	g.add_skill(0, 0, skill_library::summon_demon());
	g.add_skill(0, 0, skill_library::summon_demon());
	g.add_skill(0, 0, skill_library::summon_demon());
	g.add_skill(0, 0, skill_library::summon_demon());

	g.add_skill(0, 0, skill_library::summon_penguin());
	g.add_skill(0, 0, skill_library::summon_penguin());
	g.add_skill(0, 0, skill_library::summon_penguin());
	g.add_skill(0, 0, skill_library::summon_penguin());
	g.add_skill(0, 0, skill_library::summon_penguin());
	g.add_skill(0, 0, skill_library::summon_penguin());

	g.add_skill(0, 0, skill_library::summon_tiger());
	g.add_skill(0, 0, skill_library::summon_tiger());
	g.add_skill(0, 0, skill_library::summon_tiger());
	g.add_skill(0, 0, skill_library::summon_tiger());
	g.add_skill(0, 0, skill_library::summon_tiger());
	g.add_skill(0, 0, skill_library::summon_tiger());

	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	g.add_skill(0, 0, skill_library::summon_skeleton());
	
	g.place_squad(3, 3, squad_library::skeleton_squad(1, "Arantir"));
	g.place_squad(3, 2, squad_library::skeleton_squad(2, "Arantir"));
	g.place_squad(2, 3, squad_library::penguin_squad(99, "Arantir"));
	g.place_squad(10, 4, squad_library::demon_squad(1, "Agrail"));
	g.place_squad(10, 5, squad_library::tiger_squad(15, "Agrail"));

	g.place_squad(0, 0, squad_library::arantir_squad());
	g.place_squad(11, 0, squad_library::agrail_squad());

	g.build_a_wall(5, 5);
	g.dig_an_abyss(8, 4);

	mlevel l(g);

	while (window.isOpen())
	{
		sf::Event event;
		switch(l.get_mode())
		{
		case mlevel::battle_mode:
			l.draw(window);
			l.action(window, event);
			break;
		case mlevel::book_mode:
			l.open_book(window, event);
			break;
		default:
			break;
		}

		window.display();
	}

	return 0;
}
