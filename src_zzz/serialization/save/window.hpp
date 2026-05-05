#pragma once

#include "cstdint"

namespace Serialization::Save {
	struct Window {
		uint32_t width = 1280;
		uint32_t height = 720;
		bool maximized = false;
	};
}// namespace Serialization::Save