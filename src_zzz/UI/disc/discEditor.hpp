#pragma once

#include "core/core.hpp"

#include "disc/instance.hpp"

namespace UI {
	using namespace squi;
	struct DiscEditor : StatefulWidget {
		// Args
		Key key;
		std::optional<Disc::Instance> disc{};
		std::function<void(Disc::Instance)> onSubmit{};

		struct State : WidgetState<DiscEditor> {
			Disc::Instance disc{};
			VoidObservable closeEvent{};

			void updatePartition(Disc::Partition newPartition);
			Child createSubStat(size_t subStatIndex);

			void initState() override;

			Child build(const Element &element) override;
		};
	};
}// namespace UI