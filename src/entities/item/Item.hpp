
#pragma once
#include <string_view>
#include "../../utils/BitFlags.hpp"
#include "../Entity.hpp"
#include "Drop.hpp"
#include "../../particle/Gravitator.hpp"

namespace automa {
struct ServiceProvider;
}

namespace gui {
class Console;
}

namespace item {

enum class ItemFlags { unique, revealed, usable, equippable };
enum class UIFlags { selected };
enum class ItemState { equipped };

class Item : public entity::Entity {
  public:
	Item() = default;
	Item(automa::ServiceProvider& svc, std::string_view label);
	void update(automa::ServiceProvider& svc, int index);
	void render(automa::ServiceProvider& svc, sf::RenderWindow& win, sf::Vector2<float> cam);
	void add_item(int amount);
	void subtract_item(int amount);
	void set_id(int new_id);
	void select();
	void deselect();
	void toggle_equip();
	void reveal() { flags.set(ItemFlags::revealed); }
	void set_rarity_position(sf::Vector2<float> position);
	[[nodiscard]] auto selected() const -> bool { return ui_flags.test(UIFlags::selected); }
	[[nodiscard]] auto usable() const -> bool { return flags.test(ItemFlags::usable); }
	[[nodiscard]] auto equippable() const -> bool { return flags.test(ItemFlags::equippable); }
	[[nodiscard]] auto is_equipped() const -> bool { return state.test(ItemState::equipped); }
	[[nodiscard]] auto has_menu() const -> bool { return equippable() || usable(); }
	[[nodiscard]] auto depleted() const -> bool { return variables.quantity <= 0; }
	[[nodiscard]] auto get_id() const -> int { return metadata.id; }
	[[nodiscard]] auto get_quantity() const -> int { return variables.quantity; }
	[[nodiscard]] auto get_label() const -> std::string_view { return flags.test(ItemFlags::revealed) ? metadata.title : metadata.naive_title; }
	[[nodiscard]] auto get_position() const -> sf::Vector2<float> { return drawbox.getPosition(); }
	[[nodiscard]] auto get_description() const -> std::string_view { return flags.test(ItemFlags::revealed) ? metadata.hidden_description : metadata.naive_description; }

	std::string_view label{};
	int selection_index{};
	vfx::Gravitator gravitator{};

  private:
	struct {
		int id{};
		std::string_view title{};
		std::string_view naive_title{};
		std::string_view naive_description{};
		std::string_view hidden_description{};
		Rarity rarity{};
	} metadata{};

	util::BitFlags<ItemFlags> flags{};
	util::BitFlags<ItemState> state{};
	util::BitFlags<UIFlags> ui_flags{};

	struct {
		int quantity{};
	} variables{};

	struct {
		sf::Vector2<float> pad{80.f, 80.f};
		float spacing{56.f};
		sf::Text rarity{};
		sf::Text quantity{};
	} ui{};
};

} // namespace player
