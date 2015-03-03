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
void make_a_tree(std::istream &input, std::vector<num_t> &tree) {
    num_t val;
    std::string line;

    while(std::getline(input, line)) {
        std::vector<num_t> new_row;
        std::stringstream ss(line);

        while(ss >> val) {
            new_row.insert(new_row.begin(), val);
        }

        std::vector<num_t>::size_type max_size = new_row.size();
        while(new_row.size() != 0) {
            if (new_row.size() == max_size || new_row.size() == 1) {
                tree.push_back(new_row.back());
            } else {
                tree.push_back(new_row.back());
                tree.push_back(new_row.back());
            }
            new_row.pop_back();
        }
    }
}

void trace_vec(std::vector<Tracker> &v) {
    cout << "Tracing......" << endl;

    int cnt = 0;
    for (std::vector<Tracker>::const_iterator itr = v.begin(); itr != v.end(); ++itr) {
        cout << "Tracker #" << cnt++ << " " << itr->get_sum() << endl;

        nums_t nums = itr->get_nums();
        for (std::vector<num_t>::const_iterator itr_n = nums.begin(); itr_n != nums.end(); ++itr_n) {
            cout << *itr_n << " + ";
        } cout << endl;
    }

    cout << "------------------" << endl;
}

TEST(Euler018, SimplestTracker) {
    Tracker t1;
    t1.add(50);

    ASSERT_EQ(50, t1.get_sum());
    nums_t list = t1.get_nums();
    ASSERT_EQ(50, list.back());
}

TEST(Euler018, TreeCreation) {
    std::ifstream fin(INPUT);
    std::vector<num_t> tree;

    make_a_tree(fin, tree);
    ASSERT_EQ(75, tree[0]);
    ASSERT_EQ(23, tree.back());

    int index = 9, left = 2 * index + 1, right = 2 * index + 2;
    ASSERT_EQ(47, tree[left]);
    ASSERT_EQ(65, tree[right]);
}

//TEST(Euler018, FinalAnswer) {
//}

/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

