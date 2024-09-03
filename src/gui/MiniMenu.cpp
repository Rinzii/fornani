#include "MiniMenu.hpp"
#include "../service/ServiceProvider.hpp"
#include "../automa/GameState.hpp"
#include <algorithm>

namespace gui {

MiniMenu::MiniMenu(automa::ServiceProvider& svc, std::vector<std::string_view> opt) {
	sprite.set_texture(svc.assets.t_white_console);
	sprite.slice(static_cast<int>(corner), static_cast<int>(edge));
	for (auto& o : opt) { options.push_back(automa::Option(svc, o)); }
	selection = util::Circuit(static_cast<int>(options.size()));
	font.loadFromFile(svc.text.title_font);
	font.setSmooth(false);
	int ctr{};
	for (auto& option : options) {
		option.label.setFont(font);
		option.index = ctr;
		option.update(svc, selection.get());
		++ctr;
	}
}

void MiniMenu::update(automa::ServiceProvider& svc, sf::Vector2<float> dim, sf::Vector2<float> position) {
	if (!is_open()) { return; }
	dimensions = dim;
	sprite.update(svc, position, dim, corner, edge);
	auto spacing = 12.f;
	auto top_buffer = 18.f;
	auto ctr{0};
	for (auto& option : options) {
		option.position = {sprite.get_position().x, sprite.get_position().y + spacing + ctr * (option.label.getLocalBounds().height + spacing) - sprite.get_center().y + top_buffer};
		option.update(svc, selection.get());
		++ctr;
	}
}

void MiniMenu::render(sf::RenderWindow& win) const {
	if (!is_open()) { return; }
	sprite.render(win);
	for (auto& option : options) { win.draw(option.label); }
}

void MiniMenu::open(automa::ServiceProvider& svc, sf::Vector2<float> position) {
	state.set(MiniMenuState::open);
	sprite.start(svc, position);
}

void MiniMenu::close(automa::ServiceProvider& svc) {
	state.reset(MiniMenuState::open);
	sprite.start(svc, position);
}

void MiniMenu::set_origin(sf::Vector2<float> origin) { sprite.set_origin(origin); }

void MiniMenu::up() { selection.modulate(-1); }

void MiniMenu::down() { selection.modulate(1); }

sf::Vector2<float> MiniMenu::get_dimensions() const { return dimensions; }

} // namespace gui
