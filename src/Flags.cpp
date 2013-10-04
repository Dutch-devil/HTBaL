#include "Flags.h"

Flags::Flags(unsigned int flagCount) {
	// + 7 for rounding up
	unsigned int flagBytes = (flagCount + 7) / 8;
	flags = new unsigned char[flagBytes];
	memset(flags, 0, flagBytes * sizeof(char));
}

Flags::~Flags() {
	delete[] flags;
}

void Flags::setFlag(unsigned int flagIndex) {
	flags[flagIndex / 8] |= (1 << (flagIndex % 8));
}

void Flags::resetFlag(unsigned int flagIndex) {
	flags[flagIndex / 8] &= ~(1 << (flagIndex % 8));
}

bool Flags::getFlag(unsigned int flagIndex) {
	return (flags[flagIndex / 8] & (1 << (flagIndex % 8))) != 0;
}