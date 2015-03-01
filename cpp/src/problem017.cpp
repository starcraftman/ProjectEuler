/**
If the numbers 1 to 5 are written out in words: one, two, three, four, five, then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.

If all the numbers from 1 to 1000 (one thousand) inclusive were written out in words, how many letters would be used?

NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and forty-two) contains 23 letters and 115 (one hundred and fifteen) contains 20 letters. The use of "and" when writing out numbers is in compliance with British usage.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream> /* File operations. */
//#include <sstream> /* String stream. */
#include <string> /* C++ String class. */
//#include <exception> /* Top level exception header. */
//#include <stdexcept> /* Some useful common exceptions */
//#include <new> /* Defines bad_malloc exception, new functions. */
//#include <memory> /* std smart pointers like unique_ptr */
//#include <typeinfo> /* Casting header. */
//#include <limits> /* Inspect properties, numeric_limits<int> iLimit; */
//#include <numeric> /* Math accumulators & other ops*/
//#include <functional> /* Function declarations & std::bind */

/* C++11 Only*/
//#include <chrono> /* std::chrono contains timing for threads */
//#include <initializer_list> /* Allows class to take list like {1,2} */
//#include <random> /* Random c++ version */
//#include <regex>
//#include <thread>
//#include <atomic>
//#include <condition_variable>
//#include <mutex>

/* STL Headers */
//#include <vector>
//#include <list>
//#include <deque>
//#include <stack>
//#include <queue>
//#include <priority_queue>
//#include <set> // multiset for multiple keys allowed.
//#include <map> // multimap for multiple keys allowed.
//#include <bitset>
//#include <utility> // Has pair for map, std::swap
//#include <algorithm>
//#include <iterator> // Contains back_inserter function and like.

/* C Headers */
//#include <cstdio>
//#include <cstring>
//#include <cstdlib> /* atof, rand, malloc... */
//#include <cstddef> /* size_t, NULL */
//#include <cstdarg> /* Variable argument functions */
//#include <cctype> /* Character check functions */
//#include <climits>
//#include <cassert>
#include <cmath>
//#include <cstdint> /* C++11 only, standard u_int16 & such */

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/


/***************** Constants & Macros *********************/
static const char *I_BASE = "./src/input_e017.txt";
static const char *I_TENS = "./src/input_e017_tens.txt";


/****************** Class Definitions *********************/
class NumToWord {
public:
    NumToWord() {};
    virtual	~NumToWord() {};

    std::string phrase(int number) {
        std::string word;

        if (number < 1 || number > 1000) {
            return "not supported";
        }

        if (number == 1000) {
            return "one thousand";
        }

        /* 100 - 999 */
        if (number < 1000 && number > 99) { /* Hundreds */
            int num_hundreds = std::floor(number / 100.0);
            word += base[num_hundreds] + " hundred";
            number -= num_hundreds * 100;

            if (number != 0) {
                word += " and ";
            }
        }

        /* Number less than 100 */
        if (number < 100 && number > 19) {
            int ones_pos = number % 10;
            int ten_pos = std::floor(number / 10.0) - 2;

            word += this->tens[ten_pos];
            if (ones_pos > 0) {
                word += "-" + this->base[ones_pos];
            }
        } else if (number != 0) {
            word += this->base[number];
        }

        return word;
    }

    void init_from_file() {
        std::ifstream fin_base(I_BASE), fin_tens(I_TENS);
        base.push_back("zero");

        int val;
        std::string name;
        while(fin_base.good()) {
            fin_base >> val >> name;
            base.push_back(name);
        }

        while(fin_tens.good()) {
            fin_tens >> val >> name;
            tens.push_back(name);
        }
    };
private:
    NumToWord(const NumToWord &other);
    std::vector<std::string> base;
    std::vector<std::string> tens;
};


/************** Global Vars & Functions *******************/
int count_chars(std::string input) {
    int count = 0;

    for (std::string::const_iterator itr = input.begin(); itr != input.end(); ++itr) {
        if (*itr == ' ' || *itr == ',' || *itr == '-') {
            continue;
        } else {
            count++;
        }
    }
    return count;
}

TEST(Euler017, CountLetters) {
    std::string input = "one, two, three, four, five, twenty-one";
    int actual, expect = 28;

    actual = count_chars(input);

    ASSERT_EQ(actual, expect);
}

TEST(Euler017, TestUnderThousand) {
    NumToWord gen;
    gen.init_from_file();

    ASSERT_STREQ("three hundred and forty-two", gen.phrase(342).c_str());
    ASSERT_STREQ("one hundred and fifteen", gen.phrase(115).c_str());
    ASSERT_STREQ("two hundred", gen.phrase(200).c_str());
}

TEST(Euler017, TestUnderHundred) {
    NumToWord gen;
    gen.init_from_file();

    ASSERT_STREQ("twenty", gen.phrase(20).c_str());
    ASSERT_STREQ("twenty-one", gen.phrase(21).c_str());
    ASSERT_STREQ("thirty-five", gen.phrase(35).c_str());
    ASSERT_STREQ("seventy-eight", gen.phrase(78).c_str());
    ASSERT_STREQ("ninety-nine", gen.phrase(99).c_str());
}

TEST(Euler017, TestUnderTwenty) {
    std::string expect[] = {"zero", "one", "two", "three", "four", "five",
                            "six", "seven", "eight", "nine", "ten",
                            "eleven", "twelve", "thirteen", "fourteen", "fifteen",
                            "sixteen", "seventeen", "eighteen", "nineteen", "twenty"};
    std::string actual;

    NumToWord gen;
    gen.init_from_file();
    for (int i = 1; i < 11; ++i) {
        actual = gen.phrase(i);
        ASSERT_STREQ(expect[i].c_str(), actual.c_str());
    }
}

TEST(Euler017, FinalAnswer) {
    NumToWord gen;
    gen.init_from_file();
    long count = 0;
    for (int i = 1; i < 1001; ++i) {
        std::string words = gen.phrase(i);
        count += count_chars(words);
    }

    ASSERT_EQ(count, 21124);
    cout << "The number of characters is: " << count << endl;
}

/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

