#include "HouseRenderer.h"

HouseRenderer::HouseRenderer(){
	initialize();
}
HouseRenderer::HouseRenderer(Scene* scene) {
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
 
    // Calculate initial matrices
    Matrix::createPerspective(45.0f, (float)800 / (float)500, 0.25f, 100.0f, &projectionMatrix);
    Matrix::createLookAt(0, 1.75f, 1.35f, 0, 0, -0.15f, 0, 0.20f, -0.80f, &viewMatrix);
    Matrix::multiply(projectionMatrix, viewMatrix, &viewProjectionMatrix);

	createHouse(house);
}

void HouseRenderer::createHouse(House house) {
	Mesh* houseMesh = Mesh::createQuad(Vector3(-50, 0, -50),
                                        Vector3(-50, 0, 50),
                                        Vector3(50, 0, -50),
                                        Vector3(50, 0, 50));

	houseModel = Model::create(houseMesh);
 
	// Create the ground material
	Material* houseMaterial = houseModel->setMaterial("res/shaders/textured-unlit.vert", "res/shaders/textured-unlit.frag", "TEXTURE_REPEAT;TEXTURE_OFFSET");
	// Set render state block	
	houseMaterial->setStateBlock(stateBlock);

	Texture::Sampler* houseSampler = houseMaterial->getParameter("u_diffuseTexture")->setValue("res/asphalt.png", true);
	houseSampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);

    // Release objects that are owned by mesh instances
    SAFE_RELEASE(houseMesh);
}

Renderers HouseRenderer::update(float elapsedTime) {
	return KEEP;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
    houseModel->draw();
}