#include "MaterialManager.h"

MaterialManager::MaterialManager() {}

Material** MaterialManager::materialArray = NULL;
RenderState::StateBlock* MaterialManager::stateBlock = NULL;

Material* MaterialManager::getMaterial(MaterialType type) {
	if(materialArray == NULL) {
		materialArray = new Material*[COUNT];

		stateBlock = RenderState::StateBlock::create();
		stateBlock->setCullFace(true);
		stateBlock->setBlend(true);
		stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
		stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
	}
	if(materialArray[type] != NULL)
		return materialArray[type];
	switch(type) {
	case FLOOR_EMPTY:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(1, 1, 1));
		return materialArray[type];
	case FLOOR_HALL_START:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(1, 1, 0));
		return materialArray[type];
	case FLOOR_HALL:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(0, 0, 1));
		return materialArray[type];
	case FLOOR_ROOM:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(0, 1, 0));
		return materialArray[type];
	case FLOOR_STAIR:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(1, 0, 1));
		return materialArray[type];
	case FLOOR_EMPTY_SEL:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(1, 0.5, 0.5));
		return materialArray[type];
	case FLOOR_HALL_START_SEL:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(1, .5, 0));
		return materialArray[type];
	case FLOOR_HALL_SEL:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(0.5, 0, .5));
		return materialArray[type];
	case FLOOR_ROOM_SEL:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(0.5, 0.5, 0));
		return materialArray[type];
	case FLOOR_STAIR_SEL:
		materialArray[type] = Material::create("res/house/floor.material");
		materialArray[type]->setStateBlock(stateBlock);
		materialArray[type]->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(1, 0, 0.5));
		return materialArray[type];
	case WALL:
		materialArray[type] = Material::create("res/house/wall.material");
		materialArray[type]->setStateBlock(stateBlock);
		return materialArray[type];
	}
	GP_ERROR("No matching material found");
	return NULL;
}