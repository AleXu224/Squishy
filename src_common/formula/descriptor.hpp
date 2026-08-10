#pragma once

#include "context.hpp"// IWYU pragma: keep
#include "percentage.hpp"
#include "widgets/richText.hpp"
#include <list>
#include <string>
#include <vector>


namespace Formula {
	using Style = squi::RichText::Style;
	using Span = squi::RichText::Span;
	struct Descriptor {
		struct Value {
			std::string value;

			Value() : value() {}
			Value(std::string value) : value(std::move(value)) {}
			Value(std::integral auto value) : value(std::to_string(value)) {}
			Value(std::floating_point auto value, bool percentage) : value(Percentage({}, value, percentage)) {}
		};
		struct Parameter;
		std::string nameBuffer;
		std::optional<squi::Color> color;

		std::list<Span> spans;
		std::list<Parameter> parameters;

		[[nodiscard]] std::string toString() const;

		void pushPrefix(const std::string &prefix);
		void pushColor(const squi::Color &color);

		void addName(const std::string &name);
		void addName(const Style &name);

		void addValue(const Value &value);

		void add(const std::string &name, const Value &value);
		void add(const Style &name, const Value &value);

		void add(const std::string &name, const Value &value, const std::optional<Descriptor> &parameter);
		void add(const Style &name, const Value &value, const std::optional<Descriptor> &parameter);
	};


	struct Descriptor::Parameter {
		std::string name;
		Descriptor formula;
	};

	inline std::string Descriptor::toString() const {
		std::string ret;
		for (const auto &span: spans) {
			if (const auto *text = std::get_if<std::string>(&span)) {
				ret += *text;
			} else {
				ret += std::get<Style>(span).text;
			}
		}
		return ret;
	}

	inline void Descriptor::pushPrefix(const std::string &prefix) {
		nameBuffer += prefix;
	}

	inline void Descriptor::pushColor(const squi::Color &color) {
		this->color = color;
	}

	inline void Descriptor::addName(const std::string &name) {
		addName(Style{.text = name});
	}

	inline void Descriptor::addName(const Style &name) {
		spans.push_back(Style{
			.color = name.color.value_or(color.value_or(squi::Color::white)),
			.font = name.font.value_or(squi::FontStore::getFont(squi::FontStore::defaultFontBold)),
			.text = nameBuffer + name.text,
		});
		nameBuffer.clear();
		color = std::nullopt;
	}

	inline void Descriptor::addValue(const Value &value) {
		if (!value.value.empty())
			spans.push_back(value.value);
	}

	inline void Descriptor::add(const std::string &name, const Value &value) {
		add(Style{.text = name}, value, std::nullopt);
	}

	inline void Descriptor::add(const Style &name, const Value &value) {
		add(name, value, std::nullopt);
	}

	inline void Descriptor::add(const std::string &name, const Value &value, const std::optional<Descriptor> &parameter) {
		add(Style{.text = name}, value, parameter);
	}

	inline void Descriptor::add(const Style &name, const Value &value, const std::optional<Descriptor> &parameter) {
		auto newName = std::format("{}{} ", nameBuffer, name.text);
		nameBuffer.clear();
		spans.push_back(Style{
			.color = name.color.value_or(color.value_or(squi::Color::white)),
			.font = name.font.value_or(squi::FontStore::getFont(squi::FontStore::defaultFontBold)),
			.text = newName,
		});
		color = std::nullopt;

		if (!value.value.empty())
			spans.push_back(value.value);

		if (parameter) {
			this->parameters.emplace_back(std::move(newName), std::move(*parameter));
		}
	}
}// namespace Formula
