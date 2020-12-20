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
/* Starts at Fibonacci number 1.
 * Templated in case of bigint need. */
template <class T>
class Fibonacci {
public:
    Fibonacci() : two_back(0), one_back(1), num(1), ind(2) {};
    Fibonacci(T two, T one, T ind) : two_back(two), one_back(one),
            num(one + two), ind(ind) {};

    T next() {
        ++ind;
        T old = num;
        num = num + one_back;
        two_back = one_back;
        one_back = old;

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

class Coprimes {
public:
    Coprimes(int limit) : limit(limit) , ind(0) {
        vals[a] = 0;
        vals[b] = 1;
        vals[c] = 1;
        vals[d] = limit;
    };

    typedef std::pair<int, int> CoPair;
    typedef std::vector<CoPair> CoPairs;

    // In Farey Sequence, two elements are rational fractions: a/b c/d.
    // Then there exists a/b < q/b < c/d given by:
    // k = floor((n + b)/d) ; p = cK - a, q = dK - c;
    CoPair next() {
        int k = std::floor(double(limit + vals[b]) / (vals[d]));
        int old_a = vals[a];
        int old_b = vals[b];
        vals[a] = vals[c];
        vals[b] = vals[d];
        vals[c] = k * vals[c] - old_a;
        vals[d] = k * vals[d] - old_b;

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
    int index() { return ind; }
private:
    enum ENTRIES { a = 0, b, c, d };
    int limit;
    int vals[4]; // Maps values from enum ENTRIES
    int ind;
};

} /* end util::gens */

} /* end util:: */

#endif /* _GENS_HPP_ */

