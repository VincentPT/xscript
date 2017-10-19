#pragma once

#define DECLARE_CLASS_INVOKER_TEMPLATE2(className, classOwner, ...) \
class className<__VA_ARGS__> { \
public: \
	classOwner<__VA_ARGS__>* mOwner; \
	className(classOwner<__VA_ARGS__>* pOwner) { \
		mOwner = pOwner; \
	} \
	inline void call(void* pReturnVal, void* params[]); \
}