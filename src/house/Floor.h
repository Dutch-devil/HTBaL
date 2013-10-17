#ifndef Floor_H_
#define Floor_H_

// all different colors
#define FLOOR_WHITE new Vector3(1, 1, 1)
#define FLOOR_RED new Vector3(1, 0, 0)
#define FLOOR_GREEN new Vector3(0, 1, 0)
#define FLOOR_BLUE new Vector3(0, 0, 1)
#define FLOOR_YELLOW new Vector3(1, 1, 0)
#define FLOOR_PURPLE new Vector3(1, 0, 1)
#define FLOOR_BLACK new Vector3(0, 0, 0)
#define FLOOR_GREY new Vector3(.5, .5, .5)
#define FLOOR_TEAL new Vector3(.5, .5, 1)

#define FLOOR_UNSELECTED FLOOR_WHITE
#define FLOOR_SELECTED FLOOR_RED
#define FLOOR_HOVER FLOOR_GREEN
#define FLOOR_HOVER_SELECTED new Vector3(.5, .5, 0)

#define FLOOR_NONE FLOOR_BLACK
#define FLOOR_EMPTY FLOOR_WHITE
#define FLOOR_ENTRANCE FLOOR_YELLOW
#define FLOOR_STAIR FLOOR_PURPLE
#define FLOOR_HALL FLOOR_BLUE
#define FLOOR_ROOM FLOOR_TEAL

class Floor;
struct FloorDirection {
    enum Direction {
        TOP,    // y - 1
        LEFT,   // x - 1
        BOTTOM, // y + 1
        RIGHT,  // x + 1
        NONE   // no wall
    };
    
    static FloorDirection::Direction invert(FloorDirection::Direction dir);
};

#include <list>
#include "gameplay.h"
#include "../Flags.h"
#include "../MaterialManager.h"
#include "Wall.h"
#include "Room.h"

using namespace gameplay;
using namespace std;


class Floor {
public:
    Floor(int id);
    ~Floor();
    
    static void setHeightWidth(float height, float width);
    static float getHeight();
    static float getWidth();
    
    static Mesh* getMesh();
    static void finalize();
    int getId();
    
    Floor* setWall(FloorDirection::Direction dir, bool wall);
    bool getWall(FloorDirection::Direction dir);
    Floor* setDoor(FloorDirection::Direction dir, bool door);
    bool getDoor(FloorDirection::Direction dir);
    
    Floor* setRoom(Room* room);
    Room* getRoom();
    
    Model* getModel();
    
    void updateOverlayColor();
    void updateMaterial();
    
    bool getSelected();
    Floor* setSelected(bool selected);
    Floor* toggleSelect();
    Floor* setHover(bool hover);
    
    
    static void releaseMesh();
    
protected:
    static Mesh* mesh;
    static float width, height;
    static void calculateMesh();
    
    int id;
    Model* model;
    
    Vector3* overlayColor;
    Vector3* materialColor;
    bool selected, hover;
    
    Flags* walls;
    Flags* doors;
    Room* room;
    
    Vector3* getColor();
    bool isColor(Vector3*);
    Vector3 blendColors(Vector3* color1, Vector3* color2);
};

#endif