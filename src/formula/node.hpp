#pragma once

#include "string"

#include "atomic"
#include "compiled/node.hpp"
#include "cstdint"
#include "formula/formulaContext.hpp"
#include "misc/element.hpp"
#include "step.hpp"
#include "utility"
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

			void retain() noexcept {
				refCount.fetch_add(1, std::memory_order_relaxed);
			}

			[[nodiscard]] bool release() noexcept {
				return refCount.fetch_sub(1, std::memory_order_acq_rel) == 1;
			}

			[[nodiscard]] virtual Compiled::NodeType<RetType> compile(const Context &) const = 0;
			[[nodiscard]] virtual std::string print(const Context &, Step) const = 0;
			[[nodiscard]] virtual RetType eval(const Context &) const = 0;
			virtual ~interface() = default;

		private:
			std::atomic<uint32_t> refCount{1};
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

		private:
			Fn fn{};
		};

		template<class T>
		NodeType(const T &t)
			: fn(new implementation<T>(t)) {
		}

		NodeType(const NodeType &other) : fn(other.fn) {
			if (fn) fn->retain();
		}
		NodeType &operator=(const NodeType &other) {
			if (this != &other) {
				release();
				fn = other.fn;
				if (fn) fn->retain();
			}
			return *this;
		}
		NodeType(NodeType &&other) noexcept : fn(std::exchange(other.fn, nullptr)) {}
		NodeType &operator=(NodeType &&other) noexcept {
			if (this != &other) {
				release();
				fn = std::exchange(other.fn, nullptr);
			}
			return *this;
		}

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

		~NodeType() {
			release();
		}

		[[nodiscard]] bool hasValue() const {
			return fn != nullptr;
		}

	private:
		void release() noexcept {
			if (fn && fn->release()) {
				delete fn;
			}
			fn = nullptr;
		}

		interface *fn{};
	};

	using FloatNode = NodeType<float>;
	using BoolNode = NodeType<bool>;
	using IntNode = NodeType<int32_t>;
	using ElementNode = NodeType<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula