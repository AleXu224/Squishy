#pragma once

#include "disc/slot.hpp"
#include "string"
#include "unordered_map"


namespace Serialization::Zod {
	static inline std::unordered_map<std::string, Disc::Partition> slotKey{
		{"1", Disc::Partition::one},
		{"2", Disc::Partition::two},
		{"3", Disc::Partition::three},
		{"4", Disc::Partition::four},
		{"5", Disc::Partition::five},
		{"6", Disc::Partition::six},
	};
	static inline std::unordered_map<Disc::Partition, std::string> keySlot{
		{Disc::Partition::one, "1"},
		{Disc::Partition::two, "2"},
		{Disc::Partition::three, "3"},
		{Disc::Partition::four, "4"},
		{Disc::Partition::five, "5"},
		{Disc::Partition::six, "6"},
	};
}// namespace Serialization::Zod