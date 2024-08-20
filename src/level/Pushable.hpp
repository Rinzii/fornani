#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string_view>
#include "../setup/EnumLookups.hpp"
#include "../utils/Collider.hpp"
#include "../utils/Counter.hpp"
#include "../entities/animation/Animation.hpp"

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

enum class PushableAttributes { bulletproof };
enum class PushableState { moved };

class Pushable {
  public:
	Pushable(automa::ServiceProvider& svc, sf::Vector2<float> position, int style = 0, int size = 1);
	void update(automa::ServiceProvider& svc, Map& map, player::Player& player);
	void handle_collision(shape::Collider& other) const;
	void render(automa::ServiceProvider& svc, sf::RenderWindow& win, sf::Vector2<float> cam);
	void on_hit(automa::ServiceProvider& svc, world::Map& map, arms::Projectile& proj);
	shape::Shape& get_bounding_box() { return collider.bounding_box; }
	shape::Shape& get_hurtbox() { return collider.hurtbox; }
	[[nodiscard]] auto unmoved() { return !state.test(PushableState::moved); }

  private:
	shape::Collider collider{};
	util::BitFlags<PushableAttributes> attributes{};
	util::BitFlags<PushableState> state{};
	sf::Sprite sprite{};
	int style{};
	int size{};
	float mass{};
	float dampen{4.f};
	float speed{32.f};
};
} // namespace world