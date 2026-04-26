#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"
#include "formula/requirement.hpp"
#include "node/entry.hpp"
#include "utility"

namespace Utils {
	static inline Formula::BoolNode slotToCondition(const Node::AgentSlot &entry) {
		switch (entry) {
			case Node::AgentSlot::basic:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::dodge:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::dashAttack:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::dodgeCounter:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::assist:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::quickAssist:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::defensiveAssist:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::evasiveAssist:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::assistFollowup:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::special:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::exspecial:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::chain:
				return Formula::ConstantBool({}, true);
			case Node::AgentSlot::ultimate:
				return Formula::ConstantBool({}, true);
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
}// namespace Utils