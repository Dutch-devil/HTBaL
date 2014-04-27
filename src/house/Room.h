#ifndef Room_H_
#define Room_H_

class Room;

#include <list>
#include "gameplay.h"
#include "Floor.h"
#include "House.h"

using namespace std;
using namespace gameplay;

class Room {
public:
    enum Type {
        LIVING_QUARTERS,
        BEDROOM,
        DINING_ROOM,
        STORAGE_ROOM,
        LIBARY,
        LABORATORY,
        ARBORETUM,
        WORKSHOP,
        SMITHY,
        STINKY_SEWERS,
        DIRTPLOT,
        STABLES,
        ARSENAL_ROOM,
        AVIARY,
        POOL,
        ART_ROOM,
        BALLROOM,
        ENTRANCE,
        STAIRS_UP,
        STAIRS_DOWN,
        STAIRS_UP_DOWN,
        HALL,
        ROOM_EMPTY,
        NONE,
    };
    
    enum Direction {
        UNDEF,
        XDIR,
        YDIR
    };
    
    Room(House* house, list<Floor*> floor, list<Wall*> walls);
    Room(House* house, list<Floor*> floor, list<Wall*> walls, Room::Type roomType);
    ~Room();
    
    Room::Type getRoomType();
    void setRoomType(Room::Type roomType);
    const char* getRoomTypeString();
    static const char* getRoomTypeString(Room::Type roomType);
    
    list<Floor*> getFloor();
    void setFloor(list<Floor*> floor);
    list<Wall*> getWalls();
    
    bool contains(Floor* floorTile);
    
    int getSize();
    
    int getMaxLine();
    int getMaxSquareId();
    int getMaxRectangleId();
    int getMaxAreaSquare();
    int getMaxAreaRect();
    Direction getMaxRectDir();
    
    list<Floor*> getCorners();
    list<Floor*> getLine(Floor* startTile, FloorDirection::Direction dir);
    list<Floor*> getLine(Floor* startTile, FloorDirection::Direction dir, unsigned int maxLength);
    void calculateSize();
    
protected:
    House* house;
    Room::Type roomType;
    list<Floor*> floor;
    list<Wall*> walls;
    
    int maxLine;
    int maxSquareId;
    int maxRectangleId;
    int maxAreaSquare;
    int maxAreaRect;
    Direction maxRectDir;
    
    Room(int, int, list<Wall*>, Scene* scene);
};

#endif
