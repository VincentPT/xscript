#pragma once

#include <memory>

namespace ffscript {
	class MemoryBlock
	{
	public:
		MemoryBlock();
		virtual ~MemoryBlock();		
		virtual void* getDataRef() = 0;
	};

	class BufferBlock : public MemoryBlock
	{
		unsigned char* _buffer;
		int _size;
	public:
		BufferBlock(int size);
		virtual ~BufferBlock();
		void* getDataRef();
	};

	template <class T>
	class ObjectRefBlock : public MemoryBlock
	{
		T* _pObject;
	public:
		ObjectRefBlock(T* pObject) : _pObject(pObject) {}
		virtual ~ObjectRefBlock() {
			if (_pObject) {
				delete _pObject;
				_pObject = nullptr;
			}
		}
		void* getDataRef() {
			return (void*)_pObject;
		}
	};

	class DummyMemBlock : public MemoryBlock
	{
		void* _pObject;
	public:
		DummyMemBlock(void* pObject) : _pObject(pObject) {}
		virtual ~DummyMemBlock() {}

		void* getDataRef() {
			return _pObject;
		}
	};

	typedef std::shared_ptr<MemoryBlock> MemoryBlockRef;
}