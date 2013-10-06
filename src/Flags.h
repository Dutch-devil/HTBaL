#ifndef Flags_H_
#define Flags_H_

#include <memory.h>
#include <gameplay.h>

using namespace std;
using namespace gameplay;

class Flags {
public:
	Flags(unsigned int flagCount);
	~Flags();

	virtual void setFlag(unsigned int flagIndex);
	virtual void resetFlag(unsigned int flagIndex);
	virtual void forceFlag(unsigned int flagIndex, bool set);
	virtual bool getFlag(unsigned int flagIndex);
private:
	unsigned char* flags;
};
#endif