#pragma once

#include "string"

#include "intermediary.hpp"
#include "memory"

namespace Formula {
	struct Node {
		struct interface {
			constexpr virtual std::string print(const Stats::Loadout &) const = 0;
			constexpr virtual float eval(const Stats::Loadout &) const = 0;
			constexpr virtual std::unique_ptr<interface> clone() const = 0;
			constexpr virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}

			constexpr std::string print(const Stats::Loadout &stats) const override {
				return fn.print(stats, Step::none);
			}
			constexpr float eval(const Stats::Loadout &stats) const override {
				return fn.eval(stats);
			}
			constexpr std::unique_ptr<interface> clone() const override {
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
			if (other.fn) {
				fn = other.fn->clone();
			}
			return *this;
		}
		constexpr Node &operator=(Node &&other) {
			if (this != &other && other.fn) {
				fn = std::move(other.fn);
			}
			return *this;
		}

		[[nodiscard]] constexpr std::string print(const Stats::Loadout &stats) const {
			return fn->print(stats);
		}
		[[nodiscard]] constexpr float eval(const Stats::Loadout &stats) const {
			return fn->eval(stats);
		}

		Node() {}

		[[nodiscard]] constexpr bool hasValue() const {
			return fn != nullptr;
		}

	private:
		std::unique_ptr<interface> fn{};
	};
}// namespace Formula