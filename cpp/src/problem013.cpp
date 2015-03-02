/**
 * Find first triangle number with over 500 divisors.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream>
#include <sstream>
#include <string> /* C++ String class. */

#include "gtest/gtest.h"
#include "boost/multiprecision/cpp_int.hpp"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

static const char *INPUT = "./src/input_e013.txt";

/****************** Class Definitions *********************/


/************** Global Vars & Functions *******************/
TEST(Euler013, LoadBigInt) {
    const char *expect = "37107287533902102798797998220837590246510135740250";
    boost::multiprecision::cpp_int big(expect);
    std::stringstream ss;
    ss << big;
    ASSERT_STREQ(ss.str().c_str(), expect);
}

TEST(Euler013, ReadIntFromFile) {
    const char *expect = "37107287533902102798797998220837590246510135740250";
    std::ifstream input(INPUT);
    std::string line;
    std::getline(input, line);
    boost::multiprecision::cpp_int big(line);

    std::stringstream ss;
    ss << big;
    ASSERT_STREQ(ss.str().c_str(), expect);
}

TEST(Euler013, SumNumbers) {
    const char *expect = "5537376230390876637302048746832985971773659831892672";
    std::ifstream input(INPUT);
    std::string line;

    std::getline(input, line);
    boost::multiprecision::cpp_int counter(line);
    while (line.size() > 0) {
        std::getline(input, line);
        boost::multiprecision::cpp_int temp(line);
        counter += temp;
    }
    cout << "Full sum: " <<  counter << endl;

    std::stringstream ss;
    ss << counter;
    std::string first = ss.str();
    ASSERT_STREQ(first.c_str(), expect);

    cout << "First 10 digits: ";
    for (std::string::const_iterator itr = first.begin(); itr != first.begin() + 10; ++itr) {
        cout << *itr;
    }
    cout << endl;
}
