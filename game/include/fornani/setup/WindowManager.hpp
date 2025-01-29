
#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

#include "fornani/utils/Logger.hpp"
#include "fornani/utils/Event.hpp"

namespace fornani {

class WindowManager
{

  public:
	sf::RenderWindow& get() const { return *m_window; }

	void create(sf::Vector2u requestedWindowSize, std::string_view = "fornani", bool fullscreen = false);


	[[nodiscard]] auto is_fullscreen() const -> bool { return m_is_fullscreen; }
	[[nodiscard]] auto in_window(sf::Vector2<float> point, sf::Vector2<float> dimensions) const -> bool {
		if (point.x + dimensions.x < 0.f || point.x - dimensions.x > m_current_window_size.x) { return false; }
		if (point.y + dimensions.y < 0.f || point.y - dimensions.y > m_current_window_size.y) { return false; }
		return true;
	}

	[[nodiscard]] auto current_size() const -> sf::Vector2u { return m_current_window_size; }

	void take_screenshot();

	auto process_events() const -> void;


	// Subscribe-able windowing events
	// Will be invoked if event is valid when process_events is called

	utils::Event<void()> Closed;
	utils::Event<void()> ProcessEventsLoopTop; // Not related to sfml, but called every iteration of the loop.
	utils::Event<void(sf::Vector2u)> Resized;
	utils::Event<void()> FocusLost;
	utils::Event<void()> FocusGained;
	using KeyEvent = utils::Event<void(sf::Keyboard::Key, sf::Keyboard::Scancode, bool, bool, bool, bool)>;
	KeyEvent KeyPressed;
	KeyEvent KeyReleased;
	utils::Event<void(sf::Mouse::Wheel, float, sf::Vector2i)> MouseWheelScrolled;
	utils::Event<void(sf::Mouse::Button, sf::Vector2i)> MouseButtonPressed;
	utils::Event<void(sf::Mouse::Button, sf::Vector2i)> MouseButtonReleased;
	utils::Event<void(sf::Vector2i)> MouseMoved;
	utils::Event<void(sf::Vector2i)> MouseMovedRaw;
	utils::Event<void()> MouseEntered;
	utils::Event<void()> MouseExited;
	utils::Event<void(unsigned int, unsigned int)> JoystickButtonPressed;
	utils::Event<void(unsigned int, unsigned int)> JoystickButtonReleased;
	utils::Event<void(unsigned int, sf::Joystick::Axis, float)> JoystickMoved;
	utils::Event<void(unsigned int)> JoystickConnected;
	utils::Event<void(unsigned int)> JoystickDisconnected;

	utils::Event<void()> PolledEvents; // Called once after all events are processed.



  private:
	void OnResize(const sf::Vector2u& newSize);


	void apply_letterbox(const sf::Vector2u& windowSize);

	static constexpr sf::Vector2u m_maintained_aspect_ration{3840, 2048};
	static constexpr float m_aspect_ratio{15/8};

	const sf::Vector2u m_max_display_dimensions{static_cast<unsigned>(sf::VideoMode::getDesktopMode().size.x), static_cast<unsigned>(sf::VideoMode::getDesktopMode().size.y)};


	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<sf::View> m_view;
	sf::FloatRect m_game_port{};

	sf::Vector2u m_current_window_size;
	std::string m_title;
	bool m_is_fullscreen{};



	sf::Texture m_screencap{};

	fornani::Logger m_logger { "windowing" };
};

} // namespace fornani
