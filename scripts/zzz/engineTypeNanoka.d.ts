export interface Engine {
    id: number;
    code_name: string;
    name: string;
    desc: string;
    desc2: string;
    desc3: string;
    rarity: number;
    icon: string;
    weapon_type: { [key: string]: string };
    base_property: Property;
    rand_property: Property;
    level: { [key: string]: Level };
    stars: { [key: string]: Star };
    materials: string;
    talents: { [key: string]: Talent };
}

export interface Property {
    name: string;
    name2: string;
    format: string;
    value: number;
}

export interface Level {
    exp: number;
    rate: number;
    rate2: number;
}

export interface Star {
    star_rate: number;
    rand_rate: number;
}

export interface Talent {
    name: string;
    desc: string;
}
