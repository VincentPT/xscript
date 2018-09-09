/******************************************************************
* File:        SingleList.h
* Description: define SingleList template class. A linked single
*              connection node list.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
namespace ffscript {

	template <class T>
	class SingleList
	{
		struct SingleNode {
			T value;
			SingleNode* nextNode;
		};

	private:
		SingleNode* _head;
		SingleNode* _tail;
		int _size;

	public:
		SingleList() : _head(nullptr), _tail(nullptr), _size(0) {}
		~SingleList() {
			if (_head) {
				SingleNode* pTempNode;
				for (SingleNode* pNode = _head; pNode; ) {
					pTempNode = pNode;
					pNode = pTempNode->nextNode;
					delete pTempNode;
				}
			}
		}
		void push_back(const T& value) {

			SingleNode* newNode = new SingleNode;
			newNode->value = value;
			newNode->nextNode = nullptr;

			if (_tail == nullptr) {
				_head = newNode;
				_tail = newNode;
			}
			else {
				_tail->nextNode = newNode;
				_tail = newNode;
			}
			_size++;
		}

		void push_front(const T& value) {
			SingleNode* newNode = new SingleNode;
			newNode->value = value;
			newNode->nextNode = nullptr;

			if (_head == nullptr) {
				_head = newNode;
				_tail = newNode;
			}
			else {
				newNode->nextNode = _head;
				_head = newNode;
			}
			_size++;
		}
		
		void pop_front() {
			SingleNode* node = _head;			
			_head = node->nextNode;
			if (_head == nullptr) {
				_tail = nullptr;
			}
			delete node;
			_size--;
		}

		inline T& front() {
			return _head->value;
		}

		inline const T& front() const {
			return _head->value;
		}

		inline T& back() {
			return _tail->value;
		}

		bool isEmpty() {
			return _head == nullptr;
		}

		inline int getSize() const {
			return _size;
		}
	};
}
