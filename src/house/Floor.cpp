
#include "Floor.h"


float Floor::width, Floor::height;
Mesh* Floor::mesh = NULL;

FloorDirection::Direction FloorDirection::invert(FloorDirection::Direction dir) {
    return (FloorDirection::Direction)(((int)dir + 2) % 4);
}

Floor::Floor(int id): id(id), room(NULL) {
    this->model = Model::create(getMesh());
    this->walls = new Flags(4);
    this->doors = new Flags(5);
    
    materialColor = NULL;
    overlayColor = FLOOR_UNSELECTED;
    hover = selected = false;
    
    Material* material = MaterialManager::getMaterial(FLOOR);
    model->setMaterial(material);
    SAFE_RELEASE(material);
    updateMaterial();
}

Floor::~Floor() {
    SAFE_DELETE(walls);
    SAFE_DELETE(doors);
    SAFE_DELETE(overlayColor);
    SAFE_DELETE(materialColor);
    SAFE_RELEASE(model);
}

void Floor::finalize() {
    SAFE_RELEASE(mesh);
}

void Floor::calculateMesh() {
    SAFE_RELEASE(mesh);
    mesh = Mesh::createQuad(Vector3(-Floor::getWidth() / 2, -Floor::getHeight() / 2, 0),
                            Vector3(Floor::getWidth() / 2, -Floor::getHeight() / 2, 0),
                            Vector3(-Floor::getWidth() / 2, Floor::getHeight() / 2, 0),
                            Vector3(Floor::getWidth() / 2, Floor::getHeight() / 2, 0));
}

Mesh* Floor::getMesh() {
    if (mesh == NULL) {
        calculateMesh();
    }
    return mesh;
}

void Floor::setHeightWidth(float height, float width) {
    Floor::height = height;
    Floor::width = width;
    calculateMesh();
    Wall::calculateMesh();
}

float Floor::getHeight() {
    return height;
}

float Floor::getWidth() {
    return width;
}

Floor* Floor::setWall(FloorDirection::Direction dir, bool wall) {
    walls->forceFlag(dir, wall);
    return this;
}

bool Floor::getWall(FloorDirection::Direction dir) {
    return walls->getFlag(dir);
}

Floor* Floor::setDoor(FloorDirection::Direction dir, bool door) {
    doors->forceFlag(dir, door);
    return this;
}

bool Floor::getDoor(FloorDirection::Direction dir) {
    return doors->getFlag(dir);
}

Floor* Floor::setRoom(Room* room) {
    this->room = room;
    updateMaterial();
    return this;
}

Room* Floor::getRoom() {
    return room;
}

int Floor::getId() {
    return id;
}

Model* Floor::getModel() {
    return model;
}

Vector3* Floor::getColor() {
    return overlayColor;
}

bool Floor::isColor(Vector3* other) {
    return materialColor->x == other->x && materialColor->y == other->y && materialColor->z == other->z;
}

void Floor::updateOverlayColor() {
    SAFE_DELETE(overlayColor);
    if (selected && hover) {
        overlayColor = FLOOR_HOVER_SELECTED;
    } else if (selected) {
        overlayColor = FLOOR_SELECTED;
    } else if (hover) {
        overlayColor = FLOOR_HOVER;
    } else {
        overlayColor = new Vector3(*materialColor);
    }
    model->getMaterial()->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(blendColors(overlayColor, materialColor));
}

void Floor::updateMaterial() {
    SAFE_DELETE(materialColor);
    if (getRoom()) {
        switch (getRoom()->getRoomType()) {
            case Room::Type::LIVING_QUARTERS:
            case Room::Type::BEDROOM:
            case Room::Type::DINING_ROOM:
            case Room::Type::STORAGE_ROOM:
            case Room::Type::LIBARY:
            case Room::Type::LABORATORY:
            case Room::Type::ARBORETUM:
            case Room::Type::WORKSHOP:
            case Room::Type::SMITHY:
            case Room::Type::STINKY_SEWERS:
            case Room::Type::DIRTPLOT:
            case Room::Type::STABLES:
            case Room::Type::ARSENAL_ROOM:
            case Room::Type::AVIARY:
            case Room::Type::POOL:
            case Room::Type::ART_ROOM:
            case Room::Type::BALLROOM:
                materialColor = FLOOR_ROOM;
                break;
            case Room::Type::ENTRANCE:
                materialColor = FLOOR_ENTRANCE;
                break;
            case Room::Type::STAIRS_UP:
            case Room::Type::STAIRS_DOWN:
            case Room::Type::STAIRS_UP_DOWN:
                materialColor = FLOOR_STAIR;
                break;
            case Room::Type::HALL:
                materialColor = FLOOR_HALL;
                break;
            case Room::Type::ROOM_EMPTY:
                materialColor = FLOOR_EMPTY;
                break;
            default:
                materialColor = FLOOR_NONE;
        }
    } else {
        materialColor = FLOOR_NONE;
    }
    updateOverlayColor();
}

Vector3 Floor::blendColors(Vector3* color1, Vector3* color2) {
    return .5 * (*color1) + .5 * (*color2);
}

bool Floor::getSelected() {
    return selected;
}

Floor* Floor::setSelected(bool selected) {
    this->selected = selected;
    updateOverlayColor();
    return this;
}

Floor* Floor::toggleSelect() {
    selected = !selected;
    updateOverlayColor();
    return this;
}

Floor* Floor::setHover(bool hover) {
    this->hover = hover;
    updateOverlayColor();
    return this;
}