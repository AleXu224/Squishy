#pragma once

#include "fmt/core.h"
#include "option/option.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	struct impl_IsActive {
		Utils::HashedString name;

		[[nodiscard]] Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", name.str);
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return ::Option::getBool(context.source.options, name);
		}
	};

	struct impl_IsActivePassive {
		Utils::HashedString name;

		[[nodiscard]] Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", name.str);
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return ::Option::getBool(context.team.options, name);
		}
	};

	struct impl_GetFloat {
		Utils::HashedString name;
		float defaultValue = 0.f;

		[[nodiscard]] Compiled::FloatNode compile(const Context &context) const {
			return Compiled::ConstantFloat{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] float eval(const Context &context) const {
			return ::Option::getFloat(context.source.options, name, defaultValue);
		}
	};

	struct impl_GetInt {
		Utils::HashedString name;
		uint32_t defaultValue = 0.f;

		[[nodiscard]] Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			return ::Option::getInt(context.source.options, name, defaultValue);
		}
	};

	struct impl_GetIndex {
		Utils::HashedString name;
		uint32_t defaultValue = 0;

		[[nodiscard]] Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			return ::Option::getIndex(context.source.options, name, defaultValue);
		}
	};
}// namespace Formula