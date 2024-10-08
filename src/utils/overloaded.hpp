#pragma once

namespace Utils {
	template<class... Ts>
	struct overloaded : Ts... {
		using Ts::operator()...;
	};
}// namespace Utils
