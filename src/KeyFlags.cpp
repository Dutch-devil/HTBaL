#include "KeyFlags.h"


KeyFlags::KeyFlags(): Flags((Keyboard::Key::KEY_DOWN_ARROW - Keyboard::Key::KEY_LEFT_ARROW + 1) + ('Z' - 'A' + 1)) {}

unsigned int KeyFlags::convertFlag(unsigned int flagIndex) {
	if (flagIndex >= Keyboard::Key::KEY_LEFT_ARROW && flagIndex <= Keyboard::Key::KEY_DOWN_ARROW) {
		return flagIndex - Keyboard::Key::KEY_LEFT_ARROW;
	}else if (flagIndex >= 'A' && flagIndex <= 'Z') {
		return flagIndex - 'A' + 4;
	}else if (flagIndex >= 'a' && flagIndex <= 'z') {
		return flagIndex - 'a' + 4;
	}
	GP_WARN("Invalid flagIndex specified");
	return -1;
}

void KeyFlags::setFlag(unsigned int flagIndex) {
	unsigned int index = convertFlag(flagIndex);
	if (index != -1) {
		Flags::setFlag(index);
	}
}

void KeyFlags::resetFlag(unsigned int flagIndex) {
	int index = convertFlag(flagIndex);
	if (index != -1) {
		Flags::resetFlag(index);
	}
}

bool KeyFlags::getFlag(unsigned int flagIndex) {
	int index = convertFlag(flagIndex);
	if (index != -1) {
		return Flags::getFlag(index);
	}
	return false;
}