#pragma once

#include "disc/key.hpp"
#include "disc/set.hpp"
#include "disc/slot.hpp"
#include "stats/discSheet.hpp"
#include "utility"


namespace Stats {
	struct DiscBonus {
		const ::Disc::Set *setPtr;
		const ::Disc::SetBonus *bonusPtr;
	};

	struct Disc {
		// These slots contain the necessary data to correctly identify which modifiers should be applied
		// Sadly zenless has the possibility of having three 2pc bonuses
		std::optional<DiscBonus> bonus1{};
		std::optional<DiscBonus> bonus2{};
		std::optional<DiscBonus> bonus3{};

		DiscSheet sheet{};

		struct Slotted {
			::Disc::InstanceKey one;
			::Disc::InstanceKey two;
			::Disc::InstanceKey three;
			::Disc::InstanceKey four;
			::Disc::InstanceKey five;
			::Disc::InstanceKey six;

			[[nodiscard]] static auto getMembers() {
				return std::array{
					&Slotted::one,
					&Slotted::two,
					&Slotted::three,
					&Slotted::four,
					&Slotted::five,
					&Slotted::six,
				};
			}

			[[nodiscard]] auto &fromPartition(this auto &&self, ::Disc::Partition partition) {
				switch (partition) {
					case ::Disc::Partition::one:
						return self.one;
					case ::Disc::Partition::two:
						return self.two;
					case ::Disc::Partition::three:
						return self.three;
					case ::Disc::Partition::four:
						return self.four;
					case ::Disc::Partition::five:
						return self.five;
					case ::Disc::Partition::six:
						return self.six;
				}
				std::unreachable();
			}

			void unequipAll();
		};

		struct Theorycraft {
			struct Set {
				enum class Type : uint8_t {
					twoPc,
					fourPc,
				};

				::Disc::SetKey key;
				Type type = Type::twoPc;
			};
			// FIXME: should be fixed at some point to properly be able to support three 2pc sets
			Set set1;
			::Disc::SetKey set2;
			::Disc::SetKey set3;

			struct MainStat {
				Stat stat;
				uint8_t level = 20;
				uint8_t rarity = 5;
			};
			std::array<MainStat, 6> mainStats{
				MainStat{.stat = Stat::hp},
				MainStat{.stat = Stat::atk},
				MainStat{.stat = Stat::def},
				MainStat{.stat = Stat::atk_},
				MainStat{.stat = Stat::atk_},
				MainStat{.stat = Stat::atk_},
			};
			uint8_t hp = 0;
			uint8_t hp_ = 0;
			uint8_t atk = 0;
			uint8_t atk_ = 0;
			uint8_t def = 0;
			uint8_t def_ = 0;
			uint8_t ap = 0;
			uint8_t pen = 0;
			uint8_t cr = 0;
			uint8_t cd = 0;

			Stats::Sheet<float> sheet{};

			[[nodiscard]] auto &fromStat(this auto &self, ::Stat stat) {
				switch (stat) {
					case Stat::hp:
						return self.hp;
					case Stat::hp_:
						return self.hp_;
					case Stat::atk:
						return self.atk;
					case Stat::atk_:
						return self.atk_;
					case Stat::def:
						return self.def;
					case Stat::def_:
						return self.def_;
					case Stat::ap:
						return self.ap;
					case Stat::pen:
						return self.pen;
					case Stat::cr:
						return self.cr;
					case Stat::cd:
						return self.cd;
					default:
						return self.hp;
				}
			}

			[[nodiscard]] Stats::Sheet<float> getSheet() const {
				Stats::Sheet<float> ret{};
				for (const auto &stat: subStats) {
					ret.fromStat(stat) = static_cast<float>(fromStat(stat)) * Values::subStat.at(stat).fiveStar;
				}
				for (const auto &mainStat: mainStats) {
					ret.fromStat(mainStat.stat) += Values::mainStat.at(mainStat.stat).at(mainStat.rarity, mainStat.level);
				}

				return ret;
			}

			void updateStats() {
				this->sheet = getSheet();
			}

			[[nodiscard]] std::pair<::Disc::SetKey, ::Disc::SetKey> getSets() const {
				::Disc::SetKey twoPc = this->set1.key;
				::Disc::SetKey fourPc = this->set1.type == Set::Type::fourPc ? this->set1.key : this->set2;
				return {twoPc, fourPc};
			}
		};

		std::variant<Slotted, Theorycraft> equipped{};

		Slotted &getSlotted();
		const Slotted &getSlotted() const;
		Theorycraft &getTheorycraft();
		const Theorycraft &getTheorycraft() const;

		bool isTheorycraft() const;

		static inline std::vector subStats{
			Stat::hp,
			Stat::hp_,
			Stat::atk,
			Stat::atk_,
			Stat::def,
			Stat::def_,
			Stat::ap,
			Stat::pen,
			Stat::cr,
			Stat::cd,
		};

		static inline std::vector partition1{
			Stat::hp,
		};
		static inline std::vector partition2{
			Stat::atk,
		};
		static inline std::vector partition3{
			Stat::def,
		};
		static inline std::vector partition4{
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::cr,
			Stat::cd,
			Stat::ap,
		};
		static inline std::vector partition5{
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::penRatio,
			Stat::physicalDmg,
			Stat::fireDmg,
			Stat::iceDmg,
			Stat::electricDmg,
			Stat::etherDmg,
		};
		static inline std::vector partition6{
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::am_,
			Stat::impact_,
			Stat::er,
		};

		static inline std::vector mainStats{
			Stat::hp,
			Stat::atk,
			Stat::def,
			Stat::hp_,
			Stat::atk_,
			Stat::def_,
			Stat::er,
			Stat::am_,
			Stat::impact_,
			Stat::er,
			Stat::cr,
			Stat::cd,
			Stat::ap,
			Stat::penRatio,
			Stat::physicalDmg,
			Stat::fireDmg,
			Stat::iceDmg,
			Stat::electricDmg,
			Stat::etherDmg,
		};

		[[nodiscard]] static const std::vector<Stat> &byPartition(::Disc::Partition partition) {
			switch (partition) {
				case ::Disc::Partition::one:
					return partition1;
				case ::Disc::Partition::two:
					return partition2;
				case ::Disc::Partition::three:
					return partition3;
				case ::Disc::Partition::four:
					return partition4;
				case ::Disc::Partition::five:
					return partition5;
				case ::Disc::Partition::six:
					return partition6;
			}
			std::unreachable();
		}

		void refreshStats();
	};
}// namespace Stats

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Stats::Disc::Theorycraft::Set::Type &type) {
		switch (type) {
			case Stats::Disc::Theorycraft::Set::Type::twoPc:
				return "2pc";
			case Stats::Disc::Theorycraft::Set::Type::fourPc:
				return "4pc";
		}
		std::unreachable();
	}
}// namespace Utils