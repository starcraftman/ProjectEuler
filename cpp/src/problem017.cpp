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
#include <exception>

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/***************** Constants & Macros *********************/
static const char *INPUT = "./src/input_e017.txt";

/****************** Class Definitions *********************/
class ParseException : public std::exception {
public:
    ParseException(int number) : number(number) {};
    virtual const char * what() const throw() {
        std::string msg = "Number not supported: ";
        msg += this->number;
        return msg.c_str();
    }
private:
    int number;
};

class Parser {
public:
    Parser() { this->init(); };

    /* Decide if parser applies */
    virtual bool check(const int number) const = 0;
    /* Find appropriate word & deduct number. */
    virtual std::string parse(int &number) = 0;

    void init() {
        std::ifstream fin(INPUT);;
        std::string name;
        int val;

        this->ones.push_back("zero");
        while(fin.good()) {
            fin >> val >> name;
            this->ones.push_back(name);
        }

        fin.clear();
        std::getline(fin, name);

        while(fin.good()) {
            fin >> val >> name;
            tens.push_back(name);
        }
    }
protected:
    std::vector<std::string> ones;
    std::vector<std::string> tens;
};

class ParseThousands : public Parser {
public:
    ParseThousands() : Parser() {};

    bool check(const int number) const {
        return number > 999 && number < 1000000;
    }
    std::string parse(int &number) {
        number -= 1000;
        return "one thousand";
    }
};

class ParseHundreds : public Parser {
public:
    ParseHundreds() : Parser() {};

    bool check(const int number) const {
        return number > 99 && number < 1000;
    }
    std::string parse(int &number) {
        int num_hundreds = number / 100;
        std::string words = this->ones[num_hundreds] + " hundred";
        number -= num_hundreds * 100;

        if (number != 0) {
            words += " and ";
        }
        return words;
    }
};

class ParseTens : public Parser {
public:
    ParseTens() : Parser() {};

    bool check(const int number) const {
        return number > 0 && number < 100;
    }
    std::string parse(int &number) {
        std::string words;
        if (number > 19) {
            int ten_pos = (number / 10) - 2;
            int ones_pos = number % 10;

            words += this->tens[ten_pos];
            if (ones_pos > 0) {
                words += "-" + this->ones[ones_pos];
            }
        } else {
            words += this->ones[number];
        }

        number = 0;
        return words;
    }
};

class NumToWords {
public:
    NumToWords() {
        list.push_back(&this->parseThousands);
        list.push_back(&this->parseHundreds);
        list.push_back(&this->parseTens);
    };

    std::string to_words(int number) {
        std::string words;

        if (number < 1 || number > 1000) {
            ParseException exc(number);
            throw exc;
        }

        /* Call applicable parsers. */
        for (std::vector<Parser *>::iterator itr = list.begin();
                itr != list.end(); ++itr) {
            if ((*itr)->check(number)) {
                words += (*itr)->parse(number);
            }
        }

        return words;
    }

private:
    ParseTens parseTens;
    ParseHundreds parseHundreds;
    ParseThousands parseThousands;
    std::vector<Parser *> list;
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
    NumToWords convert;

    ASSERT_STREQ("three hundred and forty-two", convert.to_words(342).c_str());
    ASSERT_STREQ("one hundred and fifteen", convert.to_words(115).c_str());
    ASSERT_STREQ("two hundred", convert.to_words(200).c_str());
}

TEST(Euler017, TestUnderHundred) {
    NumToWords convert;

    ASSERT_STREQ("twenty", convert.to_words(20).c_str());
    ASSERT_STREQ("twenty-one", convert.to_words(21).c_str());
    ASSERT_STREQ("thirty-five", convert.to_words(35).c_str());
    ASSERT_STREQ("seventy-eight", convert.to_words(78).c_str());
    ASSERT_STREQ("ninety-nine", convert.to_words(99).c_str());
}

TEST(Euler017, TestUnderTwenty) {
    NumToWords convert;
    std::string actual, expect[] = {"zero", "one", "two", "three", "four", "five",
                            "six", "seven", "eight", "nine", "ten",
                            "eleven", "twelve", "thirteen", "fourteen", "fifteen",
                            "sixteen", "seventeen", "eighteen", "nineteen", "twenty"};

    for (int i = 1; i < 11; ++i) {
        actual = convert.to_words(i);
        ASSERT_STREQ(expect[i].c_str(), actual.c_str());
    }
}

TEST(Euler017, ExceptionalCases) {
    NumToWords convert;

    ASSERT_THROW(convert.to_words(0), ParseException);
    ASSERT_THROW(convert.to_words(10000), ParseException);
}

TEST(Euler017, FinalAnswer) {
    NumToWords convert;
    long count = 0;

    for (int i = 1; i < 1001; ++i) {
        std::string words = convert.to_words(i);
        count += count_chars(words);
    }

    ASSERT_EQ(count, 21124);
    cout << "The number of characters is: " << count << endl;
}

