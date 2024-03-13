
#pragma once

#include <string>
#include "../behavior/Animation.hpp"
#include "../../utils/Collider.hpp"

namespace automa {
struct ServiceProvider;
}

namespace item {

	enum class DropType { heart, orb };
enum Rarity {common, uncommon, rare, priceless};

	struct DropParameters {
		DropType type{};
		anim::Parameters animation_parameters{};
	};

class Drop {

  public:

	Drop() = default;
	Drop(automa::ServiceProvider& svc, std::string_view key, float probability);
	void seed(float probability);
	void set_value();
	void set_texture(automa::ServiceProvider& svc);
	void update();
	void render(sf::RenderWindow& win, sf::Vector2<float> campos);
	void set_position(sf::Vector2<float> pos);

	shape::Collider& get_collider();

  private:
	sf::Vector2<float> drop_dimensions{10.f, 10.f};
	shape::Collider collider{};
	sf::Vector2<int> spritesheet_dimensions{};
	sf::Vector2<float> sprite_dimensions{};
	sf::Vector2<float> sprite_offset{};
	anim::Animation animation{};
	sf::Sprite sprite{};

	int num_sprites{}; // 2 for hearts, 4 for orbs

	DropParameters parameters{};
	Rarity rarity{};
	int value{};

	float const priceless_constant{0.001f};
	float const rare_constant{0.01f};
	float const uncommon_constant{0.1f};
};

} // namespace item
