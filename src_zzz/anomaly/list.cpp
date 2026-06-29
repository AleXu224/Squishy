#include "list.hpp"

#include "formula/anomaly.hpp"
#include "formula/teamAgent.hpp"

namespace Anomaly::List {
	const std::vector<::Anomaly::Anomaly> &vortexList() {
		static auto list = [&]() {
			std::vector<::Anomaly::Anomaly> vortexes;
			for (size_t i = 0; i < 3; i++) {
				auto attribute = Formula::TeamAgent{
					.index = i,
					.formula = Formula::AgentAttribute{},
				};

				vortexes.push_back(::Anomaly::Anomaly{
					.name = Formula::CharacterName{
						.index = i,
						.suffix = "Vortex",
					},
					.multiplier = 1.f,
					.attribute = Formula::TeamAgent{
						.index = i,
						.formula = Formula::AgentAttribute{},
					},
					.modifier = Modifiers::combat().vortex,
					.formula = Formula::Requires{
						.requirement = Formula::IsAgentAttribute{.attribute = Misc::Attribute::wind}//
									&& Formula::IsAgentVortexableAttribute{.agentIndex = i},
						.ret = makeVortexFormula(Modifiers::combat().vortex, attribute, i),
					},
				});
			}
			return vortexes;
		}();
		return list;
	}
}// namespace Anomaly::List