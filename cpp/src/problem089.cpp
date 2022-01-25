/**
For a number written in Roman numerals to be considered valid there are basic rules
which must be followed. Even though the rules allow some numbers to be expressed in more
than one way there is always a "best" way of writing a particular number.

For example, it would appear that there are at least six ways of writing the number sixteen:

IIIIIIIIIIIIIIII
VIIIIIIIIIII
VVIIIIII
XIIIIII
VVVI
XVI

However, according to the rules only XIIIIII and XVI are valid, and the last example is
considered to be the most efficient, as it uses the least number of numerals.

The 11K text file, roman.txt (right click and 'Save Link/Target As...'),
contains one thousand numbers written in valid, but not necessarily minimal,
Roman numerals; see About... Roman Numerals for the definitive rules for this problem.

Find the number of characters saved by writing each of these in their minimal form.

Note: You can assume that all the Roman numerals in the file contain no more than
four consecutive identical units.

 * Numeral explanation: https://projecteuler.net/about=roman_numerals
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <initializer_list>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
static const std::string INPUT = "./src/input_e089.txt";
static const std::map<std::string, int> char_to_val = {
    {std::string("I"), 1},
    {std::string("IV"), 4},
    {std::string("V"), 5},
    {std::string("IX"), 9},
    {std::string("X"), 10},
    {std::string("XL"), 40},
    {std::string("L"), 50},
    {std::string("XC"), 90},
    {std::string("C"), 100},
    {std::string("CD"), 400},
    {std::string("D"), 500},
    {std::string("CM"), 900},
    {std::string("M"), 1000},
};
static const std::initializer_list<std::string> ordered_keys = {
    std::string("M"),
    std::string("CM"),
    std::string("D"),
    std::string("CD"),
    std::string("C"),
    std::string("XC"),
    std::string("L"),
    std::string("XL"),
    std::string("X"),
    std::string("IX"),
    std::string("V"),
    std::string("IV"),
    std::string("I"),
};

class Numeral {
public:
    Numeral(int value = 1) : value(value) {};
    std::string to_string() {
        int copy_value = value;
        if (this->text != "") {
            return this->text;
        }
        std::stringstream ss;

        for (auto &ele : ordered_keys) {
            int val_chr = char_to_val.at(ele);
            while (copy_value > 0 && (copy_value - val_chr) >= 0) {
                ss << ele;
                copy_value -= val_chr;
            }
        }

        this->text = ss.str();
        return ss.str();
    }

    // FIXME: Reverse the to_string process and compare substrings
    int parse_text(const std::string &text) {
        this->value = 0;

        for (const char chr : text) {
            const std::string temp(1, chr);
            this->value += char_to_val.at(temp);
        }

        return this->value;
    }

    friend std::ostream& operator<<(std::ostream &os, Numeral num) {
        os << num.to_string();
        return os;
    }

    std::string text;
    int value;
};

int saved_chars_roman_minimization(){
    return 0;
}

TEST(Euler089, NumeralToString) {
    Numeral num(57);
    ASSERT_EQ(num.to_string(), std::string("LVII"));
}

TEST(Euler089, NumeralParseText) {
    Numeral num;
    num.parse_text(std::string("LXVII"));
    ASSERT_EQ(num.value, 67);
}

TEST(Euler089, NumeralOutputOperator) {
    Numeral num(57);
    std::stringstream ss;
    ss << num;
    ASSERT_EQ(ss.str(), std::string("LVII"));
}

TEST(Euler089, SavedCharsByMinimizing) {
    std::ifstream input(INPUT, std::ifstream::in);
    std::string line;
    int diff = 0;
    while (std::getline(input, line)) {
        Numeral num;
        num.parse_text(line);
        diff += line.size() - num.to_string().size();
    }
    cout << "Saved " << diff << " chars by minimizing the output." << endl;
    // ASSERT_EQ(saved, 0);
}
