#include "option.hpp"

std::vector<Serialization::Save::OptionTypes> Serialization::Save::optionsFromInstance(const Option::TypesMap &options) {
	std::vector<Serialization::Save::OptionTypes> ret{};
	for (const auto &[key, option]: options) {
		std::visit(
			Utils::overloaded{
				[&](const Option::Boolean &opt) {
					ret.emplace_back(Serialization::Save::BooleanOption{
						.hash = key,
						.active = opt.active,
					});
				},
				[&](const Option::ValueList &opt) {
					ret.emplace_back(Serialization::Save::ValueListOption{
						.hash = key,
						.index = opt.currentIndex,
					});
				},
			},
			option
		);
	}
	return ret;
}

void Serialization::Save::optionsToInstance(const std::vector<OptionTypes> &options, Option::TypesMap &target) {
	for (const auto &option: options) {
		std::visit(
			Utils::overloaded{
				[&](const Serialization::Save::BooleanOption &opt) {
					if (!target.contains(opt.hash)) return;
					std::visit(
						Utils::overloaded{
							[&](Option::Boolean &val) {
								val.active = opt.active;
							},
							[](const Option::ValueList &val) {
								std::println("Wrong opt selected while loading save {}", val.key.str);
							},
						},
						target.at(opt.hash)
					);
				},
				[&](const Serialization::Save::ValueListOption &opt) {
					if (!target.contains(opt.hash)) return;
					std::visit(
						Utils::overloaded{
							[](const Option::Boolean &val) {
								std::println("Wrong opt selected while loading save {}", val.key.str);
							},
							[&](Option::ValueList &val) {
								val.currentIndex = opt.index;
							},
						},
						target.at(opt.hash)
					);
				},
			},
			option
		);
	}
}
