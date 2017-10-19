#pragma once
#include <functional>

class ScopeAutoFunction {
	std::function<void()> _fx;
public:

	ScopeAutoFunction(const std::function<void()>& fx) {
		_fx = fx;
	}

	ScopeAutoFunction(std::function<void()>&& fx) {
		_fx = fx;
	}

	~ScopeAutoFunction() {
		_fx();
	}
};