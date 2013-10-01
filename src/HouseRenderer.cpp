#include "HouseRenderer.h"

HouseRenderer::HouseRenderer(Scene* scene) {
	this->scene = scene;
    initialize();
}

HouseRenderer::~HouseRenderer() {
}

void HouseRenderer::initialize() {
    House house = House(5, 5);
    list<Vector2> vector = list<Vector2>();
    Room room = Room(2, 3, vector);
    house.addRoom(room);

    // Create our render state block that will be reused
    // across all materials
    stateBlock = RenderState::StateBlock::create();
    stateBlock->setCullFace(true);
    stateBlock->setBlend(true);
    stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

    Camera* camera = Camera::createPerspective(45, 1, 0.25, 100);
    Node* cameraNode = scene->addNode();
    cameraNode->setCamera(camera);
	scene->setActiveCamera(camera);

    createHouse(house);
}

void HouseRenderer::createHouse(House house) {
    float screenWidth = 100;
    float screenHeight = 100;

    float tileWidth = screenWidth / house.getWidth();
    float tileHeight = screenHeight / house.getHeight();

    for (int x = 0; x < house.getWidth(); x++) {
        for (int y = 0; y < house.getHeight(); y++) {
            Mesh* tileMesh = Mesh::createQuad(Vector3(-tileWidth / 2, -tileHeight / 2, 0),
                                               Vector3(tileWidth / 2, -tileHeight / 2, 0),
                                               Vector3(-tileWidth / 2, tileHeight / 2, 0),
                                               Vector3(tileWidth / 2, tileHeight / 2, 0));

            Model* tileModel = Model::create(tileMesh);

            // Create the ground material
            tileModel->setMaterial("res/grid.material")->setStateBlock(stateBlock);

			Node* tileNode = scene->addNode();
			tileNode->translateZ(-100 + .1);
			tileNode->translateX((x - (float)house.getWidth() / 2) * tileWidth + tileWidth / 2);
			tileNode->translateY((y - (float)house.getHeight() / 2) * tileHeight + tileHeight / 2);
			tileNode->setModel(tileModel);
			tileModels.push_back(tileModel);
        }
    };
}

Renderers HouseRenderer::update(float elapsedTime) {
    return KEEP;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
    for (Model* tileModel : tileModels) {
        tileModel->draw();
    }
}