#include "fornani/setup/WindowManager.hpp"
#include <iostream>

#include <imgui-SFML.h>

namespace fornani {

void WindowManager::create(sf::Vector2u requestedWindowSize, std::string_view title, bool fullscreen) {
	m_current_window_size = requestedWindowSize;
	m_title = title;
	m_is_fullscreen = fullscreen;
	std::uint32_t style = (m_is_fullscreen ? sf::State::Fullscreen : sf::Style::Default);
	auto mode = m_is_fullscreen ? sf::VideoMode(m_max_display_dimensions) : sf::VideoMode(requestedWindowSize);

	if (!mode.isValid() && m_is_fullscreen) {
		NANI_LOG_WARN(m_logger,
			"Number of valid fullscreen modes: {}\n"
			"Failed to extract a valid screen mode.\n",
			mode.getFullscreenModes().size());
		// TODO: This might need to be adjusted as 8k screens technically exist.
		//		 Also it might instead be better to get the resolution of there screen and adjust based on that.
		constexpr sf::Vector2u new_display_size = {m_maintained_aspect_ration.x / 4, m_maintained_aspect_ration.y / 4};
		mode = sf::VideoMode(new_display_size);
		m_is_fullscreen = false;
	}

	m_window = std::make_unique<sf::RenderWindow>(mode, m_title);
	m_window->setFramerateLimit(60);
	if (is_fullscreen) { m_window->setMouseCursorVisible(false); }

	Resized += std::bind(&WindowManager::OnResize, this, std::placeholders::_1);

}


void WindowManager::take_screenshot() {
	m_screencap.update(*m_window);
	std::time_t const now = std::time(nullptr);

	const std::time_t time = std::time({});
	char time_string[std::size("yyyy-mm-ddThh:mm:ssZ")];
	std::strftime(std::data(time_string), std::size(time_string), "%FT%TZ", std::gmtime(&time));
	std::string time_str = time_string;

	std::erase_if(time_str, [](auto const& c) { return c == ':' || isspace(c); });
	if (const std::string filename = "screenshot_" + time_str + ".png"; m_screencap.copyToImage().saveToFile(filename))
	{
		NANI_LOG_INFO(m_logger, "screenshot saved to: {}", filename);
	}
}

auto WindowManager::process_events() const -> void { // NOLINT
	// Poll for every possible event then invoke all subscribed functions. Not thread safe.
	while (const auto event = m_window->pollEvent())
	{
		if (event->is<sf::Event::Closed>()) {
			Closed();
		}
		ProcessEventsLoopTop(); // Handled second technically but might as well be considered first.
		if (const auto* ev = event->getIf<sf::Event::Resized>()) {
			Resized(ev->size);
		}
		if (event->is<sf::Event::FocusLost>()) {
			FocusLost();
		}
		if (event->is<sf::Event::FocusGained>()) {
			FocusGained();
		}
		if (const auto* ev = event->getIf<sf::Event::KeyPressed>()) {
			KeyPressed(ev->code, ev->scancode, ev->alt, ev->control, ev->shift, ev->system);
		}
		if (const auto* ev = event->getIf<sf::Event::KeyReleased>()) {
			KeyReleased(ev->code, ev->scancode, ev->alt, ev->control, ev->shift, ev->system);
		}
		if (const auto* ev = event->getIf<sf::Event::MouseWheelScrolled>()) {
			MouseWheelScrolled(ev->wheel, ev->delta, ev->position);
		}
		if (const auto* ev = event->getIf<sf::Event::MouseButtonPressed>()) {
			MouseButtonPressed(ev->button, ev->position);
		}
		if (const auto* ev = event->getIf<sf::Event::MouseButtonReleased>()) {
			MouseButtonReleased(ev->button, ev->position);
		}
		if (const auto* ev = event->getIf<sf::Event::MouseMoved>()) {
			MouseMoved(ev->position);
		}
		if (const auto* ev = event->getIf<sf::Event::MouseMovedRaw>()) {
			MouseMovedRaw(ev->delta);
		}
		if (event->is<sf::Event::MouseEntered>()) {
			MouseEntered();
		}
		if (event->is<sf::Event::MouseLeft>()) {
			MouseExited();
		}
		if (const auto* ev = event->getIf<sf::Event::JoystickButtonPressed>()) {
			JoystickButtonPressed(ev->joystickId, ev->button);
		}
		if (const auto* ev = event->getIf<sf::Event::JoystickButtonReleased>()) {
			JoystickButtonReleased(ev->joystickId, ev->button);
		}
		if (const auto* ev = event->getIf<sf::Event::JoystickMoved>()) {
			JoystickMoved(ev->joystickId, ev->axis, ev->position);
		}
		if (const auto* ev = event->getIf<sf::Event::JoystickConnected>()) {
			JoystickConnected(ev->joystickId);
		}
		if (const auto* ev = event->getIf<sf::Event::JoystickDisconnected>()) {
			JoystickDisconnected(ev->joystickId);
		}
	}
	PolledEvents();
}

void WindowManager::OnResize(const sf::Vector2u& newSize) {
	m_current_window_size = newSize;
	apply_letterbox(newSize);
	m_window->setView(*m_view);
}

// TODO: Clean this up
auto WindowManager::apply_letterbox(const sf::Vector2u& windowSize) -> void {
	// Current aspect ratio of the window
	float const currentRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

	// Calculate new viewport (in normalized coords [0..1]) that letterboxes the view
	float sizeX = 1.0f;
	float sizeY = 1.0f;
	float posX  = 0.0f;
	float posY  = 0.0f;

	if (currentRatio > m_aspect_ratio)
	{
		// Too wide, add horizontal bars
		sizeX = m_aspect_ratio / currentRatio;
		posX  = (1.f - sizeX) / 2.f;
	}
	else if (currentRatio < m_aspect_ratio)
	{
		// Too tall, add vertical bars
		sizeY = currentRatio / m_aspect_ratio;
		posY  = (1.f - sizeY) / 2.f;
	}

	m_game_port = sf::FloatRect({posX, posY}, {sizeX, sizeY});
	m_view->setViewport(m_game_port);
}
} // namespace fornani
