#include "stdafx.h"
#include "Utils.h"
#include <string>

namespace ffscript {
	std::string convertToAscii(const wchar_t* ws, size_t n) {
		char stemp[MAX_FUNCTION_LENGTH + 1];
		char* ctemp = stemp;
		char* cmax = ctemp + n;
		while (ctemp < cmax && (*ctemp++ = (char)*ws++));
		*ctemp = 0;

		return std::string(stemp);
	}

	std::string convertToAscii(const wchar_t* ws) {
		char stemp[MAX_FUNCTION_LENGTH + 1];
		char* ctemp = stemp;
		while (*ctemp++ = (char)*ws++);
		*ctemp = 0;

		return std::string(stemp);
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
}