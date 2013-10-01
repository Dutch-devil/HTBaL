#include "HouseRenderer.h"

HouseRenderer::HouseRenderer(Scene* scene, Rectangle viewport, float aspectRatio) {
	this->scene = scene;
	this->viewport = viewport;
	this->aspectRatio = aspectRatio;
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

    Camera* camera = Camera::createPerspective(45, aspectRatio, 0.25, 100);
    Node* cameraNode = scene->addNode();
    cameraNode->setCamera(camera);
	scene->setActiveCamera(camera);

    createHouse(house);
}

void HouseRenderer::createHouse(House house) {
	Vector3* destination = new Vector3();
	scene->getActiveCamera()->unproject(viewport, 0, 0, 1, destination);
	print("%f, %f, %f\n", destination->x, destination->y, destination->z);

    float screenWidth = -2 * destination->x;
    float screenHeight = 2 * destination->y;
	float screenSize = (screenWidth > screenHeight)?screenHeight:screenWidth;

    float tileWidth = screenSize / house.getWidth();
    float tileHeight = screenSize / house.getHeight();

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
			tileNode->translateZ(-scene->getActiveCamera()->getFarPlane() + .1);
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