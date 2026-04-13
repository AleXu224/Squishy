#pragma once

#include "constant.hpp"// IWYU pragma: keep
#include "fold.hpp"
#include "formula/context.hpp"// IWYU pragma: keep
#include "formula/type.hpp"
#include "memory"
#include <algorithm>
#include <memory_resource>


namespace Formula {
	thread_local inline bool enableAllocator = false;
	struct NodeAllocator {
		static inline std::pmr::monotonic_buffer_resource &getBuffer() {
			thread_local std::pmr::monotonic_buffer_resource mbr;
			return mbr;
		}

		static inline std::pmr::polymorphic_allocator<std::byte> &get() {
			thread_local std::pmr::polymorphic_allocator<std::byte> allocator{&getBuffer()};
			return allocator;
		}

		template<class T, class... Args>
		static inline T *allocate(Args &&...args) {
			if (enableAllocator) {
				return get().new_object<T>(std::forward<Args>(args)...);
			}
			return new T(std::forward<Args>(args)...);
		}

		template<class T>
		static inline void deallocate(T *ptr) {
			if (enableAllocator) {
				std::destroy_at(ptr);
				get().deallocate(std::bit_cast<std::byte *>(ptr), sizeof(T));
			} else {
				delete ptr;
			}
		}

		static inline void reset() {
			getBuffer().release();
		}
	};

	template<class T, class V>
	concept IsConvertibleNodeType = std::is_same_v<NodeType<typename std::remove_cvref_t<T>::RetType>, std::remove_cvref_t<T>>
								 && !std::is_same_v<typename std::remove_cvref_t<T>::RetType, V>
								 && std::is_convertible_v<typename std::remove_cvref_t<T>::RetType, V>;

	template<class T>
	concept isMonomial = requires(T t) {
		t.value;
		{ t.add(std::declval<typename T::RetType>()) };
		{ t.subtract(std::declval<typename T::RetType>()) };
		{ t.mult(std::declval<typename T::RetType>()) };
		{ t.divide(std::declval<typename T::RetType>()) };
	};

	template<class T, class Ret>
	concept isSumMonomial = std::is_same_v<std::remove_cvref_t<decltype(std::declval<T>().sumParam)>, Ret>;
	template<class T, class Ret>
	concept isProdMonomial = std::is_same_v<std::remove_cvref_t<decltype(std::declval<T>().multParam)>, Ret>;

	template<class TT>
	struct NodeType {
		using RetType = TT;

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

			[[nodiscard]] constexpr virtual RetType eval(const Context &) const = 0;
			[[nodiscard]] constexpr virtual NodeType fold(const Context &, const FoldArgs &) const = 0;
			[[nodiscard]] constexpr virtual Formula::Type getType() const = 0;
			[[nodiscard]] constexpr virtual interface *clone() const = 0;
			[[nodiscard]] constexpr virtual RetType getConstantValue() const = 0;
			[[nodiscard]] constexpr virtual std::string print(const Context &, Step) const = 0;

			[[nodiscard]] constexpr virtual NodeType add(RetType) const = 0;
			[[nodiscard]] constexpr virtual NodeType subtract(RetType) const = 0;
			[[nodiscard]] constexpr virtual NodeType mult(RetType) const = 0;
			[[nodiscard]] constexpr virtual NodeType divide(RetType) const = 0;
			[[nodiscard]] constexpr virtual RetType getSumParam() const = 0;
			[[nodiscard]] constexpr virtual RetType getMultParam() const = 0;
			[[nodiscard]] constexpr virtual NodeType getMonomialValue() const = 0;
			constexpr virtual ~interface() = default;

		private:
			std::atomic<uint32_t> refCount{1};
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}

			[[nodiscard]] constexpr RetType eval(const Context &context) const override {
				return fn.eval(context);
			}
			[[nodiscard]] constexpr NodeType fold(const Formula::Context &context, const FoldArgs &args) const override {
				return fn.fold(context, args);
			}
			[[nodiscard]] constexpr Formula::Type getType() const override {
				return fn.getType();
			}
			[[nodiscard]] constexpr RetType getConstantValue() const override {
				if constexpr (HasConstantValue<Fn, RetType>) {
					return fn.value;
				} else {
					return {};
				}
			}
			[[nodiscard]] constexpr std::string print(const Context &context, Step prevStep) const override {
				return fn.print(context, prevStep);
			}
			[[nodiscard]] constexpr NodeType add(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.add(val);
				} else {
					return static_cast<const interface *>(this);
				}
			}
			[[nodiscard]] constexpr NodeType subtract(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.subtract(val);
				} else {
					return static_cast<const interface *>(this);
				}
			}
			[[nodiscard]] constexpr NodeType mult(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.mult(val);
				} else {
					return static_cast<const interface *>(this);
				}
			}
			[[nodiscard]] constexpr NodeType divide(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.divide(val);
				} else {
					return static_cast<const interface *>(this);
				}
			}
			[[nodiscard]] constexpr RetType getSumParam() const override {
				if constexpr (isSumMonomial<Fn, RetType>) {
					return fn.sumParam;
				} else {
					return {};
				}
			}
			[[nodiscard]] constexpr RetType getMultParam() const override {
				if constexpr (isProdMonomial<Fn, RetType>) {
					return fn.multParam;
				} else {
					return {};
				}
			}
			[[nodiscard]] constexpr NodeType getMonomialValue() const override {
				if constexpr (isMonomial<Fn>) {
					return std::move(fn.value);
				} else {
					return static_cast<const interface *>(this);
				}
			}
			[[nodiscard]] constexpr interface *clone() const override {
				return NodeAllocator::allocate<implementation<Fn>>(fn);
			}

		private:
			Fn fn{};
		};

		NodeType(const interface *fn) : fn(const_cast<interface *>(fn)) {
			if (this->fn) this->fn->retain();
		}

		template<class T>
		constexpr NodeType(const T &t)
			: fn(NodeAllocator::allocate<implementation<T>>(t)) {}

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

		NodeType clone() const {
			if (fn) {
				return NodeType(fn->clone());
			}
			return {};
		}

		[[nodiscard]] constexpr RetType eval(const Context &context) const {
			return fn->eval(context);
		}
		[[nodiscard]] constexpr NodeType fold(const Context &context, const FoldArgs &args) const {
			return fn->fold(context, args);
		}
		[[nodiscard]] constexpr Formula::Type getType() const {
			return fn->getType();
		}
		[[nodiscard]] constexpr RetType getConstantValue() const {
			return fn->getConstantValue();
		}
		[[nodiscard]] constexpr std::string print(const Context &context, Step prevStep = Step::none) const {
			return fn->print(context, prevStep);
		}
		[[nodiscard]] constexpr NodeType add(RetType value) const {
			return fn->add(value);
		}
		[[nodiscard]] constexpr NodeType subtract(RetType value) const {
			return fn->subtract(value);
		}
		[[nodiscard]] constexpr NodeType mult(RetType value) const {
			return fn->mult(value);
		}
		[[nodiscard]] constexpr NodeType divide(RetType value) const {
			return fn->divide(value);
		}
		[[nodiscard]] constexpr RetType getSumParam() const {
			return fn->getSumParam();
		}
		[[nodiscard]] constexpr RetType getMultParam() const {
			return fn->getMultParam();
		}
		[[nodiscard]] constexpr NodeType getMonomialValue() const {
			return fn->getMonomialValue();
		}

		constexpr NodeType() = default;

		constexpr ~NodeType() {
			release();
		}

		[[nodiscard]] constexpr bool hasValue() const {
			return fn != nullptr;
		}

	private:
		void release() noexcept {
			if (fn && fn->release()) {
				NodeAllocator::deallocate(fn);
			}
			fn = nullptr;
		}

		interface *fn{};
	};

	template<class V>
		requires requires { typename std::remove_cvref_t<V>::RetType; }
	NodeType(V &&) -> NodeType<typename std::remove_cvref_t<V>::RetType>;

	using FloatNode = NodeType<float>;
	using BoolNode = NodeType<bool>;
	using IntNode = NodeType<int32_t>;
}// namespace Formula