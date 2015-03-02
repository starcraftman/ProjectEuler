/**
 * Template C++ file.
 * Function with references to pointers: func(int *&ptr);
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream> /* File operations. */
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
#include "boost/assign/list_of.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/
typedef unsigned long num_t;
typedef std::pair<num_t, int> pair_t;

/***************** Constants & Macros *********************/
static const char * const INPUT = "src/input_e018.txt";

/****************** Class Definitions *********************/


/************** Global Vars & Functions *******************/
void make_tree(std::istream input, std::vector<num_t> &tree) {
    while (input) {
        std::string line;
        std::getline(input, line);
    }
}

/* Based on previous position i in a zero index binary array. Adjacent values are:
 * left:  2*i + 0
 * right: 2*i + 1
 */
pair_t choose_max_adjacent(int last_pos, std::vector<num_t> &tree) {
    int l_index = 2 * last_pos, r_index= (2 * last_pos) + 1;
    num_t left = tree[l_index], right = tree[r_index];

    pair_t pair;
    if (left >= right) {
        pair.first = left;
        pair.second = l_index;
    } else {
        pair.first = right;
        pair.second = r_index;
    }

    return pair;
}

TEST(Euler018, SimplestTree) {
    std::ifstream fin(INPUT);
    std::vector<num_t> tree, solution;
    std::string line;
    num_t val;
    int count = 1;

    while (fin.good()) {
        fin >> val;
    };
}

TEST(Euler018, ChooseMaxTree) {
    std::vector<num_t> tree1 = boost::assign::list_of    (15) (34) (83) (22);
    std::vector<num_t> tree2 = boost::assign::list_of (6) (55) (55) (77) (77) (100) (100) (16);

    pair_t result = choose_max_adjacent(0, tree1);
    ASSERT_EQ(34, result.first);
    ASSERT_EQ(1, result.second);
    result = choose_max_adjacent(result.second, tree1);
    ASSERT_EQ(77, result.first);
    ASSERT_EQ(1, result.second);
}


/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

