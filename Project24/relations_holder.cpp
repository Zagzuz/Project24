#include "relations_holder.hpp"

relation_coeffs_t relations_holder<animalism>::relation_coeffs =
{
	1.0, // animalism
	2.4, // demonology
	3.2, // necromancy, ... in alphabetical order
};

relation_coeffs_t relations_holder<demonology>::relation_coeffs =
{
	1.2,
	0.5,
	7.6,
};

relation_coeffs_t relations_holder<necromancy>::relation_coeffs =
{
	0.7,
	0.8,
	15.9,
};