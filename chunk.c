#include <stdlib.h>

#include "chunk.h"

void initChunk(Chunk* chunk)
{
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;

	chunk->lines.count = 0;
	chunk->lines.capacity = 0;
	chunk->lines.lines = NULL;

	initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk)
{
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines.lines, chunk->lines.capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

static void addLine(Chunk* chunk, int line)
{
	if (chunk->lines.count > 0 && chunk->lines.lines[chunk->lines.count - 1] == line)
	{
		chunk->lines.lines[chunk->lines.count - 2] += 1;
		return;
	}

	if (chunk->lines.capacity < chunk->lines.count + 2)
	{
		int oldCapacity = chunk->lines.capacity;
		chunk->lines.capacity = GROW_CAPACITY(oldCapacity);
		chunk->lines.lines = GROW_ARRAY(int, chunk->lines.lines, oldCapacity, chunk->lines.capacity);
	}

	chunk->lines.lines[chunk->lines.count++] = 1;
	chunk->lines.lines[chunk->lines.count++] = line;
}

void writeChunk(Chunk* chunk, uint8_t byte, int line)
{
	if (chunk->capacity < chunk->count + 1)
	{
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}

	chunk->code[chunk->count] = byte;
	chunk->count++;

	addLine(chunk, line);
}

int addConstant(Chunk* chunk, Value value)
{
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int offset)
{
	for (int i = 0; i < chunk->lines.count; i += 2)
	{
		if (offset < chunk->lines.lines[i])
			return chunk->lines.lines[i + 1];

		offset -= chunk->lines.lines[i];
	}

	return 0;
}
