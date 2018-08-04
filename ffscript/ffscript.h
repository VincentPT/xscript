#pragma once
#include <list>
#include <memory>

#ifdef FFSCRIPT_DYNAMIC
#ifdef FFSCIPT_EXPORTS
#define FFSCRIPT_API __declspec(dllexport)
#else
#define FFSCRIPT_API __declspec(dllimport)
#endif
#else
#define FFSCRIPT_API
#endif

typedef wchar_t WCHAR;
typedef unsigned char byte;

#define MAKE_STR(s) #s
#define X_MAKE_STR(s) str(s)
#define WIDEN(quote) WIDEN2(quote)
#define WIDEN2(quote) L##quote

#define DATA_TYPE_POINTER_MASK 0x80000000
#define DATA_TYPE_ARRAY_MASK 0x40000000
#define DATA_TYPE_FUNCTION_MASK 0x20000000
#define DATA_TYPE_REF_MASK 0x10000000
#define DATA_TYPE_UNKNOWN 0x0FFFFFFF
#define DATA_TYPE_ORIGIN_MASK DATA_TYPE_UNKNOWN

#define DATA_TYPE_UNKNOWN_INSTRING "TBD"
#define POINTER_SIGN "ref"
#define ARRAY_SIGN "array"
#define SEMIREF_SIGN "&"
#define FUNCTOR_SIGN "function"
#define SYSTEM_ARRAY_FUNCTION "_makeVariantArray070517"
#define SYSTEM_ARRAY_STRUCT "_simpleVariantArray070517"
#define SYSTEM_NULL_TYPE "_null_t"
#define SYSTEM_LAMBDA_FUNCTION "_makeLambda070517"
#define SYSTEM_FUNCTION_CONSTRUCTOR "_constructor_RuntimeFunctionInfo"
#define SYSTEM_FUNCTION_DESTRUCTOR "_destructor_RuntimeFunctionInfo"
#define SYSTEM_FUNCTION_COPY_CONSTRUCTOR "_copyConstructor_RuntimeFunctionInfo"

#define MAKING_REF_FUNC "ref"
#define MAKING_SEMI_REF_FUNC "semiref"
#define SUBSCRIPT_OPERATOR "operator[]"
#define FUNCTION_OPERATOR "operator()"
#define DEREF_OPERATOR "operator*"
#define DEFAULT_COPY_OPERATOR "<--"
#define DEFAULT_DUMMY_OPERATOR "_empty__default_constructor_desctructor"

#define SCOPE_INFO_OFFSET			0
#define SCOPE_INFO_SIZE				(SCOPE_INFO_OFFSET + sizeof(void*))
#define BEGIN_FUNCTION_OFFSET_DATA	SCOPE_INFO_SIZE

#define IS_UNKNOWN_TYPE(type) (((type) & DATA_TYPE_UNKNOWN) == DATA_TYPE_UNKNOWN)

#define MAX_FUNCTION_LENGTH 254
#define MAX_DATA_SIZE (64*1024)

#define USE_DIRECT_COPY_FOR_RETURN 1
#define USE_FUNCTION_TREE 1

#if USE_FUNCTION_TREE
#undef USE_DIRECT_COPY_FOR_RETURN
#define USE_DIRECT_COPY_FOR_RETURN 1
#endif

#define SCRIPT_FUNCTION_RETURN_STORAGE_OFFSET 0
#define OPTIMIZE_CTOR_CALL 0

#pragma region ffscript types

#pragma endregion

class DFunction;
class DFunction2;

#include "function/CdeclFunction3.hpp"

namespace ffscript {
	class InstructionCommand;
	class MemoryBlock;
	class OperatorObject;
	class Variable;
	
	typedef std::shared_ptr<DFunction2> DFunction2Ref;
	typedef std::shared_ptr<DFunction> DFunctionRef;
	typedef InstructionCommand** CommandPointer;
	typedef std::list<InstructionCommand*> CommandList;
	typedef std::list<std::shared_ptr<InstructionCommand>> ScopeAutoRunList;
	typedef std::pair< CommandPointer, CommandPointer > CodeSegmentEntry;

	template <class Ret, class... Types>
	using FunctionT = typename FT::CdelFunction3<Ret, Types...>;

	typedef void* THREAD_HANDLE;
	
	struct OperatorBuidItemInfo {
		int functionId;
		int itemOffset;
	};

	struct OperatorBuidInfo
	{
		std::list<OperatorBuidItemInfo> buildItems;
		int operatorIndex;
	};


#pragma pack(push)
#pragma pack(1)
	template <class T>
	struct SimpleArray
	{
		int size;
		T* elms;
	};
#define DYNAMIC_ARRAY 0x0001
	struct DynamicArray
	{
		short dim;
		unsigned short flags;
		int size;
		int elmType;
		int elmSize;
		void* data;
	};

	enum class VariantType : int
	{
		Unkown = 0,
		Bool,
		Integer,
		Long,
		Float,
		Double,
		String,
		WString,
		Array,
		BoolRef,
		IntegerRef,
		LongRef,
		FloatRef,
		DoubleRef,
		StringRef,
		WStringRef,
		ArrayRef,
	};

	struct SimpleVariant {
		int scriptType;
		char* typeName;
		int size;
		void* pData;
	};

	struct SimpleVariantArray
	{
		int size;
		SimpleVariant elems[1];
	};

	enum class RuntimeFunctionType : unsigned char
	{
		Null = 0,
		NativeFunction,
		ScriptFunction,
	};

	struct AnoynymousDataInfo {
		unsigned int dataSize;
		unsigned int targetOffset;
		void* data;
	};

	struct RuntimeFunctionInfo
	{
		void* address;
		AnoynymousDataInfo anoynymousInfo;
		union
		{
			RuntimeFunctionType type;
			unsigned int rawInfo;
		}info;
	};

	struct StaticArrayInfo {
		char refLevel; // element ref count
		unsigned char dim; // number of dimension
		short reserved2;
		int elmType;
		int elmSize;
		int elmCount;
	};

	typedef wchar_t RawChar;
	typedef SimpleArray<RawChar> RawString;

#pragma pack(pop)

#define INDEPENDENT_CONSTRUCTOR (-1)
typedef unsigned int MaskType;
#define UMASK_NONE 0
#define UMASK_DEFAULT_CTOR 1
#define UMASK_DESTRUCTOR 2
#define UMASK_CONSTRUCTOR 4
#define UMASK_DESTRUCTORFORRETURNDATA 8
#define UMASK_CASTINGUNITNOTINEXPRESSION 16
#define UMASK_EXCLUDEFROMDESTRUCTOR 32
#define UMASK_DECLAREINEXPRESSION 64
#define UMASK_CONSTRUCT_FACTOR 128
}