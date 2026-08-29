#pragma once

#include "array"
#include "boolean.hpp"
#include "utils/hashedString.hpp"
#include "valueList.hpp"
#include "valueSlider.hpp"
#include "variant"
#include <functional>
#include <print>
#include <unordered_map>


namespace Option {
	using Types = std::variant<Boolean, ValueList, ValueSlider>;

	using TypesMap = std::unordered_map<uint32_t, Types>;

	inline bool getBool(const TypesMap &options, const Utils::HashedString &key) {
		return std::visit(
			Utils::overloaded{
				[](const Option::Boolean &option) {
					return option.active;
				},
				[](const Option::ValueList &option) {
					return option.currentIndex.has_value();
				},
				[](const Option::ValueSlider &option) {
					return true;
				},
			},
			options.at(key.hash)
		);
	}

	[[nodiscard]] inline float getFloat(const TypesMap &options, const Utils::HashedString &key, float defaultValue = 0.f) {
		return std::visit(
			Utils::overloaded{
				[](const Option::Boolean &option) {
					return option.active ? 1.f : 0.f;
				},
				[&defaultValue](const Option::ValueList &option) {
					return option.currentIndex.transform([](auto val) {
												  return static_cast<float>(val);
											  })
						.value_or(defaultValue);
				},
				[](const Option::ValueSlider &option) {
					return option.getValue();
				},
			},
			options.at(key.hash)
		);
	}

	[[nodiscard]] inline uint32_t getInt(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return static_cast<uint32_t>(std::visit(
			Utils::overloaded{
				[](const Option::Boolean &option) -> float {
					return option.active ? 1 : 0;
				},
				[&defaultValue](const Option::ValueList &option) -> float {
					return option.currentIndex.value_or(defaultValue);
				},
				[](const Option::ValueSlider &option) -> float {
					return option.getValue();
				},
			},
			options.at(key.hash)
		));
	}

	[[nodiscard]] inline uint32_t getIndex(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return std::visit(
			Utils::overloaded{
				[&defaultValue](const ValueList &option) -> uint32_t {
					return option.currentIndex.value_or(defaultValue);
				},
				[&defaultValue, &key](const Boolean &) -> uint32_t {
					std::println("Wrong opt selected while getting index {}", key.str);
					return defaultValue;
				},
				[&defaultValue, &key](const ValueSlider &) -> uint32_t {
					std::println("Wrong opt selected while getting index {}", key.str);
					return defaultValue;
				},
			},
			options.at(key.hash)
		);
	}

	struct AgentList {
		std::vector<Types> basic{};
		std::vector<Types> dodge{};
		std::vector<Types> dashAttack{};
		std::vector<Types> dodgeCounter{};
		std::vector<Types> assist{};
		std::vector<Types> quickAssist{};
		std::vector<Types> defensiveAssist{};
		std::vector<Types> evasiveAssist{};
		std::vector<Types> assistFollowup{};
		std::vector<Types> special{};
		std::vector<Types> exspecial{};
		std::vector<Types> chain{};
		std::vector<Types> ultimate{};
		std::vector<Types> core{};
		std::vector<Types> mindscape1{};
		std::vector<Types> mindscape2{};
		std::vector<Types> mindscape3{};
		std::vector<Types> mindscape4{};
		std::vector<Types> mindscape5{};
		std::vector<Types> mindscape6{};

		[[nodiscard]] static inline auto getMembers() {
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

		[[nodiscard]] inline const std::vector<Types> &fromAgentSlot(Node::AgentSlot slot) const {
			switch (slot) {
				case Node::AgentSlot::basic:
					return basic;
				case Node::AgentSlot::dodge:
					return dodge;
				case Node::AgentSlot::dashAttack:
					return dashAttack;
				case Node::AgentSlot::dodgeCounter:
					return dodgeCounter;
				case Node::AgentSlot::assist:
					return assist;
				case Node::AgentSlot::quickAssist:
					return quickAssist;
				case Node::AgentSlot::defensiveAssist:
					return defensiveAssist;
				case Node::AgentSlot::evasiveAssist:
					return evasiveAssist;
				case Node::AgentSlot::assistFollowup:
					return assistFollowup;
				case Node::AgentSlot::special:
					return special;
				case Node::AgentSlot::exspecial:
					return exspecial;
				case Node::AgentSlot::chain:
					return chain;
				case Node::AgentSlot::ultimate:
					return ultimate;
				case Node::AgentSlot::core:
					return core;
				case Node::AgentSlot::mindscape1:
					return mindscape1;
				case Node::AgentSlot::mindscape2:
					return mindscape2;
				case Node::AgentSlot::mindscape3:
					return mindscape3;
				case Node::AgentSlot::mindscape4:
					return mindscape4;
				case Node::AgentSlot::mindscape5:
					return mindscape5;
				case Node::AgentSlot::mindscape6:
					return mindscape6;
			}
			std::unreachable();
		}
		[[nodiscard]] static Formula::BoolNode conditionFromAgentSlot(Node::AgentSlot slot);
	};

	using EngineList = std::vector<Types>;

	using DiscList = std::vector<Types>;

	inline void mapOptions(TypesMap &ret, const AgentList &vals) {
		for (auto listMember: AgentList::getMembers()) {
			const auto &_ = std::invoke(listMember, vals);

			for (const auto &val: _) {
				std::visit(
					[&](auto &&v) {
						ret.insert({v.key, v});
					},
					val
				);
			}
		}
	}

	inline void mapOptions(TypesMap &ret, const EngineList &vals) {
		for (const auto &val: vals) {
			std::visit(
				[&](auto &&v) {
					ret.insert({v.key, v});
				},
				val
			);
		}
	}
}// namespace Option
