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
	class Iterator {
	private:
		T* _ptr;
		const TVector<T>& _parent;
	public:
		Iterator(T*, const TVector<T>&) noexcept;
		T& operator*() noexcept;
		Iterator& operator++() noexcept;
		bool operator!=(const Iterator&) const noexcept;
	};

	TVector() noexcept; 
	TVector(size_t) noexcept;
	TVector(size_t, T) noexcept;
	~TVector() noexcept;

	Iterator begin() const noexcept;
	Iterator end() const noexcept;

	bool operator==(const TVector<T>&) const noexcept;
	template<typename U>
	friend std::ostream& operator<<(std::ostream&, const TVector<U>&) noexcept;
};

template<typename T>
TVector<T>::TVector() noexcept : _data(nullptr), _states(nullptr), _capacity(0), _size(0), _deleted(0), _size_step(15) {}

template<typename T>
TVector<T>::TVector(size_t size) noexcept : _size_step(15), _size(size), _deleted(0), _capacity((size / 15 + 1) * 15) {
	_data = new T[_capacity];
	_states = new State[_capacity];

	for (int i = 0; i < _capacity; i++) {
		_data[i] = 0;
		_states[i] = Empty;
	}
}

template<typename T>
TVector<T>::~TVector() noexcept {
	delete[] _data;
	delete[] _states;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::begin() const noexcept {
	return Iterator(&_data[0], *this);
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::end() const noexcept {
	return Iterator(&_data[_size - 1] + 1, *this);
}

template<typename T>
bool TVector<T>::operator==(const TVector<T>& other) const noexcept {
	return _size == other._size && _deleted == other._deleted && _capacity == other._capacity;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const TVector<T>& out) noexcept{
	for (auto it = out.begin(); it != out.end(); ++it) {
		stream << *it << " ";
	}

	return stream;
}

template<typename T>
TVector<T>::Iterator::Iterator(T* ptr, const TVector<T>& parent) noexcept : _ptr(ptr), _parent(parent) {}

template <typename T>
T& TVector<T>::Iterator::operator*() noexcept {
	return *_ptr;
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator++() noexcept{
	ptrdiff_t current_index = _ptr - _parent._data;

	for (ptrdiff_t i = current_index + 1; i <= _parent._size; i++) {
		if (_parent._states[i] != Deleted) {
			_ptr = &_parent._data[i];
			break;
		}
	}

	return *this;
}

template<typename T>
bool TVector<T>::Iterator::operator!=(const Iterator& other) const noexcept{
	return _ptr != other._ptr;
}
