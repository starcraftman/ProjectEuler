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
using util::gens::Coprimes;

/************** Global Vars & Functions *******************/
TEST(UtilGens, FibSeries) {
    std::vector<int> first_ten = boost::assign::list_of(1) (1) (2)
        (3) (5) (8) (13) (21) (34) (55) (89) (144);
    util::gens::Fibonacci<int> fib;

    for (std::vector<int>::const_iterator i = first_ten.begin(); i != first_ten.end(); ++i) {
        ASSERT_EQ(*i, fib.number());
        fib.next();
    }
}

TEST(UtilGens, CoprimeSeries) {
    Coprimes cGen(4);
    Coprimes::CoPairs pairs = cGen.collect();
    std::vector<int> expect = boost::assign::list_of(1) (4) (1) (3)
        (1) (2) (2) (3) (3) (4) (1) (1);

    ASSERT_EQ(expect.size() / 2, pairs.size());
    std::vector<int>::const_iterator e = expect.begin();
    for (Coprimes::CoPairs::const_iterator i = pairs.begin();
            i != pairs.end(); ++i) {
        ASSERT_EQ(*(e++), i->first);
        ASSERT_EQ(*(e++), i->second);
    }
}

// Example, will refactor to the lib somewhere
TEST(UtilGens, TripletSeries) {
    Coprimes cGen(10);
    Coprimes::CoPairs pairs = cGen.collect();

    for (Coprimes::CoPairs::const_iterator i = pairs.begin(); i != pairs.end(); ++i) {
        int diff = i->first - i->second;
        if ((diff % 2) == 0) {
            continue;
        }
        cout << i->first << " " << i->second << endl;

        int a = std::abs(i->first * i->first - i->second * i->second);
        int b = 2 * i->first * i->second;
        int c = i->first * i->first + i->second * i->second;

        cout << "Trip: " << a << " " << b << " " << c << endl;
    }
}

