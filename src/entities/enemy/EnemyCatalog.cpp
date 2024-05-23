#include <iostream>
#include "EnemyCatalog.hpp"
#include "../enemy/catalog/Frdog.hpp"
#include "../enemy/catalog/Tank.hpp"
#include "../enemy/catalog/Eyebot.hpp"
#include "../enemy/catalog/Eyebit.hpp"
#include "../enemy/boss/Minigus.hpp"

namespace enemy {

EnemyCatalog::EnemyCatalog(automa::ServiceProvider& svc) {}

void EnemyCatalog::update() {
	std::erase_if(enemies, [this](auto const& e) { return e->gone(); });
}

void EnemyCatalog::push_enemy(automa::ServiceProvider& svc, world::Map& map, int id) {
	switch (id) {
	case 0: enemies.push_back(std::make_unique<Frdog>(svc)); break;
	case 1: enemies.push_back(std::make_unique<Tank>(svc)); break;
	case 4: enemies.push_back(std::make_unique<Eyebot>(svc)); break;
	case 5: enemies.push_back(std::make_unique<Eyebit>(svc)); break;
	case 6: enemies.push_back(std::make_unique<Minigus>(svc, map)); break;
	default: enemies.push_back(std::make_unique<Frdog>(svc)); break;
	}
}

} // namespace enemy
