#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string_view>
#include "fornani/setup/EnumLookups.hpp"
#include "fornani/utils/Collider.hpp"
#include "fornani/utils/Counter.hpp"
#include "fornani/entities/animation/Animation.hpp"

namespace automa {
struct ServiceProvider;
}

namespace player {
class Player;
}

namespace arms {
class Projectile;
}

namespace world {

enum class SpikeAttributes { no_collision };

class Spike {
  public:
	Spike(automa::ServiceProvider& svc, sf::Texture& texture, sf::Vector2<float> position, sf::Vector2<int> direction, sf::Vector2<float> size);
	void update(automa::ServiceProvider& svc, player::Player& player, world::Map& map);
	void handle_collision(shape::Collider& other) const;
	void render(automa::ServiceProvider& svc, sf::RenderWindow& win, sf::Vector2<float> cam);
	shape::Shape& get_bounding_box() { return collider.bounding_box; }
	shape::Shape& get_hurtbox() { return collider.hurtbox; }

  private:
	[[nodiscard]] auto i_size() const -> sf::Vector2<int> { return sf::Vector2<int>(static_cast<int>(size.x), static_cast<int>(size.y)); }
	sf::Vector2<float> size{};
	shape::Shape hitbox{};
	shape::Collider collider{};
	dir::Direction facing{};
	util::BitFlags<SpikeAttributes> attributes{};
	sf::Vector2<float> grid_position{};
	sf::Vector2<float> offset{};
	sf::RectangleShape drawbox{};
	sf::Sprite sprite;
	bool soft_reset{};
};
} // namespace world