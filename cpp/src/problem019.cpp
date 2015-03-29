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
#include <string> /* C++ String class. */

#include "boost/date_time.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;
namespace greg  = ::boost::gregorian;

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

