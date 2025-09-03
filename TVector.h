// Copyright 2025 Chernykh Valentin
#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>
#include <ctime>

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
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    class Iterator {
     private:
        T* _ptr;
        TVector<T>& _parent;

     public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = TVector::value_type;
        using reference = TVector::reference;
        using pointer = TVector::pointer;
        using difference_type = TVector::difference_type;

        Iterator(T*, TVector<T>&) noexcept;
        Iterator(const Iterator&) noexcept;

        inline reference operator*();
        inline pointer operator->() noexcept;
        inline Iterator& operator=(const Iterator&) noexcept;
        Iterator& operator++() noexcept;
        inline Iterator operator++(int) noexcept;
        Iterator& operator--() noexcept;
        inline Iterator operator--(int) noexcept;
        Iterator operator+(int) const;
        Iterator operator-(int) const;
        Iterator& operator+=(int);
        Iterator& operator-=(int);
        inline bool operator!=(const Iterator&) const noexcept;
        inline bool operator==(const Iterator&) const noexcept;
        difference_type operator-(const Iterator&) const;
        inline difference_type index() const noexcept;
        bool operator<(const Iterator&) const noexcept;
        bool operator>(const Iterator&) const noexcept;
        bool operator<=(const Iterator&) const noexcept;
        bool operator>=(const Iterator&) const noexcept;
        reference operator[](difference_type);
    };

    class ConstIterator {
     private:
        const T* _ptr;
        const TVector<T>& _parent;

     public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = TVector::value_type;
        using reference = TVector::const_reference;
        using pointer = TVector::const_pointer;
        using difference_type = TVector::difference_type;

        ConstIterator(const T*, const TVector<T>&) noexcept;
        ConstIterator(const ConstIterator&) noexcept;

        inline reference operator*();
        inline pointer operator->() noexcept;
        inline ConstIterator& operator=(const ConstIterator&) noexcept;
        ConstIterator& operator++() noexcept;
        inline ConstIterator operator++(int) noexcept;
        ConstIterator& operator--() noexcept;
        inline ConstIterator operator--(int) noexcept;
        ConstIterator operator+(int) const;
        ConstIterator operator-(int) const;
        ConstIterator& operator+=(int);
        ConstIterator& operator-=(int);
        inline bool operator!=(const ConstIterator&) const noexcept;
        inline bool operator==(const ConstIterator&) const noexcept;
        difference_type operator-(const ConstIterator&) const;
        inline difference_type index() const noexcept;
        bool operator<(const ConstIterator&) const noexcept;
        bool operator>(const ConstIterator&) const noexcept;
        bool operator<=(const ConstIterator&) const noexcept;
        bool operator>=(const ConstIterator&) const noexcept;
        reference operator[](difference_type);
    };

    TVector() noexcept;
    explicit TVector(size_type) noexcept;
    TVector(size_type, value_type);
    TVector(const TVector&) noexcept;
    TVector(TVector&&) noexcept;
    TVector(pointer, size_type);
    TVector(std::initializer_list<value_type>) noexcept;
    ~TVector() noexcept;

    inline pointer data() noexcept;
    inline const_pointer data() const noexcept;
    inline State* states() noexcept;
    inline const State* states() const noexcept;
    inline size_type size() const noexcept;
    inline size_type used() const noexcept;
    inline size_type capacity() const noexcept;
    inline reference front();
    inline reference back();
    inline Iterator begin() noexcept;
    inline Iterator end() noexcept;
    inline ConstIterator begin() const noexcept;
    inline ConstIterator end() const noexcept;

    void push_back(const value_type&) noexcept;
    void push_back(value_type&&) noexcept;
    void push_front(const value_type&) noexcept;
    void push_front(value_type&&) noexcept;
    Iterator insert(Iterator, const value_type&) noexcept;
    Iterator insert(Iterator position, size_type n,
        const value_type& value) noexcept;
    Iterator insert(Iterator, value_type&&) noexcept;
    template <class... Args>
    Iterator emplace(Iterator position, Args&&... args);
    void pop_back();
    void pop_front();
    Iterator erase(Iterator);

    TVector& assign(const TVector&);
    reference at(size_type);
    inline void clear() noexcept;
    void shrink_to_fit();
    void resize(size_type);
    inline bool is_empty() const noexcept;
    TVector& operator=(const TVector&) noexcept;
    TVector& operator=(TVector&&) noexcept;
    bool operator==(const TVector<value_type>&) const noexcept;
    bool operator!=(const TVector<value_type>&) const noexcept;
    reference operator[](size_type);
    const_reference operator[](size_type) const;

    template<typename U>
    friend std::ostream& operator<<(std::ostream&, const TVector<U>&) noexcept;
    template<typename U>
    friend void shuffle(TVector<U>&) noexcept;
    template<typename U>
    inline friend void tv_sort(TVector<U>&, bool(*comp)(U, U)) noexcept;
    template<typename U>
    friend int* search_all(TVector<U>&, bool(*check) (U)) noexcept;
    template<typename U>
    friend int search_begin(TVector<U>&, bool(*check) (U)) noexcept;
    template<typename U>
    friend int search_end(TVector<U>&, bool(*check) (U)) noexcept;

 private:
    void reset_memory_for_delete() noexcept;
    void reset_memory(size_type) noexcept;
    Iterator reset_memory(size_type, const Iterator&) noexcept;
    inline bool is_full() const noexcept;
    template <typename U>
    friend size_t partition(TVector<U>&, size_type,
        size_type, bool (*comp)(U, U))noexcept;
    template <typename U>
    friend void quick_sort(TVector<U>&, size_type,
        size_type, bool (*comp)(U, U)) noexcept;
    inline void swap_elem(size_type, size_type) noexcept;
};

#pragma region TVectorRealization

template<typename T>
TVector<T>::TVector() noexcept : _data(nullptr), _states(nullptr),
_capacity(0), _used(0), _deleted(0) {}

template<typename T>
TVector<T>::TVector(size_type size) noexcept: _used(size), _deleted(0) {
    _capacity = (size / _capacity_step + 1) * _capacity_step * (size > 0);
    _data = new T[_capacity];
    _states = new State[_capacity];

    for (size_type i = 0; i < _used; i++) {
        _data[i] = T();
        _states[i] = Busy;
    }

    for (size_type i = _used; i < _capacity; i++) {
        _states[i] = Empty;
    }
}

template<typename T>
TVector<T>::TVector(size_type size, value_type elem): _used(size), _deleted(0) {
    if (size == 0) {
        throw std::runtime_error("TVector with value"
                                 " can not be with zero size");
    }

    _capacity = (size / _capacity_step + 1) * _capacity_step * (size > 0);
    _data = new T[_capacity];
    _states = new State[_capacity];

    for (size_type i = 0; i < _used; i++) {
        _data[i] = elem;
        _states[i] = Busy;
    }

    for (size_type i = _used; i < _capacity; i++) {
        _states[i] = Empty;
    }
}

template<typename T>
TVector<T>::TVector(const TVector& other) noexcept : _used(other._used),
_deleted(other._deleted), _capacity(other._capacity),
_data(new T[other._capacity]), _states(new State[other._capacity]) {
    for (int i = 0; i < _used; i++) {
        _data[i] = other._data[i];
        _states[i] = other._states[i];
    }

    for (int i = _used; i < _capacity; i++) {
        _states[i] = Empty;
    }
}

template<typename T>
TVector<T>::TVector(TVector&& other) noexcept :
    _used(other._used), _deleted(other._deleted),
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
TVector<T>::TVector(pointer array, size_type size) : _used(size), _deleted(0) {
    _capacity = (size / _capacity_step + 1) * _capacity_step * (size > 0);
    _data = new T[_capacity];
    _states = new State[_capacity];

    for (size_type i = 0; i < _used; i++) {
        _data[i] = array[i];
        _states[i] = Busy;
    }

    for (size_type i = _used; i < _capacity; i++) {
        _states[i] = Empty;
    }
}

template<typename T>
TVector<T>::TVector(std::initializer_list<value_type> init) noexcept
    : _used(init.size()), _deleted(0) {
    if (init.size() <= 15)
        _capacity = _capacity_step * (init.size() > 0);
    else
        _capacity = (init.size() / _capacity_step + 1) *
        _capacity_step * (init.size() > 0);

    _data = new T[_capacity];
    _states = new State[_capacity];
    size_t index = 0;

    for (const auto& elem : init) {
        _data[index] = elem;
        _states[index] = Busy;
        index++;
    }

    for (; index < _capacity; index++) {
        _data[index] = T();
        _states[index] = Empty;
    }
}

template<typename T>
TVector<T>::~TVector() noexcept {
    delete[] _data;
    delete[] _states;
}

template<typename T>
inline typename TVector<T>::pointer TVector<T>::data() noexcept {
    return _data;
}

template<typename T>
inline typename TVector<T>::const_pointer TVector<T>::data() const noexcept {
    return _data;
}

template<typename T>
inline typename TVector<T>::size_type TVector<T>::size() const noexcept {
    return _used - _deleted;
}

template<typename T>
inline typename TVector<T>::size_type TVector<T>::capacity() const noexcept {
    return _capacity;
}

template<typename T>
inline typename TVector<T>::reference TVector<T>::front() {
    if (is_empty()) {
        throw std::runtime_error("front() called on empty TVector");
    }

    for (size_t i = 0; i < _used; i++) {
        if (_states[i] == Busy)
            return _data[i];
    }

    throw std::runtime_error("TVector internal state"
                             " corrupted: no busy elements found");
}

template<typename T>
inline typename TVector<T>::reference TVector<T>::back() {
    if (is_empty()) {
        throw std::runtime_error("back() called on empty TVector");
    }

    for (size_t i = _used; i > 0; i--) {
        if (_states[i - 1] == Busy)
            return _data[i - 1];
    }

    throw std::runtime_error("TVector internal state"
                             " corrupted: no busy elements found");
}

template<typename T>
inline typename TVector<T>::Iterator TVector<T>::begin() noexcept {
    if (size() == 0) {
        return Iterator(&_data[0], *this);
    }

    size_t begin_index = 0;

    for (size_t i = 0; i < _used; i++) {
        if (_states[i] == Busy) {
            begin_index = i;
            break;
        }
    }

    return Iterator(&_data[begin_index], *this);
}

template<typename T>
inline typename TVector<T>::Iterator TVector<T>::end() noexcept {
    if (size() == 0) {
        return Iterator(&_data[0], *this);
    }

    size_t end_index = _used;

    for (size_t i = _used; i > 0; i--) {
        if (_states[i - 1] == Busy) {
            end_index = i;
            break;
        }
    }

    return Iterator(&_data[end_index - 1] + 1, *this);
}

template<typename T>
inline typename TVector<T>::ConstIterator TVector<T>::begin() const noexcept {
    if (size() == 0) {
        return ConstIterator(&_data[0], *this);
    }

    size_t begin_index = 0;

    for (size_t i = 0; i < _used; i++) {
        if (_states[i] == Busy) {
            begin_index = i;
            break;
        }
    }

    return ConstIterator(&_data[begin_index], *this);
}

template<typename T>
inline typename TVector<T>::ConstIterator TVector<T>::end() const noexcept {
    if (size() == 0) {
        return ConstIterator(&_data[0], *this);
    }

    size_t end_index = _used;

    for (size_t i = _used; i > 0; i--) {
        if (_states[i - 1] == Busy) {
            end_index = i;
            break;
        }
    }

    return ConstIterator(&_data[end_index - 1] + 1, *this);
}

template<typename T>
void TVector<T>::push_back(const value_type& value) noexcept {
    if (_states != nullptr && _states[_used - 1] == Deleted) {
        _data[_used - 1] = value;
        _states[_used - 1] = Busy;
        _deleted--;
        return;
    }

    if (is_full()) {
        reset_memory(size() + 1);
    }

    _data[_used] = value;
    _states[_used] = Busy;
    _used++;
}

template<typename T>
void TVector<T>::push_back(value_type&& value) noexcept {
    if (_states != nullptr && _states[_used - 1] == Deleted) {
        _data[_used - 1] = std::move(value);
        _states[_used - 1] = Busy;
        _deleted--;
        return;
    }

    if (is_full()) {
        reset_memory(size() + 1);
    }

    _data[_used] = std::move(value);
    _states[_used] = Busy;
    _used++;
}

template<typename T>
void TVector<T>::push_front(const value_type& value) noexcept {
    if (_states != nullptr && _states[0] == Deleted) {
        _data[0] = value;
        _states[0] = Busy;
        _deleted--;
        return;
    }

    if (is_full())
        reset_memory(size() + 1);

    for (size_t i = _used; i > 0; i--) {
        _data[i] = _data[i - 1];
        _states[i] = _states[i - 1];
    }

    _data[0] = value;
    _states[0] = Busy;
    _used++;
}

template<typename T>
void TVector<T>::push_front(value_type&& value) noexcept {
    if (_states != nullptr && _states[0] == Deleted) {
        _data[0] = std::move(value);
        _states[0] = Busy;
        _deleted--;
        return;
    }

    if (is_full())
        reset_memory(size() + 1);

    for (size_t i = _used; i > 0; i--) {
        _data[i] = _data[i - 1];
        _states[i] = _states[i - 1];
    }

    _data[0] = std::move(value);
    _states[0] = Busy;
    _used++;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::insert(Iterator position,
    const value_type& value) noexcept {
    if (!is_full()) {
        size_t insert_index = position.index();

        for (size_t i = _used; i > insert_index; i--) {
            _data[i] = _data[i - 1];
            _states[i] = _states[i - 1];
        }

        _data[insert_index] = value;
        _states[insert_index] = Busy;
        _used++;

        return position;
    }

    Iterator new_position = reset_memory(size() + 1, position);

    size_t insert_index = new_position.index();

    for (size_t i = _used; i > insert_index; i--) {
        _data[i] = _data[i - 1];
        _states[i] = _states[i - 1];
    }

    _data[insert_index] = value;
    _states[insert_index] = Busy;
    _used++;

    return new_position;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::insert(Iterator position,
    size_type n, const value_type& value) noexcept {
    if (_capacity - _used >= n) {
        size_type insert_index = position.index();

        for (size_type i = _used + n - 1; i >= insert_index + n; i--) {
            _data[i] = _data[i - n];
            _states[i] = _states[i - n];
        }

        for (size_type i = insert_index; i < insert_index + n; i++) {
            _data[i] = value;
            _states[i] = Busy;
            _used++;
        }

        return position;
    }

    Iterator new_position = reset_memory(size() + n, position);

    size_t insert_index = new_position.index();

    for (size_t i = _used + n - 1; i >= insert_index + n; i--) {
        _data[i] = _data[i - n];
        _states[i] = _states[i - n];
    }

    for (size_t i = insert_index; i < insert_index + n; i++) {
        _data[i] = value;
        _states[i] = Busy;
        _used++;
    }

    return new_position;
}

template<typename T>
template<class ...Args>
typename TVector<T>::Iterator TVector<T>::emplace(Iterator position,
    Args && ...args) {
    if (!is_full()) {
        size_t insert_index = position.index();

        for (size_t i = _used; i > insert_index; i--) {
            _data[i] = _data[i - 1];
            _states[i] = _states[i - 1];
        }

        _data[insert_index] = T(args ...);
        _states[insert_index] = Busy;
        _used++;

        return position;
    }

    Iterator new_position = reset_memory(size() + 1, position);

    size_t insert_index = new_position.index();

    for (size_t i = _used; i > insert_index; i--) {
        _data[i] = _data[i - 1];
        _states[i] = _states[i - 1];
    }

    _data[insert_index] = T(args ...);
    _states[insert_index] = Busy;
    _used++;

    return new_position;
}

template<typename T>
typename TVector<T>::Iterator
TVector<T>::insert(Iterator position, value_type&& value)
noexcept {
    if (!is_full()) {
        size_t insert_index = position.index();

        for (size_t i = _used; i > insert_index; i--) {
            _data[i] = _data[i - 1];
            _states[i] = _states[i - 1];
        }

        _data[insert_index] = std::move(value);
        _states[insert_index] = Busy;
        _used++;

        return position;
    }

    Iterator new_position = reset_memory(size() + 1, position);

    size_t insert_index = new_position.index();

    for (size_t i = _used; i > insert_index; i--) {
        _data[i] = _data[i - 1];
        _states[i] = _states[i - 1];
    }

    _data[insert_index] = std::move(value);
    _states[insert_index] = Busy;
    _used++;

    return new_position;
}

template<typename T>
void TVector<T>::pop_back() {
    if (_states == nullptr || _data == nullptr)
        throw std::runtime_error("Pop with empty vector");

    size_t remove_index = _used - 1;

    for (size_t i = _used - 1; i > 0; i--) {
        if (_states[i] == Busy) {
            remove_index = i;
            break;
        }
    }

    _states[remove_index] = Deleted;
    _deleted++;

    if (_deleted >= _used * _removal_coefficient) {
        reset_memory_for_delete();
    }
}

template<typename T>
void TVector<T>::pop_front() {
    if (_states == nullptr || _data == nullptr)
        throw std::runtime_error("Pop with empty vector");

    size_t remove_index = 0;

    for (size_t i = 0; i < _used; i++) {
        if (_states[i] == Busy) {
            remove_index = i;
            break;
        }
    }

    _states[remove_index] = Deleted;
    _deleted++;

    if (_deleted >= _used * _removal_coefficient) {
        reset_memory_for_delete();
    }
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::erase(Iterator position) {
    if (_states == nullptr || _data == nullptr)
        throw std::runtime_error("Erase with empty vector");

    size_t deleted_index = position.index();
    _states[deleted_index] = Deleted;
    _deleted++;

    if (_deleted >= _used * _removal_coefficient) {
        reset_memory_for_delete();
    }

    return position;
}

template<typename T>
void TVector<T>::clear() noexcept {
    delete[] _data;
    delete[] _states;
    _capacity = _capacity_step;
    _deleted = 0;
    _used = 0;

    _data = new T[_capacity];
    _states = new State[_capacity];

    for (int i = 0; i < _capacity; i++) {
        _data[i] = T();
        _states[i] = Empty;
    }
}

template<typename T>
void TVector<T>::shrink_to_fit() {
    _capacity = _used;

    T* new_data = new T[_capacity];
    State* new_states = new State[_capacity];

    for (int i = 0; i < _capacity; i++) {
        new_data[i] = _data[i];
        new_states[i] = _states[i];
    }

    delete[] _data;
    delete[] _states;

    _data = new_data;
    _states = new_states;
}

template<typename T>
void TVector<T>::resize(size_type new_size) {
    reset_memory_for_delete();

    if (new_size > _capacity) {
        reset_memory(new_size);
        for (size_type i = _used; i < new_size; i++) {
            _states[i] = Busy;
        }

        _used = new_size;
    } else if (new_size > _used) {
        for (size_type i = _used; i < new_size; i++) {
            _states[i] = Busy;
        }

        _used = new_size;
    } else {
        _used = new_size;
        reset_memory_for_delete();
    }
}

template<typename T>
inline bool TVector<T>::is_empty() const noexcept {
    return (_used - _deleted) == 0;
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
bool TVector<T>::operator==(const TVector<value_type>& other) const noexcept {
    if (size() != other.size())
        return false;

    for (size_type i = 0; i < size(); i++) {
        if ((*this)[i] != other[i])
            return false;
    }

    return true;
}

template<typename T>
bool TVector<T>::operator!=(const TVector<value_type>& other) const noexcept {
    return !(*this == other);
}

template<typename T>
typename TVector<T>::reference TVector<T>::operator[](size_type index) {
    if (index >= _used) {
        throw std::out_of_range("TVector operator[]: Index out of range.");
    }

    if (_deleted == 0) {
        return _data[index];
    }

    size_type current_index = 0;

    for (size_type i = 0; i < _used; i++) {
        if (_states[i] == Busy) {
            if (current_index == index)
                return _data[i];

            current_index++;
        }
    }

    throw std::out_of_range("TVector operator[]: Index out of range.");
}

template<typename T>
typename TVector<T>::const_reference
TVector<T>::operator[](size_type index) const {
    if (index >= _used) {
        throw std::out_of_range("TVector operator[]: Index out of range.");
    }

    if (_deleted == 0) {
        return _data[index];
    }

    size_type current_index = 0;

    for (size_type i = 0; i < _used; i++) {
        if (_states[i] == Busy) {
            if (current_index == index)
                return _data[i];

            current_index++;
        }
    }

    throw std::out_of_range("TVector operator[]: Not found.");
}

template<typename T>
void TVector<T>::reset_memory_for_delete() noexcept {
    size_type correct_size = size();
    size_type new_capacity =
        (correct_size / _capacity_step + 1) * _capacity_step;
    T* new_data = new T[new_capacity];
    State* new_states = new State[new_capacity];
    size_type index = 0;

    for (size_type i = 0; i < _used; i++) {
        if (_states[i] == Busy) {
            new_data[index] = _data[i];
            new_states[index] = Busy;
            index++;
        }
    }

    for (size_type i = index; i < new_capacity; i++) {
        new_data[i] = T();
        new_states[i] = Empty;
    }

    _capacity = new_capacity;
    _deleted = 0;
    _used = correct_size;
    delete[] _data;
    delete[] _states;
    _data = new_data;
    _states = new_states;
}

template<typename T>
void TVector<T>::reset_memory(size_type new_size) noexcept {
    size_type size_diff = new_size - size();
    size_type new_capacity = (new_size / _capacity_step + 1) * _capacity_step;
    T* new_data = new T[new_capacity];
    State* new_states = new State[new_capacity];
    size_type index = 0;

    for (size_type i = 0; i < _used; i++) {
        if (_states[i] == Busy) {
            new_data[index] = _data[i];
            new_states[index] = Busy;
            index++;
        }
    }

    for (size_type i = index; i < new_capacity; i++) {
        new_data[i] = T();
        new_states[i] = Empty;
    }

    _capacity = new_capacity;
    _deleted = 0;
    _used = new_size - size_diff;
    delete[] _data;
    delete[] _states;
    _data = new_data;
    _states = new_states;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::reset_memory(size_type new_size,
    const Iterator& insert_it) noexcept {
    size_type new_insert_index = insert_it.index();
    reset_memory(new_size);

    return Iterator(&_data[new_insert_index], *this);
}

template<typename T>
inline bool TVector<T>::is_full() const noexcept {
    return _used == _capacity;
}

template<typename T>
inline void TVector<T>::swap_elem(size_type first_index, size_type second_index)
noexcept {
    T temp_elem = _data[first_index];
    _data[first_index] = _data[second_index];
    _data[second_index] = temp_elem;

    State temp_state = _states[first_index];
    _states[first_index] = _states[second_index];
    _states[second_index] = temp_state;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const TVector<T>& out) noexcept {
    stream << "size(" << out._used << ") capacity(" <<
        out._capacity << ") deleted(" << out._deleted << ") vector: [ ";

    size_t out_size = out.size() > 1000 ? 1000 : out.size();

    for (size_t i = 0; i < out_size; i++) {
        stream << out[i] << " ";
    }

    stream << " ]";

    return stream;
}

template<typename U>
void shuffle(TVector<U>& vec) noexcept {
    std::srand(std::time(0));

    for (size_t i = vec._used - 1; i > 0; --i) {
        size_t j = std::rand() % (i + 1);
        vec.swap_elem(i, j);
    }
}

template<typename U>
void quick_sort(TVector<U>& vec, size_t low, size_t high, bool(*comp)(U, U))
noexcept {
    if (low < high) {
        size_t pivot_index = partition(vec, low, high, comp);

        if (pivot_index > low) {
            quick_sort(vec, low, pivot_index - 1, comp);
        }

        if (pivot_index < high) {
            quick_sort(vec, pivot_index + 1, high, comp);
        }
    }
}

template<typename U>
size_t partition(TVector<U>& vec, size_t low, size_t high, bool(*comp)(U, U))
noexcept {
    size_t pivot_index = low + (high - low) / 2;
    U pivot = vec._data[pivot_index];
    vec.swap_elem(pivot_index, high);

    size_t i = low - 1;

    for (size_t j = low; j < high; j++) {
        if (comp(vec._data[j], pivot)) {
            i++;
            vec.swap_elem(i, j);
        }
    }

    vec.swap_elem(i + 1, high);

    return i + 1;
}

template<typename U>
inline void tv_sort(TVector<U>& vec, bool(*comp)(U, U)) noexcept {
    quick_sort(vec, 0, vec._used - 1, comp);
}

template<typename U>
int* search_all(TVector<U>& vec, bool(*check)(U)) noexcept {
    int* search_result = new int[vec.size()];
    int deleted_count = 0;
    int index = 0;

    for (int i = 0; i < vec._used; i++) {
        if (vec._states[i] == Busy) {
            if (check(vec._data[i])) {
                search_result[index] = i - deleted_count;
                index++;
            }
        } else {
            deleted_count++;
        }
    }

    for (int i = index; i < vec.size(); i++) {
        search_result[i] = -1;
    }

    return search_result;
}

template<typename U>
int search_begin(TVector<U>& vec, bool(*check)(U)) noexcept {
    int deleted_count = 0;

    for (int i = 0; i < vec._used; i++) {
        if (vec._states[i] == Busy) {
            if (check(vec._data[i]))
                return i - deleted_count;
        } else {
            deleted_count++;
        }
    }

    return -1;
}

template<typename U>
int search_end(TVector<U>& vec, bool(*check)(U)) noexcept {
    int deleted_count = 0;

    for (int i = vec._used - 1; i > 0; i--) {
        if (vec._states[i] == Busy) {
            if (check(vec._data[i]))
                return i - (vec._deleted - deleted_count);
        }
    }

    return -1;
}
#pragma endregion TVectorRealization

#pragma region IteratorsRealization
template<typename T>
TVector<T>::Iterator::Iterator(T* ptr, TVector<T>& parent) noexcept
    : _ptr(ptr), _parent(parent) {}

template<typename T>
TVector<T>::Iterator::Iterator(const Iterator& other) noexcept
    : _ptr(other._ptr), _parent(other._parent) {}

template <typename T>
inline typename TVector<T>::Iterator::reference
TVector<T>::Iterator::operator*() {
    if (_ptr == nullptr) {
        throw std::out_of_range("Iterator operator*: Nullptr.");
    }

    if (_ptr < _parent._data || _ptr >= _parent._data + _parent._used) {
        throw std::out_of_range("Iterator operator*: Index out of range.");
    }

    return *_ptr;
}

template<typename T>
inline typename TVector<T>::Iterator::pointer
TVector<T>::Iterator::operator->() noexcept {
    return _ptr;
}

template<typename T>
inline typename TVector<T>::Iterator&
TVector<T>::Iterator::operator=(const Iterator& other) noexcept {
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
inline typename TVector<T>::Iterator TVector<T>::Iterator::operator++(int)
noexcept {
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
inline typename TVector<T>::Iterator TVector<T>::Iterator::operator--(int)
noexcept {
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
inline bool TVector<T>::Iterator::operator!=(const Iterator& other)
const noexcept {
    return _ptr != other._ptr || _parent != other._parent;
}

template<typename T>
inline bool TVector<T>::Iterator::operator==(const Iterator& other)
const noexcept {
    return _ptr == other._ptr && _parent == other._parent;
}

template<typename T>
typename TVector<T>::Iterator::difference_type
TVector<T>::Iterator::operator-(const Iterator& other) const {
    if (&_parent != &other._parent)
        throw std::runtime_error("Iterator operator-: Different parents");

    int reverse = _ptr < other._ptr ? -1 : 1;
    Iterator left = _ptr < other._ptr ? *this : other;
    Iterator right = _ptr > other._ptr ? *this : other;

    ptrdiff_t distance = 0;
    T* current = left._ptr;

    while (current != right._ptr) {
        if (_parent._states[current - _parent._data] != Deleted) {
            distance++;
        }

        current++;
    }

    return distance * reverse;
}

template<typename T>
inline typename TVector<T>::Iterator::difference_type
TVector<T>::Iterator::index() const noexcept {
    return _ptr - _parent._data;
}

template<typename T>
bool TVector<T>::Iterator::operator<(const Iterator& other) const noexcept {
    return _ptr < other._ptr;
}

template<typename T>
bool TVector<T>::Iterator::operator>(const Iterator& other) const noexcept {
    return _ptr > other._ptr;
}

template<typename T>
bool TVector<T>::Iterator::operator<=(const Iterator& other) const noexcept {
    return _ptr <= other._ptr;
}

template<typename T>
bool TVector<T>::Iterator::operator>=(const Iterator& other) const noexcept {
    return _ptr >= other._ptr;
}

template<typename T>
typename TVector<T>::Iterator::reference
TVector<T>::Iterator::operator[](difference_type n) {
    if (n < 0) {
        throw std::out_of_range("Negative index not allowed");
    }

    Iterator tmp = *this;
    difference_type count = 0;

    while (tmp != _parent.end()) {
        if (_parent._states[tmp.index()] == Busy) {
            if (count == n) {
                return *tmp;
            }
            count++;
        }
        ++tmp;
    }

    throw std::runtime_error("Element not found");
}
#pragma endregion

#pragma region ConstIteratorRealisation

template<typename T>
TVector<T>::ConstIterator::ConstIterator(const T* ptr, const TVector<T>& parent) noexcept
    : _ptr(ptr), _parent(parent) {}

template<typename T>
TVector<T>::ConstIterator::ConstIterator(const ConstIterator& other) noexcept
    : _ptr(other._ptr), _parent(other._parent) {}

template <typename T>
inline typename TVector<T>::ConstIterator::reference
TVector<T>::ConstIterator::operator*() {
    if (_ptr == nullptr) {
        throw std::out_of_range("ConstIterator operator*: Nullptr.");
    }

    if (_ptr < _parent._data || _ptr >= _parent._data + _parent._used) {
        throw std::out_of_range("ConstIterator operator*: Index out of range.");
    }

    return *_ptr;
}

template<typename T>
inline typename TVector<T>::ConstIterator::pointer
TVector<T>::ConstIterator::operator->() noexcept {
    return _ptr;
}

template<typename T>
inline typename TVector<T>::ConstIterator&
TVector<T>::ConstIterator::operator=(const ConstIterator& other) noexcept {
    if (this != &other) {
        _ptr = other._ptr;
        _parent = other._parent;
    }

    return *this;
}

template<typename T>
typename TVector<T>::ConstIterator&
    TVector<T>::ConstIterator::operator++() noexcept {
    difference_type current_index = _ptr - _parent._data;

    for (difference_type i = current_index + 1; i <= _parent._used; i++) {
        if (_parent._states[i] != Deleted) {
            _ptr = &_parent._data[i];
            break;
        }
    }

    return *this;
}

template<typename T>
inline typename TVector<T>::ConstIterator
TVector<T>::ConstIterator::operator++(int) noexcept {
    ConstIterator temp = *this;
    ++(*this);

    return temp;
}

template<typename T>
typename TVector<T>::ConstIterator&
    TVector<T>::ConstIterator::operator--() noexcept {
    difference_type current_index = _ptr - _parent._data;

    for (difference_type i = current_index - 1; i >= 0; i--) {
        if (_parent._states[i] != Deleted) {
            _ptr = &_parent._data[i];
            break;
        }
    }

    return *this;
}

template<typename T>
inline typename TVector<T>::ConstIterator
TVector<T>::ConstIterator::operator--(int) noexcept {
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

template<typename T>
typename TVector<T>::ConstIterator
TVector<T>::ConstIterator::operator+(int num) const {
    int new_index = _ptr - _parent._data;

    if (new_index + num > _parent._used || new_index + num < 0) {
        throw std::out_of_range("ConstIterator operator+: Index out of range.");
    }

    while (new_index < _parent._used && num > 0) {
        new_index++;

        if (_parent._states[new_index] != Deleted) {
            num--;
        }
    }

    return ConstIterator(&_parent._data[new_index], _parent);
}

template<typename T>
typename TVector<T>::ConstIterator
TVector<T>::ConstIterator::operator-(int num) const {
    int new_index = _ptr - _parent._data;

    if (new_index - num > _parent._used || new_index - num < 0) {
        throw std::out_of_range("ConstIterator operator-: Index out of range.");
    }

    while (new_index > 0 && num > 0) {
        new_index--;

        if (_parent._states[new_index] != Deleted) {
            num--;
        }
    }

    return ConstIterator(&_parent._data[new_index], _parent);
}

template<typename T>
typename TVector<T>::ConstIterator&
    TVector<T>::ConstIterator::operator+=(int num) {
    int new_index = _ptr - _parent._data;

    if (new_index + num > _parent._used || new_index + num < 0) {
        throw std::out_of_range("ConstIterator operator+: Index out of range.");
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
typename TVector<T>::ConstIterator&
    TVector<T>::ConstIterator::operator-=(int num) {
    int new_index = _ptr - _parent._data;

    if (new_index - num > _parent._used || new_index - num < 0) {
        throw std::out_of_range("ConstIterator operator-: Index out of range.");
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
inline bool TVector<T>::ConstIterator::operator!=(const ConstIterator& other)
const noexcept {
    return _ptr != other._ptr || _parent != other._parent;
}

template<typename T>
inline bool TVector<T>::ConstIterator::operator==(const ConstIterator& other)
const noexcept {
    return _ptr == other._ptr && _parent == other._parent;
}

template<typename T>
typename TVector<T>::ConstIterator::difference_type
TVector<T>::ConstIterator::operator-(const ConstIterator& other) const {
    if (&_parent != &other._parent)
        throw std::runtime_error("ConstIterator operator-: Different parents");

    int reverse = _ptr < other._ptr ? -1 : 1;
    ConstIterator left = _ptr < other._ptr ? *this : other;
    ConstIterator right = _ptr > other._ptr ? *this : other;

    difference_type distance = 0;
    pointer current = left._ptr;

    while (current != right._ptr) {
        if (_parent._states[current - _parent._data] != Deleted) {
            distance++;
        }

        current++;
    }

    return distance * reverse;
}

template<typename T>
inline typename TVector<T>::ConstIterator::difference_type
TVector<T>::ConstIterator::index() const noexcept {
    return _ptr - _parent._data;
}

template<typename T>
bool TVector<T>::ConstIterator::operator<(const ConstIterator& other)
const noexcept {
    return _ptr < other._ptr;
}

template<typename T>
bool TVector<T>::ConstIterator::operator>(const ConstIterator& other)
const noexcept {
    return _ptr > other._ptr;
}

template<typename T>
bool TVector<T>::ConstIterator::operator<=(const ConstIterator& other)
const noexcept {
    return _ptr <= other._ptr;
}

template<typename T>
bool TVector<T>::ConstIterator::operator>=(const ConstIterator& other)
const noexcept {
    return _ptr >= other._ptr;
}

template<typename T>
typename TVector<T>::ConstIterator::reference
TVector<T>::ConstIterator::operator[](difference_type n) {
    if (n < 0) {
        throw std::out_of_range("Negative index not allowed");
    }

    ConstIterator tmp = *this;
    difference_type count = 0;

    while (tmp != _parent.end()) {
        if (_parent._states[tmp.index()] == Busy) {
            if (count == n) {
                return *tmp;
            }
            count++;
        }

        ++tmp;
    }

    throw std::runtime_error("Element not found");
}

#pragma endregion ConstIteratorRealisation
