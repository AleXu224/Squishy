#pragma once

#include "anomaly.hpp"
#include "misc/attribute.hpp"

namespace Anomaly::List {
	const inline std::vector<const ::Anomaly::Anomaly *> anomalyList{
		&::Anomaly::List::burn,
		&::Anomaly::List::shock,
		&::Anomaly::List::shatter,
		&::Anomaly::List::assault,
		&::Anomaly::List::corruption,
		&::Anomaly::List::windswept,
	};

	inline const ::Anomaly::Anomaly &fromAttribute(Misc::Attribute attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return ::Anomaly::List::assault;
			case Misc::Attribute::fire:
				return ::Anomaly::List::burn;
			case Misc::Attribute::ice:
				return ::Anomaly::List::shatter;
			case Misc::Attribute::electric:
				return ::Anomaly::List::shock;
			case Misc::Attribute::ether:
				return ::Anomaly::List::corruption;
			case Misc::Attribute::wind:
				return ::Anomaly::List::windswept;
		}
	}

	const inline std::vector<const ::Anomaly::Anomaly *> disorderList{
		&::Anomaly::List::burnDisorder,
		&::Anomaly::List::shockDisorder,
		&::Anomaly::List::frozenDisorder,
		&::Anomaly::List::frozenFrostDisorder,
		&::Anomaly::List::assaultDisorder,
		&::Anomaly::List::corruptionDisorder,
		&::Anomaly::List::windsweptDisorder,
	};

	inline const ::Anomaly::Anomaly &fromAttributeDisorder(Misc::Attribute attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return ::Anomaly::List::assaultDisorder;
			case Misc::Attribute::fire:
				return ::Anomaly::List::burnDisorder;
			case Misc::Attribute::ice:
				return ::Anomaly::List::frozenDisorder;
			case Misc::Attribute::electric:
				return ::Anomaly::List::shockDisorder;
			case Misc::Attribute::ether:
				return ::Anomaly::List::corruptionDisorder;
			case Misc::Attribute::wind:
				return ::Anomaly::List::windsweptDisorder;
		}
	}

	extern const std::vector<::Anomaly::Anomaly> &vortexList();
}// namespace Anomaly::List