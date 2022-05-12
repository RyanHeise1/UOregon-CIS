#include <stdio.h>
#include <stdlib.h>

typedef struct Line {
	unsigned char data[4];
	unsigned int tag;
	unsigned char valid;
}Line;

typedef struct Cache {
	struct Line *lines;
	int numLines;
}Cache;

unsigned int getOffset(unsigned int address) {
// 4B blocks, so offset is bits 0-1
return address & 0x3;
}

unsigned int getSet(unsigned int address) {
	// 16 sets, so offset is bits 2-6
	return (address >> 2) & 0xF;
}

unsigned int getTag(unsigned int address) {

	// Offset and set are 6 bits total, so tag is high-order 26 bits
	return address >> 6;
}

struct Cache* mallocCache(int numLines) {
	// TODO - malloc a pointer to a struct Cache, malloc a pointer to an array
	// of struct Line instances (array length is numLines). Also initialize
	// valid to 0 for each struct Line. Return the struct Cache pointer.
	
	
	struct Cache  *nc = (struct Cache *)malloc(sizeof(Cache *));
	nc->numLines = numLines;
	nc->lines = (struct Line *)malloc(numLines * sizeof(Line));

	for(int i = 0; i < numLines; ++i){
		nc->lines[i].valid = 0;
	}

	return nc;
}

void freeCache(struct Cache *cache) {
	free(cache->lines);
	free(cache);
}

void printCache(struct Cache *cache) {
	for(int i = 0; i < cache->numLines; ++i){
		if(cache->lines[i].valid == 1){
			struct Line *line = &cache->lines[i];
			printf("set: %x - tag: %x - valid: %u - data: %.2x %.2x %.2x %.2x\n", 
				i, line->tag, line->valid, 
				line->data[0], 
				line->data[1], 
				line->data[2], 
				line->data[3]);
		}
	}
}

void readValue(struct Cache *cache, unsigned int address) {
	// TODO - check the cache for a cached byte at the specified address.
	// If found, indicate a hit and print the byte. If not found, indicate
	// a miss due to either an invalid line (cold miss) or a tag mismatch
	// (conflict miss).
	unsigned int s = getSet(address);
	unsigned int t = getTag(address);

	struct Line *line = &cache->lines[s];

	if (0 == line->valid){
		printf("no valid line found - miss!\n"); // cold miss
		return;
	}
	if (t != line->tag){
		printf("tags don't match - miss!\n"); // collision
		return;
	}
	printf("found set: %x - tag: %x - valid: %u - data: %.2x %.2x %.2x %.2x\n",
		s, line->tag, line->valid,
		line->data[0],
		line->data[1],
		line->data[2],
		line->data[3]);
	printf("hit!\n");
	return;

}
void writeValue(struct Cache *cache, unsigned int address, unsigned char *newData) {
	// Calculate set and tag for address
	unsigned int s = getSet(address);
	unsigned int t = getTag(address);
	// Get pointer to cache line in the specified set
	struct Line *line = &cache->lines[s];
	// Determine if we have a valid line in the cache that does not contain the
	// specified address - we detect this by checking for a tag mismatch
	if (line->valid && line->tag != t) {
		unsigned char *data = line->data;
		printf("evicting line - set: %x - tag: %x - valid: %u - data: %.2x %.2x %.2x %.2x\n",
		s, line->tag, line->valid, data[0], data[1], data[2], data[3]);
	}
// Copy new data to line (could use memcpy here instead)
	for (int i = 0; i < 4; ++i) {
		line->data[i] = newData[i];
	}
// Update line tag, mark line as valid
line->tag = t;
line->valid = 1;
printf("wrote set: %x - tag: %x - valid: %u - data: %.2x %.2x %.2x %.2x\n",
s, line->tag, line->valid, newData[0], newData[1], newData[2], newData[3]);
}

int main() {
struct Cache *cache = mallocCache(16);
// Loop until user enters 'q'
char c;
do {
	printf("Enter 'r' for read, 'w' for write, 'p' to print, 'q' to quit: ");
	scanf(" %c", &c);
if (c == 'r') {
	printf("Enter 32-bit unsigned hex address: ");
	unsigned int a;
	scanf(" %x", &a);
	readValue(cache, a);
} else if (c == 'w') {
	printf("Enter 32-bit unsigned hex address: ");
	unsigned int a;
	scanf(" %x", &a);
	printf("Enter 32-bit unsigned hex value: ");
	unsigned int v;
	scanf(" %x", &v);
	// Get byte pointer to v
	unsigned char *data = (unsigned char *)&v;
	writeValue(cache, a, data);
} else if (c == 'p') {
	printCache(cache);
}
} while (c != 'q');
	freeCache(cache);
}