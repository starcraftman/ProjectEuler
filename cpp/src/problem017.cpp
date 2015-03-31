/**
If the numbers 1 to 5 are written out in words: one, two, three, four, five,
then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.

If all the numbers from 1 to 1000 (one thousand) inclusive were written out in
words, how many letters would be used?

NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and
forty-two) contains 23 letters and 115 (one hundred and fifteen) contains
20 letters. The use of "and" when writing out numbers is in compliance
with British usage.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <sstream>
#include <string> /* C++ String class. */
#include <exception>

#include "gtest/gtest.h"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using std::string;

/***************** Constants & Macros *********************/
static const char * const UNDER_TWENTY = "\
1 one \
2 two \
3 three \
4 four \
5 five \
6 six \
7 seven \
8 eight \
9 nine \
10 ten \
11 eleven \
12 twelve \
13 thirteen \
14 fourteen \
15 fifteen \
16 sixteen \
17 seventeen \
18 eighteen \
19 nineteen";

static const char * const UNDER_HUNDRED = "\
20 twenty \
30 thirty \
40 forty \
50 fifty \
60 sixty \
70 seventy \
80 eighty \
90 ninety";

/****************** Class Definitions *********************/
class ParseException : public std::exception {
public:
    ParseException(unsigned long number) : number(number) {};
    virtual const char * what() const throw() {
        std::string msg = "Number not supported: ";
        msg += this->number;
        return msg.c_str();
    }
private:
    unsigned long number;
};

class Parser {
public:
    Parser() { this->init(); };

    /* Decide if parser applies */
    virtual bool check(const unsigned long number) const = 0;
    /* Find appropriate word & deduct number. */
    virtual std::string parse(unsigned long &number) = 0;

    void init() {
        std::stringstream ones(UNDER_TWENTY), tens(UNDER_HUNDRED);
        std::string name;
        int val;

        this->ones.push_back("zero");
        while(ones.good()) {
            ones >> val >> name;
            this->ones.push_back(name);
        }

        while(tens.good()) {
            tens >> val >> name;
            this->tens.push_back(name);
        }
    }
protected:
    std::vector<std::string> ones;
    std::vector<std::string> tens;
};

class ParseTens : public Parser {
public:
    ParseTens() : Parser() {};

    bool check(const unsigned long number) const {
        return number > 0 && number < 100;
    }
    std::string parse(unsigned long &number) {
        std::string words;
        if (number > 19) {
            unsigned long ten_pos = (number / 10) - 2;
            unsigned long ones_pos = number % 10;

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
    std::string get_zero(void) const {
        return this->ones[0];
    }
};

class ParseHundreds : public Parser {
public:
    ParseHundreds() : Parser() {};

    bool check(const unsigned long number) const {
        return number > 99 && number < 1000;
    }
    std::string parse(unsigned long &number) {
        unsigned long num_hundreds = number / 100;
        std::string words = this->ones[num_hundreds] + " hundred";
        number -= num_hundreds * 100;

        if (number != 0) {
            words += " and ";
        }
        return words;
    }
};

/* Parses anything beyond 999 */
class ParseBeyond : public Parser {
public:
    ParseBeyond(unsigned long divisor, const char * const term) :
        Parser(), divisor(divisor), term(term) {};

    bool check(const unsigned long number) const {
        return number > this->divisor -1 && number < this->divisor * 1000;
    }
    std::string parse(unsigned long &number) {
        static ParseHundreds huns;
        static ParseTens tens;
        std::string words;

        unsigned long num_term = number / this->divisor;
        if (huns.check(num_term)) {
            words += huns.parse(num_term);
        }
        if (tens.check(num_term)) {
            words += tens.parse(num_term);
        }

        words += " " + this->term;
        if ((number % this->divisor) != 0) {
            words += " ";
        }

        number -= (number/this->divisor) * this->divisor;
        return words;
    }
private:
    unsigned long divisor;
    std::string term;
};

class NumToWords {
public:
    NumToWords(): maximum(1000000000) {
        static ParseTens tens;
        static ParseHundreds hundreds;
        static ParseBeyond thousands(1000, "thousand");
        static ParseBeyond millions(1000000, "million");

        list.push_back(&millions);
        list.push_back(&thousands);
        list.push_back(&hundreds);
        list.push_back(&tens);
    };

    std::string to_words(unsigned long number) {
        std::string words;

        if (number >= this->maximum) {
            ParseException exc(number);
            throw exc;
        }

        if (number == 0) {
            static ParseTens tens;
            return tens.get_zero();
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
    std::vector<Parser *> list;
    unsigned long maximum;
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

TEST(Euler017, TestUnderBillion) {
    NumToWords convert;

    std::string expect("one hundred and eighty-seven million six hundred and ");
    expect += "forty-three thousand nine hundred and sixty-six";
    ASSERT_STREQ(expect.c_str(), convert.to_words(187643966).c_str());
}

TEST(Euler017, TestUnderMillion) {
    NumToWords convert;

    ASSERT_STREQ("fifty-four thousand two hundred and sixty-six", convert.to_words(54266).c_str());
    ASSERT_STREQ("six hundred and twenty-one thousand nine hundred and thirty-eight",
            convert.to_words(621938).c_str());
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

    for (int i = 0; i < 20; ++i) {
        actual = convert.to_words(i);
        ASSERT_STREQ(expect[i].c_str(), actual.c_str());
    }
}

TEST(Euler017, ExceptionalCases) {
    NumToWords convert;

    ASSERT_THROW(convert.to_words(100000000000000), ParseException);
}

TEST(Euler017, FinalAnswer) {
    NumToWords convert;
    long count = 0;

    for (int i = 1; i <= 1000; ++i) {
        std::string words = convert.to_words(i);
        count += count_chars(words);
    }

    cout << "The number of characters is: " << count << endl;
    ASSERT_EQ(count, 21124);
}

