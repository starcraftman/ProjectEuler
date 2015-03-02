/**
If the numbers 1 to 5 are written out in words: one, two, three, four, five, then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.

If all the numbers from 1 to 1000 (one thousand) inclusive were written out in words, how many letters would be used?

NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and forty-two) contains 23 letters and 115 (one hundred and fifteen) contains 20 letters. The use of "and" when writing out numbers is in compliance with British usage.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream> /* File operations. */
#include <string> /* C++ String class. */

/* C Headers */
#include <cmath>

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/***************** Constants & Macros *********************/
static const char *INPUT = "./src/input_e017.txt";

/****************** Class Definitions *********************/
class NumToWord {
public:
    NumToWord() {};
    virtual	~NumToWord() {};

    std::string to_words(int number) {
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

    void init() {
        std::ifstream fin(INPUT);;
        std::string name;
        int val;

        base.push_back("zero");
        while(fin.good()) {
            fin >> val >> name;
            base.push_back(name);
        }

        fin.clear();
        std::getline(fin, name);

        while(fin.good()) {
            fin >> val >> name;
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
    NumToWord convert;
    convert.init();

    ASSERT_STREQ("three hundred and forty-two", convert.to_words(342).c_str());
    ASSERT_STREQ("one hundred and fifteen", convert.to_words(115).c_str());
    ASSERT_STREQ("two hundred", convert.to_words(200).c_str());
}

TEST(Euler017, TestUnderHundred) {
    NumToWord convert;
    convert.init();

    ASSERT_STREQ("twenty", convert.to_words(20).c_str());
    ASSERT_STREQ("twenty-one", convert.to_words(21).c_str());
    ASSERT_STREQ("thirty-five", convert.to_words(35).c_str());
    ASSERT_STREQ("seventy-eight", convert.to_words(78).c_str());
    ASSERT_STREQ("ninety-nine", convert.to_words(99).c_str());
}

TEST(Euler017, TestUnderTwenty) {
    std::string actual, expect[] = {"zero", "one", "two", "three", "four", "five",
                            "six", "seven", "eight", "nine", "ten",
                            "eleven", "twelve", "thirteen", "fourteen", "fifteen",
                            "sixteen", "seventeen", "eighteen", "nineteen", "twenty"};

    NumToWord convert;
    convert.init();
    for (int i = 1; i < 11; ++i) {
        actual = convert.to_words(i);
        ASSERT_STREQ(expect[i].c_str(), actual.c_str());
    }
}

TEST(Euler017, FinalAnswer) {
    NumToWord convert;
    convert.init();
    long count = 0;

    for (int i = 1; i < 1001; ++i) {
        std::string words = convert.to_words(i);
        count += count_chars(words);
    }

    ASSERT_EQ(count, 21124);
    cout << "The number of characters is: " << count << endl;
}

/* Notes:
 * Force call to use another version of virtual function: baseP->Item_base::net_price(42);
 *
 */

