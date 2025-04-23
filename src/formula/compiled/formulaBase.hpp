#pragma once

#include "node.hpp"
#include "type.hpp"

namespace Formula::Compiled {
	template<class T, Type type = Type::none>
	struct FormulaBase {
		[[nodiscard]] Type getType() const {
			return type;
		}

		[[nodiscard]] NodeType<T> wrap(this auto &&self) {
			return NodeType<T>(std::move(self));
		}

		operator NodeType<T>(this auto &self) {
			// static_assert(std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<decltype(self)>>, "Node type and eval return type must be the same!");
			return NodeType<T>(std::move(self));
		}
	};
}// namespace Formula::Compiled