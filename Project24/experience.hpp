#pragma once

#include "characteristic.hpp"

class experience : public characteristic
{
	explicit experience(const ChType v) : characteristic(v) {}
};