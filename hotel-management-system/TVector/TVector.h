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
	size_t _used;
	size_t _deleted;
	size_t _size_step = 15;

public:
	class Iterator {
	private:
		T* _ptr;
		const TVector<T>& _parent;
	public:
		Iterator(T*, const TVector<T>&) noexcept;
		T& operator*() noexcept;
		Iterator& operator=(const Iterator&) noexcept;
		Iterator& operator++() noexcept;
		Iterator operator++(int) noexcept;
		Iterator& operator--() noexcept;
		Iterator operator--(int) noexcept;
		Iterator operator+(int) const noexcept;
		Iterator operator-(int) const noexcept;
		Iterator& operator+=(int) noexcept;
		Iterator& operator-=(int) noexcept;
		bool operator!=(const Iterator&) const noexcept;
		ptrdiff_t operator-(const Iterator&) const;
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

	void push_back(const T& value);
	void push_front(const T& value);
	Iterator insert(Iterator position, const T& value);

	bool operator==(const TVector<T>&) const noexcept;
	T& operator[](size_t);
	template<typename U>
	friend std::ostream& operator<<(std::ostream&, const TVector<U>&) noexcept;

private:
	void reset_memory(size_t) noexcept;
	Iterator reset_memory(size_t, const Iterator&) noexcept;
};

template<typename T>
TVector<T>::TVector() noexcept : _data(nullptr), _states(nullptr), _capacity(0), _used(0), _deleted(0) {}

template<typename T>
TVector<T>::TVector(size_t size) noexcept : _used(size), _deleted(0) {
	_capacity = (size / _size_step + 1) * _size_step * (size > 0);
	_data = new T[_capacity];
	_states = new State[_capacity];

	for (int i = 0; i < _capacity; i++) {
		_data[i] = 0;
		_states[i] = Empty;
	}
}

template<typename T>
TVector<T>::TVector(size_t size, T elem) noexcept : _used(size), _deleted(0) {
	_capacity = (size / _size_step + 1) * _size_step * (size > 0);
	_data = new T[_capacity];
	_states = new State[_capacity];

	for (int i = 0; i < _capacity; i++) {
		_data[i] = elem;
		_states[i] = Busy;
	}
}

template<typename T>
TVector<T>::TVector(std::initializer_list<T> init) noexcept : _used(init.size()), _deleted(0) {
	if (init.size() <= 15)
		_capacity = _size_step * (init.size() > 0);
	else
		_capacity = (init.size() / _size_step + 1) * _size_step * (init.size() > 0);
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
	return _used - _deleted;
}

template<typename T>
size_t TVector<T>::capacity() const noexcept
{
	return _capacity;
}

template<typename T>
T& TVector<T>::front() noexcept
{
	for (int i = 0; i < _used; i++) {
		if (_states[i] == Busy)
			return _data[i];
	}
}

template<typename T>
T& TVector<T>::back() noexcept
{
	for (int i = _used - 1; i >= 0; i--) {
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
	return Iterator(&_data[_used - 1] + 1, *this);
}

template<typename T>
void TVector<T>::push_back(const T& value) {
	if (_states[_used - 1] == Deleted) {
		_data[_used - 1] = value;
		_states[_used - 1] = Busy;
		_deleted--;
		return;
	}

	if (_used == _capacity) {
		reset_memory(_used + 1);
	}

	_data[_used] = value;
	_states[_used] = Busy;
	_used++;
}

template<typename T>
void TVector<T>::push_front(const T& value) {
	if (_states[0] == Deleted) {
		_data[0] = value;
		_states[0] = Busy;
		_deleted--;
		return;
	}

	if (_used == _capacity)
		reset_memory(_used + 1);
	
	for (int i = _used; i > 0; i--) {
		_data[i] = _data[i - 1];
		_states[i] = _states[i - 1];
	}
	
	_data[0] = value;
	_states[0] = Busy;
	_used++;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::insert(Iterator position, const T& value)
{
	if (_used != _capacity) {
		int insert_index = begin() - position;

		for (int i = _used; i > insert_index; i++) {
			_data[i] = _data[i - 1];
			_states[i] = _states[i - 1];
		}

		_data[insert_index] = value;
		_states[insert_index] = Busy;
		_used++;

		return position;
	}

	Iterator new_position = reset_memory(_used + 1, position);

	int insert_index = new_position - begin();

	for (int i = _used; i > insert_index; i--) {
		_data[i] = _data[i - 1];
		_states[i] = _states[i - 1];
	}

	_data[insert_index] = value;
	_states[insert_index] = Busy;
	_used++;

	return new_position;
}

template<typename T>
bool TVector<T>::operator==(const TVector<T>& other) const noexcept {
	return _used == other._used && _deleted == other._deleted && _capacity == other._capacity;
}

template<typename T>
T& TVector<T>::operator[](size_t index)
{
	if (index >= _used) {
		throw std::out_of_range("Index out of range");
	}

	for (int i = index; i < _used; i++) {
		if (_states[i] == Busy)
			return _data[i];
	}
}

template<typename T>
void TVector<T>::reset_memory(size_t new_used) noexcept {
	int used_diff = new_used - _used;
	size_t real_size = new_used - _deleted;
	size_t new_capacity = (real_size / _size_step + 1) * _size_step;
	T* new_data = new T[new_capacity];
	State* new_states = new State[new_capacity];
	size_t index = 0;

	for (size_t i = 0; i < _capacity; i++) {
		if (_states[i] == Busy) {
			new_data[index] = _data[i];
			new_states[index] = Busy;
			index++;
		}
	}

	for (size_t i = index; i < new_capacity; i++) {
		new_states[i] = Empty;
	}

	_capacity = new_capacity;
	_deleted = 0;
	_used = real_size - used_diff;
	delete[] _data;
	delete[] _states;
	_data = new_data;
	_states = new_states;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::reset_memory(size_t new_used, const Iterator& insert_it) noexcept {
	size_t new_insert_index = insert_it - begin();
	reset_memory(new_used);

	return Iterator(&_data[new_insert_index], *this);
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const TVector<T>& out) noexcept{
	stream << "size(" << out._used << ") capacity(" << out._capacity << ") deleted(" << out._deleted << ") vector: [ ";

	for (auto it = out.begin(); it != out.end(); ++it) {
		stream << *it << " ";
	}

	stream << " ]";

	return stream;
}

#pragma region IteratorsRealization
template<typename T>
TVector<T>::Iterator::Iterator(T* ptr, const TVector<T>& parent) noexcept : _ptr(ptr), _parent(parent) {}

template <typename T>
T& TVector<T>::Iterator::operator*() noexcept {
	return *_ptr;
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator=(const Iterator& other) noexcept
{
	_ptr = other._ptr;
	_parent = other._parent;
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator++() noexcept{
	ptrdiff_t current_index = _ptr - _parent._data;

	for (ptrdiff_t i = current_index + 1; i <= _parent._used; i++) {
		if (_parent._states[i] != Deleted) {
			_ptr = &_parent._data[i];
			break;
		}
	}

	return *this;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::Iterator::operator++(int) noexcept {
	Iterator temp = *this;
	++(*this);
	return temp; 
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator--() noexcept {
	ptrdiff_t current_index = _ptr - _parent._data;

	for (ptrdiff_t i = current_index - 1; i >= 0; i--) {
		if (_parent._states[i] != Deleted) {
			_ptr = &_parent._data[i];
			break;
		}
	}

	return *this;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::Iterator::operator--(int) noexcept {
	Iterator temp = *this;
	--(*this);
	return temp;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::Iterator::operator+(int num) const noexcept {
	int new_index = _ptr - _parent._data;

	while (new_index < _parent._used && num > 0) {
		new_index++;

		if (_parent._states[new_index] != Deleted) {
			num--;
		}
	}

	return Iterator(&_parent._data[new_index], _parent);
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::Iterator::operator-(int num) const noexcept
{
	int new_index = _ptr - _parent._data;

	while (new_index >= 0 && num > 0) {
		new_index--;

		if (_parent._states[new_index] != Deleted) {
			num--;
		}
	}

	return Iterator(&_parent._data[new_index], _parent);
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator+=(int num) noexcept {
	int new_index = _ptr - _parent._data;

	while (new_index < _parent._used && num > 0) {
		new_index++;

		if (_parent._states[new_index] != Deleted) {
			num--;
		}
	}

	_ptr = &_parent._data[new_index];

	return *this;
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator-=(int num) noexcept
{
	int new_index = _ptr - _parent._data;

	while (new_index >= 0 && num > 0) {
		new_index--;

		if (_parent._states[new_index] != Deleted) {
			num--;
		}
	}

	_ptr = &_parent._data[new_index];

	return *this;
}

template<typename T>
bool TVector<T>::Iterator::operator!=(const Iterator& other) const noexcept{
	return _ptr != other._ptr;
}

template<typename T>
ptrdiff_t TVector<T>::Iterator::operator-(const Iterator& other) const {
	if (_parent.begin() != other._parent.begin())
		throw std::exception("Diffrent parrents");

	int reverse = _ptr < other._ptr ? -1 : 1;
	Iterator left = _ptr < other._ptr ? *this : other;
	Iterator right = _ptr > other._ptr ? *this : other; 
	int count = 0;

	for (auto it = left; it != right; it++) {
		count++; 
	}

	ptrdiff_t result = count * reverse;
	
	return result;
}
#pragma endregion
