/**
Work out the first ten digits of the sum of the following
oone-hundred 50-digit numbers.

Digits read from INPUT
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
using std::cout;
using std::endl;
using std::string;

static const char *INPUT = "./src/input_e013.txt";

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

TEST(Euler013, FinalAnswer) {
    std::ifstream input(INPUT);
    std::string line;

    std::getline(input, line);
    boost::multiprecision::cpp_int counter(line);
    while (line.size() > 0) {
        std::getline(input, line);
        boost::multiprecision::cpp_int temp(line);
        counter += temp;
    }

    std::string first_ten(counter.str());
    cout << first_ten << endl;
    while (first_ten.length() != 10) {
        first_ten.pop_back();
    }
    cout << "Full sum: " << counter << endl;
    cout << "First 10 digits: " << first_ten << endl;
    ASSERT_STREQ("5537376230", first_ten.c_str());
}
