import { ArtifactMainstatData } from "./artifactMainstatData.d.ts";
import { propToStat } from "./mapped.ts";

const response = await fetch(`https://gitlab.com/Dimbreath/AnimeGameData/-/raw/master/ExcelBinOutput/ReliquaryLevelExcelConfigData.json`);
if (!response.ok) {
    console.error(`Response failed with code ${response.status}, "${response.statusText}"`);
}
const contents: ArtifactMainstatData[] = await response.json();

const data = new Map<
    string, // Prop
    Map<
        number, // rarity
        Float32Array
    >
>();

for (const [key, _value] of propToStat) {
    data.set(key, new Map<number, Float32Array>());
}
for (const content of contents) {
    if (content.rank === undefined) continue;
    const level: number = content.level - 1;
    const rarity = content.rank;

    for (const [key, _value] of propToStat) {
        if (!data.get(key)?.has(rarity)) {
            data.get(key)?.set(rarity, new Float32Array(21));
        }
    }

    for (const prop of content.addProps) {
        const arr = data.get(prop.propType)?.get(rarity);
        if (arr === undefined) continue;
        arr[level] = prop.value;
    }
}

let ret: string = "";
ret += `static inline const std::unordered_map<Stat, std::array<std::array<float, 21>, 5>> mainStat{\n`;
for (const [prop, propData] of data) {
    ret += `\t{Stat::${propToStat.get(prop)}, std::array{\n`;
    for (const [_rarity, rarityData] of propData) {
        const levelData: string[] = [];
        for (const value of rarityData) {
            levelData.push(value.toFixed(3) + "f");
        }
        ret += `\t\tstd::array{${levelData.join(', ')}},\n`;
    }
    ret += `\t}},\n`;
}
ret += `};`

console.log(ret);