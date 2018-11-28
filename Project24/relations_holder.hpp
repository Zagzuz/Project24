#pragma once

#include "unit.hpp"
#include "animalism.hpp"
#include "demonology.hpp"
#include "necromancy.hpp"

#include <tuple>

class demonology;
class necromancy;
class school_of_magic;

template <typename SchoolType>
struct coeff_holder
{
	ChType coeff;
	coeff_holder(const ChType& c) : coeff(c) {}
};

using relation_coeffs_t = 
std::tuple<
	coeff_holder<animalism>,
	coeff_holder<demonology>,
	coeff_holder<necromancy>
>;

template <class SchoolType/*,
	typename = std::enable_if<std::is_base_of<school_of_magic, SchoolType>::value>*/>
	struct relations_holder
{
	static relation_coeffs_t relation_coeffs;
	template <class, class> friend ChType get_relation_coeff();
};

template <class FirstSchool, class SecondSchool>
ChType get_relation_coeff()
{
	return std::get<coeff_holder<SecondSchool>>
		(relations_holder<FirstSchool>::relation_coeffs).coeff;
}
