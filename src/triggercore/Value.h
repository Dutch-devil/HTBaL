#ifndef Value_H_
#define Value_H_

#include <string>

using namespace std;

class Value {
public:
    enum ValueType {
        NONE, INTEGER, FLOAT, CHARACTER, STRING
    };
    
    Value(int);
    Value(float);
    Value(char);
    Value(const char*);
    ~Value();
    
    ValueType getValueType();
    int getIntValue();
    float getFloatValue();
    char getCharValue();
    const char* getStringValue();
    
    bool operator==(Value& other);
    bool operator<(Value& other);
    bool operator>(Value& other);
    
private:
    ValueType type;
    void* value;
};

#endif