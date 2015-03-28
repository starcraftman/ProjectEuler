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

TEST(Util, FindDivisors) {
    std::set<util::u_int> res, expect = boost::assign::list_of(1) (2) (4) (5)
        (10) (20) (25) (50) (100);
    u_int num = 100;
    res = util::find_divisors(num);

    ASSERT_EQ(expect.size(), res.size());
    for (std::set<util::u_int>::const_iterator i = res.begin(), e = expect.begin();
            i != res.end(); ++i, ++e) {
        ASSERT_EQ(*e, *i);
    }
}

TEST(Util, IsPrime) {
    ASSERT_FALSE(util::is_prime(4));
    ASSERT_TRUE(util::is_prime(7));
    ASSERT_TRUE(util::is_prime(457));
}

TEST(Util, NextFalse) {
    std::vector<bool> example = boost::assign::list_of(true) (false) (true)
        (false) (true) (true);

    ASSERT_EQ(1, util::next_false(example, 0));
    ASSERT_EQ(3, util::next_false(example, 2));
}

TEST(Util, SimpleSieve) {
    std::set<int> expect = boost::assign::list_of(2) (3) (5) (7) (11) (13) (17)
        (19) (23) (29) (31) (37) (41) (43) (47) (53) (59) (61) (67) (71) (73)
        (79) (83) (89) (97) (101) (103) (107) (109) (113);
    std::set<int> result = util::simple_sieve(120);
    ASSERT_EQ(expect.size(), result.size());
    std::set<int>::const_iterator e = expect.begin();
    for (std::set<int>::const_iterator i = result.begin(); i != result.end(); ++i, ++e) {
        ASSERT_EQ(*i, *e);
    }


}

