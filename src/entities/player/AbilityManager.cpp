#include "AbilityManager.hpp"
#include "../../service/ServiceProvider.hpp"
#include "../../gui/Console.hpp"

namespace player {

void AbilityManager::update(automa::ServiceProvider& svc) {}

void AbilityManager::give_ability(Abilities ability) { ability_flags.set(ability); }

void AbilityManager::give_ability(int ability) { give_ability(static_cast<Abilities>(ability)); }

void AbilityManager::remove_ability(Abilities ability) { ability_flags.reset(ability); }

void AbilityManager::clear() { ability_flags = {}; }

} // namespace player
