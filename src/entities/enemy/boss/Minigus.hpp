#pragma once

#include "../../npc/NPC.hpp"
#include "../Enemy.hpp"
#define MINIGUS_BIND(f) std::bind(&Minigus::f, this)

namespace enemy {

enum class MinigusState { idle, turn, run, shoot, jump_shoot, hurt, jump, jumpsquat, reload };
enum class MinigunState { deactivated, neutral, charging, firing };
enum class MinigunFlags { exhausted, charging };

class Minigus : public Enemy, public npc::NPC {

  public:
	Minigus() = default;
	Minigus(automa::ServiceProvider& svc, world::Map& map);
	void unique_update(automa::ServiceProvider& svc, world::Map& map, player::Player& player) override;
	void unique_render(automa::ServiceProvider& svc, sf::RenderWindow& win, sf::Vector2<float> cam) override;

	fsm::StateFunction state_function = std::bind(&Minigus::update_idle, this);
	fsm::StateFunction update_idle();
	fsm::StateFunction update_shoot();
	fsm::StateFunction update_jumpsquat();
	fsm::StateFunction update_hurt();
	fsm::StateFunction update_jump();
	fsm::StateFunction update_jump_shoot();
	fsm::StateFunction update_reload();
	fsm::StateFunction update_turn();
	fsm::StateFunction update_run();

  private:
	MinigusState state{};

	struct {
		sf::Sprite sprite{};
		anim::Animation animation{};
		MinigunState state{};
		anim::Parameters neutral{21, 4, 36, -1};
		anim::Parameters deactivated{11, 2, 38, -1};
		anim::Parameters charging{0, 11, 28, -1};
		anim::Parameters firing{13, 8, 20, 1};
		sf::Vector2<float> offset{0, 64};
		sf::Vector2<int> dimensions{78, 30};
		dir::Direction direction;
		util::BitFlags<MinigunFlags> flags{};
	} minigun{};

	struct {
		shape::Collider head{};
	} colliders{};

	int fire_chance{80};
	int firing_cooldown_time{1000};

	// packages
	entity::WeaponPackage gun;
	entity::Caution caution{};
	util::Cooldown running_time{};
	util::Cooldown firing_cooldown{};

	// lookup, duration, framerate, num_loops
	anim::Parameters idle{0, 6, 48, -1};
	anim::Parameters shoot{18, 1, 38, -1};
	anim::Parameters jumpsquat{18, 1, 38, 1};
	anim::Parameters hurt{21, 4, 24, 2};
	anim::Parameters jump{14, 1, 22, -1};
	anim::Parameters jump_shoot{16, 1, 42, -1};
	anim::Parameters reload{7, 7, 28, 1};
	anim::Parameters turn{18, 2, 42, 1};
	anim::Parameters run{14, 4, 42, -1};

	automa::ServiceProvider* m_services;
	world::Map* m_map;

	bool change_state(MinigusState next, anim::Parameters params);
};

} // namespace enemy