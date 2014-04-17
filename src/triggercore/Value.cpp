
#include "Value.h"

Value::Value(int ivalue) {
    type = INTEGER;
	value = &ivalue;
}

Value::Value(float fvalue) {
    type = FLOAT;
    value = &fvalue;
}

Value::Value(char cvalue) {
    type = CHARACTER;
    value = &cvalue;
}

Value::Value(const char* svalue) {
    type = STRING;
    value = &svalue;
}

Value::~Value() {
    switch (type) {
        case INTEGER:
            //delete (int*)value;
            break;
        case FLOAT:
            delete (float*)value;
            break;
        case CHARACTER:
            delete (char*)value;
            break;
        case STRING:
            delete[] (const char**)value;
            break;
    }
    value = 0;
    type = NONE;
}

Value Value::parseValue(const char* toParse) {
    int parsed;
    char overflow;
    int intValue;
    parsed = sscanf_s(toParse, "%d%c", &intValue, &overflow);
    if (parsed == 1) {
        // no more characters after matching, must be an integer
        return Value(intValue);
    }
    float floatValue;
    parsed = sscanf_s(toParse, "%f%c", &floatValue, &overflow);
    if (parsed == 1) {
        return Value(floatValue);
    }
    char charValue;
    parsed = sscanf_s(toParse, "%c%c", &charValue, &overflow);
    if (parsed == 1) {
        return Value(charValue);
    }
    // just return the whole string
    return Value(toParse);
}



Value::ValueType Value::getValueType() {
    return type;
}

int Value::getIntValue() {
    return *((int*)value);
}

float Value::getFloatValue() {
    return *((float*)value);
}

char Value::getCharValue() {
    return *((char*)value);
}

const char* Value::getStringValue() {
    return *((const char**)value);
}


bool Value::operator==(Value& other) {
    if (getValueType() != other.getValueType()) {
        return false;
    }
    switch (getValueType()) {
        case Value::ValueType::INTEGER:
            return getIntValue() == other.getIntValue();
        case Value::ValueType::FLOAT:
            return getFloatValue() == other.getFloatValue();
        case Value::ValueType::CHARACTER:
            return getCharValue() == other.getCharValue();
        case Value::ValueType::STRING:
            return strcmp(getStringValue(), other.getStringValue()) == 0;
    }
    return false;
}

bool Value::operator<(Value& other) {
    if (getValueType() != other.getValueType()) {
        return false;
    }
    switch (getValueType()) {
        case Value::ValueType::INTEGER:
            return getIntValue() < other.getIntValue();
        case Value::ValueType::FLOAT:
            return getFloatValue() < other.getFloatValue();
        case Value::ValueType::CHARACTER:
            return getCharValue() < other.getCharValue();
        case Value::ValueType::STRING:
            return strcmp(getStringValue(), other.getStringValue()) < 0;
    }
    return false;
}

bool Value::operator>(Value& other) {
    if (getValueType() != other.getValueType()) {
        return false;
    }
    switch (getValueType()) {
        case Value::ValueType::INTEGER:
            return getIntValue() > other.getIntValue();
        case Value::ValueType::FLOAT:
            return getFloatValue() > other.getFloatValue();
        case Value::ValueType::CHARACTER:
            return getCharValue() > other.getCharValue();
        case Value::ValueType::STRING:
            return strcmp(getStringValue(), other.getStringValue()) > 0;
    }
    return false;
}