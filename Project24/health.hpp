#pragma once

#include "characteristic.hpp"

class health : public characteristic
{
	explicit health(const ChType v) : characteristic(v) {}
};