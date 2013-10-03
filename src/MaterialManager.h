#ifndef MaterialManager_H_
#define MaterialManager_H_

#include "gameplay.h"

using namespace gameplay;

enum MaterialType {
	FLOOR_EMPTY,
	FLOOR_HALL_START,
	FLOOR_HALL,
	FLOOR_ROOM,
	FLOOR_STAIR,
	FLOOR_EMPTY_SEL,
	FLOOR_HALL_START_SEL,
	FLOOR_HALL_SEL,
	FLOOR_ROOM_SEL,
	FLOOR_STAIR_SEL,
	WALL,
	COUNT
};

class MaterialManager {
public:
	static Material* getMaterial(MaterialType type);

private:
	static Material** materialArray;
	static RenderState::StateBlock* stateBlock;
	MaterialManager();
};

#endif