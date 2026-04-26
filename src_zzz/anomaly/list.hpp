#pragma once

#include "anomaly.hpp"

namespace Anomaly::List {
	static inline std::vector<const ::Anomaly::Anomaly *> anomalyList{
		&::Anomaly::List::burn,
		&::Anomaly::List::shock,
		&::Anomaly::List::shatter,
		&::Anomaly::List::assault,
		&::Anomaly::List::corruption,
	};

	static inline std::vector<const ::Anomaly::Anomaly *> disorderList{
		&::Anomaly::List::burnDisorder,
		&::Anomaly::List::shockDisorder,
		&::Anomaly::List::frozenDisorder,
		&::Anomaly::List::frozenFrostDisorder,
		&::Anomaly::List::assaultDisorder,
		&::Anomaly::List::corruptionDisorder,
	};
}// namespace Anomaly::List