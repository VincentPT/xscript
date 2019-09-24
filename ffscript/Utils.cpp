/******************************************************************
* File:        Utils.cpp
* Description: implement some common functions of the library.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "Utils.h"
#include <string>
#include <fstream>
#include <codecvt>

namespace ffscript {
	std::string convertToAscii(const wchar_t* ws, size_t n) {
		return std::string(ws, ws + n);
	}

	std::string convertToAscii(const wchar_t* ws) {
		return std::string(ws, ws + wcslen(ws));
	}

	std::wstring convertToWstring(const std::string& s) {
		return std::wstring (s.begin(), s.end());
	}

	SimpleVariantArray* createParamArray(int paramSize) {
		int sizeNeed = sizeof(SimpleVariantArray) + (paramSize - 1) * sizeof(SimpleVariantArray::elems[0]);
		SimpleVariantArray* variantArray = (SimpleVariantArray*)malloc(sizeNeed);
		memset(variantArray,0 , sizeNeed);
		return variantArray;
	}

	void freeParamArray(SimpleVariantArray** ppvarr) {
		if (ppvarr && *ppvarr) {
			SimpleVariant* arrVariant = &((*ppvarr)->elems[0]);
			int size = (*ppvarr)->size;

			for (int i = 0; i < size; i++) {
				if (arrVariant[i].pData) {
					free(arrVariant[i].pData);
				}
			}

			free(*ppvarr);
			*ppvarr = nullptr;
		}
	}

	RawString allocRawString(int size) {
		RawString rws;
		rws.size = size;
		// allocate memory to contain characters and also null character
		rws.elms = (RawChar*)malloc((size + 1) * sizeof(RawChar));

		return rws;
	}

	template <typename T>
	void addParam(SimpleVariantArray* pArray, const T& val, int type) {
		SimpleVariant& aVariant = pArray->elems[pArray->size];
		aVariant.scriptType = type;
		aVariant.size = sizeof(val);
		aVariant.pData = malloc(aVariant.size);
		*((T*)aVariant.pData) = val;

		pArray->size++;
	}

	void addParam(SimpleVariantArray* pArray, int val, TypeManager* typeManager) {
		addParam(pArray, val, typeManager->getBasicTypes().TYPE_INT);
	}

	void addParam(SimpleVariantArray* pArray, float val, TypeManager* typeManager) {
		addParam(pArray, val, typeManager->getBasicTypes().TYPE_FLOAT);
	}

	void addParam(SimpleVariantArray* pArray, const double& val, TypeManager* typeManager) {
		addParam(pArray, val, typeManager->getBasicTypes().TYPE_DOUBLE);
	}

	void addParam(SimpleVariantArray* pArray, const __int64 & val, TypeManager* typeManager) {
		addParam(pArray, val, typeManager->getBasicTypes().TYPE_LONG);
	}

	void addParam(SimpleVariantArray* pArray, bool val, TypeManager* typeManager) {
		addParam(pArray, val, typeManager->getBasicTypes().TYPE_BOOL);
	}

	void addParam(SimpleVariantArray* pArray, const char* val, TypeManager* typeManager) {
		addParam(pArray, val, typeManager->getBasicTypes().TYPE_STRING);
	}

	void addParam(SimpleVariantArray* pArray, const wchar_t* val, TypeManager* typeManager) {
		addParam(pArray, val, typeManager->getBasicTypes().TYPE_WSTRING);
	}


	std::wstring readCodeFromStream(std::wistream& wss) {
		std::wstring wstr;
		std::wstring line;
		while (std::getline(wss, line))
		{
			wstr += line;
			wstr.append(1, L'\n');
		}

		if (wstr.size()) {
			wstr.erase(wstr.size() - 1);
		}

		return wstr;
	}

	std::wstring readCodeFromUtf8File(const char* filename) {
		std::wifstream wif(filename);
		std::locale l1;
		wif.imbue(std::locale(l1, new std::codecvt_utf8<wchar_t>));

		return readCodeFromStream(wif);
	}

	std::string buildFunctionSign(const std::string& name, const std::vector<ScriptType>& paramTypes) {
		std::string funtionSignature;

		funtionSignature.append(name);
		funtionSignature.append("(");

		auto pit = paramTypes.begin();
		if (pit != paramTypes.end()) {
			while (true) {
				funtionSignature.append(pit->sType());
				pit++;
				if (pit == paramTypes.end())
					break;
				funtionSignature.append(",");
			}
		}

		funtionSignature.append(")");
		return funtionSignature;
	}
}