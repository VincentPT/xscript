#include "stdafx.h"
#include "TemplateTypeManager.h"
#include "ffscript.h"

namespace ffscript {
	TemplateTypeManager* TemplateTypeManager::s_pInstance = nullptr;

	TemplateTypeManager::TemplateTypeManager()
	{
	}

	TemplateTypeManager::~TemplateTypeManager()
	{
	}

	TemplateTypeManager* TemplateTypeManager::getInstance() {
		return s_pInstance;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// brief : parse a text to known it is a template type or not
	// params: 
	//          text: any text want to parse
	//          type: in case success, type is a registered template type
	//                in case failure, type is -1
	//          error: in case the type is seem to be a template type but it is missing something,
	//                 this parameter contains error explanation
	//                 in case this is a normal type or success, this parameter is null
	// return: return the pointer of next character behind the template type
	/////////////////////////////////////////////////////////////////////////////////////////////////
	const char* TemplateTypeManager::parseTemplateType(const char* text, int& type, const char* error) {
		return nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// brief : parse a text to known it is a template type or not
	// params: 
	//          text: any text want to parse
	//          type: in case success, type is a registered template type
	//                in case failure, type is -1
	//          error: in case the type is seem to be a template type but it is missing something,
	//                 this parameter contains error explanation
	//                 in case this is a normal type or success, this parameter is null
	// return: return the pointer of next character behind the template type
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int registerTemplateType(const char* templateTypeName, ...) {
		return DATA_TYPE_UNKNOWN;
	}
}
