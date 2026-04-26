#include "engine.hpp"

#include "engine/data.hpp"

Stats::Engine::Engine(const ::Engine::Data &data) : data(&data), sheet(data.baseStats) {}
