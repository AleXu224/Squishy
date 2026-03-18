export interface ManifestNanoka {
    gi: Gi;
}

export interface Gi {
    latest: string;
    available: string[];
    live: string;
    new: { [key: string]: number[] };
}
