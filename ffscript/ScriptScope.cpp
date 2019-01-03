/******************************************************************
* File:        ScriptScope.cpp
* Description: implement main part of ScriptScope class.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "ScriptScope.h"
#include "ObjectBlock.hpp"
#include "ExpUnitExecutor.h"
#include "Program.h"

namespace ffscript {
	ScriptScope::ScriptScope(ScriptCompiler* scriptCompiler) :
		_parent(nullptr),
		_scopeSize(0),
		_dataSize(0),
		_scriptCompiler(scriptCompiler),
		_constructorCount(0),
		_scopeBaseOffset(0)
	{
	}


	ScriptScope::~ScriptScope(){
	}
	
	Variable* ScriptScope::findVariable(const std::string& name) {
		auto it = _variableNameMap.find(name);
		if (it != _variableNameMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	Variable* ScriptScope::registVariable(const std::string& name) {
		auto it = _variableNameMap.insert(std::make_pair(name, nullptr));
		if (it.second == false) {
			return nullptr;
		}
		Variable variable(name);
		_varibles.push_back(variable);
		it.first->second = &(_varibles.back());
		it.first->second->setScope(this);
		return it.first->second;
	}

	Variable* ScriptScope::registVariable() {
		Variable variable("");
		variable.setScope(this);

		_varibles.push_back(variable);
		return &(_varibles.back());
	}

	void ScriptScope::removeVariable(Variable* v) {
		if (v->getName() == "") {
		}
		else {
			_variableNameMap.erase(v->getName());
		}

		for (auto it = _varibles.begin(); it != _varibles.end(); it++) {
			auto& variable = *it;
			if (v == &variable) {
				_varibles.erase(it);
				break;
			}
		}
	}

	Variable* ScriptScope::registTempVariable(CommandUnitBuilder* parentUnit, int offset) {
		auto it = _variableUnitMap.insert(std::make_pair(parentUnit, nullptr));
		if (it.second) {
			auto pVvariable = new Variable("ret of " + parentUnit->toString());
			pVvariable->setScope(this);
			it.first->second.reset(pVvariable);
		}		
		return it.first->second.get();
	}

	std::list<Variable>& ScriptScope::getVariables() {
		return _varibles;
	}

	Variable* ScriptScope::applyTemporaryVariableFor(CommandUnitBuilder* parentUnit, Variable* pVariable) {
		auto it = _variableUnitMap.insert(std::make_pair(parentUnit, pVariable));
		if (it.second) {
			pVariable->setName("ret of " + parentUnit->toString());
			pVariable->setScope(this);			
		}
		return it.first->second.get();
	}

	bool ScriptScope::deleteTempVariable(CommandUnitBuilder* parentUnit) {
		return _variableUnitMap.erase(parentUnit) != 0;
	}

	Variable* ScriptScope::findTempVariable(CommandUnitBuilder* parentUnit) {
		auto it = _variableUnitMap.find(parentUnit);
		if (it != _variableUnitMap.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	int ScriptScope::checkAutoOperatorForChildren(ScriptCompiler* scriptCompiler, const std::function<int(int)>& getOperator, const ScriptType& type, int beginOffset, std::list<OperatorBuidItemInfo>* operatorInfoList) {
		if (type.isRefType() || type.isSemiRefType()) {
			return -1;
		}

		int functionId = getOperator(type.iType());
		auto pStruct = scriptCompiler->getStruct(type.iType());
		if (pStruct) {
			MemberInfo memberInfo;
			bool res = pStruct->getMemberFirst(nullptr, &memberInfo);
			int childFunctionId;
			while (res)
			{
				if ( (childFunctionId = checkAutoOperatorForChildren(scriptCompiler, getOperator, memberInfo.type, beginOffset + memberInfo.offset, operatorInfoList)) >= 0) {
					OperatorBuidItemInfo buidItemInfo = { childFunctionId , beginOffset + memberInfo.offset };
					operatorInfoList->push_back(buidItemInfo);
				}
				res = pStruct->getMemberNext(nullptr, &memberInfo);
			}
		}
		else if(type.iType() & DATA_TYPE_ARRAY_MASK) {
			auto arrayInfo = (StaticArrayInfo*)scriptCompiler->getTypeInfo(type.iType());
			if (arrayInfo) {
				int elmCount = arrayInfo->elmCount;

				// add auto opreator to the list
				// first add for auto operator for the first element
				ScriptType elmType(arrayInfo->elmType, scriptCompiler->getType(arrayInfo->elmType));
				std::list<OperatorBuidItemInfo> elmBuildItem0;
				auto childOperatorId = checkAutoOperatorForChildren(scriptCompiler, getOperator, elmType, beginOffset, &elmBuildItem0);
				if (childOperatorId >= 0) {
					OperatorBuidItemInfo buidItemInfo = { childOperatorId , beginOffset };
					elmBuildItem0.push_back(buidItemInfo);
				}

				auto addRange = [](std::list<OperatorBuidItemInfo>& dest, std::list<OperatorBuidItemInfo>& src) {
					for (auto it = src.begin(); it != src.end(); it++) {
						dest.push_back(*it);
					}
				};

				auto updateOffset = [](std::list<OperatorBuidItemInfo>& src, int offset) {
					for (auto it = src.begin(); it != src.end(); it++) {
						it->itemOffset += offset;
					}
				};

				// copy operarors's build info of first element to the main list
				addRange(*operatorInfoList, elmBuildItem0);

				// copy operarors's build info of the left elements to the main list
				for (int i = 1; i < elmCount; i++) {
					updateOffset(elmBuildItem0, arrayInfo->elmSize);
					addRange(*operatorInfoList, elmBuildItem0);
				}
			}
		}

		return functionId;
	}

	Function* ScriptScope::generateDefaultAutoOperator(int operatorId, Variable* obj) {
		Function* operatorFunction = nullptr;
		auto scriptCompiler = getCompiler();

		operatorFunction = scriptCompiler->createFunctionFromId(operatorId);
		if (operatorFunction == nullptr) {
			scriptCompiler->setErrorText("constructor/destructor id = " + std::to_string(operatorId) + " cannot be found");
			return nullptr;
		}

		int makeRefId = scriptCompiler->getMakingRefFunction();
		if (makeRefId < 0) {
			scriptCompiler->setErrorText("cannot generate constructor/destructor due to missing definition of making reference operator");
			return nullptr;
		}

		auto makeRefFunc = scriptCompiler->createFunctionFromId(makeRefId);
		if (makeRefFunc == nullptr) {
			scriptCompiler->setErrorText("cannot generate constructor/destructor due to missing definition of making reference operator");
			return nullptr;
		}

		CXOperand* objUnit = new CXOperand(this, obj, obj->getDataType());
		makeRefFunc->pushParam(ExecutableUnitRef(objUnit));
		operatorFunction->pushParam(ExecutableUnitRef(makeRefFunc));

		return operatorFunction;
	}

	OperatorBuidInfo* ScriptScope::applyDefaultConstructor(const ScriptType& type, Function* constructor){
		auto getConstructorFunction = std::bind(&ScriptCompiler::getDefaultConstructor, _scriptCompiler, std::placeholders::_1);
		auto constructorBuildInfoBlockRef = generateConstructBuildInfo();
		OperatorBuidInfo* constructorBuildInfo = applyConstructBuildInfo(constructor);
		checkAutoOperatorForChildren(getCompiler(), getConstructorFunction, type, 0, &(constructorBuildInfo->buildItems));
		return constructorBuildInfo;
	}

	//void ScriptScope::checkVariableToRunConstructorNonRecursive(Variable* pVariable, Function* constructor) {
	//	auto destructorBuildInfoBlockRef = std::make_shared<ObjectBlock<OperatorBuidInfo>>();
	//	ObjectBlock<OperatorBuidInfo>* destructorBuildInfoBlock = destructorBuildInfoBlockRef.get();
	//	OperatorBuidInfo* destructorBuildInfo = (OperatorBuidInfo*)destructorBuildInfoBlock->getDataRef();

	//	int destructorFunctionId = _scriptCompiler->getDestructor(pVariable->getDataType().iType());

	//	auto constructorBuildInfoBlockRef = std::make_shared<ObjectBlock<OperatorBuidInfo>>();
	//	ObjectBlock<OperatorBuidInfo>* constructorBuildInfoBlock = constructorBuildInfoBlockRef.get();
	//	OperatorBuidInfo* constructorBuildInfo = (OperatorBuidInfo*)constructorBuildInfoBlock->getDataRef();
	//	constructorBuildInfo->operatorIndex = _constructorCount;

	//	constructor->setUserData(constructorBuildInfoBlockRef);

	//	if (destructorFunctionId >= 0) {
	//		auto destructor = generateDefaultAutoOperator(destructorFunctionId, pVariable);
	//		if (destructor == nullptr) {
	//			// it should throw exception here
	//			return;
	//		}

	//		destructorBuildInfo->operatorIndex = _constructorCount;
	//		destructor->setMask(destructor->getMask() | UMASK_DESTRUCTOR);
	//		destructor->setUserData(destructorBuildInfoBlockRef);

	//		//store last unit to the destuctor list
	//		_destructors.push_front(CommandUnitRef(destructor));
	//	}

	//	_constructorCount++;
	//}

	CommandUnitBuilder* ScriptScope::checkVariableToRunConstructor(CXOperand* xOperand) {
		auto pVariable = xOperand->getVariable();
		bool hasDestructors = checkVariableToRunDestructor(xOperand);

		auto getConstructorFunction = std::bind(&ScriptCompiler::getDefaultConstructor, _scriptCompiler, std::placeholders::_1);
		auto constructorBuildInfoBlockRef = generateConstructBuildInfo();
		auto constructorBuildInfo = (OperatorBuidInfo*)constructorBuildInfoBlockRef->getDataRef();
		int constructorFunctionId = checkAutoOperatorForChildren(getCompiler(), getConstructorFunction, pVariable->getDataType(), 0, &(constructorBuildInfo->buildItems));

		// check if current type has no constructor but member types have...
		if (constructorFunctionId < 0 && (hasDestructors || constructorBuildInfo->buildItems.size())) {
			// then use auto operator, this default operator act as container to contain the member constructor
			constructorFunctionId = _scriptCompiler->findFunction(DEFAULT_DUMMY_OPERATOR, "ref void");
		}

		if (constructorFunctionId >= 0 || hasDestructors) {
			generateNextConstructId();
		}

		if (constructorFunctionId >= 0) {

			auto constructorUnit = generateDefaultAutoOperator(constructorFunctionId, pVariable);
			if (constructorUnit == nullptr) {
				return nullptr;
			}
			// keep origin source char in new expression unit
			constructorUnit->setSourceCharIndex(xOperand->getSourceCharIndex());
			constructorUnit->setUserData(constructorBuildInfoBlockRef);
			constructorUnit->setMask(constructorUnit->getMask() | UMASK_DEFAULT_CTOR);

			// push constructor to scope
			putCommandUnit(constructorUnit);

			return constructorUnit;
		}

		return nullptr;
	}

	void ScriptScope::applyConstructorDestructor(const ExecutableUnitRef& variableUnit, Function* constructor) {
		applyDefaultConstructor(variableUnit->getReturnType(), constructor);
		applyDestructor(variableUnit);
		generateNextConstructId();
	}

	bool ScriptScope::applyDestructor(const ExecutableUnitRef& variableUnit) {
		auto xOperand = dynamic_cast<CXOperand*>(variableUnit.get());
		if (!xOperand) {
			throw exception("expression unit is not a variable");
		}
		auto pVariable = xOperand->getVariable();
		if (!pVariable) {
			throw exception("null variable");
		}

		return checkVariableToRunDestructor(xOperand);
	}

	bool ScriptScope::checkVariableToRunDestructor(CXOperand* xOperand) {
		auto pVariable = xOperand->getVariable();

		auto getDestructorFunction = std::bind(&ScriptCompiler::getDestructor, _scriptCompiler, std::placeholders::_1);

		auto destructorBuildInfoBlockRef = generateConstructBuildInfo();
		auto destructorBuildInfo = (OperatorBuidInfo*)destructorBuildInfoBlockRef->getDataRef();

		int destructorFunctionId = checkAutoOperatorForChildren(getCompiler(), getDestructorFunction, pVariable->getDataType(), 0, &(destructorBuildInfo->buildItems));
		// check if current type has no destructor but member types have...
		if (destructorFunctionId < 0 && destructorBuildInfo->buildItems.size() > 0) {
			// then use auto operator, this default operator act as container to contain the member destructor
			destructorFunctionId = _scriptCompiler->findFunction(DEFAULT_DUMMY_OPERATOR, "ref void");
		}

		if (destructorFunctionId >= 0) {
			auto destructor = generateDefaultAutoOperator(destructorFunctionId, pVariable);
			if (destructor == nullptr) {
				// it should throw exception here
				return false;
			}
			// keep origin source char in new expression unit
			destructor->setSourceCharIndex(xOperand->getSourceCharIndex());
			destructor->setMask(destructor->getMask() | UMASK_DESTRUCTOR);
			destructor->setUserData(destructorBuildInfoBlockRef);

			//store last unit to the destuctor list
			_destructors.push_front(CommandUnitRef(destructor));
			return true;
		}
		return false;
	}

	std::shared_ptr<ObjectBlock<OperatorBuidInfo>> ScriptScope::generateConstructBuildInfo() const {
		auto constructBuildInfoBlockRef = std::make_shared<ObjectBlock<OperatorBuidInfo>>();
		ObjectBlock<OperatorBuidInfo>* constructBuildInfoBlock = constructBuildInfoBlockRef.get();
		OperatorBuidInfo* constructBuildInfo = (OperatorBuidInfo*)constructBuildInfoBlock->getDataRef();
		constructBuildInfo->operatorIndex = _constructorCount;

		return constructBuildInfoBlockRef;
	}

	OperatorBuidInfo* ScriptScope::applyConstructBuildInfo(Function* constructFactor) {
		auto constructBuildInfoBlockRef = generateConstructBuildInfo();
		constructFactor->setUserData(constructBuildInfoBlockRef);

		return (OperatorBuidInfo*)constructBuildInfoBlockRef->getDataRef();
	}

	int ScriptScope::getConstructorCommandCount() const {
		return _constructorCount;
	}

	void ScriptScope::generateNextConstructId() {
		_constructorCount++;
	}

	ComandRefList* ScriptScope::getDestructorList() {
		return &_destructors;
	}

	ScopeAutoRunList* ScriptScope::getConstructorList() {
		return &_constructors;
	}

	MemberVariable* ScriptScope::registMemberVariable(Variable* parent, const std::string& name) {
		MemberVariable variable(parent, name);

		auto it = _variableNameMap.insert(std::make_pair(name, nullptr));
		if (it.second == false) {
			return nullptr;
		}

		_memberVaribles.push_back(variable);
		it.first->second = &(_memberVaribles.back());
		it.first->second->setScope(this);
		return &_memberVaribles.back();
	}

	void ScriptScope::updateVariableOffset() {
		int dataSize;

		_scopeSize = SCOPE_INFO_SIZE;
		for (auto it = _varibles.begin(); it != _varibles.end(); ++it) {
			it->setOffset(_scopeSize + _scopeBaseOffset);
			dataSize = it->getSize();
			_scopeSize += dataSize;
		}

		_dataSize = _scopeSize;
	}

	int ScriptScope::getScopeSize() const {
		return _scopeSize;
	}

	int ScriptScope::getDataSize() const {
		return _dataSize;
	}

	int ScriptScope::getBaseOffset() const {
		return _scopeBaseOffset;
	}

	void ScriptScope::setBaseOffset(int offset) {
		_scopeBaseOffset = offset;
	}

	void ScriptScope::allocate(int size) {
		_scopeSize += size;
	}

	DFunction2Ref ScriptScope::getProcessingFunc(const std::string& keyword) {
		auto it = _keywordProcessingMap.find(keyword);
		if (it != _keywordProcessingMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	bool ScriptScope::setProcessingFunc(const std::string& keyword, DFunction2Ref function) {
		auto it = _keywordProcessingMap.insert(std::make_pair(keyword, function));
		return (it.second);
	}

	ScriptScope* ScriptScope::getParent() const {
		return _parent;
	}

	ScriptScope* ScriptScope::getRoot() const {
		const ScriptScope* root = this;
		ScriptScope* parent;
		while (parent = root->getParent())
		{
			root = parent;
		}

		return (ScriptScope*)root;
	}

	void ScriptScope::setParent(ScriptScope* parent) {
		_parent = parent;
	}
	void ScriptScope::addChild(ScriptScope* child) {
		_children.push_back(ScriptScopeRef(child));
	}
	const ScopeRefList& ScriptScope::getChildren() const {
		return _children;
	}
	/*int ScriptScope::allocateMem(int size) {
		if (_currentOffset + size > _maxSize) {
			return -1;
		}
		_currentOffset += size;
		return _currentOffset;
	}*/

	ScriptCompiler* ScriptScope::getCompiler() const {
		return _scriptCompiler;
	}

	ScriptCompilerRef ScriptScope::getInternalCompiler() {
		if (_internalCompiler == nullptr) {
			if (_parent) {
				_internalCompiler = _parent->getInternalCompiler();
			}
			else
			{
				_internalCompiler = (ScriptCompilerRef)(new ScriptCompiler());
			}
		}

		return _internalCompiler;
	}

	void ScriptScope::clear() {
		_variableNameMap.clear();
		_varibles.clear();
		_memberVaribles.clear();
		_destructors.clear();		
		_children.clear();
		_parent = nullptr;
		_scopeSize = 0;
		_dataSize = 0;
		_commandBuilder.clear();
	}
}