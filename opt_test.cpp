#include "agent/agents.hpp"
#include "agent/instance.hpp"
#include "combo/source.hpp"
#include "disc/sets.hpp"
#include "enemy/instance.hpp"
#include "engine/engines.hpp"
#include "optimization/optimize.hpp"
#include "optimization/options.hpp"
#include "optimization/solution.hpp"
#include "store.hpp"
#include "team/instance.hpp"
#include "utils/storageFolder.hpp"
#include <chrono>
#include <print>

using namespace std::chrono;

int main() {
	std::println("Init...");
	Engine::initEngines();
	Agent::initAgents();
	Disc::initDiscs();

	std::println("Loading save...");
	Store::loadFromFile((Utils::getStorageFolder().value() / "Squishy" / "save_zzz.sqsh").string());

	Store::enemies.insert({
		0,
		Enemy::Instance{
			.key = 0,
			.name = "E",
			.stats{.sheet{.level{.constant = 70.f}, .stunMod{.constant = 1.f}, .baseDef{.constant = 60.f}}},
		},
	});

	std::println("Finding Miyabi...");
	Agent::DataKey miyabiKey{1091};
	Agent::Instance *miyabi = nullptr;
	for (auto &[k, a]: Store::agents) {
		if (a.dataKey == miyabiKey) {
			miyabi = &a;
			break;
		}
	}
	if (!miyabi) {
		std::println("Miyabi not found!");
		return 1;
	}
	std::println("Found, {} combos, {} discs", miyabi->combos.size(), Store::discs.size());

	auto &team = [&]() -> Team::Instance & {
		for (auto &[_, t]: Store::teams) {
			for (auto &agentPtr: t.stats.agents) {
				if (agentPtr == miyabi) return t;
			}
		}
		std::println("Miyabi not found in any team, using default team");
		return Store::defaultTeam;
	}();
	auto &enemy = Store::enemies.at(0);

	Formula::Context ctx{
		.source = miyabi->state,
		.origin = miyabi->state,
		.team = team.stats,
		.enemy = enemy.stats,
	};

	Formula::FloatNode formula;
	if (!miyabi->combos.empty()) {
		auto &c = miyabi->combos.begin()->second;
		formula = Combo::Source::Combo{.agentKey = miyabi->instanceKey, .comboKey = c.instanceKey}.resolve({}).formula;
	} else {
		formula = miyabi->state.stats.data.data->nodes.exspecial.at(0).formula;
	}

	Optimization::Options opts{};
	opts.useEquippedDiscs = false;
	Optimization::Optimization opt{.agent = *miyabi, .ctx = ctx, .optimizedNode = formula, .options = opts};

	std::println("Optimizing...");
	auto t1 = high_resolution_clock::now();
	auto sol = opt.optimize();
	auto t2 = high_resolution_clock::now();
	std::println("Best: {}, Time: {}ms", sol.maxScore, duration_cast<milliseconds>(t2 - t1).count());

	for (const auto &s: sol.solutions) {
		std::println("Solution dmg: {}, {} {} {} {} {} {}", s.score, s.discs.one.key, s.discs.two.key, s.discs.three.key, s.discs.four.key, s.discs.five.key, s.discs.six.key);
	}
	return 0;
}
