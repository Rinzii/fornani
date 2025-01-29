#pragma once
#include <imgui.h>
#include <filesystem>
#include <random>
#include "fornani/audio/MusicPlayer.hpp"
#include "fornani/automa/StateManager.hpp"
#include "fornani/entities/player/Player.hpp"
#include "fornani/service/ServiceProvider.hpp"
#include "fornani/utils/BitFlags.hpp"
#include <imgui-SFML.h>

namespace fornani {

class WindowManager;

enum class GameFlags { playtest, in_game, draw_cursor };

enum class KeyboardFlags { control };

class Game {
public:
	Game(char** argv, WindowManager& window, Version& version);
	~Game() {}
	void run(bool demo = false, int room_id = 100, std::filesystem::path levelpath = std::filesystem::path{}, sf::Vector2<float> player_position = {});
	void shutdown();

	void playtest_sync();
	void toggle_weapon(bool flag, int id);
	util::BitFlags<GameFlags> flags{};
	util::BitFlags<KeyboardFlags> key_flags{};

private:
	void OnClose();
	void OnResize(const sf::Vector2u& newSize);
	void OnKeyPress(sf::Keyboard::Key key,
				  sf::Keyboard::Scancode scancode,
				  bool alt,
				  bool control,
				  bool shift,
				  bool system);
	void OnKeyReleased(sf::Keyboard::Key key,
				   sf::Keyboard::Scancode scancode,
				   bool alt,
				   bool control,
				   bool shift,
				   bool system);

	void OnEventLoop();

	void playtester_portal(sf::RenderWindow& window);
	void take_screenshot() const;


	automa::ServiceProvider services;

	struct {
		sf::Vector2u win_size{};
		float height_ratio{};
		float width_ratio{};
	} measurements{};

	struct {
		bool m_musicplayer{};
		bool b_dash{};
		bool b_shield{};
		bool b_wallslide{};
		bool b_doublejump{};

		struct {
			bool bryn{};
			bool grenade{};
			bool plasmer{};
			bool tomahawk{};
			bool grapple{};
			bool staple_gun{};
			bool indie{};
			bool gnat{};
		} weapons{};
	} playtest{};

	struct {
		int sample{};
		int total{};
	} rng_test{};

	player::Player player;
	automa::StateManager game_state;
	sf::RectangleShape background{};

	fornani::Logger m_logger {"core"};
};

} // namespace fornani

/*
// For ease of adding new windowing event. This is basically ever possible function signature there is for a windowing event.
void OnClose();
	void OnResized(const sf::Vector2u& newSize);
	void OnFocusLost();
	void OnFocusGained();
	void OnKeyPressed(sf::Keyboard::Key key,
					  sf::Keyboard::Scancode scancode,
					  bool alt,
					  bool control,
					  bool shift,
					  bool system);
	void OnKeyReleased(sf::Keyboard::Key key,
					   sf::Keyboard::Scancode scancode,
					   bool alt,
					   bool control,
					   bool shift,
					   bool system);
	void OnMouseWheelScrolled(sf::Mouse::Wheel wheel, float delta, const sf::Vector2i& position);
	void OnMouseButtonPressed(sf::Mouse::Button button, const sf::Vector2i& position);
	void OnMouseButtonReleased(sf::Mouse::Button button, const sf::Vector2i& position);
	void OnMouseMoved(const sf::Vector2i& position);
	void OnMouseMovedRaw(const sf::Vector2i& offset);
	void OnMouseEntered();
	void OnMouseExited();
	void OnJoystickButtonPressed(unsigned int joystickId, unsigned int button);
	void OnJoystickButtonReleased(unsigned int joystickId, unsigned int button);
	void OnJoystickMoved(unsigned int joystickId, sf::Joystick::Axis axis, float position);
	void OnJoystickConnected(unsigned int joystickId);
	void OnJoystickDisconnected(unsigned int joystickId);
*/
