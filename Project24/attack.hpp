#pragma once

#include "unit.hpp"
#include "relations_holder.hpp"

#include <functional>
#include "coord_t.hpp"

template <
	coord_t X1, coord_t Y1,
	coord_t X2, coord_t Y2
>
	constexpr coord_t get_a()
{
	return (Y2 - Y1) / (X2 - X1);
}

template <
	coord_t X1, coord_t Y1,
	coord_t X2, coord_t Y2
>
	constexpr coord_t get_b()
{
	return 	Y1 - X1 * (Y2 - Y1) / (X2 - X1);
}

template <coord_t A, coord_t B>
using line_equation = coord_t(*)(const coord_t& x);

template <coord_t A, coord_t B>
coord_t get_abscissa(const coord_t& x)
{
	return A * x + B;
}

coord_t gamemap_x, gamemap_y;
bool theres_squad_on(coord_t x, coord_t y) { return x > y; }
bool theres_wall_on(coord_t x, coord_t y) { return x > y; }

class attack
{
	using CounterType = coord_t;
	const CounterType pass_through_counter_;
	const ChType damage_;
public:
	attack() : pass_through_counter_(0), damage_(0) {}
	attack(const CounterType& pass_through_counter, const ChType& damage) :
		pass_through_counter_(pass_through_counter), damage_(damage) {}

	void damage_this_squad();
	void damage_itself();

	/*template <
		template <class> class FirstUnitType, class FirstSchoolType,
		template <class> class SecondUnitType, class SecondSchoolType
	>*/
	//void attack_method(creature_squad<FirstUnitType, FirstSchoolType> attacker, 
	//	creature_squad<SecondUnitType, SecondSchoolType>& victim)
	//{
	//	ChType full_damage = (attacker.ñur_unit_chs_.get_attack() * attacker.size_ -
	//		victim.ñur_unit_chs_.get_defense() * victim.size_) *
	//		get_relation_coeff<FirstSchoolType, SecondSchoolType>() +
	//		victim.damage_dealt_to_the_last_unit_;

	//	auto units_killed = static_cast<std::size_t>(full_damage / victim.ñur_unit_chs_.get_health());
	//	victim.damage_dealt_to_the_last_unit_ = std::fmod(full_damage, victim.ñur_unit_chs_.get_health());

	//	victim.dead_ += units_killed;
	//	victim.size_ -= units_killed;
	//}

	/*template <
		coordinate_type X1, coordinate_type Y1,
		coordinate_type X2, coordinate_type Y2,
		coordinate_type A = get_a<X1, Y1, X2, Y2>(),
		coordinate_type B = get_b<X1, Y1, X2, Y2>(),
		line_equation<A, B> F = get_abscissa<A, B>()
	>
	void operator() ()
	{
		if (X1 == X2 && Y1 == Y2)
		{
			damage_itself();
			return;
		}

		auto pt_counter = pass_through_counter_;
		bool check = (X1 != X2 ? X1 < X2 : Y1 < Y2);
		coordinate_type c = (X1 != X2 ? X1 : Y1);
		coordinate_type gamemap_border = (X1 != X2 ? gamemap_x : gamemap_y);

		for (; c >= 0 && c < gamemap_border && pt_counter; c += (check ? 1 : -1))
		{
			coordinate_type x = (X1 != X2 ? c : X1);
			coordinate_type y = (X1 != X2 ? F(x) : c);

			if (theres_squad_on(x, y))
			{
				damage_this_squad();
				--pt_counter;
			}
			else if (theres_wall_on(x, y))
				break;
		}
	}*/
};
