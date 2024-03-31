
#include "NPC.hpp"
#include "../../gui/Console.hpp"
#include "../../service/ServiceProvider.hpp"
#include "../player/Player.hpp"

namespace npc {

NPC::NPC(automa::ServiceProvider& svc, int id) : id(id), animation_machine(std::make_unique<NPCAnimation>(svc, id)) {

	label = svc.tables.npc_label.at(id);

	auto const& in_data = svc.data.npc[label];
	dimensions.x = in_data["dimensions"][0].as<float>();
	dimensions.y = in_data["dimensions"][1].as<float>();
	sprite_dimensions.x = in_data["sprite_dimensions"][0].as<int>();
	sprite_dimensions.y = in_data["sprite_dimensions"][1].as<int>();
	spritesheet_dimensions.x = in_data["spritesheet_dimensions"][0].as<int>();
	spritesheet_dimensions.y = in_data["spritesheet_dimensions"][1].as<int>();
	sprite_offset.x = in_data["sprite_offset"][0].as<int>();
	sprite_offset.y = in_data["sprite_offset"][1].as<int>();

	sprite.setOrigin(in_data["sprite_origin"][0].as<float>(), in_data["sprite_origin"][1].as<float>());

	collider = shape::Collider(dimensions);
	collider.sync_components();

	collider.physics.set_global_friction(0.97f);
	collider.stats.GRAV = 4.2f;

	drawbox.setFillColor(sf::Color::Transparent);
	drawbox.setOutlineThickness(-1);
	drawbox.setSize(dimensions);

	sprite.setTexture(svc.assets.npcs.at(label));
	direction.lr = dir::LR::left;
}

void NPC::update(automa::ServiceProvider& svc, world::Map& map, gui::Console& console, player::Player& player) {
	direction.lr = (player.collider.physics.position.x < collider.physics.position.x) ? dir::LR::left : dir::LR::right;
	Entity::update(svc, map);

	if (player.collider.vicinity.overlaps(collider.vicinity)) { animation_machine->animation_flags.set(NPCAnimState::walk); }

	animation_machine->update();

	/*if(id == 0 && svc.ticker.every_x_frames(200)) {
		std::cout << "\nBRYN States: \n";
		if (animation_machine->animation_flags.test(NPCAnimState::idle)) { std::cout << "idle\n"; }
		if (animation_machine->animation_flags.test(NPCAnimState::walk)) { std::cout << "walk\n"; }
		if (animation_machine->animation_flags.test(NPCAnimState::turn)) { std::cout << "turn\n"; }
		if (animation_machine->animation_flags.test(NPCAnimState::inspect)) { std::cout << "inspect\n"; }
		std::cout << "_ \n";
	}*/

	collider.update(svc);
	collider.detect_map_collision(map);
	collider.reset();
	collider.physics.acceleration = {};

	state_flags.reset(NPCState::engaged);
	if (player.collider.bounding_box.overlaps(collider.bounding_box)) {
		if (player.controller.inspecting() && !conversations.empty()) {
			state_flags.set(NPCState::engaged);
			console.set_source(svc.text.npc);
			std::string name = std::string(label);
			std::string convo = std::string(conversations.front());
			std::string target = name + convo;
			console.load_and_launch(target);
			console.include_portrait(id);
		}
	}

	if (console.off()) {
		disengage();
		if (conversations.size() > 1) { conversations.pop_front(); }
		console.clean_off_trigger();
	}
	if (state_flags.test(NPCState::disengaged)) {
		state_flags.reset(NPCState::disengaged);
	}
}

void NPC::render(automa::ServiceProvider& svc, sf::RenderWindow& win, sf::Vector2<float> campos) {
	sprite.setPosition(collider.physics.position.x - campos.x + sprite_offset.x, collider.physics.position.y - campos.y + sprite_offset.y);
	int u = (int)(animation_machine->animation.get_frame() / spritesheet_dimensions.y) * sprite_dimensions.x;
	int v = (int)(animation_machine->animation.get_frame() % spritesheet_dimensions.y) * sprite_dimensions.y;
	sprite.setTextureRect(sf::IntRect({u, v}, {(int)sprite_dimensions.x, (int)sprite_dimensions.y}));

	if (svc.greyblock_mode()) {
		drawbox.setPosition(collider.physics.position - campos);
		state_flags.test(NPCState::engaged) ? drawbox.setOutlineColor(svc.styles.colors.green) : drawbox.setOutlineColor(svc.styles.colors.dark_orange);
		win.draw(drawbox);
		collider.render(win, campos);
	} else {
		win.draw(sprite);
	}
}

void NPC::set_position(sf::Vector2<float> pos) { collider.physics.position = pos; }

void NPC::set_position_from_scaled(sf::Vector2<float> scaled_pos) { collider.physics.position = scaled_pos * 32.f; }

void NPC::set_id(int new_id) { id = new_id; }

void NPC::disengage() { state_flags.set(NPCState::disengaged); }

void NPC::push_conversation(std::string_view convo) { conversations.push_back(convo); }

} // namespace npc
