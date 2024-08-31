#pragma once

#include "sheet.hpp"
#include "value.hpp"


namespace Stats {
	struct Enemy {
		using _Value = Value<Formula::Context, 1>;
		EnemySheet<_Value> sheet{
			.resistance{
				.pyro{.constant = 0.1f},
				.hydro{.constant = 0.1f},
				.cryo{.constant = 0.1f},
				.electro{.constant = 0.1f},
				.dendro{.constant = 0.1f},
				.anemo{.constant = 0.1f},
				.geo{.constant = 0.1f},
				.physical{.constant = 0.1f},
			},
		};

		Enemy();
	};
}// namespace Stats