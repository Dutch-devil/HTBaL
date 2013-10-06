#include "MaterialManager.h"

MaterialManager::MaterialManager() {}

Properties** MaterialManager::propertiesArray = NULL;
RenderState::StateBlock* MaterialManager::stateBlock = NULL;

Material* MaterialManager::getMaterial(MaterialType type) {
	if (propertiesArray == NULL) {
		propertiesArray = new Properties*[COUNT*2];
		memset(propertiesArray, NULL, COUNT*2*sizeof(Properties*));

		stateBlock = RenderState::StateBlock::create();
		//stateBlock->setCullFace(false);
		stateBlock->setBlend(true);
		stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
		stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
	}
	if(propertiesArray[type*2] == NULL) {
		switch(type) {
		case FLOOR:
			propertiesArray[type*2+1] = Properties::create("res/house/floor.material");
			propertiesArray[type*2] = fixProperties(propertiesArray[type*2+1]);
			print("creating: %d at %d\n",propertiesArray[type], type);
			break;
		case WALL:
			propertiesArray[type*2+1] = Properties::create("res/house/wall.material");
			propertiesArray[type*2] = fixProperties(propertiesArray[type*2+1]);
			print("creating: %d at %d\n",propertiesArray[type], type);
			break;
		case DOOR:
			propertiesArray[type*2+1] = Properties::create("res/house/door.material");
			propertiesArray[type*2] = fixProperties(propertiesArray[type*2+1]);
			print("creating: %d at %d\n",propertiesArray[type], type);
			break;
		default:
			GP_ERROR("No matching material found");
		}
	}

	Material* material = Material::create(propertiesArray[type*2]);
	material->setStateBlock(stateBlock);
	return material;
}

Properties* MaterialManager::fixProperties(Properties* properties) {
	return (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
}

void MaterialManager::finalize(){
	SAFE_RELEASE(stateBlock);
	for(int i = 0; i < COUNT*2; i += 2) {
		propertiesArray[i] = NULL;
		SAFE_DELETE(propertiesArray[i+1]);
	}
	SAFE_DELETE_ARRAY(propertiesArray);
}