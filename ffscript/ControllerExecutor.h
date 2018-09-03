/******************************************************************
* File:        ControllerExecutor.h
* Description: declare ControllerExecutor class. A command container
*              class used to store controlling commands that control
*              the flow code.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "Executor.h"
namespace ffscript {
	class ControllerExecutor :
		public Executor
	{
	public:
		ControllerExecutor();
		virtual ~ControllerExecutor();
	};
}
