import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import { camelCase } from "https://deno.land/x/case@2.2.0/mod.ts";
import { statMap } from "./mapped.ts";
import lowerCase from "https://deno.land/x/case@2.2.0/lowerCase.ts";
import { NanokaAgent, SkillData } from "./agentTypeNanoka.d.ts";
import { ManifestNanoka } from "../manifestNanoka.d.ts";

if (Deno.args[0] == undefined) {
    console.error("Usage: agentGenerator.ts <characer id>\neg: deno run .\\scripts\\zzz\\agentGenerator.ts 1401");
    Deno.exit(1);
}

try {
    const cmakeFile = Deno.openSync("./CMakeLists.txt");
    cmakeFile.close();
} catch (_error) {
    console.error("You must run this script from the project root directory (where main.cpp and CMakeLists.txt reside)");
    Deno.exit(1);
}

const manifestNanokaResponse = await fetch("https://static.nanoka.cc/manifest.json");
if (!manifestNanokaResponse.ok) {
    console.error(`Failed to get the manifest with code ${manifestNanokaResponse.status}, "${manifestNanokaResponse.statusText}"`);
    Deno.exit(1);
}
const manifestNanoka: ManifestNanoka = await manifestNanokaResponse.json();

const responseNanoka = await fetch(`https://static.nanoka.cc/zzz/${manifestNanoka.zzz.latest}/en/character/${Deno.args[0]}.json`);
if (!responseNanoka.ok) {
    console.error(`Response failed with code ${responseNanoka.status}, "${responseNanoka.statusText}"`);
}
const contentsNanoka: NanokaAgent = await responseNanoka.json();

const data = {
    key: Deno.args[0],
    name: contentsNanoka.name,
    caseableName: contentsNanoka.name.replaceAll("'", ""),
    coreStat1: statMap.get(contentsNanoka.extra_level["1"].extra[Object.keys(contentsNanoka.extra_level["1"].extra)[0]].name),
    coreStat2: statMap.get(contentsNanoka.extra_level["1"].extra[Object.keys(contentsNanoka.extra_level["1"].extra)[1]].name),
    icon: `https://static.nanoka.cc/assets/zzz/${contentsNanoka.partner_info.icon_path.split("/").pop()?.replace(".png", ".webp")}`,
    iconCard: `https://static.nanoka.cc/assets/zzz/${contentsNanoka.icon}.webp`,
};

const promotionUpgradeHp = new Float32Array(6);
const promotionUpgradeDef = new Float32Array(6);
const promotionUpgradeAtk = new Float32Array(6);
const coreStat1Upgrade = new Float32Array(6);
const coreStat2Upgrade = new Float32Array(6);
let i = 0;
for (const value of Object.values(contentsNanoka.level)) {
    promotionUpgradeHp[i] = value.hp_max;
    promotionUpgradeDef[i] = value.defence;
    promotionUpgradeAtk[i] = value.attack;
    i++;
}
i = 0;
for (const value of Object.values(contentsNanoka.extra_level)) {
    coreStat1Upgrade[i] = Object.values(value.extra)[0].value as number;
    coreStat2Upgrade[i] = Object.values(value.extra)[1].value as number;
    i++;
}

function strGenerator(entries: Float32Array, precision: number = 3): string[] {
    const ret: string[] = [];
    for (const val of entries) {
        ret.push(val.toFixed(precision));
    }
    return ret;
}

function skillContentsGenerator(skill: SkillData, source: string): string {
    let ret: string = "";
    for (const data of skill.description) {
        if (data.param === undefined) continue;
        for (const param of data.param) {
            if (param.param === undefined) {
                ret += `
                    Node::Info{
                        .name = "${param.name}",
                        .type = Utils::EntryType::energy,
                        .formula = Multiplier(Utils::EntryType::energy, LevelableSkill::${source}, ${param.desc}.f, 0.f)
                    },`;
                continue;
            }
            const value = Object.values(param.param)[0];
            const sourceStr = `\n						.source = Misc::AttackSource::${source},`;
            ret += `
                    Node::Atk{
                        .name = "${param.name}",${sourceStr}
                        .formula = Multiplier(combat.atk, LevelableSkill::${source}, ${(value.main / 10000).toFixed(4)}f, ${(value.growth / 10000).toFixed(4)}f)
                    },`;
        }
    }

    return ret;
}

const retHeader: string = `#pragma once

#include "agent/data.hpp"

namespace Agent::Datas {
    const extern Agent::Data ${camelCase(data.caseableName)};
}// namespace Agent::Datas
`;

const retSource: string = `#include "${pascalCase(data.caseableName)}.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::${camelCase(data.caseableName)}{
    .key{${data.key}},
    .goodKey{"${pascalCase(data.caseableName)}"},
    .name{"${data.name}"},
    .baseStats{
        .baseHp = ${contentsNanoka.stats.hp_max},
        .baseAtk = ${contentsNanoka.stats.attack},
        .baseDef = ${contentsNanoka.stats.defence},
        .baseImpact = ${contentsNanoka.stats.break_stun},
        .baseAm = ${contentsNanoka.stats.element_abnormal_power},
        .baseAp = ${contentsNanoka.stats.element_mystery},
        .coreStat1 = Stat::${data.coreStat1},
        .coreStat2 = Stat::${data.coreStat2},
        .rarity = ${contentsNanoka.rarity + 1},
        .attribute = Misc::Attribute::${lowerCase(Object.values(contentsNanoka.element_type)[0])},
        .specialty = Misc::Specialty::${lowerCase(Object.values(contentsNanoka.weapon_type)[0])},
        .hpGrowth = ${contentsNanoka.stats.hp_growth / 10000},
        .atkGrowth = ${contentsNanoka.stats.attack_growth / 10000},
        .defGrowth = ${contentsNanoka.stats.defence_growth / 10000},
        .hpUpgrade = {${strGenerator(promotionUpgradeHp).join(", ")}},
        .atkUpgrade = {${strGenerator(promotionUpgradeAtk).join(", ")}},
        .defUpgrade = {${strGenerator(promotionUpgradeDef).join(", ")}},
        .coreStat1Upgrade = {${strGenerator(coreStat1Upgrade).join(", ")}},
        .coreStat2Upgrade = {${strGenerator(coreStat2Upgrade).join(", ")}},
    },
    .setup = []() -> Data::Setup {
        return Data::Setup{
            .mods{},
            .opts{},
            .nodes{
                .basic{${skillContentsGenerator(contentsNanoka.skill.basic, "basic")}
                },
                .dodge{${skillContentsGenerator(contentsNanoka.skill.dodge, "dodge")}
                },
                .assist{${skillContentsGenerator(contentsNanoka.skill.assist, "assist")}
                },
                .special{${skillContentsGenerator(contentsNanoka.skill.special, "special")}
                },
                .chain{${skillContentsGenerator(contentsNanoka.skill.chain, "chain")}
                },
            },
        };
    },
};
`;

console.log(retHeader);
console.log(retSource);

console.log(data.icon);

Deno.mkdirSync(`./assets/Agents/${data.name}`, { recursive: true });

const avatarResponse = await fetch(data.icon);
if (!avatarResponse.ok) {
    console.log(`Failed getting the icon with code ${avatarResponse.status}, "${avatarResponse.statusText}" (${data.icon})`);
}
Deno.writeFileSync(`./assets/Agents/${data.name}/avatar.webp`, await avatarResponse.bytes());

const cardResponse = await fetch(data.iconCard);
if (!cardResponse.ok) {
    console.log(`Failed getting the card icon with code ${cardResponse.status}, "${cardResponse.statusText}" (${data.iconCard})`);
}
Deno.writeFileSync(`./assets/Agents/${data.name}/card.webp`, await cardResponse.bytes());


Deno.writeFileSync(`./src_zzz/agent/agents/${pascalCase(data.caseableName)}.hpp`, new TextEncoder().encode(retHeader));
Deno.writeFileSync(`./src_zzz/agent/agents/${pascalCase(data.caseableName)}.cpp`, new TextEncoder().encode(retSource));