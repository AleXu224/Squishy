#pragma once

#include "string"

#include "intermediary.hpp"
#include "memory"

namespace Stats {
	struct Sheet;
}

namespace Formula {
	struct Node {
		bool hasValue;

		struct interface {
			constexpr virtual std::string print(const Stats::Sheet &) const = 0;
			constexpr virtual float eval(const Stats::Sheet &) const = 0;
			constexpr virtual std::unique_ptr<interface> clone() const = 0;
			constexpr virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}

			constexpr std::string print(const Stats::Sheet &stats) const override {
				return fn.print(stats, Step::none);
			}
			constexpr float eval(const Stats::Sheet &stats) const override {
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
			: hasValue(true), fn(std::make_unique<implementation<T>>(t)) {
		}

		constexpr Node(const Node &other) : hasValue(other.hasValue) {
			if (other.fn) {
				fn = other.fn->clone();
			}
		}
		constexpr Node &operator=(Node &&other) {
			if (this != &other) {
				hasValue = other.hasValue;
				fn = std::move(other.fn);
				other.hasValue = false;
			}
			return *this;
		}

		[[nodiscard]] constexpr std::string print(const Stats::Sheet &stats) const {
			return fn->print(stats);
		}
		[[nodiscard]] constexpr float eval(const Stats::Sheet &stats) const {
			return fn->eval(stats);
		}

		Node() : hasValue(false) {}

	private:
		std::unique_ptr<interface> fn{};
	};
}// namespace Formula