// Copyright 2025 Chernykh Valentin
#pragma once
#include <iostream>
#include <stdexcept> 
#include <utility>

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
	size_t _capacity_step = 15;
	float _removal_coefficient = 0.15f;

public:
	class Iterator {
	private:
		T* _ptr;
		const TVector<T>& _parent;
	public:
		Iterator(T*, const TVector<T>&) noexcept;
		T& operator*() noexcept;
		T* operator->() noexcept;
		Iterator& operator=(const Iterator&) noexcept;
		Iterator& operator++() noexcept;
		Iterator operator++(int) noexcept;
		Iterator& operator--() noexcept;
		Iterator operator--(int) noexcept;
		Iterator operator+(int) const;
		Iterator operator-(int) const;
		Iterator& operator+=(int);
		Iterator& operator-=(int);
		bool operator!=(const Iterator&) const noexcept;
		bool operator==(const Iterator&) const noexcept;
		ptrdiff_t operator-(const Iterator&) const;
	};

	TVector() noexcept; 
	TVector(size_t) noexcept;
	TVector(size_t, T) noexcept;
	TVector(const TVector&) noexcept;
	TVector(TVector&&) noexcept;
	TVector(std::initializer_list<T>) noexcept;
	~TVector() noexcept;

	T* data() noexcept;
	const T* data() const noexcept;
	State* states() noexcept;
	const State* states() const noexcept;
	size_t size() const noexcept;
	size_t used() const noexcept;
	size_t capacity() const noexcept;
	T& front() noexcept;
	T& back() noexcept;
	Iterator begin() const noexcept;
	Iterator end() const  noexcept;

	void push_back(const T&) noexcept;
	void push_back(T&&) noexcept;
	void push_front(const T&) noexcept;
	void push_front(T&&) noexcept;
	Iterator insert(Iterator position, const T& value) noexcept;
	Iterator insert(Iterator position, size_t n, const T& value) noexcept;
	Iterator insert(Iterator position, T&& value) noexcept;
	void pop_back() noexcept;
	void pop_front() noexcept;
	Iterator erase(Iterator position) noexcept;


	bool is_empty() const noexcept;
	TVector& operator=(const TVector&) noexcept;
	TVector& operator=(TVector&&) noexcept;
	bool operator==(const TVector<T>&) const noexcept;
	bool operator!=(const TVector<T>&) const noexcept;
	T& operator[](size_t);
	const T& operator[](size_t) const;

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
	_capacity = (size / _capacity_step + 1) * _capacity_step * (size > 0);
	_data = new T[_capacity];
	_states = new State[_capacity];

	for (int i = 0; i < _used; i++) {
		_data[i] = T();
		_states[i] = Busy;
	}

	for (int i = _used; i < _capacity; i++) {
		_states[i] = Empty;
	}
}

template<typename T>
TVector<T>::TVector(size_t size, T elem) noexcept : _used(size), _deleted(0) {
	_capacity = (size / _capacity_step + 1) * _capacity_step * (size > 0);
	_data = new T[_capacity];
	_states = new State[_capacity];

	for (int i = 0; i < _used; i++) {
		_data[i] = elem;
		_states[i] = Busy;
	}

	for (int i = _used; i < _capacity; i++) {
		_states[i] = Empty;
	}
}

template<typename T>
TVector<T>::TVector(const TVector& other) noexcept : _used(other._used), _deleted(other._deleted), 
_capacity(other._capacity), _data(new T[other._capacity]), _states(new State[other._capacity]) {
	for (int i = 0; i < _used; i++) {
		_data[i] = other._data[i];
		_states[i] = other._states[i];
	}

	for (int i = _used; i < _capacity; i++) {
		_states[i] = Empty;
	}
}

template<typename T>
TVector<T>::TVector(TVector&& other) noexcept : _used(other._used), _deleted(other._deleted),
_capacity(other._capacity) {
	_states = other._states;
	other._states = nullptr;
	_data = other._data;
	other._data = nullptr;
	other._used = 0;
	other._deleted = 0;
	other._capacity = 0;
}

template<typename T>
TVector<T>::TVector(std::initializer_list<T> init) noexcept : _used(init.size()), _deleted(0) {
	if (init.size() <= 15)
		_capacity = _capacity_step * (init.size() > 0);
	else
		_capacity = (init.size() / _capacity_step + 1) * _capacity_step * (init.size() > 0);

	_data = new T[_capacity];
	_states = new State[_capacity];
	size_t index = 0;

	for (const auto& elem : init) {
		_data[index] = elem;
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
const T* TVector<T>::data() const noexcept {
	return _data;
}

template<typename T>
size_t TVector<T>::size() const noexcept {
	return _used - _deleted;
}

template<typename T>
size_t TVector<T>::capacity() const noexcept {
	return _capacity;
}

template<typename T>
T& TVector<T>::front() noexcept {
	for (int i = 0; i < _used; i++) {
		if (_states[i] == Busy)
			return _data[i];
	}
}

template<typename T>
T& TVector<T>::back() noexcept {
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
void TVector<T>::push_back(const T& value) noexcept {
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
void TVector<T>::push_back(T&& value) noexcept {
	if (_states[_used - 1] == Deleted) {
		_data[_used - 1] = std::move(value);
		_states[_used - 1] = Busy;
		_deleted--;
		return;
	}

	if (_used == _capacity) {
		reset_memory(_used + 1);
	}

	_data[_used] = std::move(value);
	_states[_used] = Busy;
	_used++;
}

template<typename T>
void TVector<T>::push_front(const T& value) noexcept {
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
void TVector<T>::push_front(T&& value) noexcept {
	if (_states[0] == Deleted) {
		_data[0] = std::move(value);
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

	_data[0] = std::move(value);
	_states[0] = Busy;
	_used++;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::insert(Iterator position, const T& value) noexcept {
	if (_used != _capacity) {
		int insert_index = position - begin();

		for (int i = _used; i > insert_index; i--) {
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
typename TVector<T>::Iterator TVector<T>::insert(Iterator position, T&& value) noexcept {
	if (_used != _capacity) {
		int insert_index = position - begin();

		for (int i = _used; i > insert_index; i--) {
			_data[i] = _data[i - 1];
			_states[i] = _states[i - 1];
		}

		_data[insert_index] = std::move(value);
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

	_data[insert_index] = std::move(value);
	_states[insert_index] = Busy;
	_used++;

	return new_position;
}

template<typename T>
void TVector<T>::pop_back() noexcept {
	_states[_used - 1] = Deleted;
	_deleted++;

	if (_deleted >= _used * _removal_coefficient) {
		reset_memory(_used - _deleted);
	}
}

template<typename T>
void TVector<T>::pop_front() noexcept {
	_states[0] = Deleted;
	_deleted++;

	if (_deleted >= _used * _removal_coefficient) {
		reset_memory(_used - _deleted);
	}
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::erase(Iterator position) noexcept {
	int deleted_index = position - begin();
	_states[deleted_index] = Deleted;
	_deleted++;

	return position;
}

template<typename T>
bool TVector<T>::is_empty() const noexcept {
	return (_used - _deleted) > 0;
}

template<typename T>
TVector<T>& TVector<T>::operator=(const TVector& other) noexcept {
	if (this != &other) { 
		delete[] _data;
		delete[] _states;

		_capacity = other._capacity;
		_used = other._used;
		_deleted = other._deleted;
		_data = new T[_capacity];
		_states = new State[_capacity];

		for (size_t i = 0; i < _used; i++) {
			_data[i] = other._data[i];
			_states[i] = other._states[i];
		}

		for (size_t i = _used; i < _capacity; i++) {
			_states[i] = Empty;
		}
	}

	return *this;
}

template<typename T>
TVector<T>& TVector<T>::operator=(TVector&& other) noexcept {
	if (this != &other) {
		delete[] _data;
		delete[] _states;
		_capacity = other._capacity;
		_used = other._used;
		_deleted = other._deleted;
		_data = other._data;
		other._data = nullptr;
		_states = other._states;
		other._states = nullptr;
		other._capacity = 0;
		other._used = 0;
		other._deleted = 0;
	}

	return *this;
}

template<typename T>
bool TVector<T>::operator==(const TVector<T>& other) const noexcept {
	if (size() != other.size()) 
		return false;

	for (size_t i = 0; i < size(); i++) {
		if ((*this)[i] != other[i])
			return false;
	}

	return true;
}

template<typename T>
bool TVector<T>::operator!=(const TVector<T>& other) const noexcept {
	return !(*this == other);
}

template<typename T>
T& TVector<T>::operator[](size_t index) {
	if (index >= _used) {
		throw std::out_of_range("TVector operator[]: Index out of range.");
	}

	size_t current_index = 0;

	for (size_t i = 0; i < _used; i++) {
		if (_states[i] == Busy) {
			if (current_index == index)
				return _data[i];

			current_index++;
		}
	}

	throw std::out_of_range("TVector operator[]: Index out of range.");
}

template<typename T>
const T& TVector<T>::operator[](size_t index) const {
	if (index >= _used) {
		throw std::out_of_range("TVector operator[]: Index out of range.");
	}

	size_t current_index = 0;

	for (size_t i = 0; i < _used; i++) {
		if (_states[i] == Busy) {
			if (current_index == index)
				return _data[i];

			current_index++;
		}
	}

	throw std::out_of_range("TVector operator[]: Not found.");
}

template<typename T>
void TVector<T>::reset_memory(size_t new_used) noexcept {
	int used_diff = new_used - _used;
	size_t real_size = new_used - _deleted;
	size_t new_capacity = (real_size / _capacity_step + 1) * _capacity_step;
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
T* TVector<T>::Iterator::operator->() noexcept {
	return _ptr;
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator=(const Iterator& other) noexcept {
	if (this != &other) {
		_ptr = other._ptr;
		_parent = other._parent;
	}

	return *this;
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator++() noexcept {
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
typename TVector<T>::Iterator TVector<T>::Iterator::operator+(int num) const {
	int new_index = _ptr - _parent._data;

	if (new_index + num > _parent._used || new_index + num < 0) {
		throw std::out_of_range("Iterator operator+: Index out of range.");
	}

	while (new_index < _parent._used && num > 0) {
		new_index++;

		if (_parent._states[new_index] != Deleted) {
			num--;
		}
	}

	return Iterator(&_parent._data[new_index], _parent);
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::Iterator::operator-(int num) const {
	int new_index = _ptr - _parent._data;

	if (new_index - num > _parent._used || new_index - num < 0) {
		throw std::out_of_range("Iterator operator-: Index out of range.");
	}

	while (new_index > 0 && num > 0) {
		new_index--;
		 
		if (_parent._states[new_index] != Deleted) {
			num--;
		}
	}

	return Iterator(&_parent._data[new_index], _parent);
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator+=(int num) {
	int new_index = _ptr - _parent._data;

	if (new_index + num > _parent._used || new_index + num < 0) {
		throw std::out_of_range("Iterator operator+: Index out of range.");
	}

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
typename TVector<T>::Iterator& TVector<T>::Iterator::operator-=(int num) {
	int new_index = _ptr - _parent._data;

	if (new_index - num > _parent._used || new_index - num < 0) {
		throw std::out_of_range("Iterator operator-: Index out of range.");
	}

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
bool TVector<T>::Iterator::operator==(const Iterator& other) const noexcept {
	return _ptr == other._ptr;
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
