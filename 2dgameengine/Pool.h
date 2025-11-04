#pragma once
#include <iostream>
#include <vector>

template<typename T>
class Pool {
private:
	std : vector<T> m_data;
public:
	Pool(int size = 100) { Resize(size); }
	~Pool() = default;

	bool IsEmpty() const {
		return m_data.empty();
	}

	int GetSize() const { return m_data.size(); }

	void Resize(int n) {
		m_data.resize(n);
	}

	void Clear() {
		m_data.clear();
	}

	void Add(T Object) {
		m_data.push_back(Object);
	}

	void Set(int index, T object) {
		m_data[index] = object;
	}

	T& Get(int index) {
		return static_cast<T&>(m_data[index]);
	}

	T& operator[] (unsigned int index) {
		return m_data[index];
	}












};