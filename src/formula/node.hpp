#pragma once

#include "string"

#include "intermediary.hpp"
#include "memory"

namespace Formula {
	struct Node {
		struct interface {
			[[nodiscard]] constexpr virtual std::string print(const Stats::Loadout &, const Stats::Team &) const = 0;
			[[nodiscard]] constexpr virtual float eval(const Stats::Loadout &, const Stats::Team &) const = 0;
			[[nodiscard]] constexpr virtual std::unique_ptr<interface> clone() const = 0;
			constexpr virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}

			[[nodiscard]] constexpr std::string print(const Stats::Loadout &stats, const Stats::Team &team) const override {
				return fn.print(stats, team, Step::none);
			}
			[[nodiscard]] constexpr float eval(const Stats::Loadout &stats, const Stats::Team &team) const override {
				return fn.eval(stats, team);
			}
			[[nodiscard]] constexpr std::unique_ptr<interface> clone() const override {
				return std::make_unique<implementation<Fn>>(fn);
			}

		private:
			Fn fn{};
		};

		template<IntermediaryLike T>
		constexpr Node(const T &t)
			: fn(std::make_unique<implementation<T>>(t)) {
		}

		constexpr Node(const Node &other) {
			if (other.fn) {
				fn = other.fn->clone();
			}
		}
		constexpr Node &operator=(const Node &other) {
			if (this != &other && other.fn) {
				fn = other.fn->clone();
			}
			return *this;
		}
		constexpr Node &operator=(Node &&other) noexcept {
			if (this != &other && other.fn) {
				fn = std::move(other.fn);
			}
			return *this;
		}

		[[nodiscard]] constexpr std::string print(const Stats::Loadout &stats, const Stats::Team &team) const {
			return fn->print(stats, team);
		}
		[[nodiscard]] constexpr float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return fn->eval(stats, team);
		}

		Node() = default;

		[[nodiscard]] constexpr bool hasValue() const {
			return fn != nullptr;
		}

	private:
		std::unique_ptr<interface> fn{};
	};
}// namespace Formula