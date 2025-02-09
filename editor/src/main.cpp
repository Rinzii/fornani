
#include "editor/setup/EditorApplication.hpp"

#include <SFML/Graphics.hpp>
#include "editor/util/Lookup.hpp"
#include <iostream>
#include <chrono>
#include <imgui-SFML.h>
#include <imgui.h>
#include <random>
#include <steam/steam_api.h>

#ifndef FORNANI_STEAM_APP_ID
#error "FORNANI_STEAM_APP_ID was not defined!"
#endif

#if defined(FORNANI_STEAM_APP_ID) && FORNANI_STEAM_APP_ID < 0
#error "FORNANI_STEAM_APP_ID was defined as a negative number!"
#endif

int main(int argc, char** argv) {
	assert(argc > 0);
	std::cout << "Current passed steam ID: " << FORNANI_STEAM_APP_ID << "\n";

	if (SteamAPI_RestartAppIfNecessary(FORNANI_STEAM_APP_ID)) {
		std::cout << "Re-launching through Steam.\n";
		return EXIT_SUCCESS;
	}
	SteamErrMsg errMsg;
	if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK) {
		std::cout << "Failed to init Steam: " << static_cast<const char *>(errMsg) << "\n";
		return EXIT_FAILURE;
	}

	std::cout << "SteamAPI has been initialized.\n";

	pi::EditorApplication app{argv};
	app.launch(argv);
    ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}
