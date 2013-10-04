#include "MaterialManager.h"

MaterialManager::MaterialManager() {}

Properties** MaterialManager::propertiesArray = NULL;
RenderState::StateBlock* MaterialManager::stateBlock = NULL;

Material* MaterialManager::getMaterial(MaterialType type) {
	if (propertiesArray == NULL) {
		propertiesArray = new Properties*[COUNT];
		memset(propertiesArray, NULL, COUNT*sizeof(Properties*));

		stateBlock = RenderState::StateBlock::create();
		//stateBlock->setCullFace(false);
		stateBlock->setBlend(true);
		stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
		stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
	}
	if(propertiesArray[type] == NULL) {
		switch(type) {
		case FLOOR:
			propertiesArray[type] = fixProperties(Properties::create("res/house/floor.material"));
			break;
		case WALL:
			propertiesArray[type] = fixProperties(Properties::create("res/house/wall.material"));
			break;
		case DOOR:
			propertiesArray[type] = fixProperties(Properties::create("res/house/door.material"));
			break;
		default:
			GP_ERROR("No matching material found");
		}
	}
	Material* material = Material::create(propertiesArray[type]);
	material->setStateBlock(stateBlock);
	return material;
}

Properties* MaterialManager::fixProperties(Properties* properties) {
	return (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
}