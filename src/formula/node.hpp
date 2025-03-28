#pragma once

#include "string"

#include "formula/formulaContext.hpp"
#include "memory"
#include "misc/element.hpp"
#include "step.hpp"
#include "utils/optional.hpp"


namespace Formula {
	template<class RetType>
	struct NodeType {
		struct interface {
			interface() = default;
			[[nodiscard]] interface(const interface &) = delete;
			interface(interface &&) = delete;
			interface &operator=(const interface &) = delete;
			interface &operator=(interface &&) = delete;

			[[nodiscard]] constexpr virtual std::string print(const Context &) const = 0;
			[[nodiscard]] constexpr virtual RetType eval(const Context &) const = 0;
			[[nodiscard]] constexpr virtual std::unique_ptr<interface> clone() const = 0;
			constexpr virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}

			[[nodiscard]] constexpr std::string print(const Context &context) const override {
				return fn.print(context, Step::none);
			}
			[[nodiscard]] constexpr RetType eval(const Context &context) const override {
				return fn.eval(context);
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
			if (!other.fn)
				fn.reset();
			else
				fn = other.fn->clone();
		}
		constexpr NodeType &operator=(const NodeType &other) {
			if (this != &other) {
				if (!other.fn)
					fn.reset();
				else
					fn = other.fn->clone();
			}
			return *this;
		}
		constexpr NodeType &operator=(NodeType &&other) noexcept {
			if (this != &other) {
				fn = std::move(other.fn);
			}
			return *this;
		}
		constexpr NodeType(NodeType &&other) : fn(std::move(other.fn)) {}

		[[nodiscard]] constexpr std::string print(const Context &context) const {
			return fn->print(context);
		}
		[[nodiscard]] constexpr RetType eval(const Context &context) const {
			return fn->eval(context);
		}

		NodeType() = default;

		~NodeType() = default;

		[[nodiscard]] constexpr bool hasValue() const {
			return fn != nullptr;
		}

	private:
		std::unique_ptr<interface> fn{};
	};

	using FloatNode = NodeType<float>;
	using BoolNode = NodeType<bool>;
	using IntNode = NodeType<uint32_t>;
	using ElementNode = NodeType<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula