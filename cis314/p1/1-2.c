#include <stdio.h>

unsigned int replace(unsigned int x, int i, unsigned int y){
	// bin 0xFF => 0b11111111
	// Shifting i 3 times will give you intervals of 8 bits
	//    ie: 0 << 3 = 0, 1 << 3 = 8, 2 << 3 = 16...
	//    we can take this and shift it by 0xFF to get the hex
	//       value in the right spot
	int i_shift = i << 3;
	// Move the 0xFF value to the correct index
	unsigned int mask = 0xFF << i_shift;
	// Here we are getting rid of the specific place with the ~mask
	//    so we can replace it with y
	unsigned int new_x = x & ~mask;
	// Shifts the new hex value to the significant bit
	unsigned int new_y = y << i_shift;
	// Taking the 2 values and adding in the new hex value
	return new_x | new_y;
}

int main(){
	printf("%X\n", replace(0x12345678, 3, 0xAB)); // 0xAB345678
	printf("%X\n", replace(0x12345678, 0, 0xAB)); // 0x123456AB
}
