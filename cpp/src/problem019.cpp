/**
You are given the following information, but you may prefer to do some research for yourself.

    1 Jan 1900 was a Monday.
    Thirty days has September,
    April, June and November.
    All the rest have thirty-one,
    Saving February alone,
    Which has twenty-eight, rain or shine.
    And on leap years, twenty-nine.
    A leap year occurs on any year evenly divisible by 4, but not on a century unless it is divisible by 400.

Q: How many Sundays fell on the first of the month during the twentieth century (1 Jan 1901 to 31 Dec 2000)?
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

#include "boost/date_time.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;
namespace greg  = ::boost::gregorian;

/******************* Type Definitions *********************/


/***************** Constants & Macros *********************/


/****************** Class Definitions *********************/


/************** Global Vars & Functions *******************/
/* Simple boost solution */
int main(void) {
    greg::date start(1901, greg::Jan, 1), end(2001, greg::Jan, 1);
    long counter = 0;

    // Keep moving until we hit Sunday.
    while (start.day_of_week() != greg::Sunday) {
        start += greg::days(1);
    }

    for (greg::week_iterator itr(start); itr < end; ++itr) {
        if (itr->day() == 1 && itr->day_of_week() == greg::Sunday) {
            cout << *itr << endl;
            ++counter;
        }
    }

    cout << "The number of Sundays on the 1st of a month in 20th Century is: " << counter << endl;

    return 0;
}


/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

