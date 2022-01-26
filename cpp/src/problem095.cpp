/**
The proper divisors of a number are all the divisors excluding the number itself.
For example, the proper divisors of 28 are 1, 2, 4, 7, and 14. As the sum of these divisors
is equal to 28, we call it a perfect number.

Interestingly the sum of the proper divisors of 220 is 284 and the sum of the proper
divisors of 284 is 220, forming a chain of two numbers. For this reason, 220 and 284
are called an amicable pair.

Perhaps less well known are longer chains. For example, starting with 12496,
we form a chain of five numbers:

12496 → 14288 → 15472 → 14536 → 14264 (→ 12496 → ...)

Since this chain returns to its starting point, it is called an amicable chain.

Find the smallest member of the longest amicable chain with no element exceeding one million.
// TODO: Can this be improved with caching? I'm not certain
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
typedef std::uint64_t num_t;
const num_t MAX_SEEN = 50;
std::map<num_t, std::pair<std::size_t, num_t> > cache;

class NoChainFound : public std::exception {
public:
    explicit NoChainFound(num_t number) :
        number(number) {}
    virtual const char * what() const throw() {
        return "Failed to find a chain in time.";
    }
    num_t number;
private:
};

inline
num_t sum_divisors(num_t num) {
    auto divs = util::find_divisors(num, true);
    return std::accumulate(divs.begin(), divs.end(), 0);
}

class ChainTrack {
public:
    ChainTrack() : lowest(0) {}
    explicit ChainTrack(num_t initial) : lowest(initial) {
        ordered.push_back(initial);
    }
    void add(num_t num) {
        if (num > 1'000'000 || cycle_too_long()) {
            throw NoChainFound(num);
        }

        if (num < lowest || lowest == 0) {
            lowest = num;
        }
        // Stop adding once amicable chain formed
        if (ordered.front() == num) {
            amicable = true;
        } else {
            ordered.push_back(num);
        }
    }
    bool cycle_too_long() const {
        return ordered.size() > MAX_SEEN;
    }
    std::size_t cycle_len() const {
        return ordered.size();
    }

    //data
    std::vector<num_t> ordered;
    num_t lowest = 0;
    bool amicable = false;
};

/*
 * Keeps searching for an amicable chain until
 * 1) found amicable chain
 * 2) an element in chain is OVER 1'000'000
 * 3) the max size of the chain is reached
 * In last two cases, an exception will divert normal return
 */
ChainTrack find_chain(num_t num) {
    ChainTrack tracker(num);
    do {
        num_t next = sum_divisors(num);;
        tracker.add(next);
        num = next;
    } while (!tracker.amicable);

    return tracker;
}

TEST(Euler095, FindDivisors) {
    auto divs = util::find_divisors(28, true);
    int sum = std::accumulate(divs.begin(), divs.end(), 0);
    ASSERT_EQ(divs.size(), 5);
    ASSERT_EQ(sum, 28);
}

TEST(Euler095, SumDivisors) {
    int sum = sum_divisors(28);
    ASSERT_EQ(sum, 28);
}

TEST(Euler095, FindChain) {
    auto res = find_chain(12496);
    ASSERT_EQ(res.cycle_len(), 5);
    ASSERT_EQ(res.lowest, 12496);
}

TEST(Euler095, FindChainFail) {
    try {
        auto res = find_chain(138);
        ASSERT_TRUE(false);
    } catch (NoChainFound &e) {
        ASSERT_TRUE(true);
    }
}

TEST(Euler095_ChainTrack, Constructor) {
    ChainTrack track(220);
    ASSERT_EQ(track.lowest, 220);
    ASSERT_EQ(track.cycle_len(), 1);
    ASSERT_FALSE(track.amicable);
}

TEST(Euler095_ChainTrack, Add) {
    ChainTrack track(220);
    track.add(284);
    ASSERT_EQ(track.lowest, 220);
    ASSERT_EQ(track.cycle_len(), 2);
    ASSERT_FALSE(track.amicable);
}

TEST(Euler095_ChainTrack, AddAmicable) {
    ChainTrack track(220);
    track.add(284);
    track.add(220);
    ASSERT_EQ(track.lowest, 220);
    ASSERT_EQ(track.cycle_len(), 2);
    ASSERT_TRUE(track.amicable);
}

TEST(Euler095, AmicableChains) {
    num_t num = 28;
    const num_t max = 1'000'000;

    ChainTrack best = find_chain(num);
    while (++num != max) {
        cout << "Num " << num << endl;
        try {
            auto result = find_chain(num);
            if (result.amicable && result.cycle_len() > best.cycle_len()) {
                best = result;
            }
        } catch (NoChainFound &e) {
            continue;
        }
    }

    cout << "Best amicable cycle found under: " << max << endl;
    cout << "Length of " << best.cycle_len() << " had lowest value of: " << best.lowest << endl;
    cout << "Elements of cycle (in no order):" << endl;
    for (auto ele : best.ordered) {
        cout << ele << ", ";
    }
    cout << endl;
    cout << "Next one from last for proof: " << sum_divisors(best.ordered.back()) << endl;
}
