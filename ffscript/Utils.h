/******************************************************************
* File:        Utils.h
* Description: declare and define some common functions of the library.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "Variable.h"
#include "GlobalScope.h"
#include "ffscript.h"
#include "TypeManager.h"
#include <string>
#include <istream>

namespace ffscript {

	template <typename T>
	void setVariableValue(Variable& varible, const T& value) {
		GlobalScope* scope = dynamic_cast<GlobalScope*>(varible.getScope());
		if (scope) {
			void* pValue = scope->getGlobalAddress(varible.getOffset());
			*((T*)pValue) = value;
		}
	}

	template <typename T>
	T* getVaribleRef(Variable& varible) {
		GlobalScope* scope = dynamic_cast<GlobalScope*>(varible.getScope());
		if (scope) {
			return (T*)scope->getGlobalAddress(varible.getOffset());

		}
		return nullptr;
	}

	template <typename T>
	inline bool checkBeginToken(T c) {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
	}

	template <typename T>
	const T* lastCharInToken(const T* text, const T* end) {
		const T* c = text;

		while (c < end && (checkBeginToken(*c) || (*c >= '0' && *c <= '9') || *c == '.')) {
			c++;
		}

		return c;
	}

	template <typename T>
	const T* lastCharInToken2(const T* text, const T* end) {
		const T* c = text;

		while (c < end && (checkBeginToken(*c) || (*c >= '0' && *c <= '9'))) {
			c++;
		}

		return c;
	}

	template <class T>
	SimpleArray<T> newSimpleArray(int size) {
		SimpleArray<T> arr;
		arr.size = size;
		arr.elms = new T[size];

		return arr;
	}

	template <class T>
	SimpleArray<T> allocSimpleArray(int size) {
		SimpleArray<T> arr;
		arr.size = size;
		arr.elms = (T*)malloc(size * sizeof(T));

		return arr;
	}

	template <typename T>
	void deleteSimpleArray(SimpleArray<T>& arr) {
		if (arr.elms) {
			delete[] arr.elms;
			arr.elms = nullptr;
		}
		arr.size = 0;
	}

	template <typename T>
	void freeSimpleArray(SimpleArray<T>& arr) {
		if (arr.elms) {
			free(arr.elms);
			arr.elms = nullptr;
		}
		arr.size = 0;
	}

	RawString allocRawString(int size);

#define freeRawString freeSimpleArray<RawChar>

	std::string convertToAscii(const wchar_t* ws, size_t n);
	std::string convertToAscii(const wchar_t* ws);
	std::wstring convertToWstring(const std::string& s);
	std::wstring readCodeFromStream(std::wistream&);
	std::wstring readCodeFromUtf8File(const char*);

	template< typename T >
	std::string int_to_hex(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}

	SimpleVariantArray* createParamArray(int paramSize);
	void freeParamArray(SimpleVariantArray** ppvarr);

	void addParam(SimpleVariantArray* pArray, int val, TypeManager* typeManager);
	void addParam(SimpleVariantArray* pArray, float val, TypeManager* typeManager);
	void addParam(SimpleVariantArray* pArray, const double& val, TypeManager* typeManager);
	void addParam(SimpleVariantArray* pArray, const __int64 & val, TypeManager* typeManager);
	void addParam(SimpleVariantArray* pArray, bool val, TypeManager* typeManager);
	void addParam(SimpleVariantArray* pArray, const char* val, TypeManager* typeManager);
	void addParam(SimpleVariantArray* pArray, const wchar_t* val, TypeManager* typeManager);

#pragma region list all paths for a tree

	template <class T1, class T2>
	T2 defaultCast(const T1& val) {
		return (T2)val;
	}

	template <class T, class NodeCollection, typename T2>
	void listPaths(const std::vector<std::shared_ptr<NodeCollection>>& nodeCollections, std::list<std::vector<T2>>& paramsPaths,
		T2 (*convertT)(const T&) = defaultCast<T,T2>) {
		//count path
		int pathCount = 1;
		for (auto it = nodeCollections.begin(); it != nodeCollections.end(); ++it) {
			pathCount *= (int)(it->get()->size());
		}

		////initialize paths with empty
		std::vector<T2> emptyPath(nodeCollections.size());
		for (int i = 0; i < pathCount; i++) {
			paramsPaths.push_back(emptyPath);
		}

		int maxLoop = pathCount;
		int loopCount;
		for (size_t i = 0; i < nodeCollections.size(); i++) {
			auto& candidateCollectionRef = nodeCollections[i];
			maxLoop /= (int)candidateCollectionRef->size();

			loopCount = 0;
			auto pit = candidateCollectionRef->begin();
			for (auto it = paramsPaths.begin(); it != paramsPaths.end(); it++) {
				if (loopCount == maxLoop) {
					++pit;
					loopCount = 0;
				}
				if (pit == candidateCollectionRef->end()) {
					pit = candidateCollectionRef->begin();
				}
				loopCount++;
				it->at(i) = convertT(*pit);
			}
		}
	}

	std::string buildFunctionSign(const std::string& name, const std::vector<ScriptType>& paramTypes);

#pragma endregion
}