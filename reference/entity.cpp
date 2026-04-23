#include "entity.hpp"
#include <stdbool.h>

const int starting_entity_array_size = 10;

bool inited = false;

entity_prototype entity_prototypes[NUM_ET];

entity_prototype entity_prototype_get(entity_type et) {
    if (!inited) {
        entity_prototypes[ET_PLAYER] = (entity_prototype){"player", "pP", {64, 0, 16, 16}, true, 0};
        entity_prototypes[ET_CRATE] = (entity_prototype){"crate", "cC", {96, 0, 16, 16}, true, 1};
        entity_prototypes[ET_PRESENT] = (entity_prototype){"present", "bB", {48, 0, 16, 16}, true, 1};
        entity_prototypes[ET_TARGET] = (entity_prototype){"target", "tT", {80, 0, 16, 16}, false, 0};
        entity_prototypes[ET_NONE] = (entity_prototype){"none", "", {0, 0, 0, 0}, false, 0};
    }
    inited = true;

    return entity_prototypes[et];
}