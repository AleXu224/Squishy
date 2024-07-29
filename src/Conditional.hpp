#pragma once

#include "vector"

enum class ConditionalType {
	boolean,
	numeric,
};

struct Conditional {
	ConditionalType type = ConditionalType::boolean;
	unsigned int value = 0;
	std::vector<unsigned int> numericValues{};
};
