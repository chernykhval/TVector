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
	size_t _size_step;

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
	TVector(std::initializer_list<T>) noexcept;
	~TVector() noexcept;

	T* data() noexcept;
	const T* data() const noexcept;
	size_t size() const noexcept;
	size_t capacity() const noexcept;
	T& front() noexcept;
	T& back() noexcept;
	Iterator begin() const noexcept;
	Iterator end() const noexcept;

	bool operator==(const TVector<T>&) const noexcept;
	T& operator[](size_t);
	template<typename U>
	friend std::ostream& operator<<(std::ostream&, const TVector<U>&) noexcept;
};

template<typename T>
TVector<T>::TVector() noexcept : _data(nullptr), _states(nullptr), _capacity(0), _size(0), _deleted(0), _size_step(15) {}

template<typename T>
TVector<T>::TVector(size_t size) noexcept : _size_step(15), _size(size), _deleted(0), _capacity((size / 15 + 1) * 15 * (size > 0)) {
	_data = new T[_capacity];
	_states = new State[_capacity];

	for (int i = 0; i < _capacity; i++) {
		_data[i] = 0;
		_states[i] = Empty;
	}
}

template<typename T>
TVector<T>::TVector(size_t size, T elem) noexcept : _size_step(15), _size(size), _deleted(0), _capacity((size / 15 + 1) * 15 * (size > 0)) {
	_data = new T[_capacity];
	_states = new State[_capacity];

	for (int i = 0; i < _capacity; i++) {
		_data[i] = elem;
		_states[i] = Busy;
	}
}

template<typename T>
TVector<T>::TVector(std::initializer_list<T> init) noexcept : _size_step(15), _size(init.size()), _deleted(0), _capacity((init.size() / 15 + 1) * 15 * (init.size() > 0)) {
	_data = new T[_capacity];
	_states = new State[_capacity];
	size_t index = 0;

	for (auto it = init.begin(); it != init.end(); it++) {
		_data[index] = *it;
		_states[index] = Busy;
		index++;
	}
}

template<typename T>
TVector<T>::~TVector() noexcept {
	delete[] _data;
	delete[] _states;
}

template<typename T>
T* TVector<T>::data() noexcept {
	return _data;
}

template<typename T>
const T* TVector<T>::data() const noexcept
{
	return _data;
}

template<typename T>
size_t TVector<T>::size() const noexcept
{
	return _size;
}

template<typename T>
size_t TVector<T>::capacity() const noexcept
{
	return _capacity;
}

template<typename T>
T& TVector<T>::front() noexcept
{
	for (int i = 0; i < _size + _deleted; i++) {
		if (_states[i] == Busy)
			return _data[i];
	}
}

template<typename T>
T& TVector<T>::back() noexcept
{
	for (int i = _size + _deleted - 1; i >= 0; i--) {
		if (_states[i] == Busy)
			return _data[i];
	}
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
T& TVector<T>::operator[](size_t index)
{
	if (index >= _size + _deleted) {
		throw std::out_of_range("Index out of range");
	}
	for (int i = index; i < _size + _deleted; i++) {
		if (_states[i] == Busy)
			return _data[i];
	}
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const TVector<T>& out) noexcept{
	stream << "size(" << out._size << ") capacity(" << out._capacity << ") deleted(" << out._deleted << ") vector: [ ";

	for (auto it = out.begin(); it != out.end(); ++it) {
		stream << *it << " ";
	}

	stream << " ]";

	return stream;
}

#pragma region IteratorRealization
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
#pragma endregion

