/**
In the 20×20 grid below, four numbers along a diagonal line have
been marked in red.

08 02 22 97 38 15 00 40 00 75 04 05 07 78 52 12 50 77 91 08
49 49 99 40 17 81 18 57 60 87 17 40 98 43 69 48 04 56 62 00
81 49 31 73 55 79 14 29 93 71 40 67 53 88 30 03 49 13 36 65
52 70 95 23 04 60 11 42 69 24 68 56 01 32 56 71 37 02 36 91
22 31 16 71 51 67 63 89 41 92 36 54 22 40 40 28 66 33 13 80
24 47 32 60 99 03 45 02 44 75 33 53 78 36 84 20 35 17 12 50
32 98 81 28 64 23 67 10 26 38 40 67 59 54 70 66 18 38 64 70
67 26 20 68 02 62 12 20 95 63 94 39 63 08 40 91 66 49 94 21
24 55 58 05 66 73 99 26 97 17 78 78 96 83 14 88 34 89 63 72
21 36 23 09 75 00 76 44 20 45 35 14 00 61 33 97 34 31 33 95
78 17 53 28 22 75 31 67 15 94 03 80 04 62 16 14 09 53 56 92
16 39 05 42 96 35 31 47 55 58 88 24 00 17 54 24 36 29 85 57
86 56 00 48 35 71 89 07 05 44 44 37 44 60 21 58 51 54 17 58
19 80 81 68 05 94 47 69 28 73 92 13 86 52 17 77 04 89 55 40
04 52 08 83 97 35 99 16 07 97 57 32 16 26 26 79 33 27 98 66
88 36 68 87 57 62 20 72 03 46 33 67 46 55 12 32 63 93 53 69
04 42 16 73 38 25 39 11 24 94 72 18 08 46 29 32 40 62 76 36
20 69 36 41 72 30 23 88 34 62 99 69 82 67 59 85 74 04 36 16
20 73 35 29 78 31 90 01 74 31 49 71 48 86 81 16 23 57 05 54
01 70 54 71 83 51 54 69 16 92 33 48 61 43 52 01 89 19 67 48

The product of these numbers is 26 × 63 × 78 × 14 = 1788696.

What is the greatest product of four adjacent numbers in the same
direction (up, down, left, right, or diagonally) in the 20×20 grid?
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <iomanip>
#include <fstream> /* File operations. */
#include <string> /* C++ String class. */
#include <vector>
#include <algorithm>
#include <exception>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;
using util::u_int;
using util::u_shrt;

typedef std::vector<u_shrt> row_t;
typedef std::vector<row_t> grid_t;
typedef row_t::size_type grid_ind;

/***************** Constants & Macros *********************/
static const std::string INPUT = "src/input_e011.txt";

/****************** Class Definitions *********************/
/* Simple struct allows me to track product & numbers. */

class Vals {
public:
    Vals() : product(1) {};
    void append(u_shrt val) {
        product *= val;
        nums.push_back(val);
    }

    u_shrt operator[](grid_ind index) const {
        return nums[index];
    }

    inline bool operator==(const Vals &other) const { return product == other.product; }
    inline bool operator!=(const Vals &other) const { return !operator==(other); }
    inline bool operator<(const Vals &other) const  { return product < other.product; }
    inline bool operator>(const Vals &other) const  { return other.operator<(*this); }
    inline bool operator<=(const Vals &other) const { return !operator>(other); }
    inline bool operator>=(const Vals &other) const { return !operator<(other); }

    friend std::ostream& operator<<(std::ostream &out, const Vals &obj);

    enum { MAX = 4 };
    row_t nums;
    u_int product;
};

std::ostream & operator<<(std::ostream &out, const Vals &obj) {
    for (grid_ind i = 0; i < Vals::MAX; ++i) {
        out << std::setfill('0') << std::setw(2) << obj.nums[i];
        if (i != Vals::MAX - 1) {
            out << " x ";
        }
    }
    out << " = "<< obj.product;

    return out;
}

class Grid {
public:
    Grid(const std::string &file, grid_ind nrows,
            grid_ind ncols) : nrows(nrows), ncols(ncols) {
        std::ifstream fin(file.c_str());
        u_shrt val = 0;

        while (rows.size() != nrows) {
            row_t row;
            while (row.size() != ncols) {
                fin >> val;
                row.push_back(val);
            }
            rows.push_back(row);
        }
    }

    grid_ind num_rows() const { return rows.size(); }
    grid_ind num_cols() const { return rows[0].size(); }

    /* public on purpose */
    grid_t rows;
    const grid_ind nrows;
    const grid_ind ncols;
};

class Scanner {
public:
    Scanner(const Grid &grid) : grid(grid) , cur_row(0), cur_col(0) {}
    virtual	~Scanner() {};
    virtual bool has_next() = 0;
    virtual bool next() = 0;
    virtual Vals vals() = 0;
protected:
    const Grid &grid;
    grid_ind cur_row;
    grid_ind cur_col;
};

class RowScanner : public Scanner {
public:
    RowScanner(const Grid &grid) : Scanner(grid) { cur_col = -1; };

    bool has_next() {
        return cur_row != grid.nrows;
    }
    bool next() {
        if (cur_col + Vals::MAX == grid.ncols) {
            cur_row += 1;
            cur_col = -1;
        }
        cur_col++;

        return has_next();
    }
    Vals vals() {
        Vals res;

        for (grid_ind i = cur_col; i < cur_col + Vals::MAX; ++i) {
            res.append(grid.rows[cur_row][i]);
        }

        return res;
    }
};

class ColScanner : public Scanner {
public:
    ColScanner(const Grid &grid) : Scanner(grid) { cur_row = -1; };

    bool has_next() {
        return cur_col != grid.ncols;
    }
    bool next() {
        if (cur_row + Vals::MAX == grid.nrows) {
            cur_col += 1;
            cur_row = -1;
        }
        cur_row++;

        return has_next();
    }
    Vals vals() {
        Vals res;

        for (grid_ind i = cur_row; i < cur_row + Vals::MAX; ++i) {
            res.append(grid.rows[i][cur_col]);
        }

        return res;
    }
};

class LRDiagScanner : public Scanner {
public:
    LRDiagScanner(const Grid &grid) : Scanner(grid) { cur_row = -1; };

    bool has_next() {
        return cur_col != (grid.ncols - Vals::MAX + 1);
    }
    bool next() {
        if (cur_row + Vals::MAX == grid.nrows) {
            cur_col += 1;
            cur_row = -1;
        }
        cur_row++;

        return has_next();
    }
    Vals vals() {
        Vals res;

        for (grid_ind i = 0; i < Vals::MAX; ++i) {
            res.append(grid.rows[cur_row + i][cur_col + i]);
        }

        return res;
    }
};

class RLDiagScanner : public Scanner {
public:
    RLDiagScanner(const Grid &grid) : Scanner(grid) { cur_row = 2; };

    bool has_next() {
        return cur_col != (grid.ncols - Vals::MAX + 1);
    }
    bool next() {
        if (cur_row == (grid.nrows - 1)) {
            cur_col += 1;
            cur_row = 2;
        }
        cur_row++;

        return has_next();
    }
    Vals vals() {
        Vals res;

        for (grid_ind i = 0; i < Vals::MAX; ++i) {
            res.append(grid.rows[cur_row - i][cur_col + i]);
        }

        return res;
    }
};
/************** Global Vars & Functions *******************/
TEST(Euler011, Vals) {
    Vals v;

    for (u_shrt i = 1; i < 6; ++i) {
        v.append(i);
    }

    ASSERT_EQ(1, v[0]);

    ASSERT_EQ(120, v.product);
}

TEST(Euler011, GridCreate) {
    Grid grid(INPUT, 20, 20);

    ASSERT_EQ(20, grid.num_rows());
    ASSERT_EQ(20, grid.num_cols());
    ASSERT_EQ(8, grid.rows[0][0]);
    ASSERT_EQ(48, grid.rows[19][19]);
}

TEST(Euler011, HorizontalSearch) {
    Grid grid(INPUT, 20, 20);
    RowScanner scan(grid);

    Vals biggest;
    while (scan.next()) {
        Vals temp = scan.vals();
        if (temp > biggest) {
            biggest = temp;
            cout << temp << endl;
        }
    }

    ASSERT_EQ(48477312, biggest.product);
}

TEST(Euler011, ColumnScanner) {
    Grid grid(INPUT, 20, 20);
    ColScanner scan(grid);

    Vals biggest;
    while (scan.next()) {
        Vals temp = scan.vals();
        if (temp > biggest) {
            biggest = temp;
            cout << temp << endl;
        }
    }

    ASSERT_EQ(51267216, biggest.product);
}

TEST(Euler011, LRDiagonalScanner) {
    Grid grid(INPUT, 20, 20);
    LRDiagScanner scan(grid);

    Vals biggest;
    while (scan.next()) {
        Vals temp = scan.vals();
        if (temp > biggest) {
            biggest = temp;
            cout << temp << endl;
        }
    }

    ASSERT_EQ(40304286, biggest.product);
}

TEST(Euler011, RLDiagonalScanner) {
    Grid grid(INPUT, 20, 20);
    RLDiagScanner scan(grid);

    Vals biggest;
    while (scan.next()) {
        Vals temp = scan.vals();
        if (temp > biggest) {
            biggest = temp;
            cout << temp << endl;
        }
    }

    ASSERT_EQ(70600674, biggest.product);
}

TEST(Euler011, FinalAnswer) {
    typedef std::vector<Scanner *> scan_v_t;;
    Grid grid(INPUT, 20, 20);
    Vals temp, biggest;
    scan_v_t scanners;

    scanners.push_back(new RowScanner(grid));
    scanners.push_back(new ColScanner(grid));
    scanners.push_back(new LRDiagScanner(grid));
    scanners.push_back(new RLDiagScanner(grid));

    for (scan_v_t::const_iterator i = scanners.begin();
            i != scanners.end(); ++i) {
        Scanner * const scan = *i;

        while (scan->next()) {
            temp = scan->vals();
            if (temp > biggest) {
                biggest = temp;
            }
        }

        delete *i;
    }

    cout << "The biggest product is: " << biggest << endl;
    ASSERT_EQ(70600674, biggest.product);
}

