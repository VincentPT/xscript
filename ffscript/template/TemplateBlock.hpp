#pragma once
template <int size>
#pragma pack(push)
#pragma pack(1)
struct MemoryBlock
{
	char data[size];
};
#pragma pack(pop)