#include "fornani/level/Tile.hpp"
#include "fornani/service/ServiceProvider.hpp"
#include "fornani/entities/player/Player.hpp"
#include "fornani/weapon/Projectile.hpp"
#include "fornani/level/Map.hpp"
#include <iostream>

namespace world {

Tile::Tile(sf::Vector2<uint32_t> i, sf::Vector2<float> p, uint32_t val, uint32_t odi) : index(i), value(val), one_d_index(odi), bounding_box({32.f, 32.f}, evaluate(val)) {
	set_type();
	bounding_box.set_position(p);
}

void Tile::on_hit(automa::ServiceProvider& svc, player::Player& player, world::Map& map, arms::Projectile& proj) {
	if (proj.transcendent()) { return; }
	if (!map.nearby(bounding_box, proj.get_bounding_box())) {
		return;
	} else {
		collision_check = true;
		if ((proj.get_bounding_box().overlaps(bounding_box) && is_occupied())) {
			if (!is_collidable() || is_platform()) { return; }
			if (!proj.transcendent()) {
				if (!proj.destruction_initiated()) {
					map.effects.push_back(entity::Effect(svc, proj.get_destruction_point() + proj.get_position(), {}, proj.effect_type(), 2));
					if (proj.get_direction().lr == dir::LR::neutral) { map.effects.back().rotate(); }
				}
				proj.destroy(false);
			}
		}
	}
}

void Tile::render(sf::RenderWindow& win, sf::RectangleShape& draw, sf::Vector2<float> cam) {
	draw.setSize({32.f, 32.f});
	draw.setFillColor(sf::Color{17, 230, 187, 45});
	if (collision_check) {
		draw.setFillColor(sf::Color{190, 255, 7, 180});
		if (!surrounded) {}
	}
	if (ramp_adjacent()) { draw.setFillColor(sf::Color{240, 155, 7, 180}); }
	if (covered()) { draw.setFillColor(sf::Color{0, 155, 130, 180}); }
	draw.setPosition(bounding_box.position - cam);
	if (is_occupied() && collision_check) { win.draw(draw); }
	collision_check = false;
}

void Tile::draw(sf::RenderTexture& tex) {
	if (is_occupied()) { bounding_box.draw(tex); }
}

void Tile::set_type() {
	auto special_index{448};
	type = TileType::empty;
	if (value < special_index && value > 0) {
		type = TileType::solid;
		return;
	}
	if ((value < special_index + 16 && value >= special_index) || (value >= ceiling_single_ramp && value <= ceiling_single_ramp + 3)) {
		type = TileType::ceiling_ramp;
		return;
	}
	if ((value < special_index + 32 && value >= special_index + 16) || (value >= floor_single_ramp && value <= floor_single_ramp + 3)) {
		type = TileType::ground_ramp;
		return;
	}
	if (value < special_index + 48 && value >= special_index + 44) {
		type = TileType::platform;
		return;
	}
	if (value < special_index + 38 && value >= special_index + 36) {
		type = TileType::pushable;
		return;
	}
	if (value == special_index + 38) {
		type = TileType::target;
		return;
	}
	if (value == special_index + 39) {
		type = TileType::spawner;
		return;
	}
	if (value == special_index + 52) {
		type = TileType::bonfire;
		return;
	}
	if (value == special_index + 53) {
		type = TileType::campfire;
		return;
	}
	if (value == special_index + 54) {
		type = TileType::checkpoint;
		return;
	}
	if (value == special_index + 55) {
		type = TileType::breakable;
		return;
	}
	if (value == special_index + 62) { type = TileType::big_spike; }
	if (value == special_index + 63) { type = TileType::spike; }
}

} // namespace world
