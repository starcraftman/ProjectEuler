/**
Sudoku (Japanese meaning number place) is the name given to a popular puzzle concept.
Its origin is unclear, but credit must be attributed to Leonhard Euler who invented a similar,
and much more difficult, puzzle idea called Latin Squares. The objective of Sudoku puzzles,
however, is to replace the blanks (or zeros) in a 9 by 9 grid in such that each row, column,
and 3 by 3 box contains each of the digits 1 to 9. Below is an example of a typical starting
puzzle grid and its solution grid.

p096_1.png     p096_2.png

A well constructed Sudoku puzzle has a unique solution and can be solved by logic, although it may be necessary to
employ "guess and test" methods in order to eliminate options (there is much contested opinion over this).
The complexity of the search determines the difficulty of the puzzle; the example above is considered easy because
it can be solved by straight forward direct deduction.

The 6K text file, sudoku.txt (right click and 'Save Link/Target As...'), contains fifty different Sudoku puzzles ranging in difficulty,
but all with unique solutions (the first puzzle in the file is the example above).

By solving all fifty puzzles find the sum of the 3-digit numbers found in the top left corner of each solution grid; for example,
483 is the 3-digit number found in the top left corner of the solution grid above.
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
static const std::string INPUT = "./src/input_e096.txt";
static const std::string INPUT_SMALL = "./src/input_e096.small.txt";
typedef std::uint64_t num_t;
static const std::initializer_list<num_t> possible_vals = {1, 2, 3, 4, 5, 6, 7, 8, 9};
// #define DETAIL_CELL true;

class Cell {
public:
    Cell(num_t row = 0, num_t col = 0, num_t value = 0) : row(row), col(col), value(value) {
        block = (row / 3) * 3 + col / 3;
    };
    void reset_possible() {
        possible.clear();
    }
    void remove_possible(num_t val) {
        possible.erase(val);
    }
    void add_possible(num_t val) {
        possible.insert(val);
    }
    void set_num(num_t val) {
        value = val;
        possible.clear();
    }

    friend std::ostream & operator<<(std::ostream &os, const Cell &cell) {
#ifdef DETAIL_CELL
    os << "Cell: " << cell.value << endl;
    if (cell.possible.size() != 0) {
        os << "Possible: ";
        for (auto ele : cell.possible) {
            os << ele << ", ";
        }
    }
#else
    os << cell.value << " ";
#endif

        return os;
    }

    // Data
    num_t row = 0;
    num_t col = 0;
    num_t block = 0;
    num_t value = 0;
    std::set<num_t> possible;
};

// TODO: Sketch
// 1) Have "views" of row, column and blocks that can be iterated easily.
// 2) Be able to sort cells in a big list by possible values left so can always look at next cell with least possibles.
// 3) Have ability to pick value based on deductions of possible values.
// 4) When no deductions possible, need a fallback to select the cell with LEAST possible values and
//    "try and check" possibles temporarily if it can solve it.

// Only for classic 9x9 grid.
// Any positioning starts in top left corner.
// So block 0 is top left, row 0 is first and so on.
class Sudoku {
public:
    Sudoku() {
        for (int row = 0; row < 9; ++row) {
            std::vector<Cell> cell_row;
            for (int col  = 0; col  < 9; ++col ) {
                Cell cell(row, col);
                cell_row.push_back(cell);
            }

            this->cells.push_back(cell_row);
        }
    }
    void solve() {}
    num_t top_cells() {
        num_t cnt = 3;
        num_t num = 0;
        for (auto &cell : cells.front()) {
            num = num * 10 + cell.value;
            if (--cnt == 0) {
                break;
            }
        }

        return num;
    }

    friend std::ostream & operator<<(std::ostream &os, const Sudoku &puzzle) {
        for (auto cell_row : puzzle.cells) {
            for (auto cell : cell_row) {
                os << cell << " ";
            }
            os << endl;
        }

        return os;
    }
    friend std::istream & operator>>(std::istream &is, Sudoku &puzzle) {
        char chr;
        for (auto &cell_row : puzzle.cells) {
            for (auto &cell : cell_row) {
                do {
                    is.get(chr);
                } while (chr == '\n');
                cell.value = int(chr) - int('0');
            }
        }
        std::string line;
        std::getline(is, line);

        return is;
    }

    // Data
    std::vector<std::vector<Cell> > cells;
};

TEST(Euler096, SudokuInputOperator) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    std::stringstream ss;
    ss << puzzle;
    std::string expect = "0  0  3  0  2  0  6  0  0";

    ASSERT_TRUE(ss.str().find(expect) != std::string::npos);
}

// TEST(Euler096, FinalSolution) {
    // std::ifstream input(INPUT, std::ifstream::in);
    // Sudoku puzzle;
    // int corner_sum = 0;

    // while (input) {
        // std::string grid_line;
        // std::getline(input, grid_line);
        // cout << grid_line << endl;
        // input >> puzzle;
        // puzzle.solve();
        // cout << puzzle;
        // corner_sum += puzzle.top_cells();
    // }

    // cout << "The sum of the top left cells of solved sudoku puzzles is: " << corner_sum << endl;
// }
