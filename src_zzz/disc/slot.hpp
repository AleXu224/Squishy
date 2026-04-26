#pragma once

#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"
#include <vector>


namespace Disc {
	enum class Partition : uint8_t {
		one,
		two,
		three,
		four,
		five,
		six,
	};

	static inline std::vector<Partition> partitions{
		Partition::one,
		Partition::two,
		Partition::three,
		Partition::four,
		Partition::five,
		Partition::six,
	};

	enum class SetSlot : uint8_t {
		twoPiece,
		fourPiece,
	};

	static inline std::vector<SetSlot> setSlots{
		SetSlot::twoPiece,
		SetSlot::fourPiece,
	};
}// namespace Disc


namespace Utils {
	template<>
	inline std::string Stringify<>(const Disc::Partition &partition) {
		switch (partition) {
			case Disc::Partition::one:
				return "One";
			case Disc::Partition::two:
				return "Two";
			case Disc::Partition::three:
				return "Three";
			case Disc::Partition::four:
				return "Four";
			case Disc::Partition::five:
				return "Five";
			case Disc::Partition::six:
				return "Six";
		}
		std::unreachable();
	}
}// namespace Utils