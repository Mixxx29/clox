
#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[])
{
	Chunk chunk;
	initChunk(&chunk);

	int constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 1);
	writeChunk(&chunk, constant, 1);

	writeChunk(&chunk, OP_RETURN, 1);

	int constant2 = addConstant(&chunk, 2.56);
	writeChunk(&chunk, OP_CONSTANT, 2);
	writeChunk(&chunk, constant2, 2);

	writeChunk(&chunk, OP_RETURN, 2);
	writeChunk(&chunk, OP_RETURN, 2);
	writeChunk(&chunk, OP_RETURN, 2);

	disassembleChunk(&chunk, "test chunk");

	freeChunk(&chunk);
	return 0;
}
