export interface Equipment {
    id: number;
    icon: string;
    need: number[];
    rank: number[];
    affix: Affix[];
}

export interface Affix {
    add_props: AddProp[];
    affix_id: number;
    id: number;
    level: number;
    open_config: string;
    param_list: number[];
    name: string;
    desc: string;
}

export interface AddProp {
    prop_type: string;
    value: number;
}