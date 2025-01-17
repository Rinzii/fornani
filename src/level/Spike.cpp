#include "Spike.hpp"
#include <cmath>
#include "../entities/player/Player.hpp"
#include "../service/ServiceProvider.hpp"
#include "../level/Map.hpp"
#include "../particle/Effect.hpp"
#include <algorithm>

namespace world {

Spike::Spike(automa::ServiceProvider& svc, sf::Texture& texture, sf::Vector2<float> position, sf::Vector2<int> direction, sf::Vector2<float> size)
	: size(size), hitbox(i_size().x == 1 ? size * 32.f : size * 24.f), sprite{texture}, grid_position(position + sf::Vector2<float>{16.f, 16.f}) {
	direction.x = direction.y != 0 ? 0 : direction.x;
	i_size().x == 1 ? sprite.setTextureRect(sf::IntRect{{480, 480}, {32, 32}}) : sprite.setTextureRect(sf::IntRect{{0, 0}, {192, 128}});
	collider = shape::Collider(size * 32.f);
	auto adjustment = 22.f;
	facing.und = (direction.y == 1) ? dir::UND::up : facing.und;
	facing.und = (direction.y == -1) ? dir::UND::down : facing.und;
	offset.y = (facing.und == dir::UND::up) ? adjustment : facing.und == dir::UND::down ? -adjustment : offset.y;
	facing.lr = (direction.x == 1) ? dir::LR::left : facing.lr;
	facing.lr = (direction.x == -1) ? dir::LR::right : facing.lr;
	offset.x = (facing.lr == dir::LR::left) ? adjustment : facing.lr == dir::LR::right ? -adjustment : offset.x;
	sprite.setOrigin(size * 16.f);
	if (svc.random.percent_chance(50)) { sprite.setScale({-1.f, 1.f}); }
	if (facing.lr == dir::LR::left) { sprite.setRotation(sf::degrees(-90)); }
	if (facing.lr == dir::LR::right) { sprite.setRotation(sf::degrees(90)); }
	if (facing.und == dir::UND::down) { sprite.setRotation(sf::degrees(180)); }
	collider.physics.position = position + offset;
	collider.sync_components();
	i_size().x == 1 ? hitbox.set_position(position + offset * 0.5f) : hitbox.set_position(position + offset * 0.5f - sf::Vector2<float>{56.f, 48.f});
}

void Spike::update(automa::ServiceProvider& svc, player::Player& player, world::Map& map) {
	if (map.soft_reset.is_done()) { player.controller.unrestrict(); }
	if (soft_reset && map.soft_reset.is_done()) {
		player.set_position(map.last_checkpoint());
		player.collider.physics.zero();
		player.controller.prevent_movement();
		player.controller.restrict_movement();
		map.soft_reset.end();
		soft_reset = false;
	}
	if (player.hurtbox.overlaps(hitbox) && map.soft_reset.not_started() && !player.invincible()) {
		player.hurt();
		if (!player.is_dead()) {
			soft_reset = true;
			map.soft_reset.start();
		}
	}
	//handle_collision(player.collider);
}

void Spike::handle_collision(shape::Collider& other) const {
	if (attributes.test(SpikeAttributes::no_collision)) { return; }
	other.handle_collider_collision(collider);
}

void Spike::render(automa::ServiceProvider& svc, sf::RenderWindow& win, sf::Vector2<float> cam) {
	sprite.setPosition(grid_position - cam);
	win.draw(sprite);
	if (svc.greyblock_mode()) {
		drawbox.setPosition(hitbox.position - cam);
		drawbox.setSize(hitbox.dimensions);
		drawbox.setFillColor(sf::Color{249, 12, 48, 64});
		collider.render(win, cam);
		win.draw(drawbox);
	}
}

} // namespace world
