#include "MaterialManager.h"

MaterialManager::MaterialManager() {}

Properties** MaterialManager::propertiesArray = NULL;
RenderState::StateBlock* MaterialManager::stateBlock = NULL;

Material* MaterialManager::getMaterial(MaterialType type) {
	if (propertiesArray == NULL) {
		propertiesArray = new Properties*[COUNT];
		memset(propertiesArray, NULL, COUNT * sizeof(Properties*));

		stateBlock = RenderState::StateBlock::create();
		//stateBlock->setCullFace(false);
		stateBlock->setBlend(true);
		stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
		stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
	}
	if (propertiesArray[type] == NULL) {
		switch(type) {
		case FLOOR:
			propertiesArray[type] = Properties::create("res/house/floor.material");
			break;
		case WALL:
			propertiesArray[type] = Properties::create("res/house/wall.material");
			break;
		case DOOR:
			propertiesArray[type] = Properties::create("res/house/door.material");
			break;
		case MENU_WHEEL:
			propertiesArray[type] = Properties::create("res/menu/menuWheel.material");
			break;
		default:
			GP_ERROR("No matching material found");
		}
	}

	Material* material = Material::create(fixProperties(propertiesArray[type]));
	material->setStateBlock(stateBlock);
	return material;
}

Properties* MaterialManager::fixProperties(Properties* properties) {
	properties->rewind();
	return (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
}

void MaterialManager::finalize(){
	SAFE_RELEASE(stateBlock);
	for (int i = 0; i < COUNT; i++) {
		SAFE_DELETE(propertiesArray[i]);
	}
	SAFE_DELETE_ARRAY(propertiesArray);
}