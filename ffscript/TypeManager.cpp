#include "stdafx.h"
#include "TypeManager.h"
#include "Internal.h"
#include "ffscript.h"
#include "ScriptCompiler.h"
#include "ffutil.hpp"

namespace ffscript {
	TypeManager::TypeManager() {}

	TypeManager::~TypeManager()
	{
	}

	int TypeManager::registType(const std::string& type, int mask) {
		if (checkPointerType(type)) {
			return DATA_TYPE_UNKNOWN;
		}

		int typeInInt = (int)_typesInString.size();
		typeInInt |= mask;

		auto it = _typeStringIntMap.insert(std::pair<const std::string&, int>(type, typeInInt));

		/* type is already exist */
		if (it.second == false) {
			return DATA_TYPE_UNKNOWN;
		}

		//add registered type
		TypeInfo typeInfo;
		typeInfo.name = &(it.first->first);
		typeInfo.size = 0;
		_typesInString.push_back(typeInfo);

		return typeInInt;
	}

	std::string TypeManager::getType(int typeId) const {
		std::string typeInString;
		if (typeId & DATA_TYPE_POINTER_MASK) {
			typeInString.append(POINTER_SIGN " ");
		}

		typeId &= DATA_TYPE_UNKNOWN;
		typeInString.append(*(_typesInString.at(typeId).name));

		return typeInString;
	}

	bool TypeManager::registTypeInfo(int type, MemoryBlockRef typeInfoRef) {
		auto it = _typeInfoMap.insert(std::make_pair(type, typeInfoRef));
		return it.second;
	}

	void* TypeManager::getTypeInfo(int type) {
		auto it = _typeInfoMap.find(type);
		if (it != _typeInfoMap.end()) {
			return it->second->getDataRef();
		}

		return nullptr;
	}

	int TypeManager::getTypeSizeInStack(int typeId) const {
		if ((typeId & DATA_TYPE_POINTER_MASK) || (typeId & DATA_TYPE_REF_MASK)) {
			return sizeof(void*);
		}
		typeId &= DATA_TYPE_UNKNOWN;
		int realSize = _typesInString.at(typeId).size;
		return ((realSize - 1)/sizeof(void*) + 1) * sizeof(void*);
	}

	int TypeManager::getTypeSize(int typeId) const {
		if ( (typeId & DATA_TYPE_POINTER_MASK) || (typeId & DATA_TYPE_REF_MASK)) {
			return sizeof(void*);
		}
		typeId &= DATA_TYPE_UNKNOWN;
		return _typesInString.at(typeId).size;
	}

	void TypeManager::setTypeSize(int typeId, int size) {
		typeId &= DATA_TYPE_UNKNOWN;
		_typesInString.at(typeId).size = size;
	}

	int TypeManager::getType(const std::string& type) const {
		auto it = _typeStringIntMap.find(type);
		if (it == _typeStringIntMap.end()) {
			return DATA_TYPE_UNKNOWN;
		}

		return it->second;
	}

	int TypeManager::registStruct(StructClass* pStruct) {
		int type = registType(pStruct->getName());

		if (!IS_UNKNOWN_TYPE(type)) {
			setTypeSize(type, pStruct->getSize());
			_structMap.insert(std::make_pair(type, StructClassRef(pStruct)));
		}

		return type;
	}

	const StructClass* TypeManager::getStruct(int type) {
		auto it = _structMap.find(type);
		if (it != _structMap.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	void TypeManager::registerBasicTypes(ScriptCompiler* scriptCompiler) {
		_basicTypes.registerBasicTypes(scriptCompiler);
	}

	void TypeManager::registerBasicTypeCastFunctions(ScriptCompiler* scriptCompiler, FunctionRegisterHelper& functionLib) {
		_basicTypes.registerBasicTypeCastFunctions(scriptCompiler, functionLib);
	}

	void TypeManager::registerConstants(ScriptCompiler* scriptCompiler) {
		_basicTypes.registerConstants(scriptCompiler);
	}

	const BasicTypes& TypeManager::getBasicTypes() const {
		return _basicTypes;
	}

	BasicTypes& TypeManager::getBasicTypes() {
		return _basicTypes;
	}

	void TypeManager::beginUserTypes() {
		_systemTypeMarkEnd = (LibraryMarkInfoRef)(new LibraryMarkInfo);
		_systemTypeMarkEnd->typeIdx = (int)_typesInString.size();		
	}

	void TypeManager::clearUserTypes() {
		if (_systemTypeMarkEnd) {

			for (int i = _systemTypeMarkEnd->typeIdx; i < (int)_typesInString.size(); i++) {
				_structMap.erase(i);
				_typeInfoMap.erase(i);
				
				for (auto it = _typeStringIntMap.begin(); it != _typeStringIntMap.end(); it++) {
					if (it->second == i) {
						_typeStringIntMap.erase(it);
						break;
					}
				}
			}

			_typesInString.resize(_systemTypeMarkEnd->typeIdx);
			_systemTypeMarkEnd.reset();
		}
	}
}