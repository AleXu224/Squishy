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
		std::map<Stat, bool> &partition4MainStats;
		std::map<Stat, bool> &partition5MainStats;
		std::map<Stat, bool> &partition6MainStats;

		struct State : WidgetState<OptimizationMainStatChooser> {
			Child generateFor(std::string name, std::map<Stat, bool> &mainStats);

			Child build(const Element &element) override;
		};
	};
}// namespace UI