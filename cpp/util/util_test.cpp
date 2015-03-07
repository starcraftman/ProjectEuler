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

#include "gtest/gtest.h"
#include "util.hpp"
#include "boost/assign/list_of.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/


/***************** Constants & Macros *********************/


/****************** Class Definitions *********************/


/************** Global Vars & Functions *******************/
TEST(Util, Placeholder) {
    util::u_long num(88);
    ASSERT_EQ(88, num);
}

TEST(Util, Divisors) {
    std::set<util::u_int> res, expect = boost::assign::list_of(1) (2) (4) (5) (10) (20) (25) (50) (100);
    u_int num = 100;
    res = util::find_divisors(num);

    ASSERT_EQ(expect.size(), res.size());
    for (std::set<util::u_int>::const_iterator i = res.begin(), e = expect.begin();
            i != res.end(); ++i, ++e) {
        ASSERT_EQ(*e, *i);
    }
}

