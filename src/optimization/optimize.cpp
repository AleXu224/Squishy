#include "optimize.hpp"

#include "artifactFilter.hpp"

void Optimization::Optimization::optimize() const {
	std::vector<ArtifactFilter> filters{};

	// 4pc filters
	for (const auto &[key, _]: Artifact::sets) {
		ArtifactFilter pattern{};
		for (auto &entry: pattern.filters) {
			entry.set = key;
		}

		for (size_t i = 0; i < 5; i++) {
			auto ret = pattern;
			ret.filters.at(i).set = std::nullopt;
			filters.emplace_back(ret);
		}
	}

	// 2pc filters
	for (auto it = Artifact::sets.begin(); it != Artifact::sets.end(); it++) {
		auto key = it->first;
		for (size_t i = 0; i < 5; i++) {
			for (size_t j = i + 1; j < 5; j++) {
				ArtifactFilter ret{};
				ret.filters.at(i).set = key;
				ret.filters.at(j).set = key;
				filters.emplace_back(ret);

				// 2pc2pc
				for (auto it2 = std::next(it); it2 != Artifact::sets.end(); it2++) {
					auto key2 = it2->first;
					if (key == key2) continue;

					for (size_t k = 0; k < 5; k++) {
						if (k == i || k == j) continue;
						for (size_t l = k + 1; l < 5; l++) {
							if (l == i || l == j) continue;
							auto ret2 = ret;
							ret2.filters.at(k).set = key2;
							ret2.filters.at(l).set = key2;

							filters.emplace_back(ret2);
						}
					}
				}
			}
		}
	}

	std::println("{}", filters.size());
}
