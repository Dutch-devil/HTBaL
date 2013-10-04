#ifndef KeyFlags_H_
#define KeyFlags_H_

#include "Flags.h"
#include "gameplay.h"

using namespace gameplay;

class KeyFlags: Flags {
public:
	KeyFlags();
	
	unsigned int convertFlag(unsigned int flagIndex);

	void setFlag(unsigned int flagIndex);
	void resetFlag(unsigned int flagIndex);
	bool getFlag(unsigned int flagIndex);
private:
	
};
#endif