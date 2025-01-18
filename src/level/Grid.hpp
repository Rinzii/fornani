
#pragma once

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "../setup/EnumLookups.hpp"
#include "Tile.hpp"
#include <djson/json.hpp>

namespace world {

int const CEIL_SLANT_INDEX = 192;
int const FLOOR_SLANT_INDEX = 208;
int const ceiling_single_ramp = 224;
int const floor_single_ramp = 240;

class Grid {
  public:
	Grid() = default;
	Grid(sf::Vector2<uint32_t> d, dj::Json& source);

	sf::Vector2<uint32_t> dimensions{};

	void check_neighbors(int i);
	sf::Vector2<int> get_solid_neighbors(int index);
	void seed_vertex(int index);
	void destroy_cell(sf::Vector2<int> pos);
	void render(sf::RenderWindow& win, sf::Vector2<float> cam);
	void draw(sf::RenderTexture& tex);
	std::size_t get_index_at_position(sf::Vector2<float> position) const;
	Tile& get_cell(int index);

	std::vector<Tile> cells{};

  private:
	float spacing{32.f};
	sf::RectangleShape drawbox{};
};

} // namespace squid
