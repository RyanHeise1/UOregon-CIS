#include <stdio.h>

unsigned int extract(unsigned int x, int i){
	// Move byte to most significant position
	signed int move = x << ((3 - i) << 3);
	// Extend bit to 32 bytes
	return move >> 24;
}

int main() {
	printf("0x%X\n", extract(0x12345678, 0)); // 0x00000078
	printf("0x%X\n", extract(0xABCDEF00, 2)); // 0xFFFFFFCD
}

