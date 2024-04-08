
#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdio>
#include <memory>
#include "../components/PhysicsComponent.hpp"
#include "../entities/player/Player.hpp"
#include "../graphics/Background.hpp"
#include "../gui/Console.hpp"
#include "../gui/Portrait.hpp"
#include "../gui/HUD.hpp"
#include "../level/Map.hpp"
#include "../gui/InventoryWindow.hpp"

namespace player {
class Player;
}

namespace automa {

// globals

enum class STATE { STATE_NULL, STATE_INIT, STATE_EXIT, STATE_MENU, STATE_OPTIONS, STATE_FILE, STATE_MAIN, STATE_DOJO };

enum class MenuSelection { new_game, load_game, options };

inline MenuSelection selection{};
inline int file_selection{0};

constexpr inline float dot_force{0.9f};
constexpr inline float dot_fric{0.86f};
constexpr inline float dot_speed{20.f};

struct ServiceProvider;

class GameState {

  public:
	using Time = std::chrono::duration<float>;

	GameState() = default;
	GameState(ServiceProvider& svc, player::Player& player, int id = 0) : player(&player), hud(svc, player, {20, 20}), inventory_window(svc) {}
	GameState& operator=(GameState&&) = delete;

	virtual void init(ServiceProvider& svc, std::string_view room = ""){};
	virtual void setTilesetTexture(ServiceProvider& svc, sf::Texture& t){};
	virtual void handle_events(ServiceProvider& svc, sf::Event& event){};
	virtual void tick_update(ServiceProvider& svc){};
	virtual void frame_update(ServiceProvider& svc){};
	virtual void render(ServiceProvider& svc, sf::RenderWindow& win){};

	STATE state = STATE::STATE_NULL;
	bool debug_mode{false};

	gui::Console console{};
	gui::InventoryWindow inventory_window;

	vfx::Gravitator left_dot{};
	vfx::Gravitator right_dot{};
	sf::Vector2<float> dot_pad{24.f, 8.f};

	player::Player* player;
	gui::HUD hud;
	sf::Font font{};
};

} // namespace automa
