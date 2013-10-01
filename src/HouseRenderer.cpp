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

    createHouse(house);
}

void HouseRenderer::createHouse(House house) {
    float screenWidth = 100;
    float screenHeight = 100;

    float tileWidth = screenWidth / house.getWidth();
    float tileHeight = screenHeight / house.getHeight();

    for (int x = 0; x < house.getWidth(); x++) {
        for (int y = 0; y < house.getHeight(); y++) {
            Mesh* tileMesh = Mesh::createQuad(Vector3(-50, 0, -50),
                                               Vector3(-50, 0, 50),
                                               Vector3(50, 0, -50),
                                               Vector3(50, 0, 50));

            Model* tileModel = Model::create(tileMesh);

            // Create the ground material
            Material* houseMaterial = tileModel->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "TEXTURE_REPEAT;TEXTURE_OFFSET");
            // Set render state block
            houseMaterial->setStateBlock(stateBlock);

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