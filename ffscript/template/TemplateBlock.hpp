/******************************************************************
* File:        TemplateBlock.hpp
* Description: declare MemoryBlock template structure. A class is
*              designed to make a space in an template object
*              to store a custom data.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#pragma pack(push)
#pragma pack(1)
template <int size>
struct MemoryBlock
{
	char data[size];
};
#pragma pack(pop)