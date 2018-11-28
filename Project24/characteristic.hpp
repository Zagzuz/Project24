#pragma once

using ChType = double;

class characteristic
{
protected:
	ChType value_;
public:
	explicit characteristic(const ChType v) : value_(v) {}
};