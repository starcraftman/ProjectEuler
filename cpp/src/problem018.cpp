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
typedef unsigned long product_t;
typedef unsigned short num_t;
typedef std::vector<num_t> nums_t;

/***************** Constants & Macros *********************/
static const char * const INPUT = "src/input_e018.txt";

/****************** Class Definitions *********************/
/* Tracks a path as we go down the pyramid. */
class Tracker {
public:
    Tracker() : product(1) {};
    Tracker(const Tracker &other) : product(other.product), nums(other.nums) {};
    virtual	~Tracker() {};

    /* Operators */
    inline Tracker & operator=(const Tracker &other) {
        Tracker temp(other);
        swap(*this, temp);
        return *this;
    }
    void swap(Tracker &first, Tracker &second) {
        using std::swap;
        swap(first.product, second.product);
        swap(first.nums, second.nums);
    }

    inline void multiply(num_t val) {
        product *= val;
        nums.push_back(val);
    }
    inline product_t get_proudct() const { return product; }
    inline nums_t get_nums() const { return nums; }

private:
    product_t product;
    nums_t nums;
};

/************** Global Vars & Functions *******************/
void process_line(std::string line, std::vector<Tracker> &originals) {
    std::stringstream vals(line);
    std::vector<num_t> next_row;
    std::vector<Tracker> new_trackers;

    num_t temp;
    while (vals.good()) {
        vals >> temp;
        next_row.push_back(temp);
    }

    int pos = 0;
    for (std::vector<Tracker>::const_iterator itr = originals.begin(); itr != originals.end(); ++itr)  {
        Tracker temp_l(*itr), temp_r(*itr);
        temp_l.multiply(next_row[pos]);
        temp_r.multiply(next_row[pos+1]);

        new_trackers.push_back(temp_l);
        new_trackers.push_back(temp_r);
        ++pos;
    }

    originals = new_trackers;
}

TEST(Euler018, SimplestTracker) {
    Tracker t1;
    t1.multiply(50);

    ASSERT_EQ(50, t1.get_proudct());
    nums_t list = t1.get_nums();
    ASSERT_EQ(50, list.back());
}

/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

