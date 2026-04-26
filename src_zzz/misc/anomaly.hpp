#pragma once

#include "cstdint"
#include "utils/stringify.hpp"
#include <utility>
#include <vector>


namespace Misc {
	enum class Anomaly : uint8_t {
		burn,
		shock,
		shatter,
		assault,
		corruption,
		burnDisorder,
		shockDisorder,
		frozenDisorder,
		frozenFrostDisorder,
		assaultDisorder,
		corruptionDisorder,
	};

	enum class DamageAnomaly {
		burn,
		shock,
		shatter,
		assault,
		corruption,
		disorder,
		burnDisorder,
		shockDisorder,
		frozenDisorder,
		frozenFrostDisorder,
		assaultDisorder,
		corruptionDisorder,
		abloom,
		allAnomaly,
	};

	const inline std::vector<Anomaly> anomalies{
		Anomaly::burn,
		Anomaly::shock,
		Anomaly::shatter,
		Anomaly::assault,
		Anomaly::corruption,
		Anomaly::burnDisorder,
		Anomaly::shockDisorder,
		Anomaly::frozenDisorder,
		Anomaly::frozenFrostDisorder,
		Anomaly::assaultDisorder,
		Anomaly::corruptionDisorder,
	};

	const inline std::vector<DamageAnomaly> damageAnomalies{
		DamageAnomaly::burn,
		DamageAnomaly::shock,
		DamageAnomaly::shatter,
		DamageAnomaly::assault,
		DamageAnomaly::corruption,
		DamageAnomaly::disorder,
		DamageAnomaly::burnDisorder,
		DamageAnomaly::shockDisorder,
		DamageAnomaly::frozenDisorder,
		DamageAnomaly::frozenFrostDisorder,
		DamageAnomaly::assaultDisorder,
		DamageAnomaly::corruptionDisorder,
		DamageAnomaly::abloom,
		DamageAnomaly::allAnomaly,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::Anomaly &anomaly) {
		switch (anomaly) {
			case Misc::Anomaly::burn:
				return "Burn";
			case Misc::Anomaly::shock:
				return "Shock";
			case Misc::Anomaly::shatter:
				return "Shatter";
			case Misc::Anomaly::assault:
				return "Assault";
			case Misc::Anomaly::corruption:
				return "Corruption";
			case Misc::Anomaly::burnDisorder:
				return "Burn Disorder";
			case Misc::Anomaly::shockDisorder:
				return "Shock Disorder";
			case Misc::Anomaly::frozenDisorder:
				return "Frozen Disorder";
			case Misc::Anomaly::frozenFrostDisorder:
				return "Frozen Disorder (Frost)";
			case Misc::Anomaly::assaultDisorder:
				return "Assault Disorder";
			case Misc::Anomaly::corruptionDisorder:
				return "Corruption Disorder";
		}
		std::unreachable();
	}

	template<>
	constexpr std::string Stringify<>(const Misc::DamageAnomaly &anomaly) {
		switch (anomaly) {
			case Misc::DamageAnomaly::burn:
				return "Burn";
			case Misc::DamageAnomaly::shock:
				return "Shock";
			case Misc::DamageAnomaly::shatter:
				return "Shatter";
			case Misc::DamageAnomaly::assault:
				return "Assault";
			case Misc::DamageAnomaly::corruption:
				return "Corruption";
			case Misc::DamageAnomaly::disorder:
				return "Disorder";
			case Misc::DamageAnomaly::burnDisorder:
				return "Burn Disorder";
			case Misc::DamageAnomaly::shockDisorder:
				return "Shock Disorder";
			case Misc::DamageAnomaly::frozenDisorder:
				return "Frozen Disorder";
			case Misc::DamageAnomaly::frozenFrostDisorder:
				return "Frozen Disorder (Frost)";
			case Misc::DamageAnomaly::assaultDisorder:
				return "Assault Disorder";
			case Misc::DamageAnomaly::corruptionDisorder:
				return "Corruption Disorder";
			case Misc::DamageAnomaly::abloom:
				return "Abloom";
			case Misc::DamageAnomaly::allAnomaly:
				return "All Anomaly";
		}
	}
}// namespace Utils