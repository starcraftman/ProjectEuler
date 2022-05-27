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
1.5) Map all anagrams to a group with same HASH first, that is, all the words are anagrams.
     Next map all anagrams to a PATTERN, the pattern may differ by anagram depending on arrangement.
2) Identify the maximum length of a word in the file, LEN_MAX.
3) Compute all square numbers that contain digits <= LEN_MAX.
4) Group the above square numbers into a hash_map by PATTERN.
5) To find the largest number, iterate over all groupings of words and attempt to
   create square anagram pairs by starting with first word and a known square number that fits.
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
typedef std::size_t num_t;
static const std::string INPUT = "./src/input_e098.txt";
static const std::string CACHE_SQUARES = "./cache.98.private";
static const std::vector<char> letters = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

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


class AnagramContainer {
public:
    inline
    void add_word(const std::string &word);
    num_t read_file(const std::string &filename);
    num_t longest_pair_size() const;
    inline
    std::vector<std::string> get_by_hash(const std::string &key) const;

    friend std::ostream & operator<<(std::ostream &os, const AnagramContainer &anagrams);

    // Data
    // hash_word => list of similar hash strings
    std::map<std::string, std::vector<std::string> > group_by_hash;
    // pattern_word => list of patterned strings
    std::map<std::string, std::vector<std::string> > group_by_pattern;
};

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
    ss >> word;
    while (ss.good()) {
        add_word(word);
        ss >> word;
    }

    return longest_pair_size();
}

/**
 * @brief Look over existing anagram patterns and find all groups with at least a pair of anagrams.
 *  Find the anagram pair with longest char length and return that length.
 *
 */
std::size_t AnagramContainer::longest_pair_size() const {
    std::size_t max = 0;

    for (auto group : group_by_pattern) {
        if (group.second.size() >= 2 && group.second[0].size() > max) {
            max = group.second[0].size();
        }
    }

    return max;
}


std::vector<std::string> AnagramContainer::get_by_hash(const std::string &key) const {
    auto found = group_by_hash.find(key);
    if (found != group_by_hash.end()) {
        return found->second;
    } else {
        return std::vector<std::string>();;
    }
}

std::ostream & operator<<(std::ostream &os, const AnagramContainer &anagrams) {
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

class SquaresContainer {
public:
    inline
    void add_square(num_t number);
    void generate_below_length(num_t len);
    void generate_range(num_t start, num_t end);
    void save(const std::string &filename) const;
    void load(const std::string &filename);
    bool is_square(num_t number) const;
    inline
    std::vector<num_t> get_by_pattern(const std::string &pattern) const;

    friend std::ostream & operator<<(std::ostream &os, const SquaresContainer &squares);

    // Data
    std::map<std::string, std::vector<num_t> > group_by_pattern;
};

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
    num_t stop = 9;
    while (--len > 0) {
        stop = stop * 10 + 9;
    }

    while (true) {
        base++;
        num_t squared = (num_t) std::pow(base, 2);
        if (squared > stop) {
            break;
        }
        add_square(squared);
    };
}

void SquaresContainer::generate_range(num_t start, num_t end) {
    double base = start;
    double base_end = end;

    while (base < base_end) {
        num_t squared = (num_t) std::pow(base, 2);
        add_square(squared);
        ++base;
    };
}

/**
 * @brief Return the numbers matching the pattern requested.
 *
 * @param pattern The pattern of the word.
 */
std::vector<num_t> SquaresContainer::get_by_pattern(const std::string &pattern) const {
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
void SquaresContainer::save(const std::string &filename) const {
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

bool SquaresContainer::is_square(num_t number) const {
    std::string temp = std::to_string(number);
    auto found_nums = group_by_pattern.find(pattern_word(temp));
    if (found_nums != group_by_pattern.end()) {
        auto found = std::find(found_nums->second.begin(), found_nums->second.end(), number);
        return found != found_nums->second.end();
    }

    return false;
}

std::ostream & operator<<(std::ostream &os, const SquaresContainer &squares) {
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
 * To use this class to map out possible solutions:
 *  1) Set first word and chosen square, i.e. POST => 1024
 *  2) Add all other words that are anagrams of POST to set.
 *     During this addition, the values of the numbers will be computed and checked if square.
 */
class Solution {
public:
    explicit Solution(const SquaresContainer &squares) : squares(squares), best_square(0) {};
    explicit Solution(const Solution &other) : squares(other.squares) {
        if (this != &other) {
            *this = other;
        }
    }
    explicit Solution(Solution &&other) : squares(other.squares) {
        if (this != &other) {
            *this = std::move(other);
        }
    }

    void set_first(const std::string &word, num_t num);
    num_t set_word(const std::string &word);
    // Determine if a square anagram is present.
    // At least two anagrams must map to square numbers.
    inline
    num_t is_square_anagram();
    Solution & operator=(Solution &&other);
    Solution & operator=(const Solution &other);

    friend std::ostream & operator<<(std::ostream &os, const Solution &squares);

    // Data
    const SquaresContainer &squares;
    std::string first_word;
    num_t best_square;
    std::map<char, num_t> char_to_num;
    std::map<std::string, num_t> word_nums;
};

void Solution::set_first(const std::string &word, num_t num) {
    word_nums[word] = num;
    first_word = word;
    best_square = 0;

    // Set the map for the first word to write others.
    auto ritr = word.crbegin();
    while (ritr != word.crend()) {
        char_to_num[*ritr] = num % 10;
        num /= 10;
        ritr++;
    }
}
num_t Solution::set_word(const std::string &word) {
    num_t num = 0;
    bool leading_zero = false;
    for (auto char_ : word) {
        num = num * 10 + char_to_num[char_];
        if (num == 0) {
            leading_zero = true;
        }
    }

    // N.B. Stipulation of problem, leading zeroes aren't to be counted.
    //      Technically in computer you are losing the 0 place.
    if (!leading_zero && squares.is_square(num) && num > best_square) {
        best_square = num;
    }
    word_nums[word] = num;
    return num;
}

// Determine if a square anagram is present.
// At least two anagrams must map to square numbers.
num_t Solution::is_square_anagram() {
    // best_square is set to max square seen that IS NOT first_word square. If not 0, have anagram group.
    if (best_square) {
        return std::max(best_square, word_nums[first_word]);
    }

    return 0;
}

Solution & Solution::operator=(const Solution &other) {
    if (this == &other) {
        return *this;
    }

    first_word = other.first_word;
    best_square = other.best_square;
    char_to_num.clear();
    for (auto pair : other.char_to_num) {
        char_to_num[pair.first] = pair.second;
    }
    word_nums.clear();
    for (auto pair : other.word_nums) {
        word_nums[pair.first] = pair.second;
    }

    return *this;
}

Solution & Solution::operator=(Solution &&other) {
    if (this == &other) {
        return *this;
    }

    std::swap(other.best_square, this->best_square);
    std::swap(other.first_word, this->first_word);
    std::swap(other.char_to_num, this->char_to_num);
    std::swap(other.word_nums, this->word_nums);

    return *this;
}

std::ostream & operator<<(std::ostream &os, const Solution &sol) {
    os << "Base Word: " << sol.first_word << endl;
    os << "Base Map: " << endl;
    std::string pad = "    ";
    for (auto pair : sol.char_to_num) {
        os << pad << pair.first << " => " << pair.second << endl;
    }

    for (auto pair : sol.word_nums) {
        os << pair.first << " -> " << pair.second << " is square: " << sol.squares.is_square(pair.second) << endl;
    }

    return os;
}

num_t find_possible_square_pairs(const AnagramContainer &anagrams, const SquaresContainer &squares,
        const std::string &hash_key, Solution &r_sol) {
    num_t best = 0;
    auto words = anagrams.get_by_hash(hash_key);

    for (std::size_t i = 0; i < words.size(); ++i) {
        std::string first = words.back();
        words.pop_back();
        const auto &squares_first = squares.get_by_pattern(pattern_word(first));
        Solution sol(squares);

        for (auto square : squares_first) {
            sol.set_first(first, square);
            for (auto word : words) {
                sol.set_word(word);
            }

            if (sol.is_square_anagram() && sol.is_square_anagram() > best) {
                best = sol.is_square_anagram();
                r_sol = sol;
            }
        }

        words.insert(words.begin(), first);
    }

    return best;
}

/**
 * @brief Solve the problem.
 *
 * @return The largest number that fit into an anagram pair.
 */
num_t largest_square_number(Solution &best_sol) {
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
    for (auto group : anagrams.group_by_hash) {
        if (group.second.size() < 2) {
            continue;
        }

        Solution sol(squares);
        num_t best = find_possible_square_pairs(anagrams, squares, group.first, sol);
        if (best != 0 && best > biggest_square) {
            biggest_square = best;
            best_sol = sol;
        }
    }

    return best_sol.is_square_anagram();
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

TEST(Euler098, SquaresGenerateRange) {
    SquaresContainer squares;
    squares.generate_range(1, 100);

    std::vector<num_t> expect = {16, 25, 36, 49, 64, 81};
    ASSERT_EQ(squares.group_by_pattern["AB"], expect);
}

TEST(Euler098, SquaresGenerateBelow) {
    SquaresContainer squares;
    squares.generate_below_length(5);

    std::vector<num_t> expect = {1225, 2116, 4225, 5776, 6889, 7225};
    ASSERT_EQ(squares.group_by_pattern["ABBC"], expect);
}

TEST(Euler098, SquaresIsSquare) {
    SquaresContainer squares;
    squares.generate_range(1, 2000);

    ASSERT_EQ(squares.is_square(1111), 0);
    ASSERT_EQ(squares.is_square(1024), 1);
}

TEST(Euler098, SolutionSetFirst) {
    SquaresContainer squares;
    squares.generate_range(1, 2000);
    Solution sol(squares);
    sol.set_first("POST", 1024);

    ASSERT_EQ(sol.word_nums["POST"], 1024);
    ASSERT_EQ(sol.first_word, "POST");
    ASSERT_EQ(sol.char_to_num['P'], 1);
}

TEST(Euler098, SolutionSetWord) {
    SquaresContainer squares;
    squares.generate_range(1, 2000);
    Solution sol(squares);
    sol.set_first("POST", 1024);

    ASSERT_EQ(sol.set_word("STOP"), 2401);
    ASSERT_EQ(sol.word_nums["POST"], 1024);
    ASSERT_EQ(sol.word_nums["STOP"], 2401);
}

TEST(Euler098, SolutionIsSquareAnagram) {
    SquaresContainer squares;
    squares.generate_range(1, 2000);

    Solution sol(squares);
    sol.set_first("POST", 1024);
    sol.set_word("STOP");
    ASSERT_TRUE(sol.is_square_anagram());

    Solution sol2(squares);
    sol2.set_first("POST", 1029);
    sol2.set_word("STOP");
    ASSERT_FALSE(sol2.is_square_anagram());
}

TEST(Euler098, SolutionOutputStream) {
    SquaresContainer squares;
    squares.generate_range(1, 2000);
    Solution sol(squares);
    sol.set_first("POST", 1024);

    std::stringstream ss;
    ss << sol;

    ASSERT_NE(
            ss.str().find("POST -> 1024 is square: 1"),
            std::string::npos
    );
}

TEST(Euler098, SolutionOperatorAssign) {
    SquaresContainer squares;
    squares.generate_range(1, 2000);
    Solution sol(squares);
    sol.set_first("POST", 1024);
    sol.set_word("STOP");
    Solution sol2(squares);
    sol2.set_first("CARE", 4096);
    sol2.set_word("RACE");

    sol = sol2;
    cout << sol << endl << sol2;
}

TEST(Euler098, SolutionOperatorMove) {
    SquaresContainer squares;
    squares.generate_range(1, 2000);
    Solution sol(squares);
    sol.set_first("POST", 1024);
    sol.set_word("STOP");
    Solution sol2(squares);
    sol2.set_first("CARE", 4096);
    sol2.set_word("RACE");

    cout << sol << endl << sol2;
    cout << "Move sol2 into sol" << endl;
    sol = std::move(sol2);
    cout << sol << endl << sol2;
}

TEST(Euler098, FindPossibleSquarePairs) {
    AnagramContainer anagrams;
    anagrams.add_word("POST");
    anagrams.add_word("STOP");
    anagrams.add_word("SPOT");

    SquaresContainer squares;
    squares.generate_below_length(4);

    Solution sol(squares);
    auto result = find_possible_square_pairs(anagrams, squares, hash_word("POST"), sol);
    ASSERT_EQ(result, 9604);
}

TEST(Euler098, FinalAnswer) {
    SquaresContainer squares;
    Solution sol(squares);

    auto result = largest_square_number(sol);
    ASSERT_EQ(result, 18769);
    cout << sol << endl;
}
