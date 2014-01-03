#ifndef House_H_
#define House_H_

class House;

#include <list>
#include <cmath>
#include "../MaterialManager.h"
#include "Wall.h"
#include "Room.h"
#include "Floor.h"

#include "../triggercore/statetrigger/HouseStateTrigger.h"

using namespace std;

class House : public HouseStateTrigger {
public:
    House(int, int, float);
    ~House(void);
    
    bool floorUp();
    bool floorDown();
    void setFloorMiddle();
    bool setFloor(unsigned int floorIndex);
    void addFloorBottom(Scene* scene);
    void addFloorTop(Scene* scene);
    void removeFloorTop();
    void removeFloorBottom();
    
    void addRoom(Room* room);
    
    int getWidth();
    int getHeight();
    
    int getIdByXY(int x, int y);
    int getXById(int id);
    int getYById(int id);
    
    Floor** getFloorTiles();
    Floor* getFloorTile(int id);
    Floor* getFloorTile(int x, int y);
    Floor* getFloorInDirection(int id, FloorDirection::Direction dir);
    Floor* getFloorInDirection(int x, int y, FloorDirection::Direction dir);
    int getFloorIdInDirection(int id, FloorDirection::Direction dir);
    int getFloorIdInDirection(int x, int y, FloorDirection::Direction dir);
    
    list<Room*> getRooms();
    Room* getRoom(Floor* tile);
    
protected:
    int height, width;
    float screenSize;
    unsigned int floorIndex, middleIndex;
    Floor** floorTiles;
    vector<Floor**> floors;
    vector<list<Room*>> rooms;
    
    void addFloor(Scene* scene, Floor** refFloor);
};

#endif