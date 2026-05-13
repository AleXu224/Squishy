#include "option.hpp"

#include "formula/constant.hpp"
#include "formula/requirement.hpp"

[[nodiscard]] Formula::BoolNode Option::AgentList::conditionFromAgentSlot(Node::AgentSlot slot) {
	switch (slot) {
		case Node::AgentSlot::basic:
		case Node::AgentSlot::dodge:
		case Node::AgentSlot::dashAttack:
		case Node::AgentSlot::dodgeCounter:
		case Node::AgentSlot::assist:
		case Node::AgentSlot::quickAssist:
		case Node::AgentSlot::defensiveAssist:
		case Node::AgentSlot::evasiveAssist:
		case Node::AgentSlot::assistFollowup:
		case Node::AgentSlot::special:
		case Node::AgentSlot::exspecial:
		case Node::AgentSlot::chain:
		case Node::AgentSlot::ultimate:
		case Node::AgentSlot::core:
			return Formula::ConstantBool({}, true);
		case Node::AgentSlot::mindscape1:
			return Requirement::mindscape1;
		case Node::AgentSlot::mindscape2:
			return Requirement::mindscape2;
		case Node::AgentSlot::mindscape3:
			return Requirement::mindscape3;
		case Node::AgentSlot::mindscape4:
			return Requirement::mindscape4;
		case Node::AgentSlot::mindscape5:
			return Requirement::mindscape5;
		case Node::AgentSlot::mindscape6:
			return Requirement::mindscape6;
	}
	std::unreachable();
}
