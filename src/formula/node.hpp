#pragma once

#include "string"

#include "compiled/node.hpp"
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

			[[nodiscard]] virtual Compiled::NodeType<RetType> compile(const Context &) const = 0;
			[[nodiscard]] virtual std::string print(const Context &, Step) const = 0;
			[[nodiscard]] virtual RetType eval(const Context &) const = 0;
			[[nodiscard]] virtual std::unique_ptr<interface> clone() const = 0;
			virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			explicit(true) implementation(Fn fn) : fn{fn} {}

			[[nodiscard]] Compiled::NodeType<RetType> compile(const Context &context) const override {
				auto compiled = fn.compile(context);
				return compiled;
			}
			[[nodiscard]] std::string print(const Context &context, Step step) const override {
				return fn.print(context, step);
			}
			[[nodiscard]] RetType eval(const Context &context) const override {
				return fn.eval(context);
			}
			[[nodiscard]] std::unique_ptr<interface> clone() const override {
				return std::make_unique<implementation<Fn>>(fn);
			}

		private:
			Fn fn{};
		};

		template<class T>
		NodeType(const T &t)
			: fn(std::make_unique<implementation<T>>(t)) {
		}

		NodeType(const NodeType &other) {
			if (!other.fn)
				fn.reset();
			else
				fn = other.fn->clone();
		}
		NodeType &operator=(const NodeType &other) {
			if (this != &other) {
				if (!other.fn)
					fn.reset();
				else
					fn = other.fn->clone();
			}
			return *this;
		}
		NodeType &operator=(NodeType &&other) noexcept {
			if (this != &other) {
				fn = std::move(other.fn);
			}
			return *this;
		}
		NodeType(NodeType &&other) : fn(std::move(other.fn)) {}

		[[nodiscard]] Compiled::NodeType<RetType> compile(const Context &context) const {
			return fn->compile(context);
		}
		[[nodiscard]] std::string print(const Context &context, Step step = Step::none) const {
			return fn->print(context, step);
		}
		[[nodiscard]] RetType eval(const Context &context) const {
			return fn->eval(context);
		}

		NodeType() = default;

		~NodeType() = default;

		[[nodiscard]] bool hasValue() const {
			return fn != nullptr;
		}

	private:
		std::unique_ptr<interface> fn{};
	};

	using FloatNode = NodeType<float>;
	using BoolNode = NodeType<bool>;
	using IntNode = NodeType<int32_t>;
	using ElementNode = NodeType<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula