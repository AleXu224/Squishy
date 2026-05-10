#pragma once

#include "core/core.hpp"
#include "stats/stat.hpp"
#include <map>

namespace UI {
	using namespace squi;

	struct OptimizationMainStatChooser : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		std::map<Stat, bool> &sandsMainStats;
		std::map<Stat, bool> &gobletMainStats;
		std::map<Stat, bool> &circletMainStats;

		struct State : WidgetState<OptimizationMainStatChooser> {
			Child generateFor(std::string name, std::map<Stat, bool> &mainStats);

			Child build(const Element &element) override;
		};
	};
}// namespace UI