/**
 * Template C++ file.
 * Function with references to pointers: func(int *&ptr);
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
//#include <fstream> /* File operations. */
//#include <sstream> /* String stream. */
#include <string> /* C++ String class. */
//#include <exception> /* Top level exception header. */
//#include <stdexcept> /* Some useful common exceptions */
//#include <new> /* Defines bad_malloc exception, new functions. */
//#include <memory> /* std smart pointers like unique_ptr */
//#include <typeinfo> /* Casting header. */
//#include <limits> /* Inspect properties, numeric_limits<int> iLimit; */
//#include <numeric> /* Math accumulators & other ops*/
//#include <functional> /* Function declarations & std::bind */

/* C++11 Only*/
//#include <chrono> /* std::chrono contains timing for threads */
//#include <initializer_list> /* Allows class to take list like {1,2} */
//#include <random> /* Random c++ version */
//#include <regex>
//#include <thread>
//#include <atomic>
//#include <condition_variable>
//#include <mutex>

/* STL Headers */
//#include <vector>
//#include <list>
//#include <deque>
//#include <stack>
//#include <queue>
//#include <priority_queue>
//#include <set> // multiset for multiple keys allowed.
//#include <map> // multimap for multiple keys allowed.
//#include <bitset>
//#include <utility> // Has pair for map, std::swap
//#include <algorithm>
//#include <iterator> // Contains back_inserter function and like.

/* C Headers */
//#include <cstdio>
//#include <cstring>
//#include <cstdlib> /* atof, rand, malloc... */
//#include <cstddef> /* size_t, NULL */
//#include <cstdarg> /* Variable argument functions */
//#include <cctype> /* Character check functions */
//#include <climits>
//#include <cassert>
//$include <cmath>
//$include <cstdint> /* C++11 only, standard u_int16 & such */

#include "gtest/gtest.h"
#include "problem012_thread.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/
/* For enums: Try to namesapce the common elements.
 * typedef enum {
 *	VAL_,
 * } name_e;
 */

/* For structs:
 * typedef struct name_s {
 *	int index;
 * } name_t;
 */

/***************** Constants & Macros *********************/


/****************** Class Definitions *********************/


/************** Global Vars & Functions *******************/
TEST(Euler012, TriangleGenerator) {
    int desired_index = 50;
    long expected_triangle = 1275;
    e012::TriangleGenerator tg;
    for (int i = 0; i < desired_index; ++i) {
        tg.next();
    }

    ASSERT_EQ(expected_triangle, tg.number());
}

TEST(Euler012, TriangleAlternative) {
    int desired_index = 50;
    long expected = 1275, actual = e012::triangle(desired_index);
    ASSERT_EQ(expected, actual);
}

TEST(Euler012, FindDivisors) {
    long triangle = 28;
    std::vector<int> expect, actual;

    int ex_a[] = {1, 2, 4, 7, 14, 28};
    for (int i = 0; i != sizeof(ex_a)/sizeof(int); ++i) {
        expect.push_back(ex_a[i]);
    }

    e012::find_divisors(triangle, actual);
    std::vector<int>::const_iterator e_itr = expect.begin();
    for (std::vector<int>::const_iterator a_itr = actual.begin(); a_itr != actual.end(); ++e_itr, ++a_itr) {
        ASSERT_EQ(*a_itr, *e_itr);
    }
}

/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */
