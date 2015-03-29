/**
 * Test cases for Number Generators
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */

#include "gtest/gtest.h"
#include "gens.hpp"
#include "boost/assign/list_of.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/************** Global Vars & Functions *******************/
TEST(Util, FibSeries) {
    std::vector<int> first_ten = boost::assign::list_of(1) (1) (2)
        (3) (5) (8) (13) (21) (34) (55) (89) (144);
    util::gens::Fibonacci<int> fib;

    for (std::vector<int>::const_iterator i = first_ten.begin(); i != first_ten.end(); ++i) {
        ASSERT_EQ(*i, fib.number());
        fib.next();
    }
}

