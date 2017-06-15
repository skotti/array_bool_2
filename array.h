#include <iostream>
#include <initializer_list>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <typeinfo>

#include <new>
#include "array_utils.h"
/*
class array_exception : public std::exception {
public:
	array_exception(std::string m) : msg(m){}
	~array_exception(){}
	//const char* what() {return msg.c_str();}

private:
	std::string msg;
};



#ifdef NDEBUG
#define DEBUG_INFO(message, func)
#define DEBUG_PRINT(message)
#else 
#define DEBUG_INFO(message, func) std::cout<<"LINE : "<<__LINE__<<std::endl<<message<<std::endl; func
#define DEBUG_PRINT(message) std::cout<<message<<std::endl<<std::endl<<std::endl;
#endif

#ifdef NDEBUG
#define ASSERT(cond, exp) if (!(cond)) {\
                          std::cout<<"ASSERTION FAILED"<<std::endl; \
                          std::cout<<"FILE : "<<__FILE__<<std::endl;\
                          std::cout<<"LINE : "<<__LINE__<<std::endl;\
                          std::cout<<"MESSAGE : "<<(exp)<<std::endl;\
                          abort();}
 #else
 #define ASSERT(cond, exp)
 #endif

 #ifdef NDEBUG
 #define SOFT_ASSERT(cond, exp)
 #else
 #define SOFT_ASSERT(cond, exp)
 #endif                         
*/
template<typename T>
class Array{
private:
	typedef T 								value_type;
	typedef size_t 							size_type;
	typedef std::ptrdiff_t 					difference_type;
	typedef value_type& 					reference;
	typedef const value_type&				const_reference;
	typedef value_type* 					pointer;
	typedef const value_type*				const_pointer;
	typedef value_type*						iterator;
	typedef const value_type*				const_iterator;
	typedef value_type*						input_iterator;
	typedef value_type*						reverse_iterator;

	int m_size;
	int m_capacity = 10; 
	value_type* m_data;
public:

	iterator begin() {
		DEBUG_INFO("Begin iterator", dump();)
		return m_data;
	}

	iterator end() {
		DEBUG_INFO("End iterator", dump();)
		return (m_data + m_size);
	}

	reverse_iterator rend() {
		DEBUG_INFO("Reverse end iterator", dump();)
		std::reverse_iterator<pointer> it(m_data + m_size);
		return it;
	}

	reverse_iterator rbegin() {
		DEBUG_INFO("Reverse begin iterator", dump();)
		std::reverse_iterator<pointer> it(m_data + m_size - 1);
		return it;
	}

	Array():m_size(0),
			m_data (static_cast<value_type*>(operator new[] (m_capacity * sizeof(value_type)))) 
	{
		DEBUG_INFO("Default constructor", dump();)
	}

	Array(int capacity) {
		ASSERT((m_capacity > 0), "Capacity is less than zero")
		try {
			m_data = static_cast<value_type*>(operator new[] (m_capacity * sizeof(value_type)));
			m_size = 0;
			m_capacity = capacity;
		} catch (std::exception& e) {
			throw array_exception("Bad allocation in capacity constructor");
		}

		DEBUG_INFO("Capacity constructor", dump();)
	}

	Array(const_pointer data, int size) {
		ASSERT((size > 0), "Size is less than zero")
		ASSERT((data != NULL), "Array in parameters is not initialized")

		m_data = static_cast<value_type*>(operator new[] (size * sizeof(value_type)));

		m_size = size;
		m_capacity = m_size;

		std::size_t cur = 0;
		try {
    		for (std::size_t i = 0; i < size; ++i, ++cur)
        	 new (m_data + i) value_type(data[i]);
		}
		catch (std::exception& e) {
    		for (std::size_t i = 0; i != cur; ++i)
        	(m_data+ i)->~value_type();
    		throw array_exception("Bad allocation in pointer constructor");
		}
		DEBUG_INFO("Pointer constructor", dump();)
	}

	Array(std::initializer_list<value_type> list) {
		ASSERT((list.size() >0), "List is empty")
		
		m_data = static_cast<value_type*>(operator new[] (list.size() * sizeof(value_type)));
		m_size = list.size();
		m_capacity = m_size;

		int count = 0;
		try {
			for (int i = 0; i < list.size(); i++) {
				new (m_data + i) value_type(list[i]);
				count += 1;
			}
		} catch(std::exception& e) {
			for (int i = 0; i < count; i++) {
				m_data[i].~value_type();
			}
			throw array_exception("Bad allocation in list constructor");
		}
		
		DEBUG_INFO("List constructor", dump();)
	}

	Array(Array&& data) :
	m_size(std::move(data.m_size)),
	m_capacity(std::move(data.m_capacity)),
	m_data (std::move(data.m_data)){
	  DEBUG_INFO("Move constructor", data.dump();)
	
	  data.m_data = nullptr;
	
	  DEBUG_INFO("Move constructor (after moving)", dump();)
	}

	Array(const Array<value_type>& data) :
	m_size(data.m_size),
	m_capacity(data.m_capacity) {
		DEBUG_INFO("Copy constructor", data.dump();)
		
		m_data = static_cast<value_type*>(operator new[] (m_size * sizeof(value_type)));

		int count = 0;
		try {
			for (int i = 0; i < m_size; i++) {
				new (m_data + i) value_type(data[i]);
				count += 1;
			}
		} catch(std::exception& e) {
			for (int i = 0; i < count; i++) {
				m_data[i].~value_type();
			}
			throw array_exception("Bad allocation in copy constructor");
		}

		DEBUG_INFO("Copy constructor (after construction)", dump();)
	}

	reference at(int index) {
		DEBUG_INFO("At operator", dump();)
		if ( (index > m_size) || (index < 0)) 
			throw std::out_of_range("Out of size range");
		return m_data[index];
	}

	const_reference at(int index) const {
		DEBUG_INFO("Const at operator", dump();)
    	if ( (index > m_size) || (index < 0)) 
        	throw std::out_of_range("Out of size range");
        return m_data[index];
	}

	reference operator[] (int index) {
		DEBUG_INFO("Operator[]", dump();)
		return m_data[index];
	}

	const_reference operator[] (int index) const{
		DEBUG_INFO("Const operator[]", dump();)
		return m_data[index];
	}

	pointer data() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function data()", dump();)
		return m_data;
	}

	const_pointer data() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function data()", dump();)
		return m_data;
	}
	
	reference front() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function front()", dump();)
		return m_data[0];
	}
	const_reference front() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function front()", dump();)
		return m_data[0];
	}

	reference back() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function back()", dump();)
		return m_data[m_size - 1];
	}
	const_reference back() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function back()", dump();)
		return m_data[m_size - 1];
	}

	int size() {
		DEBUG_INFO("Function size()", dump();)
		return m_size;
	}

	int max_size() {
		DEBUG_INFO("Function max_size()", dump();)
		return m_capacity;
	}

	void reserve(int size) {
		DEBUG_INFO("Function reserve()", dump();)
		ASSERT(ok(), "Cannot reserve, array is broken")
		if (size > max_size())
			throw std::length_error("Size is greater than max size");
		if (size > m_size) {
			resize(size);
		}
		DEBUG_INFO("Function reserve() (after reserving)", dump();)
		ASSERT(ok(), "Array is broken after resizing");
	}


	void push_back(const_reference elem) {
		DEBUG_INFO("Function push_back()", dump();)
		ASSERT(ok(), "Cannot push, array is broken")

		if ((m_size + 1) > m_capacity)
			resize(2 * m_size);
		new (m_data + m_size) value_type(elem);
		m_size+=1;

		DEBUG_INFO("Function push_back() ( after pushing)", dump();)
		ASSERT(ok(), "Array is broken after push()")
	}

	void pop_back() {
		DEBUG_INFO("Function pop_back()", dump();)
		ASSERT((m_size > 0), "size is less than zero, cannot pop")
		m_data[m_size - 1].~value_type();
		m_size -=1;
	}

	const Array<value_type>& operator=(const Array<value_type>& data) {
		ASSERT(data.ok(), "Invalid input data")

		if (this == &data) { 
			DEBUG_INFO("Operator = (eqaulity was true)", dump();) 
			return *this;
		}
		if (m_size == 0)
			m_data = static_cast<value_type*>(operator new[] (data.m_size * sizeof(value_type)));
		else if (m_size < data.m_size) {
			resize(data.m_size);
		}
		auto a(data);
		a.swap(*this);

		DEBUG_INFO("Operator = (after =)", dump();)
		ASSERT(ok(), "Array is broken after operator = ")
		return *this;
	}

	Array<value_type>& operator=(Array<value_type>&& data) {
		DEBUG_INFO("Move operator =", dump();)
		ASSERT(data.ok(), "Passing array is broken, cannot move")

		m_size (std::move(data.m_size));
		m_capacity (std::move(data.m_capacity));
		m_data (std::move(data.m_data));
		data.m_data = nullptr;

		DEBUG_INFO("Move operator = (after move)", dump();)
		ASSERT(ok(), "Array is broken after move =")
	}

	void swap(Array<value_type>& data) noexcept{
		DEBUG_INFO("Function swap() ", dump();)

		std::swap(data.m_data, this->m_data);
		std::swap(data.m_size, this->m_size);
		std::swap(data.m_capacity, this->m_capacity);
		
		DEBUG_INFO("Function swap() (after swap)", dump();)
	}

	void raw_resize(int size) {
		DEBUG_INFO("Function raw_resize() ", dump();)
		delete[] m_data;
		m_data = static_cast<value_type*>(operator new[] (size * sizeof(value_type)));
		m_capacity = size;
		DEBUG_INFO("Function raw_resize() (after resizing) ", dump();)
	}

	int findByIterator(const_iterator it) {
		DEBUG_INFO("Function find position by iterator ", dump();)
		int i = 0;
		while (m_data[i] != *it) {
				i+=1;
			}
		return i;
		DEBUG_INFO("Function find position by iterator (have found)", dump();)
	}

	template <typename ...Args>
	pointer emplace(int pos, Args&&... args) {
		DEBUG_INFO("Function emplace()", dump();)

		if ((m_size + 1) > m_capacity) {
			auto tmp(*this);
			raw_resize(2 * m_size);
			for (int i = 0; i < pos ; i++) {
				new (m_data + i) value_type(tmp[i]);
			}
			m_data[pos] = *(new value_type(std::forward<value_type>(args)...));
			int i = pos + 1;
			while (i != (m_size + 1)) {
				new (m_data + i) value_type(tmp[i - 1]);
				i+=1;
			}
		} else {
			auto tmp(*this);
			m_data[pos] = *(new value_type(std::forward<value_type>(args)...));
			std::copy(&tmp.m_data[pos], &tmp.m_data[m_size],&m_data[pos + 1]);
		}
		m_size+=1;

		DEBUG_INFO("Function emplace() (after emplace)", dump();)
		ASSERT(ok(), "Array is broken after emplace")
		return &m_data[pos];
	}
			
	void resize(int size) {
		DEBUG_INFO("Function resize()", dump();)
		ASSERT(ok(), "Array is broken, cannot be resized")

		auto tmp(*this);
		delete[] m_data;
		m_data = static_cast<value_type*>(operator new[] (size * sizeof(value_type)));
		m_capacity = size;
		int count = 0;
		try {
			for (int i = 0; i < m_size; i++) {
				new (m_data + i) value_type(tmp.m_data[i]);
				count += 1;
			}
		} catch(std::exception& e) {
			for (int i = 0; i < count; i++) {
				m_data[i].~value_type();
			}
			throw array_exception("Bad allocation in resize, cannot resize");
		}

		DEBUG_INFO("Function resize() (after resize)", dump();)
		ASSERT(ok(), "Array is broken after resizing")
	}

	Array<value_type>& operator+=(const Array<value_type>& other) {
		DEBUG_INFO("Operator += ", dump();)
		ASSERT(ok(), "Current array is broken, cannot add")
		ASSERT(other.ok(), "Another array is broken, cannot add")
		
		auto size = std::min(m_size, other.m_size);
		for (int i = 0; i < size; i++) {m_data[i] += other.m_data[i];}
		
		DEBUG_INFO("Operator +=(after +=) ", dump();)
		ASSERT(ok(), "Current array is broken after add")
		ASSERT(other.ok(), "Another array is broken after add")
		
		return *this;
	}

	Array<value_type>& operator-=(Array<value_type>& other) {
		DEBUG_INFO("Operator -= ", dump();)
		ASSERT(ok(), "Current array is broken, cannot sub")
		ASSERT(other.ok(), "Another array is broken, cannot sub")
		
		auto size = std::min(m_size, other.m_size);
		for (int i = 0; i < size; i++) {m_data[i] -= other.m_data[i];}
		
		DEBUG_INFO("Operator -=(after -=)", dump();)
		ASSERT(ok(), "Current array is broken, cannot sub")
		ASSERT(other.ok(), "Another array is broken, cannot sub")
		
		return *this;
	}

	void clear() {
		DEBUG_INFO("Function clear() ", dump();)
		ASSERT(ok(), "Current array is broken, cannot clear")

		delete[] m_data;
		m_size = 0;
		m_data = static_cast<value_type*>(operator new[] (m_capacity * sizeof(value_type)));

		DEBUG_INFO("Function clear() (after clearing)", dump();)
		ASSERT(ok(), "Array is broken after clear")
	}


	iterator insert(const_iterator it, const_reference elem) {
		DEBUG_INFO("Function insert() ", dump();)
		ASSERT(std::addressof(it) <= std::addressof(m_data + m_size), "Invalid position for insertion")

		if ((m_size + 1) > m_capacity) {
			DEBUG_INFO("Insert function(): resize was nedded ", dump();)
			auto tmp(*this);
			raw_resize(2 * m_size);
			int i = 0;
			while (tmp[i] != *it) {
				new (m_data + i) value_type(tmp[i]);
				i+=1;
			}
			new (m_data + i) value_type(elem); i+=1;
			while (i != (m_size + 1)) {
				new (m_data + i) value_type(tmp[i - 1]);
				i+=1;
			}
		} else {
			//оставлю уж копирование, выделение новой памяти ничем не лучше.
			DEBUG_INFO("Insert function(): inserting into current array ", dump();)
			auto tmp(*this);
			int i = findByIterator(it);
			m_data[i] = elem;
			std::copy(&tmp.m_data[i], &tmp.m_data[m_size],&m_data[i + 1]);
		}
		m_size+=1;

		DEBUG_INFO("Function insert() (after insertion)", dump();)
		return begin();
	}

	void dump() const{
		if (ok()) {std::cout<<"ARRAY IS IN OK STATE. PRINTING INFO ..."<<std::endl;}
		else { 
			std::cout<<" ARRAY IS BROKEN. PRINTING INFO..."<<std::endl;
			std::cout<<"Address of broken array : "<<std::addressof(m_data)<<std::endl;
		};
		std::cout<<"ARRAY SIZE : "<<m_size<<std::endl;
		std::cout<<"ARRAY CAPACITY : "<<m_capacity<<std::endl;
		std::cout<<"DATA TYPE : "<<typeid(T).name()<<std::endl;
		std::cout<<"ARRAY ELEMENTS : "<<std::endl;
		std::cout<<"-----------------------------------------"<<std::endl;
		for (int i = 0; i < m_size ; i++)
			std::cout <<"Element ["<<i<<"] : "<<m_data[i]<<std::endl;
		std::cout<<std::endl;
		std::cout<<"-----------------------------------------"<<std::endl;
		std::cout<<std::endl;
	}

	bool ok() const{
		return (m_data!= NULL) && (m_size >= 0) && (m_capacity >= 0) && (m_size <= m_capacity);
	}

	~Array() {
		DEBUG_PRINT("DESTRUCTOR is called")
		m_data = 0;
		m_capacity = 0;
		operator delete[] (m_data);
		m_data = nullptr;
	}
};

template <typename T>
Array<T> operator+(Array<T> a1, Array<T>& a2) {
	DEBUG_INFO("Operator + with move", a1.dump(); a2.dump();)
	std::cout<<"After copy construction"<<std::endl;
	a1+=a2;
	DEBUG_INFO("Operator + with move (after add)", a1.dump(); a2.dump();)
	return a1;
}

template <typename T>
Array<T> operator-(Array<T> a1, Array<T>& a2) {
	DEBUG_INFO("Operator + with move", a1.dump(); a2.dump();)
	std::cout<<"After copy construction"<<std::endl;
	a1-=a2;
	DEBUG_INFO("Operator + with move (after add)", a1.dump(); a2.dump();)
	return a1;
}
/*
#include <stdio.h>
#include <math.h>


template<>
class Array<bool>{
private:
	typedef bool              value_type;
	typedef size_t            size_type;
	typedef std::ptrdiff_t    difference_type;
	typedef bool              const_reference;
	typedef value_type*       pointer;
	typedef const value_type* const_pointer;

	int m_size;
	int m_capacity = 8; 
	char* m_data;

public:
	class reference {
	private:
		int m_pos;
		char* m_data;
	public:
		reference() {
			m_pos = 0;
		}

		reference(char* data, int pos) {
			m_data = data;
			m_pos = pos;
		}

		reference(const reference& ref) {
			m_data = ref.m_data;
			m_pos = ref.m_pos;
		}

		~reference() {
			m_data = nullptr;
			m_pos = 0;
		}

		reference& operator=(const reference& ref) {
		 
			if (bool(ref))
				m_data[(m_pos/8)] |= 1<<(m_pos%8);
			else 
				m_data[(m_pos/8)] &= ~(1 << (m_pos%8));
			return *this;
		}

		reference& operator=( bool x ) {
			if (x)
				m_data[(m_pos/8)] |= 1<<(m_pos%8);
			else 
				m_data[(m_pos/8)] &= ~(1 << (m_pos%8));
			return *this;
		}

		operator bool() const {
			bool tmp = ((m_data[m_pos/8]>>(m_pos%8)) & 1) ? 1 : 0;
			return tmp;
		}

		void setData(char* data) {
			m_data = data;
		}

		void setPos(int pos) {
			m_pos = pos;
		}

	};

	class const_iterator;

	class iterator : public std::iterator<std::random_access_iterator_tag, bool>{

		char* m_data;
		int m_offset;
		friend class Array::const_iterator;
		
	public:
		iterator(char* data, int offset) :m_data(data), m_offset(offset) {}

		Array::reference operator* () const {
			return Array::reference(m_data, m_offset);
		}

		iterator& operator++ () {
			++m_offset;
			return *this;
		}

		iterator operator++ (int) {
			iterator tmp = *this;
			++m_offset;
			return tmp;
		}

		iterator& operator-- () {
			--m_offset;
			return *this;
		}

		iterator operator-- (int) {
			iterator tmp = *this;
			--m_offset;
			return tmp;
		}

		Array::reference operator[](int index) const {
			return Array::reference(m_data, index);
		}

		iterator& operator+=(difference_type i) {
			m_offset+=i;
			return *this;
		}

		iterator& operator-=(difference_type i) {
			*this += (-i);
			return *this;
		}

		iterator operator+(difference_type i) const{
			iterator tmp = *this;
			return tmp += i;
		}

		iterator operator-(difference_type i) const{
			iterator tmp = *this;
			return tmp -= i;
		}

		bool operator!=(const iterator& it) {
			return (m_offset != it.m_offset);
		}
		
		std::ptrdiff_t operator-(iterator it) const {
		  return (m_offset - it.m_offset);
		}

	};

	class const_iterator : public std::iterator<std::random_access_iterator_tag, bool>{

		char* m_data;
		int m_offset;
		
	public:
		const_iterator(char* data, int offset) :m_data(data), m_offset(offset) {}

		const_iterator(Array::iterator it) : m_data(it.m_data), m_offset(it.m_offset){}

		Array::const_reference operator* () const {
			return Array::reference(m_data, m_offset);
		}

		const_iterator& operator++ () {
			++m_offset;
			return *this;
		}

		const_iterator operator++ (int) {
			const_iterator tmp = *this;
			++m_offset;
			return tmp;
		}

		const_iterator& operator-- () {
			--m_offset;
			return *this;
		}

		const_iterator operator-- (int) {
			const_iterator tmp = *this;
			--m_offset;
			return tmp;
		}

		Array::const_reference operator[](int index) const {
			return Array::reference(m_data, index);
		}

		const_iterator& operator+=(difference_type i) {
			m_offset+=i;
			return *this;
		}

		const_iterator& operator-=(difference_type i) {
			*this += (-i);
			return *this;
		}

		const_iterator operator+(difference_type i) const{
			const_iterator tmp = *this;
			return tmp += i;
		}

		const_iterator operator-(difference_type i) const{
			const_iterator tmp = *this;
			return tmp -= i;
		}

		int getPos() {
			return m_offset;
		}
		
		std::ptrdiff_t operator-(const_iterator it) const {
		  return (m_offset - it.m_offset);
		}
	};

	
	reference operator[](int pos) {
		return reference(m_data, pos);
	}

	const_reference operator[](int pos) const{
		return reference(m_data, pos);
	}
	
	reference at(int index) {
		DEBUG_INFO("At operator", dump();)
		if ( (index > m_size) || (index < 0) || (index == 0 && m_size ==0)) 
			throw std::out_of_range("Out of size range");
		return reference(m_data, index);
	}

	const_reference at(int index) const {
		DEBUG_INFO("Const at operator", dump();)
		if ( (index > m_size) || (index < 0)) 
			throw std::out_of_range("Out of size range");
		return reference(m_data, index);
	}

	
	iterator begin() noexcept{
	  return iterator(m_data, 0);
	}
	
	const_iterator begin() const{
	  return const_iterator(m_data, 0);
	}
	
	const_iterator cbegin() const{
	  return const_iterator(m_data, 0);
	}
	
	iterator end() noexcept{
	  return iterator(m_data, m_size);
	}
	
	const_iterator end() const {
	  return const_iterator(m_data, m_size);
	}
	  
	const_iterator cend() const {
	  return const_iterator(m_data, m_size);
	}

	Array():m_size(0), m_capacity(8) 
	{
	  size_t size = ceil(m_capacity/8.0);
	  m_data = static_cast<char*>(operator new[] (size));
	}

	Array(int capacity) {
		ASSERT((m_capacity > 0), "Capacity is less than zero")
		try {
			size_t size = ceil(m_capacity/8.0);
			m_data = static_cast<char*>(operator new[] (size, 0));
			m_size = 0;
			m_capacity = capacity;

		} catch (std::exception& e) {
			throw array_exception("Bad allocation in capacity constructor");
		}

		DEBUG_INFO("Capacity constructor", dump();)
	}

	Array(const bool* data, int size) {
		ASSERT((size > 0), "Size is less than zero")
		ASSERT((data != NULL), "Array in parameters is not initialized")

		size_t cur_size = ceil(size/8.0);
		m_data = static_cast<char*>(operator new[] (cur_size, 0));

		m_size = size;
		m_capacity = m_size;
		for (int i = 0; i < size; i++) {
			(*this)[i] = data[i];
		}
		DEBUG_INFO("Pointer constructor ", dump();)
	}
	
	Array(std::initializer_list<value_type> list) {
		ASSERT((size > 0), "Size is less than zero")
		ASSERT((data != NULL), "Array in parameters is not initialized")

		size_t cur_size = ceil(list.size()/8.0);
		m_data = static_cast<char*>(operator new[] (cur_size, 0));

		m_size = list.size();
		m_capacity = m_size;
		int i = 0;
		for (auto object : list) {
			(*this)[i] = object;
			i+=1;
		}
		DEBUG_INFO("Pointer constructor ", dump();)
	}
	
	Array(const Array<value_type>& data) :
	m_size(data.m_size),
	m_capacity(data.m_capacity) {
		DEBUG_INFO("Copy constructor", data.dump();)
		
		int cur_size = ceil(data.m_capacity/8.0);
		m_data = static_cast<char*>(operator new[] (cur_size, 0));

		m_size = data.m_size;
		m_capacity = data.m_capacity;
		for (int i = 0; i < m_size; i++) {
			(*this)[i] = data[i];
		}

		DEBUG_INFO("Copy constructor (after construction)", dump();)
	}
	
	Array(Array&& data) :
	m_size(std::move(data.m_size)),
	m_capacity(std::move(data.m_capacity)),
	m_data (std::move(data.m_data)){
		DEBUG_INFO("Move constructor ", dump();)
		data.m_size = 0;
		data.m_capacity = 0;
		data.m_data = nullptr;
		DEBUG_INFO("Move constructor (after construction)", dump();)
	}
	
	const Array<value_type>& operator=(const Array<value_type>& data) {
		ASSERT(data.ok(), "Invalid input data")
		DEBUG_INFO("Operator= ", dump();)

		if (this == &data) { 
			DEBUG_INFO("Operator = (eqaulity was true)", dump();) 
			return *this;
		}
		if (m_size == 0) {
			int cur_size = ceil(data.m_size/8.0);
			m_data = static_cast<char*>(operator new[] (cur_size, 0));
		}
		else if (m_size < data.m_size) {
			reserve(data.m_size);
		}
		auto a(data);
		a.swap(*this);

		DEBUG_INFO("Operator = (after =)", dump();)
		ASSERT(ok(), "Array is broken after operator = ")
		return *this;
	}

	void swap(Array<value_type>& data) noexcept{
		DEBUG_INFO("Function swap() ", dump();)

		std::swap(data.m_data, this->m_data);
		std::swap(data.m_size, this->m_size);
		std::swap(data.m_capacity, this->m_capacity);
		
		DEBUG_INFO("Function swap() (after swap)", dump();)
	}

	int size() {
	  return m_size;
	}
	
	int capacity() {
	  return m_capacity;
	}
	
	int max_size() {
	  return m_capacity;
	}
	
	pointer data() noexcept(noexcept(value_type())) {
		ASSERT(ok(), "Cannot extract data")
		bool* tmp = new bool(m_size);
		for (int i = 0; i < m_size; i++)
			tmp[i] = (*this)[i];
		DEBUG_INFO("Function data()", dump();)
		return tmp;
	}

	const_pointer data() const noexcept(noexcept(value_type())){
		ASSERT(ok(), "Cannot extract data")
		bool* tmp = new bool(m_size);
		for (int i = 0; i < m_size; i++)
			tmp[i] = (*this)[i];
		DEBUG_INFO("Const function data()", dump();)
		return tmp;
	}
	
	reference front() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function front()", dump();)
		return reference(m_data, 0);
	}
	
	const_reference front() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function front()", dump();)
		return reference(m_data, 0);
	}

	reference back() noexcept(noexcept(value_type())) {
		DEBUG_INFO("Function back()", dump();)
		return reference(m_data, m_size - 1);
	}
	
	const_reference back() const noexcept(noexcept(value_type())){
		DEBUG_INFO("Const function back()", dump();)
		return reference(m_data, m_size - 1);
	}

	
	void reserve(int size) {
		DEBUG_INFO("Function reserve()", dump();)
		ASSERT(ok(), "Cannot reserve, array is broken")
		
		if (size > m_capacity) {
			auto tmp(*this);
			delete[] m_data;
			int cur_size = ceil(size/8.0);
			m_data = static_cast<char*>(operator new[] (cur_size, 0));
			
			m_capacity = size;
			for (int i = 0; i < m_size; i++) {
			  (*this)[i] = tmp[i];
			}
		}
		DEBUG_INFO("Function reserve() (after reserving)", dump();)
		ASSERT(ok(), "Array is broken after resizing");
	}
	
	void resize(int size) {
		DEBUG_INFO("Function resize()", dump();)
		ASSERT(ok(), "Array is broken, cannot be resized")
		auto tmp(*this);
		delete[] m_data;
		int cur_size = ceil(size/8.0);
		m_data = static_cast<char*>(operator new[] (cur_size, 0));

		if (m_capacity < size) {m_capacity = size;}
		int interm = m_size;
		m_size = size;
		for (int i = 0; i < m_size; i++) {
		  if (i < interm) (*this)[i] = tmp[i];
		  else (*this)[i] = 0;
		}

		DEBUG_INFO("Function resize() (after resize)", dump();)
		ASSERT(ok(), "Array is broken after resizing")
	}

	
	iterator insert(const_iterator it, const_reference elem) {
		DEBUG_INFO("Function insert() 1 elem ", dump();)
		ASSERT(ok(), "Cannot insert data")
		if ((m_size + 1) > m_capacity) {
			DEBUG_INFO("Insert function(): resize was nedded ", dump();)
			auto tmp(*this);
			reserve(2 * m_size);
			//int pos = it.getPos();
			difference_type pos = it - cbegin();
			
			for (int i = 0; i < pos; i++)
			  (*this)[i] = tmp[i];
			(*this)[pos] = elem;
			for (int i = pos+1; i < m_size+1; i++)
			  (*this)[i] = tmp[i-1];
			
			m_capacity = 2 * m_size;
		} else {
			DEBUG_INFO("Insert function(): inserting into current array ", dump();)
			auto tmp(*this);
			//int pos = it.getPos();
			difference_type pos = it - cbegin();
			(*this)[pos] = elem;
			for (int i = pos+1; i < m_size+1; i++) 
			  (*this)[i] = tmp[i-1];
		}
		m_size+=1;

		DEBUG_INFO("Function insert() 1 elem (after insertion)", dump();)
		return begin();
	}
	
	iterator insert(const_iterator it, std::initializer_list<value_type> list) {
		DEBUG_INFO("Function insert() list", dump();)
		ASSERT(ok(), "Cannot insert data")
		if ((m_size + list.size()) > unsigned(m_capacity)) {
			DEBUG_INFO("Insert function(): resize was nedded ", dump();)
			auto tmp(*this);
			reserve(2 * m_size + list.size());
			//int pos = it.getPos();
			difference_type pos = it - cbegin();
			
			for (int i = 0; i < pos; i++)
			  (*this)[i] = tmp[i];
			int i = pos;
			for(auto object : list) {
			  (*this)[i] = object;
			  i+=1;
			}
			for (unsigned int j = i; j < (m_size+list.size()); j++)
			  (*this)[j] = tmp[j-list.size()];
			m_capacity = 2 * m_size + list.size();
		} else {
			DEBUG_INFO("Insert function(): inserting into current array ", dump();)
			auto tmp(*this);
			//int pos = it.getPos();
			difference_type pos = it - cbegin();
			for (auto object : list) {
			  (*this)[pos] = object;
			  pos+=1;
			}
			for (unsigned int i = pos; i < (m_size+list.size()); i++) 
			  (*this)[i] = tmp[i-list.size()];
		}
		m_size+=list.size();

		DEBUG_INFO("Function insert() list (after insertion)", dump();)
		return begin();
	}
	
	iterator insert(const_iterator it,size_type count, const_reference val) {
		DEBUG_INFO("Function insert() count val", dump();)
		ASSERT(ok(), "Cannot insert data")
		if ((m_size + count) > unsigned(m_capacity)) {
			DEBUG_INFO("Insert function(): resize was nedded ", dump();)
			auto tmp(*this);
			reserve(2 * m_size + count);
			//int pos = it.getPos();
			difference_type pos = it - cbegin();
			
			for (int i = 0; i < pos; i++)
			  (*this)[i] = tmp[i];
			for(unsigned int i = pos; i < (pos + count); i++) {
			  (*this)[i] = val;
			}
			for (unsigned int j = (pos + count); j < (m_size+count); j++)
			  (*this)[j] = tmp[j-count];
			m_capacity = 2 * m_size + count;
		} else {
			DEBUG_INFO("Insert function(): inserting into current array ", dump();)
			auto tmp(*this);
			//int pos = it.getPos();
			difference_type pos = it - cbegin();
			for (unsigned int i = pos; i < (pos+count); i++) {
			  (*this)[pos] = val;
			}
			for (unsigned int i = (pos + count); i < (m_size+count); i++) 
			  (*this)[i] = tmp[i-count];
		}
		m_size+=count;

		DEBUG_INFO("Function insert() count val (after insertion)", dump();)
		return begin();
	}
	
	iterator insert(const_iterator it, iterator first, iterator last) {
		DEBUG_INFO("Function insert() list", dump();)
		ASSERT(ok(), "Cannot insert data")
		const difference_type count = last - first;
		if ((m_size + count) > unsigned(m_capacity)) {
			DEBUG_INFO("Insert function(): resize was nedded ", dump();)
			auto tmp(*this);
			reserve(2 * m_size + count);
			//int pos = it.getPos();
			difference_type pos = it - cbegin();
			
			for (int i = 0; i < pos; i++)
			  (*this)[i] = tmp[i];
			int i = pos;
			for (iterator iter = first; iter != last; ++iter){
			  (*this)[i] = *iter;
			  i+=1;
			}
			for (unsigned int j = i; j < (m_size+count); j++)
			  (*this)[j] = tmp[j-count];
			m_capacity = 2 * m_size + count;
		} else {
			DEBUG_INFO("Insert function(): inserting into current array ", dump();)
			auto tmp(*this);
			//int pos = it.getPos();
			difference_type pos = first - begin();
			for (iterator iter = first; iter != last; ++iter) {
			  (*this)[pos] = *iter;
			  pos+=1;
			}
			for (unsigned int i = pos; i < (m_size+count); i++) 
			  (*this)[i] = tmp[i-count];
		}
		m_size+=count;

		DEBUG_INFO("Function insert() list (after insertion)", dump();)
		return begin();
	}
	
	void push_back(const_reference elem) {
		DEBUG_INFO("Function push_back()", dump();)
		ASSERT(ok(), "Cannot push, array is broken")

		if ((m_size + 1) > m_capacity)
			reserve(2 * m_size);
		(*this)[m_size] = elem;
		m_size+=1;

		DEBUG_INFO("Function push_back() ( after pushing)", dump();)
		ASSERT(ok(), "Array is broken after push()")
	}

	void pop_back() {
		DEBUG_INFO("Function pop_back()", dump();)
		ASSERT((m_size > 0), "size is less than zero, cannot pop")
		(*this)[m_size - 1] = 0;
		m_size -=1;
	}
	
	void erase(const_iterator pos) {
		DEBUG_INFO("Function erase() ", dump();)
		difference_type place = pos - cbegin();
		ASSERT(((place >= 0) && (place < m_size)), "size is less than zero, cannot pop")
		for (int i = place; i < (m_size - 1); i++) {
			(*this)[i] = (*this)[i+1];
		}
		(*this)[m_size - 1] = 0;
		m_size-=1;
		DEBUG_INFO("Function erase() (after erasing)", dump();)
	}

	void erase(const_iterator first, const_iterator last) {
		DEBUG_INFO("Function erase() [first, last) ", dump();)
		difference_type place = first - cbegin();
		difference_type count = last - first;
		ASSERT(((place >= 0) && (place < m_size)), "size is less than zero, cannot pop")
		
		for (int i = place; i < (m_size - count); i++) {
			(*this)[place] = (*this)[i + count];
			place+=1;
		}
		for (int i = (m_size - count); i < m_size; i++) {
			(*this)[i] = 0;
		}
		m_size-=count;
		DEBUG_INFO("Function erase() [first, last] (after erasing)", dump();)
	}
	
	void clear() noexcept{
		DEBUG_INFO("Function clear()", dump();)
		ASSERT(ok(), "Current array is broken, cannot clear")
		m_size = 0;
		m_capacity = 0;
		int size = ceil(m_size/8.0);
		memset(m_data, 0, size);
	}
	
	void dump() const{
		if (ok()) {std::cout<<"ARRAY IS IN OK STATE. PRINTING INFO ..."<<std::endl;}
		else { 
			std::cout<<" ARRAY IS BROKEN. PRINTING INFO..."<<std::endl;
			std::cout<<"Address of broken array : "<<std::addressof(m_data)<<std::endl;
		};
		std::cout<<"ARRAY SIZE : "<<m_size<<std::endl;
		std::cout<<"ARRAY CAPACITY : "<<m_capacity<<std::endl;
		std::cout<<"DATA TYPE : "<<"bool"<<std::endl;
		std::cout<<"ARRAY ELEMENTS : "<<std::endl;
		std::cout<<"-----------------------------------------"<<std::endl;
		for (int i = 0; i < m_size ; i++)
			std::cout <<"Element ["<<i<<"] : "<<(*this)[i]<<std::endl;
		std::cout<<std::endl;
		std::cout<<"-----------------------------------------"<<std::endl;
		std::cout<<std::endl;
	}

	bool ok() const{
		return (m_data!= NULL) && (m_size >= 0) && (m_capacity >= 0) && (m_size <= m_capacity);
	}

	~Array() {
		DEBUG_INFO("Destructor is called", dump();)
		delete[] m_data;
		m_size = 0;
		m_capacity = 0;
		m_data = nullptr;
	}
	};
	*/
//make own exception (конструктор big exception)
//пользовательские ассерты превращаются в exception
//std clog
//soft assert

//код ошибки возвращать,  завимсимости от этого throw или что то еще
//ok с разным уровнем жесткости - типа ok с ассертом и ok с throw
//писать адрес объекта ( типа delete был вызван или нет)