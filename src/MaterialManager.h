#ifndef MaterialManager_H_
#define MaterialManager_H_

#include "gameplay.h"

using namespace gameplay;

enum MaterialType {
	FLOOR,
	WALL,
	DOOR,
	MENU_WHEEL,
	COUNT
};

class MaterialManager {
public:
	static Material* getMaterial(MaterialType type);
	static void finalize();

private:
	static Properties** propertiesArray;
	static RenderState::StateBlock* stateBlock;
	static Properties* fixProperties(Properties* properties);
	MaterialManager();
};

#endif