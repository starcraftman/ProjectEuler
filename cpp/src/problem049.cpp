/**
The arithmetic sequence, 1487, 4817, 8147, in which each of the terms increases by 3330, is unusual in two ways: (i) each of the three terms are prime, and, (ii) each of the 4-digit numbers are permutations of one another.

There are no arithmetic sequences made up of three 1-, 2-, or 3-digit primes, exhibiting this property, but there is one other 4-digit increasing sequence.

What 12-digit number do you form by concatenating the three terms in this sequence?
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

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
std::vector<int> generate_prime_list(int min, int max) {
    std::vector<int> primes = util::sieve_erat(max);

    auto itr = primes.begin();
    while (*itr < min) {
        ++itr;
    }
    primes.erase(primes.begin(), itr);

    return primes;
}

int sort_digits(int num) {
    std::vector<int> digits;;
    while (num != 0) {
        digits.push_back(num % 10);
        num /= 10;
    }
    std::sort(digits.begin(), digits.end());

    auto iter = digits.cbegin();
    int sorted_num = *iter;
    while (++iter != digits.cend()) {
        sorted_num = sorted_num * 10 + *iter;
    }

    return sorted_num;
}

std::string search_permutation_gap(const std::vector<int> &primes) {
    // Find ends for inner loops
    auto two_before_end = primes.cbegin();
    auto before_end = primes.cbegin();
    before_end++;
    auto end = primes.cbegin();
    end++;
    end++;

    while (end != primes.cend()) {
        two_before_end++;
        before_end++;
        end++;
    }
    // Inner stopping positions are located, proceed to iterate.

    std::string found;
    auto first = primes.cbegin();
    while (first != two_before_end) {
        auto second = first;

        while (++second != before_end) {
            auto third = second;

            while (++third != end) {
                if ( (*second - *first) == (*third - *second) ) {
                    found = std::to_string(*first) + std::to_string(*second) + std::to_string(*third);
                    goto end_loops;
                }
            }
        }

        ++first;
    }

end_loops:
    return found;
}

// primes_map, map of the SORTED digits, to prime permutations.
std::vector<std::string> search_prime_permutations(const std::vector<int> &primes) {
    std::map<int, std::vector<int>> primes_map;

    for (auto prime: primes) {
        primes_map[prime] = std::vector<int>();
        int sorted_prime = sort_digits(prime);
        primes_map[sorted_prime].push_back(prime);
    }
    // All prime permutations grouped

    std::vector<std::string> found;
    for (auto apair : primes_map) {
        if (apair.second.size() < 3) {
            continue;
        }

        std::string result = search_permutation_gap(apair.second);
        if (result != "") {
            found.push_back(result);
        }
    }

    return found;
}

std::string prime_permutation_string() {
    auto primes = generate_prime_list(1000, 10000);
    std::vector<std::string> found = search_prime_permutations(primes);
    for (auto word : found) {
        if (word.find("1487") == std::string::npos) {
            return word;
        }
    }

    return "0";
}

TEST(Euler049, GenPrimeList) {
    std::vector<int> primes = generate_prime_list(1000, 10000);
    ASSERT_EQ(primes.front(), 1009);
    ASSERT_EQ(primes.back(), 9973);
}

TEST(Euler049, SortDigits) {
    ASSERT_EQ(sort_digits(9973), 3799);
}

TEST(Euler049, SearchPermutationGap) {
    std::vector<int> primes = {1487, 1847, 4817, 4871, 7481, 7841, 8147, 8741};
    std::string found = search_permutation_gap(primes);
    std::string expect = "148748178147";
    ASSERT_EQ(found, expect);
}

TEST(Euler054, PrimePermutationsConcatenation) {
    std::string result = prime_permutation_string();
    cout << "The prime permutations string is: " << result << endl;
    ASSERT_EQ(result, std::string("296962999629"));
}
