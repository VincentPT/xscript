/******************************************************************
* File:        MakePathsUT.cpp
* Description: Test cases for building a full path of tree that
*              connect a higher node to all lower nodes in any level
*              of a tree.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include <Utils.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ffscript;

namespace ffscriptUT
{
	TEST_CLASS(MakePathsUT)
	{
	public:

		template <class T, class NodeCollection>
		bool isContain(const std::vector<std::shared_ptr<NodeCollection>>& nodeCollections, const T& val) {
			for (auto it = nodeCollections.begin(); it != nodeCollections.end(); ++it) {
				auto itBegin = it->get()->begin();
				auto itEnd = it->get()->end();
				auto itRes = std::find(itBegin, itEnd, val);
				if (itRes != itEnd) {
					return true;
				}
			}

			return false;
		}

		template <class T, class NodeCollection>
		bool checkPath(const std::vector<std::shared_ptr<NodeCollection>>& nodeCollections, std::list<std::vector<T>>& paramsPaths) {
			for (auto it = paramsPaths.begin(); it != paramsPaths.end(); it++) {
				const std::vector<T>& path = *it;

				//path size must equal to tree depth
				if (path.size() != nodeCollections.size()) {
					return false;
				}
				for (auto jt = path.begin(); jt != path.end(); jt++) {
					//check all element in path must in node collection
					if (isContain(nodeCollections, *jt) == false) {
						return false;
					}
					//check element in path must appear only once
					auto kt = jt;
					for (++kt; kt != path.end(); kt++) {
						if (*kt == *jt) {
							return false;
						}
					}
				}

				//path must be unique
				auto ht = it;
				size_t i;
				for (++ht; ht != paramsPaths.end(); ht++) {
					const std::vector<T>& otherPath = *ht;
					if (path.size() != otherPath.size()) {
						return false;
					}
					for (i = 0; i < otherPath.size(); i++) {
						if (path[i] != otherPath[i]) {
							break;
						}
					}
					if (i == otherPath.size()) {
						return false;
					}
				}
			}

			return true;
		}
		

		TEST_METHOD(makePath01)
		{
			std::vector<std::shared_ptr<std::vector<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::vector<int>>();
			auto nodeLevel2 = std::make_shared<std::vector<int>>();
			auto nodeLevel3 = std::make_shared<std::vector<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);

			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);
			nodeLevel2->push_back(5);

			nodeLevel3->push_back(6);
			nodeLevel3->push_back(7);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int,std::vector<int>,int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath02)
		{
			std::vector<std::shared_ptr<std::vector<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::vector<int>>();
			auto nodeLevel2 = std::make_shared<std::vector<int>>();
			auto nodeLevel3 = std::make_shared<std::vector<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);

			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);
			nodeLevel2->push_back(5);

			nodeLevel3->push_back(6);
			nodeLevel3->push_back(7);
			nodeLevel3->push_back(8);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int,std::vector<int>,int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath03)
		{
			std::vector<std::shared_ptr<std::vector<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::vector<int>>();
			auto nodeLevel2 = std::make_shared<std::vector<int>>();
			auto nodeLevel3 = std::make_shared<std::vector<int>>();
			auto nodeLevel4 = std::make_shared<std::vector<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);

			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);
			nodeLevel2->push_back(5);

			nodeLevel3->push_back(6);
			nodeLevel3->push_back(7);
			nodeLevel3->push_back(8);

			nodeLevel4->push_back(9);
			nodeLevel4->push_back(10);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int,std::vector<int>,int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath04)
		{
			std::vector<std::shared_ptr<std::vector<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::vector<int>>();
			auto nodeLevel2 = std::make_shared<std::vector<int>>();
			auto nodeLevel3 = std::make_shared<std::vector<int>>();
			auto nodeLevel4 = std::make_shared<std::vector<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);

			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);
			nodeLevel2->push_back(5);

			nodeLevel3->push_back(6);
			nodeLevel3->push_back(7);
			nodeLevel3->push_back(8);

			nodeLevel4->push_back(9);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int,std::vector<int>,int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath04InvalidPath)
		{
			std::vector<std::shared_ptr<std::vector<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::vector<int>>();
			auto nodeLevel2 = std::make_shared<std::vector<int>>();
			auto nodeLevel3 = std::make_shared<std::vector<int>>();
			auto nodeLevel4 = std::make_shared<std::vector<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);

			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);
			nodeLevel2->push_back(5);

			nodeLevel3->push_back(6);
			nodeLevel3->push_back(2);
			nodeLevel3->push_back(8);

			nodeLevel4->push_back(9);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int,std::vector<int>,int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsFalse(res, L"At leat one of path must be invalid because of invalid input");
		}

		TEST_METHOD(makePath05)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();
			auto nodeLevel4 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);

			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);
			nodeLevel2->push_back(5);

			nodeLevel3->push_back(6);
			nodeLevel3->push_back(7);
			nodeLevel3->push_back(8);

			nodeLevel4->push_back(9);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int,std::list<int>,int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath05Invalid)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();
			auto nodeLevel4 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);

			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);
			nodeLevel2->push_back(5);

			nodeLevel3->push_back(6);
			nodeLevel3->push_back(8);
			nodeLevel3->push_back(8);

			nodeLevel4->push_back(9);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsFalse(res, L"Invalid input(two node are same value) must lead to invalid output");
		}


		TEST_METHOD(makePath06)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);

			nodesCollection.push_back(nodeLevel1);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"Path is invalid");
		}

		TEST_METHOD(makePath07)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();
			auto nodeLevel4 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel2->push_back(2);
			nodeLevel3->push_back(3);
			nodeLevel4->push_back(4);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath08)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();
			auto nodeLevel4 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel2->push_back(2);
			nodeLevel3->push_back(3);
			nodeLevel4->push_back(4);
			nodeLevel4->push_back(5);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath09)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();
			auto nodeLevel4 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(5);
			nodeLevel2->push_back(2);
			nodeLevel3->push_back(3);
			nodeLevel4->push_back(4);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath10)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();
			auto nodeLevel4 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(5);
			nodeLevel2->push_back(2);
			nodeLevel3->push_back(3);
			nodeLevel4->push_back(4);
			nodeLevel4->push_back(6);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath11)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();
			auto nodeLevel4 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(5);
			nodeLevel2->push_back(2);
			nodeLevel3->push_back(3);
			nodeLevel3->push_back(7);
			nodeLevel4->push_back(4);
			nodeLevel4->push_back(6);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);
			nodesCollection.push_back(nodeLevel4);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size() * nodeLevel4->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath12)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();
			auto nodeLevel3 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel2->push_back(2);
			nodeLevel2->push_back(3);
			nodeLevel3->push_back(4);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);
			nodesCollection.push_back(nodeLevel3);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size() * nodeLevel3->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath13)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel2->push_back(2);
			nodeLevel2->push_back(3);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath14)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);
			nodeLevel2->push_back(3);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}

		TEST_METHOD(makePath15)
		{
			std::vector<std::shared_ptr<std::list<int>>> nodesCollection;

			auto nodeLevel1 = std::make_shared<std::list<int>>();
			auto nodeLevel2 = std::make_shared<std::list<int>>();

			nodeLevel1->push_back(1);
			nodeLevel1->push_back(2);
			nodeLevel2->push_back(3);
			nodeLevel2->push_back(4);

			nodesCollection.push_back(nodeLevel1);
			nodesCollection.push_back(nodeLevel2);

			std::list<std::vector<int>> paramsPaths;

			listPaths<int, std::list<int>, int>(nodesCollection, paramsPaths);

			Assert::AreEqual(nodeLevel1->size() * nodeLevel2->size(), paramsPaths.size(), L"Paths is not fully covered");
			bool res = checkPath(nodesCollection, paramsPaths);
			Assert::IsTrue(res, L"At leat one of path is invalid");
		}
	};
}