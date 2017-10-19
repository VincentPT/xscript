#pragma once
#include "StructClass.h"
#include "MemoryBlock.h"
#include "BasicType.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace ffscript {

	class TypeManager
	{
		struct TypeInfo {
			const std::string* name;
			int size;
		};

		ffscript::BasicTypes _basicTypes;

		std::vector<TypeInfo> _typesInString;
		std::map<const std::string, int> _typeStringIntMap;
		std::map<int, StructClassRef> _structMap;
		std::map<int, MemoryBlockRef> _typeInfoMap;

		struct LibraryMarkInfo {
			int typeIdx;
		};
		typedef std::shared_ptr<LibraryMarkInfo> LibraryMarkInfoRef;
		LibraryMarkInfoRef _systemTypeMarkEnd;

	public:
		TypeManager();
		~TypeManager();
		bool registTypeInfo(int type, MemoryBlockRef typeInfo);
		void* getTypeInfo(int type);
		int getTypeSizeInStack(int) const;
		int registType(const std::string&, int mask = 0);
		int getTypeSize(int) const;
		void setTypeSize(int typeId, int size);
		std::string getType(int) const;
		int getType(const std::string&) const;

		int registStruct(StructClass* pStruct);
		const StructClass* getStruct(int type);

		void registerBasicTypes(ScriptCompiler* scriptCompiler);
		void registerBasicTypeCastFunctions(ScriptCompiler* scriptCompiler, FunctionRegisterHelper& functionLib);
		void registerConstants(ScriptCompiler* scriptCompiler);
		const BasicTypes& getBasicTypes() const;
		BasicTypes& getBasicTypes();

		void beginUserTypes();
		void clearUserTypes();
	};

	typedef std::shared_ptr<TypeManager> TypeManagerRef;
}
