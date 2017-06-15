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