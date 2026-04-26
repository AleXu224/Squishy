#pragma once

#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"
#include "vector"


namespace Node {
	enum class AgentSlot : uint8_t {
		basic,
		dodge,
		dashAttack,
		dodgeCounter,
		assist,
		quickAssist,
		defensiveAssist,
		evasiveAssist,
		assistFollowup,
		special,
		exspecial,
		chain,
		ultimate,
		core,
		mindscape1,
		mindscape2,
		mindscape3,
		mindscape4,
		mindscape5,
		mindscape6,
	};

	static inline std::vector<AgentSlot> agentSlots{
		AgentSlot::basic,
		AgentSlot::dodge,
		AgentSlot::dashAttack,
		AgentSlot::dodgeCounter,
		AgentSlot::assist,
		AgentSlot::quickAssist,
		AgentSlot::defensiveAssist,
		AgentSlot::evasiveAssist,
		AgentSlot::assistFollowup,
		AgentSlot::special,
		AgentSlot::exspecial,
		AgentSlot::chain,
		AgentSlot::ultimate,
		AgentSlot::core,
		AgentSlot::mindscape1,
		AgentSlot::mindscape2,
		AgentSlot::mindscape3,
		AgentSlot::mindscape4,
		AgentSlot::mindscape5,
		AgentSlot::mindscape6,
	};
}// namespace Node

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Node::AgentSlot &agentSlot) {
		switch (agentSlot) {
			case Node::AgentSlot::basic:
				return "Basic";
			case Node::AgentSlot::dodge:
				return "Dodge";
			case Node::AgentSlot::dashAttack:
				return "Dash Attack";
			case Node::AgentSlot::dodgeCounter:
				return "Dodge Counter";
			case Node::AgentSlot::assist:
				return "Assist";
			case Node::AgentSlot::quickAssist:
				return "Quick Assist";
			case Node::AgentSlot::defensiveAssist:
				return "Defensive Assist";
			case Node::AgentSlot::evasiveAssist:
				return "Evasive Assist";
			case Node::AgentSlot::assistFollowup:
				return "Assist Followup";
			case Node::AgentSlot::special:
				return "Special";
			case Node::AgentSlot::exspecial:
				return "EX Special";
			case Node::AgentSlot::chain:
				return "Chain";
			case Node::AgentSlot::ultimate:
				return "Ultimate";
			case Node::AgentSlot::core:
				return "Core";
			case Node::AgentSlot::mindscape1:
				return "Mindscape 1";
			case Node::AgentSlot::mindscape2:
				return "Mindscape 2";
			case Node::AgentSlot::mindscape3:
				return "Mindscape 3";
			case Node::AgentSlot::mindscape4:
				return "Mindscape 4";
			case Node::AgentSlot::mindscape5:
				return "Mindscape 5";
			case Node::AgentSlot::mindscape6:
				return "Mindscape 6";
		}
		std::unreachable();
	}
}// namespace Utils