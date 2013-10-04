#ifndef Flags_H_
#define Flags_H_

#include <memory.h>

using namespace std;

class Flags {
public:
	Flags(unsigned int flagCount);
	~Flags();

	virtual void setFlag(unsigned int flagIndex);
	virtual void resetFlag(unsigned int flagIndex);
	virtual bool getFlag(unsigned int flagIndex);
private:
	unsigned char* flags;
};
#endif