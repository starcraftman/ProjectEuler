/**
By replacing each of the letters in the word CARE with 1, 2, 9, and 6 respectively,
we form a square number: 1296 = 362.
What is remarkable is that, by using the same digital substitutions,
the anagram, RACE, also forms a square number: 9216 = 962.
We shall call CARE (and RACE) a square anagram word pair and
specify further that leading zeroes are not permitted, neither may a different letter have the
same digital value as another letter.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text file containing nearly two-thousand common English words, find all the square anagram word pairs (a palindromic word is NOT considered to be an anagram of itself).

What is the largest square number formed by any member of such a pair?

NOTE: All anagrams formed must be contained in the given text file.
=============================
This problem requires a multistage solution.
1) Identify all anagram groupings within the word file provided as input.
1.5) Group all the anagram groupings into a hashmap based on word length to list of groups.
2) Identify the maximum length of a word in the file, LEN_MAX.
3) Compute all square numbers that contain digits <= LEN_MAX.
4) Group the above square numbers into a hash_map by digit length.
5) To find the largest number, iterate over all groupings of words and attempt to apply relevant digit masks over them.
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

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
typedef std::size_t num_t;
static const std::string INPUT = "./src/input_e098.txt";
static const std::string CACHE_SQUARES = "./cache.private";
static const std::vector<char> letters = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

class AnagramContainer {
public:
    inline
    void add_word(const std::string &word);
    num_t read_file(const std::string &filename);
    num_t longest_pair_size();

    friend std::ostream & operator<<(std::ostream &os, AnagramContainer anagrams);

    // Data
    // hash_word => list of similar hash strings
    std::map<std::string, std::vector<std::string> > group_by_hash;
    // pattern_word => list of patterned strings
    std::map<std::string, std::vector<std::string> > group_by_pattern;
};

class SquaresContainer {
public:
    inline
    void add_square(num_t number);
    void generate_below_length(num_t len);
    std::vector<num_t> get_group(const std::string &pattern);
    void save(const std::string &filename);
    void load(const std::string &filename);

    // Data
    std::map<std::string, std::vector<num_t> > group_by_pattern;
};

/**
 * @brief Map any string to a universal form pattern that describes it.
 *
 * Examples of this pattern mapping, string => string.
 * 1952 => ABCD
 * 1992 => ABBC
 * 1223 => ABBC
 * WILLING => ABCCBDE
 * CARE => ABCD
 *
 * @param pattern The string describing the pattern.
 */
std::string pattern_word(const std::string &word) {
    std::stringstream ss;
    std::map<num_t, char> chars_seen;
    auto letter_itr = letters.begin();
    for (auto letter : word) {
        try {
            chars_seen.at(letter);
        } catch (std::out_of_range &exc) {
            chars_seen[letter] = *letter_itr;
            ++letter_itr;
        }

        ss << chars_seen.at(letter);
    }

    return ss.str();
}

/**
 * @brief Hash a given number, all numbers that are anagrams will have same hash.
 *
 * @param num The number to create a hash for.
 */
std::string hash_number(num_t num) {
    std::vector<num_t> digits;
    while (num != 0) {
        digits.push_back(num % 10);
        num /= 10;
    }
    std::sort(digits.begin(), digits.end());

    std::stringstream ss;
    for (auto digit : digits) {
        ss << digit;
    }

    return ss.str();
}

/**
 * @brief Two words have the same hash if they have the same characters in them.
 * For example:
 *    care, race, acre all have the same hash, acer
 *
 * @param word The string to look up.
 */
std::string hash_word(const std::string &word) {
    std::vector<char> letters;
    std::copy(word.begin(), word.end(), std::back_inserter(letters));
    std::sort(letters.begin(), letters.end());

    std::stringstream ss;
    for (auto letter : letters) {
        ss << letter;
    }

    return ss.str();
}

/**
 * @brief The number of digits in a number
 *
 * @tparam T Any integer number class.
 * @param num The number to count digits of.
 * @return The number of digits.
 */
template <class T>
num_t num_digits(T num) {
    num_t number = (num_t) num;
    num_t cnt = 0;
    while (number != 0) {
        cnt++;
        number /= 10;
    };

    return cnt;
}

/**
 * @brief Add a word to the maps. Mainly a shorthand.
 *
 *  See the associated hashing functions for more information.
 *
 * @param word The string to be added to the maps.
 */
void AnagramContainer::add_word(const std::string &word) {
    group_by_hash[hash_word(word)].push_back(word);
    group_by_pattern[pattern_word(word)].push_back(word);
}

/**
 * @brief Take the input file and read in all potential word candidates.
 *
 * Filename has following general format:
 *      "WORD","WORD2", .....
 *
 * @param filename The filename to read.
 */
num_t AnagramContainer::read_file(const std::string &filename) {
    std::string word;
    std::ifstream fin(filename);
    fin >> word;

    std::replace(word.begin(), word.end(), ',', ' ');
    std::replace(word.begin(), word.end(), '"', ' ');

    std::stringstream ss(word);
    while (ss.good()) {
        ss >> word;
        add_word(word);
    }

    return longest_pair_size();
}

/**
 * @brief Look over existing anagram patterns and find all groups with at least a pair of anagrams.
 *  Find the anagram pair with longest char length and return that length.
 *
 */
std::size_t AnagramContainer::longest_pair_size() {
    std::size_t max = 0;

    for (auto group : group_by_pattern) {
        if (group.second.size() >= 2 && group.second[0].size() > max) {
            max = group.second[0].size();
        }
    }

    return max;
}

/**
 * @brief Add a single squared number to the container.
 *
 * @param number The squared number to add.
 */
void SquaresContainer::add_square(num_t number) {
    std::string key = pattern_word(std::to_string(number));
    group_by_pattern[key].push_back(number);
}

/**
 * @brief Generate all possible squares with a digit length less than len.
 *
 * @param len The cutoff for digit length.
 */
void SquaresContainer::generate_below_length(num_t len) {
    double base = 0;

    while (true) {
        base++;
        num_t squared = (num_t) std::pow(base, 2);
        add_square(squared);
        if (num_digits(squared) > len) {
            break;
        }
    };
}

/**
 * @brief Return the numbers matching the pattern requested.
 *
 * @param pattern The pattern of the word.
 */
std::vector<num_t> SquaresContainer::get_group(const std::string &pattern) {
    auto found = group_by_pattern.find(pattern);
    if (found != group_by_pattern.end()) {
        return found->second;
    } else {
        return std::vector<num_t>();;
    }
}

/**
 * @brief Save a cache of the computed squares for subsequent runs.
 *
 * @param filename The filename of the cache.
 */
void SquaresContainer::save(const std::string &filename) {
    std::ofstream fout(filename);

    for (auto group : group_by_pattern) {
        fout << group.first << " ";
        for (auto square : group.second) {
            fout << square << " ";
        }
        fout << endl;
    }
}

/**
 * @brief Load the squares from the cache.
 *
 * @param filename The filename of the cache.
 */
void SquaresContainer::load(const std::string &filename) {
    std::ifstream fin(filename);
    std::string line;

    while (fin.good()) {
        std::getline(fin, line);
        std::stringstream ss(line);

        std::string word, pattern;
        ss >> pattern;
        while (ss.good()) {
            ss >> word;
            num_t num = std::stoul(word);
            if (num != 0) {
                group_by_pattern[pattern].push_back(num);
            }
        }

    };
}

std::ostream & operator<<(std::ostream &os, AnagramContainer anagrams) {
    os << "Anagrams" << endl
        << "========" << endl;
    for (auto anagram : anagrams.group_by_hash) {
        os << "Hash: " << anagram.first << endl << "  ";
        for (auto word : anagram.second) {
            os << word << ", ";
        }
        os << endl;
    }

    os << endl << "Patterns" << endl
        << "========" << endl;
    for (auto pattern : anagrams.group_by_pattern) {
        os << "Pattern: " << pattern.first << endl << "  ";
        for (auto word : pattern.second) {
            os << word << ", ";
        }
        os << endl;
    }

    return os;
}

std::ostream & operator<<(std::ostream &os, SquaresContainer squares) {
    os << "Squares" << endl
        << "=======" << endl;
    for (auto square : squares.group_by_pattern) {
        os << "Pattern: " << square.first << endl << "  ";
        for (auto word : square.second) {
            os << word << ", ";
        }
        os << endl;
    }

    return os;
}

/**
 * @brief Solve the problem.
 *
 * @return The largest number that fit into an anagram pair.
 */
num_t largest_square_number() {
    AnagramContainer anagrams;
    num_t max = anagrams.read_file(INPUT);

    SquaresContainer squares;
    std::ifstream check(CACHE_SQUARES);
    if (check.good()) {
        squares.load(CACHE_SQUARES);
    } else {
        // TODO: Can this go faster? Apart from just parallelize.
        squares.generate_below_length(max);
        squares.save(CACHE_SQUARES);
    }

    num_t biggest_square = 0;
    // Have all the information, now compare patterns.
    for (auto group : anagrams.group_by_hash) {
        if (group.second.size() < 2) {
            continue;
        }

        // FIXME: Broken
        // cout << "Found: " << group.first << endl;
        // for (auto word : group.second) {
            // auto possible_squares = squares.get_group(pattern_word(word));
            // num_t square_for_word = *std::max_element(possible_squares.begin(), possible_squares.end());
            // if (square_for_word > biggest_square) {
                // cout << "Word: " << word << " Square: " << square_for_word << " is square of: " << std::sqrt((double) square_for_word) << endl;
                // biggest_square = square_for_word;
            // }
        // }
    }

    return biggest_square;
}

TEST(Euler098, HashNumber) {
    std::string expect("34799");
    ASSERT_EQ(hash_number(99734), expect);
}

TEST(Euler098, HashWord) {
    std::string expect("ACER");
    const std::string input("RACE");
    ASSERT_EQ(hash_word(input), expect);
}

TEST(Euler098, PatternWord) {
    std::string expect("ABCD");
    ASSERT_EQ(pattern_word(std::string("1952")), expect);
    expect = "ABBC";
    ASSERT_EQ(pattern_word(std::string("1992")), expect);
    expect = "ABCCBDE";
    ASSERT_EQ(pattern_word(std::string("WILLING")), expect);
}

TEST(Euler098, NumDigits) {
    ASSERT_EQ(num_digits(45432), 5);
}

TEST(Euler098, AnagramCreate) {
    AnagramContainer anagrams;
    anagrams.add_word(std::string("CARE"));
    anagrams.add_word(std::string("ACRE"));
    anagrams.add_word(std::string("RACE"));
    anagrams.add_word(std::string("WILLING"));

    std::string look = "ABCD";
    auto found = anagrams.group_by_pattern.find(look);
    ASSERT_EQ(found->second[0], "CARE");
}

TEST(Euler098, AnagramLongestPair) {
    AnagramContainer anagrams;
    anagrams.add_word(std::string("CARE"));
    anagrams.add_word(std::string("ACRE"));
    anagrams.add_word(std::string("RACE"));
    anagrams.add_word(std::string("WILLING"));

    ASSERT_EQ(anagrams.longest_pair_size(), 4);
}

TEST(Euler098, AnagramReadFile) {
    AnagramContainer anagrams;
    anagrams.read_file(INPUT);

    std::string look = "ABCDEFGHIJ";
    auto found = anagrams.group_by_pattern.find(look);
    ASSERT_EQ(found->second[0], "BACKGROUND");
}

TEST(Euler098, SquaresCreate) {
    SquaresContainer squares;
    squares.add_square(1296);
    squares.add_square(9216);
    squares.add_square(25);
    squares.add_square(36);

    cout << squares << endl;
}

TEST(Euler098, SquaresGenerateBelow) {
    SquaresContainer squares;
    squares.generate_below_length(5);

    cout << squares << endl;
}

TEST(Euler098, FinalAnswer) {
    int result = largest_square_number();
    cout << "The prime with longest sum: " << result << endl;
    ASSERT_EQ(result, 0);
}
