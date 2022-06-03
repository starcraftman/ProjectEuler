/**
 *

Comparing two numbers written in index form like 211 and 37 is not difficult, as any calculator would confirm that 211 = 2048 < 37 = 2187.

However, confirming that 632382518061 > 519432525806 would be much more difficult, as both numbers contain over three million digits.

Using base_exp.txt (right click and 'Save Link/Target As...'), a 22K text file containing one thousand lines with a base/exponent pair on each line, determine which line number has the greatest numerical value.

NOTE: The first two lines in the file represent the numbers in the example given above.
----------------------
To solve this, use exponent rebasing. i.e.
    65 ^ y
    2 ^ x^ y
    2 ^x = 65
    log2(2 ^x) = 65
    x = log2(65)

 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <string>
#include <numeric>
#include <functional>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
const int NUM_CHARS = 1024;
const char* INPUT = "./src/input_e099.txt";
typedef double num_t;

class ExpNum {
public:
    explicit ExpNum(num_t base = 0, num_t expo = 0, long line = 0): base(base), expo(expo), line(line) {};

    friend std::ostream & operator<<(std::ostream &os, const ExpNum &expnum);

    // Data
    num_t base;
    num_t expo;
    long line;
};

std::ostream& operator<<(std::ostream &os, const ExpNum &expnum) {
    os << "Line No: " << expnum.line << " = " << expnum.base << "^" << expnum.expo;

    return os;
}

// Rebase to exponent to base 2
ExpNum rebase_to_2(num_t base, num_t exponent, long line_no = 0) {
    num_t expo_factor = std::log2(base);
    return ExpNum(2, exponent * expo_factor, line_no);
}

// Replace string commas with space
inline
std::string & replace_comma(std::string &line) {
    std::size_t index = 0;
    while (true) {
        index = line.find(",", index);
        if (index == std::string::npos) {
            break;
        }
        line.replace(index, 1, " ");
        index += 1;
    }

    return line;
}

ExpNum best_exponent(const char* file) {
    std::ifstream fin(file);
    ExpNum sol;

    long line_no = 0;
    std::string line;
    while (fin.good()) {
        std::getline(fin, line);
        line_no++;
        line = replace_comma(line);

        std::stringstream ss(line);
        std::string base, expo;
        ss >> base >> expo;
        ExpNum temp = rebase_to_2(std::stod(base), std::stod(expo), line_no);

        if (temp.expo > sol.expo) {
            sol = temp;
        }
    }

    return sol;
}

TEST(Euler099, RebaseTo2) {
    auto result = rebase_to_2(5, 20);
    ASSERT_EQ(result.base, 2);
    ASSERT_EQ(std::floor(result.expo), 46);
}

TEST(Euler099, ReplaceComma) {
    std::string input("1,2,3");
    std::string expect("1 2 3");
    std::string result = replace_comma(input);
    ASSERT_EQ(result, expect);
}

TEST(Euler099, FinalAnswer) {
    ExpNum result = best_exponent(INPUT);
    ASSERT_EQ(result.line, 709);
}
