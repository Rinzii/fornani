
#include "FileMenu.hpp"
#include "../../service/ServiceProvider.hpp"

namespace automa {

FileMenu::FileMenu(ServiceProvider& svc, player::Player& player, std::string_view scene, int id) : GameState(svc, player, scene, id), map(svc, player, console), file_select_menu(svc, {"Play", "Delete"}) {
	current_selection = util::Circuit(num_files);
	svc.data.load_blank_save(player);
	hud.set_corner_pad(svc, true); // display hud preview for each file in the center of the screen
	svc.state_controller.next_state = svc.data.load_progress(player, current_selection.get());
	player.set_position({(float)(svc.constants.screen_dimensions.x / 2) + 80, 360});
	player.antennae.at(0).set_position({(float)(svc.constants.screen_dimensions.x / 2) + 80, 360});
	player.antennae.at(1).set_position({(float)(svc.constants.screen_dimensions.x / 2) + 80, 360});

	loading.start(1);

	title.setPosition(0, 0);
	title.setSize(static_cast<sf::Vector2f>(svc.constants.screen_dimensions));
	title.setFillColor(svc.styles.colors.ui_black);

	auto ctr{0};
	for (auto& save : svc.data.files) {
		if (save.is_new()) { options.at(ctr).label.setString(options.at(ctr).label.getString() + " (new)"); }
		++ctr;
	}
	for (auto& option : options) { option.update(svc, current_selection.get()); }

	left_dot.set_position(options.at(current_selection.get()).left_offset);
	right_dot.set_position(options.at(current_selection.get()).right_offset);
}

void FileMenu::init(ServiceProvider& svc, int room_number) {}

void FileMenu::handle_events(ServiceProvider& svc, sf::Event& event) {
	svc.controller_map.handle_mouse_events(event);
	svc.controller_map.handle_joystick_events(event);
	if (event.type == sf::Event::EventType::KeyPressed) { svc.controller_map.handle_press(event.key.code); }
	if (event.type == sf::Event::EventType::KeyReleased) { svc.controller_map.handle_release(event.key.code); }

	if (svc.controller_map.label_to_control.at("down").triggered()) {
		if (file_select_menu.is_open()) {
			file_select_menu.down();
		} else {
			current_selection.modulate(1);
			svc.data.load_blank_save(*player);
			svc.state_controller.next_state = svc.data.load_progress(*player, current_selection.get());
			svc.soundboard.flags.menu.set(audio::Menu::shift);
		}
	}
	if (svc.controller_map.label_to_control.at("up").triggered()) {
		if (file_select_menu.is_open()) {
			file_select_menu.up();
		} else {
			current_selection.modulate(-1);
			svc.data.load_blank_save(*player);
			svc.state_controller.next_state = svc.data.load_progress(*player, current_selection.get());
			svc.soundboard.flags.menu.set(audio::Menu::shift);
		}
	}
	if (svc.controller_map.label_to_control.at("left").triggered() && !svc.controller_map.is_gamepad()) {
		svc.state_controller.actions.set(Actions::exit_submenu);
		svc.soundboard.flags.menu.set(audio::Menu::backward_switch);
	}
	if (svc.controller_map.label_to_control.at("menu_forward").triggered() || svc.controller_map.label_to_control.at("main_action").triggered()) {
		file_select_menu.open(options.at(current_selection.get()).position);
		/*svc.state_controller.next_state = svc.data.load_progress(*player, current_selection.get(), true);
		svc.state_controller.actions.set(Actions::trigger);
		svc.state_controller.actions.set(Actions::save_loaded);
		svc.soundboard.flags.menu.set(audio::Menu::select);
		svc.soundboard.flags.world.set(audio::World::load);*/
	}
	if (svc.controller_map.label_to_control.at("menu_back").triggered() || svc.controller_map.label_to_control.at("menu_toggle_secondary").triggered()) {
		if (file_select_menu.is_open()) {
			file_select_menu.close();
		} else {
			svc.state_controller.submenu = menu_type::main;
			svc.state_controller.actions.set(Actions::exit_submenu);
			svc.soundboard.flags.menu.set(audio::Menu::backward_switch);
		}
	}
	svc.controller_map.reset_triggers();
}

void FileMenu::tick_update(ServiceProvider& svc) {
	for (auto& option : options) { option.update(svc, current_selection.get()); }

	auto& opt = options.at(current_selection.get());
	auto minimenu_dim = sf::Vector2<float>{128.f, 128.f};
	auto minimenu_pos = opt.position - sf::Vector2<float>(opt.label.getLocalBounds().width * 0.5f + minimenu_dim.x + 2.f * spacing, minimenu_dim.y * 0.5f); 
	file_select_menu.update(svc, minimenu_dim, minimenu_pos);

	left_dot.update(svc);
	right_dot.update(svc);
	left_dot.set_target_position(options.at(current_selection.get()).left_offset);
	right_dot.set_target_position(options.at(current_selection.get()).right_offset);

	hud.update(svc, *player);

	player->animation.state = player::AnimState::run;
	player->collider.physics.acceleration = {};
	player->collider.physics.velocity = {};
	player->collider.physics.zero();
	player->collider.reset();
	player->controller.autonomous_walk();
	player->collider.flags.state.set(shape::State::grounded);

	player->set_position({svc.constants.screen_dimensions.x * 0.5f + 80, 360});
	player->update(map, console, inventory_window);
	player->controller.direction.lr = dir::LR::left;
	svc.soundboard.flags.player.reset(audio::Player::step);

	loading.update();

	svc.soundboard.play_sounds(svc);
	svc.controller_map.reset_triggers();
}

void FileMenu::frame_update(ServiceProvider& svc) {}

void FileMenu::render(ServiceProvider& svc, sf::RenderWindow& win) {
	win.draw(title);
	for (auto& option : options) { win.draw(option.label); }
	player->render(svc, win, {});
	if (loading.is_complete()) {
		left_dot.render(svc, win, {});
		right_dot.render(svc, win, {});
		hud.render(*player, win);
		file_select_menu.render(win);
	}
}

} // namespace automa
