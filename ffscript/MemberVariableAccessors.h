#pragma once
namespace ffscript {
	class MemberVariableAccessor
	{
	public:
		MemberVariableAccessor();
		virtual ~MemberVariableAccessor();
		virtual void* access(void* address) = 0;
	};

	class MVContextAccessor : public MemberVariableAccessor {
	public:
		void* access(void* address);
	};

	class MVGlobalAccessor : public MemberVariableAccessor {
		void* _address;
	public:
		MVGlobalAccessor(void* address);
		void* access(void* address);
	};

	class MVOffsetAccessor : public MemberVariableAccessor {
		int _offset;
	public:
		MVOffsetAccessor(int offset);
		void* access(void* address);
	};

	class MVPointerAccessor : public MemberVariableAccessor {
	public:
		void* access(void* address);
	};
}

