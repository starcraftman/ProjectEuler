#ifndef _GENS_HPP_
#define _GENS_HPP_

/********************* Header Files ***********************/
#include <vector>
#include <utility>

namespace util {

namespace gens {

/************** Class & Func Declarations *****************/
/* Starts at Fibonacci number 1. */
template <class T>
class Fibonacci {
public:
    Fibonacci() : two_back(0), one_back(1), num(1), ind(1) {};
    Fibonacci(T two, T one, T ind) : two_back(two), one_back(one),
            num(one + two), ind(ind) {};

    T next() {
        num = one_back + two_back;
        two_back = one_back;
        one_back = num;

        return num;
    }

    T number() { return num; }
    T index() { return ind; }
private:
    T two_back;
    T one_back;
    T num;
    T ind;
};

template <class T>
class Coprimes {
public:
    typedef std::pair<T, T> CoPair;
    typedef std::vector<CoPair> CoPairs;
    Coprimes(CoPair origin) : origin(origin) {};
    static Coprimes odd_even_coprimes() {
        return Coprimes(std::make_pair(2,1));
    }
    static Coprimes odd_odd_coprimes() {
        return Coprimes(std::make_pair(3,1));
    }

    // Collect max coprime pairs & return
    // TODO: Change to Farey Sequence, allows better ordering of generation
    std::vector<CoPair> collect(int max) {
        std::vector<CoPair> to_visit; // Use vec as stack
        to_visit.push_back(origin);
        std::vector<CoPair> pairs;
        pairs.push_back(origin);

        /* Using bfs, so results are fairly balanced */
        while (pairs.size() < max) {
            CoPair cur = to_visit.back(), child;
            to_visit.pop_back();

            child = std::make_pair(2 * cur.first - cur.second, cur.first);
            to_visit.insert(to_visit.begin(), child);
            pairs.push_back(child);
            child = std::make_pair(2 * cur.first + cur.second, cur.first);
            to_visit.insert(to_visit.begin(), child);
            pairs.push_back(child);
            child = std::make_pair(2 * cur.second + cur.first, cur.second);
            to_visit.insert(to_visit.begin(), child);
            pairs.push_back(child);
        }

        return pairs;
    }

private:
    CoPair origin;
};

} /* end util::gens */

} /* end util:: */

#endif /* _GENS_HPP_ */

