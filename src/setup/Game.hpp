#pragma once
#include <imgui.h>
#include <random>
#include "../automa/StateManager.hpp"
#include "../service/ServiceProvider.hpp"
#include "../entities/player/Player.hpp"
#include "../audio/MusicPlayer.hpp"
#include <imgui-SFML.h>

namespace fornani {

class Game {
  public:
	Game() = default;
	Game(char** argv);
	void run();

  private:
	void debug_window();
	void take_screenshot();
	bool debug();

	struct automa::ServiceProvider services{};

	struct {
		sf::Vector2<uint32_t> win_size{};
		float height_ratio{};
		float width_ratio{};
	} measurements{};

	struct {
		int draw_calls{};
	} trackers{};

	player::Player player;
	automa::StateManager game_state{};
	sf::RenderWindow window{};
	sf::Texture screencap{};
	sf::RectangleShape background{};
	audio::MusicPlayer music_player{};
};

} // namespace fornani