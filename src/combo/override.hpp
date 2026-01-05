#pragma once
#include "option.hpp"


namespace Combo {
	struct Overrides {
		std::unordered_map<uint64_t, Option> overrides{};
		mutable const Overrides *parent = nullptr;

		void push(const Option &option) {
			overrides.insert_or_assign(Utils::hashCombine(option.key.key, option.hash), option);
		}

		void push(const std::vector<Option> &options) {
			for (const auto &option: options) {
				push(option);
			}
		}

		Overrides merge(const Overrides &other) const {
			Overrides ret = *this;
			for (const auto &[hash, option]: other.overrides) {
				ret.overrides.insert_or_assign(hash, option);
			}
			return ret;
		}

		[[nodiscard]] inline bool empty() const {
			return overrides.empty() && (parent == nullptr || parent->empty());
		}

		std::optional<std::reference_wrapper<const Option>> getOption(std::same_as<uint64_t> auto hash) const {
			if (auto it = overrides.find(hash); it != overrides.end()) {
				return std::ref(it->second);
			} else if (parent != nullptr) {
				return parent->getOption(hash);
			} else {
				return std::nullopt;
			}
		}
	};
}// namespace Combo