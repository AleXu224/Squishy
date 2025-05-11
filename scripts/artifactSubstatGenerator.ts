import { ArtifactSubstatData } from "./artifactSubstatData.d.ts";
import { propToStat } from "./mapped.ts";

const response = await fetch(`https://gitlab.com/Dimbreath/AnimeGameData/-/raw/master/ExcelBinOutput/ReliquaryAffixExcelConfigData.json`);
if (!response.ok) {
    console.error(`Response failed with code ${response.status}, "${response.statusText}"`);
}
const contents: ArtifactSubstatData[] = await response.json();

type SubstatsByPropType = Record<string, Record<number, number[]>>;
const data: SubstatsByPropType = {};

function depotIdToRarity(id: number): number | null {
    switch (id) {
        case 101: return 1;
        case 201: return 2;
        case 301: return 3;
        case 401: return 4;
        case 501: return 5;
        default: return null;
    }
}
for (const content of contents) {
    const rarity = depotIdToRarity(content.depotId);
    if (!rarity) continue;

    const propType = content.propType;
    const value = content.propValue;

    // Ensure the propType entry exists
    if (!data[propType]) {
        data[propType] = {};
    }

    // Ensure the rarity entry exists for this propType
    if (!data[propType][rarity]) {
        data[propType][rarity] = [];
    }

    data[propType][rarity].push(value);
}

let ret: string = "";
ret += `inline const std::unordered_map<Stat, SubstatData> subStat{\n`;
for (const [prop, propData] of Object.entries(data)) {
    ret += `\t{Stat::${propToStat.get(prop)}, SubstatData{`;
    for (const [_rarity, rarityData] of Object.entries(propData)) {
        const levelData: string[] = [];
        for (const value of rarityData) {
            levelData.push(value.toFixed(4) + "f");
        }
        ret += `std::array{${levelData.join(', ')}},`;
    }
    ret += `}},\n`;
}
ret += `};`

let ret2: string = "";
ret2 += `inline const std::unordered_map<Stat, std::array<float, 5>> averageSubStat{\n`;
for (const [prop, propData] of Object.entries(data)) {
    ret2 += `\t{Stat::${propToStat.get(prop)}, std::array{`;
    for (const [_rarity, rarityData] of Object.entries(propData)) {
        let total: number = 0;
        for (const value of rarityData) {
            total += value;
        }
        total /= rarityData.length;
        ret2 += total.toFixed(4) + "f,";
    }
    ret2 += `}},\n`;
}
ret2 += `};`

console.log(ret);
console.log(ret2);
// console.log(data);