#ifndef _PROBLEM012_THREAD_HPP_
#define _PROBLEM012_THREAD_HPP_

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

/* Project Headers */
#include "boost/thread.hpp"

/************** Class & Func Declarations *****************/
namespace e012 {
class TriangleGenerator {
public:
    TriangleGenerator() {
        this->t_add = 1;
        this->t_number = 0;
    }
    virtual	~TriangleGenerator() {};

    inline TriangleGenerator & operator=(TriangleGenerator other) {
        swap(*this, other);
        return *this;
    }
    void swap(TriangleGenerator &first, TriangleGenerator &second) {
        using std::swap;
        swap(first.t_add, second.t_add);
        swap(first.t_number, second.t_number);
    }

    long next() {
        this->t_number += t_add;
        this->t_add += 1;
        return this->t_number;
    }

    long index() {
        return this->t_add - 1;
    }

    long number() {
        return this->t_number;
    }

private:
    long t_add;
    long t_number;
};

class Worker {
public:
    Worker(TriangleGenerator &tg) :
        threadRunning(true), triangle(tg) { }
    virtual ~Worker() {}

    virtual void execute() = 0;
    virtual void start() = 0;
    inline void stop() {
        threadRunning = false;
        thread.join();
    }
    inline bool isRunning() { return threadRunning; }
protected:
    bool threadRunning;
    boost::thread thread;
    TriangleGenerator triangle;
    static boost::mutex mtx;
};
boost::mutex Worker::mtx;

void find_divisors(long tval, std::vector<int> &v) {
    for (int i = 1; i <= tval; ++i) {
        if ((tval % i) == 0) {
            v.push_back(i);
        }
    }
}
}

#endif /* _PROBLEM012-THREAD_HPP_ */

