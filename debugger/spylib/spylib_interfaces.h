#pragma once

// user enum's declaration
enum class UserCommandId : unsigned short {
	READ_UNIT_LIST = 0,
	READ_EXPRESSION_NODE,

	// this must be placed at the end
	PRE_DEFINED_COMMAND_COUNT,
};

enum class ReturnCode : int {
	Success = 0,
	Fail
};

// user structure's declaration
#pragma pack(push, 1)
struct StringBufferArray {
	unsigned int size;
	int elmCount;
	char data[1];// string buffers seperated by null character
};

struct RawBuffer {
	unsigned int size;
	char data[1];
};

#pragma pack(pop)


#define EXP_UNIT_KEY_NAME "name"
#define EXP_UNIT_KEY_CHILD "params"
#define EXP_UNIT_KEY_VALUE "value"