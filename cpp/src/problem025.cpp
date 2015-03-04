/**
Find first fibonacci number with 1000 digits.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <string> /* C++ String class. */
#include <sstream>

#include "gtest/gtest.h"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/assign/list_of.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/
typedef boost::multiprecision::cpp_int fib_t;

/****************** Class Definitions *********************/
class FibGen {
public:
    FibGen(): term(1), last(1), before_last(0) {};

    fib_t next(void) {
        fib_t current = last + before_last;
        before_last = last;
        last = current;

        term += 1;
        return current;
    }

    unsigned int index(void) { return term; }
private:
    /* The ith fibonacci number */
    unsigned int term;
    /* These are fib numbers n-1, n-2 */
    fib_t last;
    fib_t before_last;
};

/************** Global Vars & Functions *******************/
TEST(Euler025, FirstTen) {
    std::vector<fib_t> expect = boost::assign::list_of (1)
        (2) (3) (5) (8) (13) (21) (34) (55) (89);
    FibGen fibG;

    for (std::vector<fib_t>::const_iterator itr = expect.begin(); itr != expect.end(); ++itr) {
        ASSERT_EQ(*itr, fibG.next());
    }
}

TEST(Euler025, FindAnswer) {
    bool fib_1000 = false;
    FibGen fibG;

    while (!fib_1000) {
        std::stringstream ss;
        ss << fibG.next();

        if (ss.str().size() >= 1000) {
            fib_1000 = true;
            cout << fibG.index() << " fibonacci with 1000 digits is: " << ss .str() << endl;
        }
    }
}

