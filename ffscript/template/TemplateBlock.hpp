/******************************************************************
* File:        TemplateBlock.hpp
* Description: declare MemoryBlock template structure. A class is
*              designed to make a space in an template object
*              to store a custom data.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
template <int size>
#pragma pack(push)
#pragma pack(1)
struct MemoryBlock
{
	char data[size];
};
#pragma pack(pop)