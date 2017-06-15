#include <iostream>
#include "array.h"
#include "array_bool.h"
#include <exception>
#include <stdexcept>
#include <fstream>
#include <vector>
using namespace std;


#define CHECK(cond, message, out) if (!cond) {\
							out<<"LINE : "<<__LINE__<<endl;\
							out<<"TEST CONDITION FAILED WITH RESULT"<<endl;\
							out<<message<<endl;\
							return -1;\
							}\

#define TEST_VALIDITY(cond, expr, out) if (!cond){\
								passed.push_back('-');\
								out<<expr<<endl;}\
					else {passed.push_back('+');}  

vector<char> passed;

void test_run();
int main() {
	test_run();
	return 0;
}

								
int test1(ofstream& out) {
		bool* a = new bool[10];
		for (int i = 0; i < 10; i++)
			if (i < 5)
				a[i] = 0;
			else 
				a[i] = 1;
		Array<bool> ar(a, 10);
		CHECK((ar[6] == 1), "Invalid work of pointer constructor", out)
		CHECK((ar.size() == 10), "Invalid work of pointer constructor", out)
		return 1;
}

int test2(ofstream& out) {
  
		Array<bool> ar = {false, true, false};
		CHECK((ar[1] == true), "Invalid work of initializer list", out)
		CHECK((ar.size() == 3), "Invalid work of initializer list", out)
		return 1;
}

//CONSTRUCTORS
int test3(ofstream& out) {
		Array<bool> v1 = {false, false, 1, true, false};
		Array<bool> v2(10);
		Array<bool> v3(15);
		Array<bool> v4 = {true, false, true, true};
		Array<bool> v5(v1);
		Array<bool> v6(std::move(v2));
		Array<bool> v7 = v3;
		Array<bool> v8 = std::move(v4);
		
		CHECK((v5.size() == 5), "Invalid work of copy constructor (1)", out)
		CHECK((v6.capacity() == 10), "Invalid work of move constructor (2)", out)
		CHECK((v3.capacity() == 15), "Invalid work of capacity constructor (3)", out)
		CHECK((v2.capacity() == 0), "Invalid work of capacity constructor(4)", out)
		CHECK((v4.size() == 0), "Invalid work of capacity constructor(5)", out)
		CHECK((v7.capacity() == 15), "Invalid work of capacity constructor(6)", out)
		CHECK((v8.size() == 4), "Invalid work of move constructor(7)", out)
		CHECK((v8[0] == true), "Invalid work of move constructor(8)", out)
		CHECK((v8[1] == false), "Invalid work of move constructor(9)", out)
		
		return 1;
}

//RESIZING
int test4(ofstream& out) {
		Array<bool> v1{true, true, false};
		v1.reserve(70);
		CHECK((v1.capacity() == 70), "Invalid work of reserve(1)", out)
		CHECK((v1.size() == 3), "Invalid work of reserve(2)", out)
		v1.resize(50);
		CHECK((v1.size() == 50), "Invalid work of resize", out)
		CHECK((v1.capacity() == 70), "Invalid work of resize", out)
		
		return 1;
}

//OPERATOR =
int test5(ofstream& out) {
		Array<bool> v1{true, true, false};
		Array<bool> v2(5);
		v2 = v1;
		
		CHECK((v1.size() == 3), "Invalid work of operator= (1)", out)
		CHECK((v1.capacity() == 3), "Invalid work of operator= (2)", out)
		CHECK((v1[0] == true), "Invalid work of operator= (3)", out)
		CHECK((v2.size() == 3), "Invalid work of operator= (4)", out)
		CHECK((v1.capacity() == 3), "Invalid work of operator= (5)", out)
		CHECK((v2[0] == true), "Invalid work of operator= (6)", out)
		return 1;
  
}

//ITERATORS
int test6(ofstream& out) {
		Array<bool> v1 = {true, false, true, false};
		Array<bool>::iterator it = v1.begin();
		CHECK((*it == true), "Invalid work of iterator", out)
		it++;
		CHECK((*it == false), "Invalid work of iterator", out)
		it++;
		CHECK((*it == true), "Invalid work of iterator", out)
		it++;
		CHECK((*it == false), "Invalid work of iterator", out)
		it--;
		CHECK((*it == true), "Invalid work of iterator", out)
		CHECK((it != v1.begin()), "Invalid work of iterator", out)
		return 1;
}
//INSERTS
int test7(ofstream& out) {
		Array<bool> v2(2);
		v2.push_back(false);
		v2.push_back(true);
		CHECK((v2[0] == false), "Invalid work of operator[] (6)", out)
		CHECK((v2[1] == true), "Invalid work of at() (6)", out)
		v2.insert(v2.begin()+1, {true, false, false, true});
		CHECK((v2.size() == 6), "Invalid work of insert (6)", out)
		v2.insert(v2.begin()+2, true);
		CHECK((v2.size() == 7), "Invalid work of insert (6)", out)
		CHECK((v2[2] == true), "Invalid work of insert (6)", out)
		return 1;
}
//PUSH_BACK - POP_BACK
int test8(ofstream& out) {
		Array<bool> v;
		for (int i = 0; i != 13; ++i) {
			v.push_back(i % 2);
		}
		CHECK((v.size() == 13), "Invalid work of push_back", out)
		for (int i = 0; i != 13; ++i) {
			v.pop_back();
		}
		CHECK((v.size() == 0), "Invalid work of pop_back", out)
		return 1;
}

//INSERT SAME VALUE
int test9(ofstream& out) {
		Array<bool> v2(2);
		v2.push_back(false);
		v2.push_back(true);
		v2.insert(v2.begin()+2, 3, true);
		v2.insert(v2.begin()+4, 7, false);
		CHECK((v2.size() == 12), "Invalid work of insert", out)
		CHECK(v2[v2.size() - 1] == true, "Invalid work of insert", out)
		CHECK(v2[v2.size() - 2] == false, "Invalid work of insert", out)
		return 1;
}

//INSERT RANGE
int test10(ofstream& out) {
		Array<bool> v2{1, 0, 1, 0, 1};
		Array<bool> v1 = {1, 0, 1, 1, 1, 1, 0, 1};
		v2.insert(v2.begin()+2, v1.begin(), v1.end());
		//ASSERT_EQ(v2, Vector<bool>({1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1}));
		CHECK((v2[0] == 1), "Invalid work of insert", out)
		CHECK((v2[2] == 1), "Invalid work of insert", out)
		CHECK((v2[3] == 0), "Invalid work of insert", out)
		CHECK((v2.size() == 13), "Invalid work of insert", out)
		return 1;
}

//ERASE RANGE
int test11(ofstream& out) {
		Array<bool> v = {1, 0, 0, 1, 1, 1};
		v.erase(v.begin()+1, v.begin()+3);
		CHECK((v.size() == 4), "Invalid work of erase", out)
		CHECK((v[0] == 1), "Invalid work of erase", out)
		CHECK((v[1] == 1), "Invalid work of erase", out)
		
		return 1;
}

//CLEAR
int test12(ofstream& out) {
		Array<bool> v = {1, 0, 0, 1, 1, 1};
		v.clear();
		v.push_back(true);
		CHECK((v.size() == 1), "Invalid work of erase", out)
		CHECK((v[0] == 1), "Invalid work of erase", out)
		return 1;
}

void test_run() {
		try {	
			ofstream out("file");
			int result;
			result = test1(out);
			TEST_VALIDITY((result == 1), "TEST 1 HAS FAILED", out)
			result = test2(out);
			TEST_VALIDITY((result == 1), "TEST 2 HAS FAILED", out)
			result = test3(out);
			TEST_VALIDITY((result == 1), "TEST 3 HAS FAILED", out)
			result = test4(out);
			TEST_VALIDITY((result == 1), "TEST 4 HAS FAILED", out)
			result = test5(out);
			TEST_VALIDITY((result == 1), "TEST 5 HAS FAILED", out)
			result = test6(out);
			TEST_VALIDITY((result == 1), "TEST 6 HAS FAILED", out)
			result = test7(out);
			TEST_VALIDITY((result == 1), "TEST 7 HAS FAILED", out)
			result = test8(out);
			TEST_VALIDITY((result == 1), "TEST 8 HAS FAILED", out)
			result = test9(out);
			TEST_VALIDITY((result == 1), "TEST 9 HAS FAILED", out)
			result = test10(out);
			TEST_VALIDITY((result == 1), "TEST 10 HAS FAILED", out)
			result = test11(out);
			TEST_VALIDITY((result == 1), "TEST 11 HAS FAILED", out)
			result = test12(out);
			TEST_VALIDITY((result == 1), "TEST 11 HAS FAILED", out)
			int size = passed.size();
			cout<<"Completed tests : ";
			for (int i = 0; i < size; i++) {
			  cout<<"["<<i<<"]"<<passed[i]<<" ";
			}
			//result = test2(out);
			//TEST_VALIDITY((result == 1), "TEST 12 HAS FAILED", out)
			//result = test3(out);
			//TEST_VALIDITY((result == 1), "TEST 12 HAS FAILED", out)
		} catch(exception& e) {
				cout<<"Exception occured"<<endl;
		}
}


