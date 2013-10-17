#ifndef HouseFactory_H_
#define HouseFactory_H_

#include "../house/House.h"
#include "RoomFactory.h"

class HouseFactory {
public:
    struct WalledTile {
        WalledTile(int id): id(id), dir(FloorDirection::Direction::NONE) {}
        WalledTile(int id, FloorDirection::Direction dir): id(id), dir(dir) {}
        int id;
        FloorDirection::Direction dir;
    };
    
    static House* createHouse(Scene* scene, float screenSize);
    static House* createHouse(Scene* scene, float screenSize, unsigned int width, unsigned int height);
    
    static House* createRandomHouse(Scene* scene, float screenSize);
    static House* createRandomHouse(Scene* scene, float screenSize, unsigned int width, unsigned int height);
    
protected:
    static void setDoor(House* house, list<Floor*> tiles, WalledTile adjacent);
    
    static void clearAllAround(House* house, vector<WalledTile>* ids, int startId);
    static void pushAllHallAround(House* house, vector<WalledTile>* ids, int x, int y);
    static bool canBeHallway(House* house, Floor* tile);
    static bool floorTouchesSide(House* house, int id);
    static bool floorHasNeighbours(House* house, int id);
    
    static void pushAllRoomAround(House* house, vector<int>* ids, int id);
    static void pushAllRoomAround(House* house, vector<WalledTile>* ids, int id);
    static bool canBeRoom(Floor* tile);
    static void removeId(vector<int>* ids, int id);
    static void removeId(vector<WalledTile>* ids, int id);
    static list<Floor*> getGaps(House* house, vector<WalledTile>* toCheck);
    static list<Floor*> getGaps(House* house, vector<WalledTile>* toCheck, unsigned int maxSize);
    static bool getEnclosed(House* house, int startId, list<Floor*>* others);
};

#endif