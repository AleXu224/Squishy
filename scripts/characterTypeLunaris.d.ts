export interface LunarisCharacter {
    info: Info;
    icons: Icons;
    ascension: Ascension;
    skills: Skills;
    passives: Passives;
    constellations: Constellations;
    energy: Energy[];
    attacks: Attack[];
    hyperlinks: Hyperlink[];
}

export interface Ascension {
    exp: { [key: string]: number };
    speciality: { [key: string]: number };
    elemental: { [key: string]: number };
    worldmonster: { [key: string]: number };
    elitemonster: { [key: string]: number };
}

export interface Attack {
    name: string;
    icd_source: string;
    icd_rule: string;
    gauge: string;
    poise: string;
    element: string;
}

export interface Constellations {
    const1: Skill;
    const2: Skill;
    const3: Skill;
    const4: Skill;
    const5: Skill;
    const6: Skill;
}

export interface Skill {
    name: string;
    description: string;
    icon: string;
    multipliers?: { [key: string]: string[] };
}

export interface Energy {
    source: string;
    particles: string;
    chance: string;
    cd: string;
}

export interface Hyperlink {
    id: number;
    name: string;
    description: string;
    params: string[];
}

export interface Icons {
    forward: string;
    coop_img: string;
}

export interface Info {
    name: string;
    description: string;
    rarity: string;
    weapon: string;
    birthday: string;
    element: string;
    constellation: string;
    attributes: Attribute[];
}

export interface Attribute {
    level: number;
    ascension: number;
    hp: number;
    atk: number;
    def: number;
    EM?: number;
    "CRIT DMG%"?: string;
    "CRIT Rate%"?: string;
    "Geo%"?: string;
    "HP%"?: string;
    "Dendro%"?: string;
}

export interface Passives {
    p1: Skill;
    p2: Skill;
    p3: Skill;
    p4: Skill | string;
}

export interface Skills {
    normalattack: Skill;
    elementalskill: Skill;
    elementalburst: Skill;
    leveling: Leveling;
}

export interface Leveling {
    crown: Crown;
    books: { [key: string]: number };
    worldmonster: { [key: string]: number };
    weekly: { [key: string]: number };
}

export interface Crown {
    "104319": number;
}
