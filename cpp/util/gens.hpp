#ifndef _GENS_HPP_
#define _GENS_HPP_

/********************* Header Files ***********************/
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstring>

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
    Coprimes(T limit) : limit(limit) , ind(0) {
        vals[a] = 0;
        vals[b] = 1;
        vals[c] = 1;
        vals[d] = limit;
    };

    typedef std::pair<T, T> CoPair;
    typedef std::vector<CoPair> CoPairs;

    // In Farey Sequence, two elements are rational fractions: a/b c/d.
    // Then there exists a/b < q/b < c/d given by:
    // k = floor((n + b)/d) ; p = cK - a, q = dK - c;
    // Arrays indexed in order, so cur_nums[0] = 'a'
    CoPair next() {
        T k = std::floor(double(limit + vals[b]) / (vals[d]));
        vals[old_a] = vals[a];
        vals[old_b] = vals[b];
        vals[a] = vals[c];
        vals[b] = vals[d];
        vals[c] = k * vals[c] - vals[old_a];
        vals[d] = k * vals[d] - vals[old_b];

        ind++;
        return CoPair(vals[a], vals[b]);
    }

    // Collect coprime pairs & return
    CoPairs collect() {
        CoPairs pairs;

        while (vals[c] <= limit) {
            pairs.push_back(next());
        }

        return pairs;
    }

    CoPair number() { return CoPair(vals[a], vals[b]); }
    T index() { return ind; }
private:
    enum ENTRIES { a = 0, b, c, d, old_a, old_b };
    T limit;
    T vals[6]; // Maps values from enum ENTRIES
    T ind;
};

} /* end util::gens */

} /* end util:: */

#endif /* _GENS_HPP_ */

