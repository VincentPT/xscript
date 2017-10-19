#pragma once

template <class T1, class T2>
auto findLastIterator(std::map<T1, T2>& theMap) {
	auto it = theMap.begin();
	auto prev_it = theMap.end();
	for (; ; it++) {
		if (it == theMap.end()) {
			break;
		}
		prev_it = it;
	}

	return prev_it;
}
