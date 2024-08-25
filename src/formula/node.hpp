#pragma once

#include "string"

#include "intermediary.hpp"
#include "memory"

namespace Formula {
	template<class RetType>
	struct NodeType {
		struct interface {
			[[nodiscard]] constexpr virtual std::string print(const Stats::Loadout &, const Stats::Team &) const = 0;
			[[nodiscard]] constexpr virtual RetType eval(const Stats::Loadout &, const Stats::Team &) const = 0;
			[[nodiscard]] constexpr virtual std::unique_ptr<interface> clone() const = 0;
			constexpr virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}

			[[nodiscard]] constexpr std::string print(const Stats::Loadout &stats, const Stats::Team &team) const override {
				return fn.print(stats, team, Step::none);
			}
			[[nodiscard]] constexpr RetType eval(const Stats::Loadout &stats, const Stats::Team &team) const override {
				return fn.eval(stats, team);
			}
			[[nodiscard]] constexpr std::unique_ptr<interface> clone() const override {
				return std::make_unique<implementation<Fn>>(fn);
			}

		private:
			Fn fn{};
		};

		template<class T>
		constexpr NodeType(const T &t)
			: fn(std::make_unique<implementation<T>>(t)) {
		}

		constexpr NodeType(const NodeType &other) {
			if (other.fn) {
				fn = other.fn->clone();
			}
		}
		constexpr NodeType &operator=(const NodeType &other) {
			if (this != &other && other.fn) {
				fn = other.fn->clone();
			}
			return *this;
		}
		constexpr NodeType &operator=(NodeType &&other) noexcept {
			if (this != &other && other.fn) {
				fn = std::move(other.fn);
			}
			return *this;
		}

		[[nodiscard]] constexpr std::string print(const Stats::Loadout &stats, const Stats::Team &team) const {
			return fn->print(stats, team);
		}
		[[nodiscard]] constexpr RetType eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return fn->eval(stats, team);
		}

		NodeType() = default;

		[[nodiscard]] constexpr bool hasValue() const {
			return fn != nullptr;
		}

	private:
		std::unique_ptr<interface> fn{};
	};

	using Node = NodeType<float>;
	using ElementNode = NodeType<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula