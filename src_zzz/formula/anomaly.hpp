#pragma once

#include "agent/data.hpp"
#include "agent/instance.hpp"
#include "anomaly/list.hpp"
#include "formula/teamAgent.hpp"
#include "node/abloomNode.hpp"
#include "node/node.hpp"
#include "node/polarityDisorder.hpp"
#include "stats/team.hpp"


namespace Formula {
	struct CharacterName : FormulaBase<std::string, Type::constant> {
		size_t index;
		std::string suffix;

		void print(Descriptor &descriptor, const Context &context, Step) const {
			descriptor.addValue(eval(context));
		}

		[[nodiscard]] std::string eval(const Context &context) const {
			auto &character = context.team.agents.at(index);
			if (!character) return "None";
			return std::format("{} {}", character->state.stats.data.name, suffix);
		}
	};

	/**
	 * Adds Abloom nodes to the given vector of nodes.
	 * Multipliers must be in the order of Misc::attributes
	 */
	inline void addAblooms(std::array<FloatNode, 6> multipliers, std::vector<Node::Types> &nodes, const std::string &suffix = "Abloom", const Formula::Modifier &modifier = {}) {
		for (size_t i = 0; i < 3; i++) {
			nodes.emplace_back(Node::AbloomNode{
				.name = CharacterName{
					.index = i,
					.suffix = suffix,
				},
				.attribute = TeamAgent{
					.index = i,
					.formula = AgentAttribute{},
				},
				.multiplier = Evaluator{
					.evaluated = Index{
						.index = IndexOf{
							.range = Misc::attributes,
							.formula = TeamAgent{
								.index = i,
								.formula = AgentAttribute{},
							},
						},
						.indexable = multipliers,
					},
				},
				.index = i,
				.modifier = modifier,
			});
		}
	}

	inline void addPolarityDisorders(FloatNode multiplier, std::vector<Node::Types> &nodes) {
		for (size_t i = 0; i < 3; i++) {
			nodes.emplace_back(Node::PolarityDisorder{
				.name = CharacterName{
					.index = i,
					.suffix = "Polarity Disorder",
				},
				.attribute = TeamAgent{
					.index = i,
					.formula = AgentAttribute{},
				},
				.multiplier = multiplier,
				.index = i,
			});
		}
	}

	template<AttributeFormula T>
	struct AnomalyFromAttribute : FormulaBase<float> {
		T attribute;

		[[nodiscard]] auto fold(const Context &context, const FoldArgs &args) const {
			auto foldedAttribute = attribute.fold(context, args);

			if (foldedAttribute.getType() == Type::constant) {
				return Anomaly::List::fromAttribute(foldedAttribute.eval(context)).formula.fold(context, args);
			}

			return AnomalyFromAttribute<decltype(foldedAttribute)>{
				.attribute = foldedAttribute,
			};
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			auto attribute = this->attribute.eval(context);
			Anomaly::List::fromAttribute(attribute).formula.print(descriptor, context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto attribute = this->attribute.eval(context);
			return Anomaly::List::fromAttribute(attribute).formula.eval(context);
		}
	};

	template<AttributeFormula T>
	struct AnomalyAbloomFromAttribute : FormulaBase<float> {
		T attribute;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			auto foldedAttribute = attribute.fold(context, args);

			if (foldedAttribute.getType() == Type::constant) {
				return Anomaly::List::fromAttribute(foldedAttribute.eval(context)).formulaAbloom.fold(context, args);
			}

			return AnomalyAbloomFromAttribute<decltype(foldedAttribute)>{
				.attribute = foldedAttribute,
			};
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			auto attribute = this->attribute.eval(context);
			Anomaly::List::fromAttribute(attribute).formulaAbloom.print(descriptor, context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto attribute = this->attribute.eval(context);
			return Anomaly::List::fromAttribute(attribute).formulaAbloom.eval(context);
		}
	};

	template<AttributeFormula T>
	struct AnomalyVortexFromAttribute : FormulaBase<float> {
		T attribute;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			auto foldedAttribute = attribute.fold(context, args);

			if (foldedAttribute.getType() == Type::constant) {
				return Anomaly::List::fromAttribute(foldedAttribute.eval(context)).formulaVortex.fold(context, args);
			}

			return AnomalyAbloomFromAttribute<decltype(foldedAttribute)>{
				.attribute = foldedAttribute,
			};
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			auto attribute = this->attribute.eval(context);
			Anomaly::List::fromAttribute(attribute).formulaVortex.print(descriptor, context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto attribute = this->attribute.eval(context);
			return Anomaly::List::fromAttribute(attribute).formulaVortex.eval(context);
		}
	};

	template<AttributeFormula T>
	struct AnomalyDisorderFromAttribute : FormulaBase<float> {
		T attribute;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			auto foldedAttribute = attribute.fold(context, args);

			if (foldedAttribute.getType() == Type::constant) {
				return Anomaly::List::fromAttributeDisorder(foldedAttribute.eval(context)).formulaAbloom.fold(context, args);
			}

			return AnomalyAbloomFromAttribute<decltype(foldedAttribute)>{
				.attribute = foldedAttribute,
			};
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			auto attribute = this->attribute.eval(context);
			Anomaly::List::fromAttributeDisorder(attribute).formulaAbloom.print(descriptor, context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto attribute = this->attribute.eval(context);
			return Anomaly::List::fromAttributeDisorder(attribute).formulaAbloom.eval(context);
		}
	};
}// namespace Formula