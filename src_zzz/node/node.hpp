#pragma once

#include "array"
#include "entry.hpp"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "vector"


namespace Node {
	struct Instance {
		Data data;
		Formula::FloatNode formula;

		template<class T>
		Instance(const T &t) : data(t._data), formula(t._formula) {}
	};

	using Types = Instance;

	struct AgentList {
		std::vector<Node::Types> basic{};
		std::vector<Node::Types> dodge{};
		std::vector<Node::Types> dashAttack{};
		std::vector<Node::Types> dodgeCounter{};
		std::vector<Node::Types> assist{};
		std::vector<Node::Types> quickAssist{};
		std::vector<Node::Types> defensiveAssist{};
		std::vector<Node::Types> evasiveAssist{};
		std::vector<Node::Types> assistFollowup{};
		std::vector<Node::Types> special{};
		std::vector<Node::Types> exspecial{};
		std::vector<Node::Types> chain{};
		std::vector<Node::Types> ultimate{};
		std::vector<Node::Types> core{};
		std::vector<Node::Types> mindscape1{};
		std::vector<Node::Types> mindscape2{};
		std::vector<Node::Types> mindscape3{};
		std::vector<Node::Types> mindscape4{};
		std::vector<Node::Types> mindscape5{};
		std::vector<Node::Types> mindscape6{};

		[[nodiscard]] static auto getMembers() {
			return std::array{
				&AgentList::basic,
				&AgentList::dodge,
				&AgentList::dashAttack,
				&AgentList::dodgeCounter,
				&AgentList::assist,
				&AgentList::quickAssist,
				&AgentList::defensiveAssist,
				&AgentList::evasiveAssist,
				&AgentList::assistFollowup,
				&AgentList::special,
				&AgentList::exspecial,
				&AgentList::chain,
				&AgentList::ultimate,
				&AgentList::core,
				&AgentList::mindscape1,
				&AgentList::mindscape2,
				&AgentList::mindscape3,
				&AgentList::mindscape4,
				&AgentList::mindscape5,
				&AgentList::mindscape6,
			};
		}

		[[nodiscard]] const auto &fromEntry(AgentSlot entry) const {
			switch (entry) {
				case AgentSlot::basic:
					return basic;
				case AgentSlot::dodge:
					return dodge;
				case AgentSlot::dashAttack:
					return dashAttack;
				case AgentSlot::dodgeCounter:
					return dodgeCounter;
				case AgentSlot::assist:
					return assist;
				case AgentSlot::quickAssist:
					return quickAssist;
				case AgentSlot::defensiveAssist:
					return defensiveAssist;
				case AgentSlot::evasiveAssist:
					return evasiveAssist;
				case AgentSlot::assistFollowup:
					return assistFollowup;
				case AgentSlot::special:
					return special;
				case AgentSlot::exspecial:
					return exspecial;
				case AgentSlot::chain:
					return chain;
				case AgentSlot::ultimate:
					return ultimate;
				case AgentSlot::core:
					return core;
				case AgentSlot::mindscape1:
					return mindscape1;
				case AgentSlot::mindscape2:
					return mindscape2;
				case AgentSlot::mindscape3:
					return mindscape3;
				case AgentSlot::mindscape4:
					return mindscape4;
				case AgentSlot::mindscape5:
					return mindscape5;
				case AgentSlot::mindscape6:
					return mindscape6;
			}
			std::unreachable();
		}
	};

	using EngineList = std::vector<Node::Types>;

	using DiscList = std::vector<Node::Types>;
}// namespace Node
