#ifndef _UTIL_HPP_
#define _UTIL_HPP_

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
#include <vector>
//#include <list>
//#include <deque>
//#include <stack>
//#include <queue>
//#include <priority_queue>
#include <set> // multiset for multiple keys allowed.
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
#include <cmath>
//$include <cstdint> /* C++11 only, standard u_int16 & such */

/* Project Headers */

namespace util {

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short u_shrt;

/************** Class & Func Declarations *****************/
template<class T>
T next_false(std::vector<bool> const & sieve, T start_index) {
    for (T i = start_index; i < sieve.size(); ++i) {
        if (sieve[i] == false) {
            return i;
        }
    }

    return -1;
}

/* Return all primes under max. */
template <class T>
std::set<T> simple_sieve(T max) {
    std::set<T> res;

    // Ignore 1 & 0
    std::vector<bool> sieve;
    sieve.push_back(true);
    sieve.push_back(true);
    for (T i = 2; i <= max; ++i) {
        sieve.push_back(false);
    }

    T prime = 2;
    while (prime != -1) {
        res.insert(prime);

        T index = prime;
        while (index <= max) {
            sieve[index] = true;
            index += prime;
        }

        prime = next_false(sieve, prime);
    }

    return res;
}

template <class T>
std::set<T> find_divisors(T num, bool proper=false) {
    std::set<T> res;
    T root = std::floor(std::sqrt(num));

    for (int i = 1; i <= root; ++i) {
        if ((num % i) == 0) {
            res.insert(i);
            res.insert(num / i);
        }
    }

    if (proper) {
        res.erase(num);
    }

    return res;
}

template <class T>
bool is_prime(T num) {
    if (num <= 3) {
        return num > 1;
    }

    if ((num % 2) == 0 || (num % 3) == 0) {
        return false;
    }

    T root = std::floor(std::sqrt(num));
    for (int i = 5; i <= root; i += 6) {
        if ((num % i) == 0 || (num % (i + 2)) == 0) {
            return false;
        }
    }

    return true;
}

} /* end util:: */

#endif /* _UTIL_HPP_ */

