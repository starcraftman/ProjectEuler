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
#include <algorithm>
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
typedef unsigned long sum_t;
typedef unsigned short num_t;
typedef std::vector<num_t> nums_t;

/***************** Constants & Macros *********************/
static const char * const INPUT = "src/input_e018.txt";

/****************** Class Definitions *********************/
/* Tracks a path as we go down the pyramid. */
class Tracker {
public:
    Tracker() : sum(0) {};
    Tracker(const Tracker &other) : sum(other.sum), nums(other.nums) {};
    virtual	~Tracker() {};

    /* Operators */
    inline Tracker & operator=(const Tracker &other) {
        Tracker temp(other);
        swap(*this, temp);
        return *this;
    }
    void swap(Tracker &first, Tracker &second) {
        using std::swap;
        swap(first.sum, second.sum);
        swap(first.nums, second.nums);
    }

    inline bool operator<(const Tracker &other) const { return this->sum < other.sum; }

    inline void add(num_t val) {
        sum += val;
        nums.push_back(val);
    }
    inline sum_t get_sum() const { return sum; }
    inline nums_t get_nums() const { return nums; }

private:
    sum_t sum;
    nums_t nums;
};

/************** Global Vars & Functions *******************/
void process_line(std::string line, std::vector<Tracker> &originals) {
    std::stringstream vals(line);
    std::vector<num_t> next_row;
    std::vector<Tracker> old_trackers(originals);
    originals.clear();

    num_t temp = 0;
    while (vals.good()) {
        vals >> temp;
        next_row.push_back(temp);
    }

    std::vector<num_t>::size_type pos(0);
    while ((pos + 1) != next_row.size()) {
        Tracker temp_l(old_trackers[pos]), temp_r(old_trackers[pos]);
        temp_l.add(next_row[pos]);
        temp_r.add(next_row[pos+1]);

        originals.push_back(temp_l);
        originals.push_back(temp_r);
        ++pos;
    }
}

void trace_vec(std::vector<Tracker> &v) {
    cout << "Tracing......" << endl;

    int cnt = 0;
    for (std::vector<Tracker>::const_iterator itr = v.begin(); itr != v.end(); ++itr) {
        cout << "Tracker #" << cnt << " " << itr->get_sum() << endl;

        nums_t nums = itr->get_nums();
        for (std::vector<num_t>::const_iterator itr_n = nums.begin(); itr_n != nums.end(); ++itr_n) {
            cout << *itr_n << " + ";
        } cout << endl;
    }

    cout << "------------------" << endl;
}

TEST(Euler018, ProcessALine) {
    std::ifstream fin(INPUT);
    std::string line;
    std::vector<Tracker> v;

    /* Setup Initial Node */
    num_t temp;
    fin >> temp;
    std::getline(fin, line);
    Tracker root;
    root.add(temp);

    v.push_back(root);

    std::getline(fin, line);
    process_line(line, v);

    ASSERT_EQ(2, v.size());
    ASSERT_EQ(139, v.back().get_sum());
    v.pop_back();
    ASSERT_EQ(170, v.back().get_sum());
}

TEST(Euler018, SimplestTracker) {
    Tracker t1;
    t1.add(50);

    ASSERT_EQ(50, t1.get_sum());
    nums_t list = t1.get_nums();
    ASSERT_EQ(50, list.back());
}

TEST(Euler018, FinalAnswer) {
    std::ifstream fin(INPUT);
    std::string line;
    std::vector<Tracker> v;

    /* Setup Initial Node */
    num_t temp;
    fin >> temp;
    std::getline(fin, line);
    Tracker root;
    root.add(temp);

    v.push_back(root);

    while (std::getline(fin, line)) {
        process_line(line, v);
        trace_vec(v);
    }

    /* Sort vec */
    std::sort(v.begin(), v.end());

    /* Answer at back. */
    root = v.back();
    cout << "Final Sum: " << endl << root.get_sum() << endl
        << "Sum of:" << endl;

    nums_t nums = root.get_nums();
    for (std::vector<num_t>::const_iterator itr_n = nums.begin(); itr_n != nums.end(); ++itr_n) {
        cout << *itr_n << " + ";
    }
    cout << endl;
}

/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

