/******************************************************************
* File:        TemplateTypeManager.h
* Description: declare TemplateTypeManager class. A class is designed
*              to manage template types of the C Lambda language. It
*              may be used in future when the library support template
*              functions and template structures.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once

namespace ffscript {
	class TemplateTypeManager
	{
	public:
		virtual ~TemplateTypeManager();

		static TemplateTypeManager* getInstance();

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
		const char* parseTemplateType(const char* text, int& type, const char* error);

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
		int registerTemplateType(const char* templateTypeName, ...);

	private:
		TemplateTypeManager();
		static TemplateTypeManager* s_pInstance;
	};
}