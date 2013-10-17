#include "RoomFactory.h"

Room* RoomFactory::createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles) {
    return createRoomFromFloor(scene, house, roomTiles, Room::Type::ROOM_EMPTY);
}

Room* RoomFactory::createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles, Room::Type roomType) {
    Floor** floorTiles = new Floor*[house->getWidth() * house->getHeight()];
    memset(floorTiles, NULL, sizeof(Floor*) * house->getWidth() * house->getHeight());
    
    for (Floor * floor : roomTiles) {
        floorTiles[floor->getId()] = floor;
    }
    
    list<Wall*> walls;
    
    for (Floor * floorTile : roomTiles) {
        int i = floorTile->getId();
        
        Floor* floor = house->getFloorInDirection(i, FloorDirection::LEFT);
        if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, FloorDirection::LEFT));
        }
        
        floor = house->getFloorInDirection(i, FloorDirection::TOP);
        if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, FloorDirection::TOP));
        }
        
        floor = house->getFloorInDirection(i, FloorDirection::RIGHT);
        if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, FloorDirection::RIGHT));
        }
        
        floor = house->getFloorInDirection(i, FloorDirection::BOTTOM);
        if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, FloorDirection::BOTTOM));
        }
    }
    
    SAFE_DELETE_ARRAY(floorTiles);
    Room* room = new Room(house, roomTiles, walls, roomType);
    return room;
}

Wall* RoomFactory::createWall(Scene* scene, Floor* floorTile, FloorDirection::Direction dir) {
    floorTile->setWall(dir, true);
    Wall* wall = new Wall(!(dir == FloorDirection::LEFT || dir == FloorDirection::BOTTOM), floorTile->getDoor(dir));
    Node* wallNode = scene->addNode();
    floorTile->getModel()->getNode()->addChild(wallNode);
    switch (dir) {
        case FloorDirection::LEFT:
            wallNode->rotateZ(MATH_PI);
            break;
        case FloorDirection::BOTTOM:
            wallNode->rotateZ(MATH_PI / 2);
            break;
        case FloorDirection::TOP:
            wallNode->rotateZ(-MATH_PI / 2);
            break;
    }
    Model* model = wall->getModel();
    wallNode->setModel(model);
    return wall;
}