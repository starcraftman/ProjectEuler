/**
Find first fibonacci number with 1000 digits.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <sstream>

#include "gtest/gtest.h"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/assign/list_of.hpp"
#include "gens.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::gens::Fibonacci;

/******************* Type Definitions *********************/
typedef boost::multiprecision::cpp_int fib_t;

/************** Global Vars & Functions *******************/
TEST(Euler025, FirstTen) {
    std::vector<fib_t> expect = boost::assign::list_of (1)
        (2) (3) (5) (8) (13) (21) (34) (55) (89);
    Fibonacci<fib_t> fib;

    for (std::vector<fib_t>::const_iterator itr = expect.begin(); itr != expect.end(); ++itr) {
        ASSERT_EQ(*itr, fib.number());
        fib.next();
    }
}

TEST(Euler025, FindAnswer) {
    bool fib_1000 = false;
    Fibonacci<fib_t> fib;

    while (!fib_1000) {
        std::stringstream ss;
        ss << fib.next();

        if (ss.str().size() >= 1000) {
            fib_1000 = true;
            cout << fib.index() << " fibonacci with 1000 digits is: " << ss .str() << endl;
        }
    }
}

