#pragma once

#include "sheet.hpp"
#include "value.hpp"


namespace Stats {
	struct Enemy {
		using _Value = Value<float, 0>;
		EnemySheet<_Value> sheet{
			.resistance{
				.physical{},
				.fire{},
				.ice{},
				.electric{},
				.ether{},
			},
		};
	};
}// namespace Stats