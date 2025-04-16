// Copyright 2025 Chernykh Valentin
#pragma once
#include <iostream>

enum State {
	Empty,
	Busy,
	Deleted
};

template<typename T>
class TVector {
private:
	T* _data;
	State* _states;
	size_t _capacity;
	size_t _size;
	size_t _deleted;
	int _size_step;

public:
	TVector();
	TVector(size_t);
	TVector(size_t, T);
	~TVector();

	bool operator==(const TVector<T>&) const;
	template<typename U>
	friend std::ostream& operator<<(std::ostream&, const TVector<U>&);

	template<typename U>
	class Iterator {
	private:
		U* _ptr;

	public:
		Iterator(U* ptr);
	};

};

template<typename T>
TVector<T>::TVector() : _data(nullptr), _states(nullptr), _capacity(0), _size(0), _deleted(0), _size_step(15) {}

template<typename T>
TVector<T>::TVector(size_t size) {
	_size_step = 15;
	_size = size;
	_deleted = 0;
	_capacity = (_size / _size_step) * _size_step;
	_data = new T[_capacity];
	_states = new State[_capacity];
}

template<typename T>
TVector<T>::~TVector() {
	delete[] _data;
	delete[] _states;
}

template<typename T>
bool TVector<T>::operator==(const TVector<T>& other) const{
	return _size == other._size && _deleted == other._deleted && _capacity == other._capacity;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const TVector<T>& out) {
	stream << out._size " " << out._capacity " " << out._deleted;
	return stream;
}
