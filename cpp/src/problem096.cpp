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
#include <iterator>
#include <cstdlib>
#include <numeric>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
static const std::string INPUT = "./src/input_e096.txt";
static const std::string INPUT_SMALL = "./src/input_e096.small.txt";
static const std::string INPUT_SMALL_SOLVED = "./src/input_e096.small_solved.txt";
const static std::string line_break = "==========================================";

typedef std::uint64_t num_t;
static const std::initializer_list<num_t> all_values = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// Debug info for CellCheck
enum CheckType { Row, Column, Block };
const static std::map<CheckType, std::string> checktype_to_str = {
    {CheckType::Row, std::string("Row")},
    {CheckType::Column, std::string("Column")},
    {CheckType::Block, std::string("Block")},
};

class Cell {
public:
    Cell(num_t row = 0, num_t col = 0, num_t value = 0) : row(row), col(col), value(value) {
        block = (row / 3) * 3 + col / 3;
        if (this->value == 0) {
            possible.insert(all_values.begin(), all_values.end());
        }
    };
    void set_possible(std::vector<num_t> &vals) {
        possible.clear();
        possible.insert(vals.begin(), vals.end());
    }
    void remove_possible(num_t val) {
        possible.erase(val);
    }
    void remove_all_possible(const std::set<num_t> &vals) {
        for (auto val : vals) {
            possible.erase(val);
        }
    }
    void add_possible(num_t val) {
        possible.insert(val);
    }
    void set_value(num_t val) {
        last_value = value;
        value = val;
        possible.clear();
    }
    void restore(const Cell &cell) {
        value = cell.last_value;
        last_value = 0;
        possible.clear();
        possible.insert(cell.possible.begin(), cell.possible.end());
    }
    void print_details(std::ostream &os) {
        os << "Cell (" << this->row << ", " << this->col << ")"
            << ": " << this->value;

        if (this->possible.size() != 0) {
            os << " Possible: ";
            for (auto ele : this->possible) {
                os << ele << ", ";
            }
        }

        os << endl;
    }

    friend std::ostream & operator<<(std::ostream &os, const Cell &cell) {
        os << cell.value;
        return os;
    }
    bool operator==(const Cell &other) const {
        return this->row == other.row &&
            this->col == other.col &&
            this->value == other.value;
    }
    bool operator!=(const Cell &other) const { return !(*this == other); }
    bool operator<(const Cell &other) const {
        if (this->possible.size() != other.possible.size()) {
            return this->possible.size() < other.possible.size();
        }
        if (this->row == other.row) {
            return this->col < other.col;
        } else {
            return this->row < other.row;
        }
    }
    bool operator<=(const Cell &other) const { return (*this == other) || *this < other; }
    bool operator>(const Cell &other) const { return !(*this <= other); }
    bool operator>=(const Cell &other) const { return !(*this < other); }

    // Data
    num_t row = 0;
    num_t col = 0;
    num_t block = 0;
    num_t value = 0;
    num_t last_value = 0;
    std::set<num_t> possible;
};

// Simple change set records changes to the puzzle.
// By storing the new value and old possible values we can revert the move by setting old cell to 0 and retoring possible.
class ChangeSet {
public:
    explicit ChangeSet(const Cell &cell, num_t new_val) : cell(cell) {
        this->cell.last_value = this->cell.value;
        this->cell.value = new_val;
    }
    ChangeSet(num_t row = 0, num_t col = 0, num_t value = 0) : cell(Cell(row, col, value)) {};

    friend std::ostream & operator<<(std::ostream &os, const ChangeSet &change) {
        os << "Change (" << change.cell.row << "," << change.cell.col << ") => " << change.cell.value << endl;
        return os;
    }
    bool operator==(const ChangeSet &other) const {
        return this->cell.row == other.cell.row &&
            this->cell.col == other.cell.col &&
            this->cell.value == other.cell.value;
    }
    bool operator!=(const ChangeSet &other) const { return !(*this == other); }

    Cell cell;
};

// This class holds references to exactly 9 cells of a sudoku puzzle
// Depending on how references are added, it can accomodate row, col and block checks.
// TODO: Ability to create a "Task" of cells check and execute later.
class CellsCheck {
public:
    CellsCheck(CheckType type = CheckType::Row, num_t num = 0) :
        type(type), num(num) {
        remains.insert(all_values.begin(), all_values.end());
    }
    void add(Cell &cell ) {
        if (cell.value != 0) {
            remains.erase(cell.value);
        }
        cells.push_back(std::ref(cell));
    }
    void mark_off_possible() {
        std::set<num_t> found;
        for (Cell &cell : cells) {
            found.insert(cell.value);
        }
        found.erase(0);
        for (auto val : found) {
            remains.erase(val);
        }

        for (Cell &cell : cells) {
            if (cell.value == 0) {
                cell.remove_all_possible(found);
            }
        }
    }
    void restore(num_t value) {
        remains.insert(value);
    }
    void eliminate(num_t value) {
        remains.erase(value);
    }

    friend std::ostream & operator<<(std::ostream &os, const CellsCheck &check) {
        os << "Check " << checktype_to_str.at(check.type) << " " << check.num << ": ";
        for (auto ele : check.remains) {
            os << ele << " ";
        }
        return os;
    }
    bool operator==(const CellsCheck &other) const {
        if (this->type != other.type &&
                this->num != other.num &&
                this->remains.size() != other.remains.size()) {
            return false;
        }

        auto other_itr = other.remains.cbegin();
        for (auto &remain : remains) {
            if (remain != *other_itr) {
                return false;
            }
            ++other_itr;
        }

        return true;
    }
    bool operator!=(const CellsCheck &other) const { return !(*this == other); }

    CheckType type;
    num_t num;
    std::set<num_t> remains;
    std::vector<std::reference_wrapper<Cell> > cells;
};

// Equal to: (A U B U C)
//  Not sure if will use.
inline
std::vector<num_t> cells_check_union(const CellsCheck &row_check, const CellsCheck &col_check, const CellsCheck &block_check) {
    std::vector<num_t> inter;
    std::vector<num_t> result;
    std::set_union(row_check.remains.begin(), row_check.remains.end(),
                   col_check.remains.begin(), col_check.remains.end(),
                   std::back_inserter(inter));
    std::set_union(inter.begin(), inter.end(),
                   block_check.remains.begin(), block_check.remains.end(),
                   std::back_inserter(result));

    return result;
}

// Based on the classic 9x9 grid.
// Any positioning starts in top left corner.
// So block 0 is top left, row 0 is first and so on.
class Sudoku {
public:
    Sudoku() {
        for (int row = 0; row < 9; ++row) {
            std::vector<Cell> cell_row;
            for (int col  = 0; col  < 9; ++col ) {
                cell_row.push_back(Cell(row, col));
            }

            this->cells.push_back(cell_row);
        }
    }
    // Should be called after reading in values
    void init_checkers() {
        for (int i = 0; i < 9; ++i) {
            row_checks.push_back(CellsCheck(CheckType::Row, i));
            col_checks.push_back(CellsCheck(CheckType::Column, i));
            block_checks.push_back(CellsCheck(CheckType::Block, i));
        }

        for (auto &row : cells) {
            for (Cell &cell : row) {
                row_checks[cell.row].add(cell);
                col_checks[cell.col].add(cell);
                block_checks[cell.block].add(cell);
                if (cell.value == 0) {
                    cells_left.push_back(cell);
                }
            }
        }
    }

    // A single pass over all row, column and block checks to mark off possible solutions.
    void check_possible() {
        std::vector<std::vector<CellsCheck> > all_checks = {row_checks, col_checks, block_checks};
        for (std::vector<CellsCheck> &checks : all_checks) {
            for (CellsCheck &check : checks) {
                check.mark_off_possible();
            }
        }
    }

    // Iterate over cells left and itemize changes required
    void check_cells_for_values(std::vector<ChangeSet> &changes) {
        // for (Cell &cell : cells_left) {
        for (auto itr = cells_left.begin(); itr != cells_left.end(); ++itr) {
            Cell &cell = *itr;
            if (cell.possible.size() == 1) {
                ChangeSet change(cell, *cell.possible.begin());
                changes.push_back(change);
                itr = cells_left.erase(itr) - 1;
            }
        }
    }

    void apply_changes(const std::vector<ChangeSet> &changes) {
        for (const auto &change : changes) {
            history.push_back(change);
            const Cell &cell = change.cell;

            cells[cell.row][cell.col].set_value(cell.value);
            row_checks[cell.row].eliminate(cell.value);
            col_checks[cell.col].eliminate(cell.value);
            block_checks[cell.block].eliminate(cell.value);
        }
    }

    void reverse_changes(const std::vector<ChangeSet> &changes) {
        for (auto itr = changes.rbegin(); itr != changes.rend(); ++itr) {
            history.pop_back();
            const Cell &cell = itr->cell;

            cells[cell.row][cell.col].restore(cell);
            row_checks[cell.row].restore(cell.value);
            col_checks[cell.col].restore(cell.value);
            block_checks[cell.block].restore(cell.value);
        }
    }

    // Sort the cells remaining that aren't definite.
    void sort_cells_left() {
        std::sort(cells_left.begin(), cells_left.end(),
                [](std::reference_wrapper<Cell> &left, std::reference_wrapper<Cell> &right) {
                return left.get() < right.get(); }
        );
    }

    // Take a low possibilities cell and try them temporarily and check.
// FIXME: This part needs more work
    bool try_and_check() {
        sort_cells_left();

        cout << "Try sorted : " << endl;
        for (Cell &cell : cells_left) {
            cout << "Cell (" << cell.row << ", " << cell.col << ")" << endl;
            for (auto val : cell.possible) {
                cell.print_details(cout);
                std::vector<ChangeSet> changes;
                changes.push_back(ChangeSet(cell, val));
                cout << "Calling solve for: " << val <<  endl;
                apply_changes(changes);

                if (!solve(true)) {
                    reverse_changes(changes);
                } else {
                    return true;
                }
            }
        }

        return false;
    }

    // Returns true if was able to solve without issue.
    // TODO: When trial is true, don't affect changes like removing from cells_left
    bool solve(bool trial = false, num_t cnt = 0) {
        cout << "Called solve with history size: " << history.size() << endl;
        std::vector<ChangeSet> changes_so_far;
        while (!is_solved()) {
            cout << "Round : " << ++cnt << endl;
            std::vector<ChangeSet> changes;
            // Mark down possible cells.
            check_possible();

            // Visit cells left and determine possible changes, returned in vector
            check_cells_for_values(changes);

            // If deduced changes possible, make them
            if (changes.size() != 0) {
                for (auto change : changes) {
                    changes_so_far.push_back(change);
                    cout << change;
                }

                // Affect the changes
                apply_changes(changes);

            } else {
                cout << "Trial time " << endl;
                cout << *this << endl;

                // Deductions have failed, pick a possible value of node and check solution.
                return try_and_check();
            }
        }

        if (trial) {
            reverse_changes(changes_so_far);
        }

        return is_solved();
    }

    /*
     * Just a short hand now.
     */
    inline bool is_solved() {
        return is_valid(true);
    }

    /*
     * Check if a puzzle is valid or alternatively if it is solved.
     */
    bool is_valid(bool also_solved = false) {
        for (auto &checks : {row_checks, col_checks, block_checks}) {
            for (auto &check : checks) {
                num_t cnt = 0;
                std::set<num_t> found;
                for (Cell &cell : check.cells) {
                    if (cell.value != 0) {
                        found.insert(cell.value);
                        ++cnt;
                    }
                }
                // Check for dupes
                if (cnt != found.size()) {
                    return false;
                }

                // Solution has to be solved for every checker.
                if (also_solved) {
                    if (cnt != all_values.size()) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    // Just there to answer Euler problem
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

    void print_details() {
        for (auto &row : cells) {
            for (auto &cell : row) {
                cell.print_details(cout);
            }
            cout << endl << line_break << endl;
        }
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
                num_t val = int(chr) - int('0');
                if (val != 0) {
                    cell.set_value(val);
                }
            }
        }
        std::string line;
        std::getline(is, line);

        return is;
    }
    bool operator==(const Sudoku &other) const {
        if (cells.size() != other.cells.size()) {
            return false;
        }

        for (int row = 0; row < 9; ++row) {
            if (cells[row].size() != other.cells[row].size()) {
                return false;
            }
            for (int col = 0; col < 9; ++col) {
                if (cells[row][col] != other.cells[row][col]) {
                    return false;
                }
            }
        }

        return true;
    }
    bool operator!=(const Sudoku &other) const { return !(*this == other); }

    // Data
    std::vector<ChangeSet> history;
    std::vector<CellsCheck> row_checks;
    std::vector<CellsCheck> col_checks;
    std::vector<CellsCheck> block_checks;
    std::vector<std::reference_wrapper<Cell> > cells_left; // Cells that aren't solved with certainty.
    std::vector<std::vector<Cell> > cells;
};

TEST(Euler096_Cell, Constructor) {
    Cell cell(7, 0, 5);
    ASSERT_EQ(cell.row, 7);
    ASSERT_EQ(cell.col, 0);
    ASSERT_EQ(cell.block, 6);
    ASSERT_EQ(cell.value, 5);
}

TEST(Euler096_Cell, AddPossible) {
    Cell cell(7, 0, 0);
    cell.possible.clear();
    cell.add_possible(7);
    cell.add_possible(2);
    cell.add_possible(3);
    std::set<num_t> expect = {2, 3, 7};
    ASSERT_EQ(cell.possible, expect);
}

TEST(Euler096_Cell, RemovePossible) {
    Cell cell(7, 0, 0);
    cell.possible.clear();
    cell.add_possible(7);
    cell.add_possible(3);
    cell.remove_possible(7);
    std::set<num_t> expect = {3};
    ASSERT_EQ(cell.possible, expect);
}

TEST(Euler096_Cell, SetValue) {
    Cell cell(7, 0, 0);
    cell.add_possible(7);
    cell.add_possible(2);
    cell.add_possible(3);
    cell.set_value(7);
    std::set<num_t> expect;
    ASSERT_EQ(cell.possible, expect);
}

TEST(Euler096_Cell, OutputOperator) {
    Cell cell(7, 0, 9);
    std::stringstream ss;
    ss << cell;
    ASSERT_EQ(ss.str(), std::string("9"));
}

TEST(Euler096_CellsCheck, Add) {
    CellsCheck check;
    Cell cell(0, 0, 9);
    check.add(cell);
    cell = Cell(0, 1, 1);
    check.add(cell);
    cell = Cell(0, 2, 5);
    check.add(cell);

    Cell back = check.cells.back();
    ASSERT_EQ(back, cell);
    ASSERT_EQ(check.remains.size(), 6);
}

TEST(Euler096_CellsCheck, MarkOffPossible) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();

    auto check = puzzle.row_checks[0];
    ASSERT_EQ(puzzle.cells[0][0].possible.size(), 9);
    check.mark_off_possible();
    ASSERT_EQ(puzzle.cells[0][0].possible.size(), 6);
}

TEST(Euler096_CellsCheck, OutputOperator) {
    CellsCheck check;
    Cell cell(0, 0, 9);
    check.add(cell);
    cell = Cell(0, 1, 1);
    check.add(cell);
    cell = Cell(0, 2, 5);
    check.add(cell);
    std::stringstream ss;
    ss << check;
    std::string expect("Check Row 0: 2 3 4 6 7 8 ");
    ASSERT_EQ(ss.str(), expect);
}

TEST(Euler096_CellsCheck, CellsCheckUnion) {
    CellsCheck row_check;
    row_check.remains = {3, 2, 6};
    CellsCheck col_check;
    col_check.remains = {9, 7, 8};
    CellsCheck block_check;
    block_check.remains = {1, 3, 9};

    std::vector<num_t> result = cells_check_union(row_check, col_check, block_check);

    std::vector<num_t> expect = { 1, 2, 3, 6, 7, 8, 9 };
    ASSERT_EQ(result, expect);
}

TEST(Euler096_Sudoku, InputOperator) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    std::stringstream ss;
    ss << puzzle;
    std::string expect = "0 0 3 0 2 0 6 0 0";

    ASSERT_TRUE(ss.str().find(expect) != std::string::npos);
}

TEST(Euler096_Sudoku, InitCheckers) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();

    std::set<num_t> expect = {1, 4, 5, 7, 8, 9};
    ASSERT_EQ(puzzle.row_checks.at(0).remains, expect);
}

TEST(Euler096_Sudoku, CheckPossible) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();

    ASSERT_EQ(puzzle.cells[4][5].value, 0);
    ASSERT_EQ(puzzle.cells[4][6].value, 0);
    ASSERT_EQ(puzzle.cells[4][5].possible.size(), 9);
    ASSERT_EQ(puzzle.cells[4][6].possible.size(), 9);
    puzzle.check_possible();
    ASSERT_EQ(puzzle.cells[4][5].possible.size(), 1);
    ASSERT_EQ(puzzle.cells[4][6].possible.size(), 1);
    ASSERT_EQ(*puzzle.cells[4][5].possible.begin(), 4);
    ASSERT_EQ(*puzzle.cells[4][6].possible.begin(), 1);
}

TEST(Euler096_Sudoku, CheckCellsForValues) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    puzzle.check_possible();
    std::vector<ChangeSet> changes;
    puzzle.check_cells_for_values(changes);

    ASSERT_EQ(changes.size(), 3);
    auto front = changes.front();
    ASSERT_EQ(front.cell.row, 4);
    ASSERT_EQ(front.cell.col, 5);
    ASSERT_EQ(front.cell.value, 4);
}

TEST(Euler096_Sudoku, IsSolvedTrue) {
    std::ifstream input(INPUT_SMALL_SOLVED);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    ASSERT_TRUE(puzzle.is_solved());
}

TEST(Euler096_Sudoku, IsSolvedFalse) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    ASSERT_FALSE(puzzle.is_solved());
}

TEST(Euler096_Sudoku, IsValidTrue) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    ASSERT_TRUE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, IsValidFalseRow) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    puzzle.cells[0][2].value = 2;
    ASSERT_FALSE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, IsValidFalseCol) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    puzzle.cells[2][0].value = 9;
    ASSERT_FALSE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, IsValidFalseBlock) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    puzzle.cells[2][1].value = 9;
    ASSERT_FALSE(puzzle.is_valid());
}

TEST(Euler096_Sudoku, TopCells) {
    std::ifstream input(INPUT_SMALL_SOLVED);
    Sudoku puzzle;
    input >> puzzle;
    ASSERT_EQ(puzzle.top_cells(), 483);
}

TEST(Euler096_Sudoku, ApplyChanges) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    std::vector<ChangeSet> changes;
    changes.push_back(ChangeSet(0, 0, 5));
    changes.push_back(ChangeSet(1, 1, 9));
    puzzle.apply_changes(changes);

    ASSERT_EQ(puzzle.cells[0][0].value, 5);
    ASSERT_EQ(puzzle.cells[1][1].value, 9);
}

TEST(Euler096_Sudoku, ReverseChanges) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    std::vector<ChangeSet> changes;
    changes.push_back(ChangeSet(0, 0, 5));
    changes.push_back(ChangeSet(1, 1, 9));
    puzzle.apply_changes(changes);
    puzzle.reverse_changes(changes);

    ASSERT_EQ(puzzle.cells[0][0].value, 0);
    ASSERT_EQ(puzzle.cells[1][1].value, 0);
}

TEST(Euler096_Sudoku, SortCellsLeft) {
    std::ifstream input(INPUT_SMALL);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();
    puzzle.check_possible();

    Cell &cell = puzzle.cells_left[0];
    ASSERT_EQ(cell.row, 0);
    ASSERT_EQ(cell.col, 0);

    puzzle.sort_cells_left();

    Cell &cell2 = puzzle.cells_left[0];
    ASSERT_EQ(cell2.row, 4);
    ASSERT_EQ(cell2.col, 5);
}

// TODO: Better test method.
TEST(Euler096_Sudoku, Solve) {
    std::ifstream input(INPUT_SMALL, std::ifstream::in);
    Sudoku puzzle;
    input >> puzzle;
    puzzle.init_checkers();

    cout << puzzle;
    puzzle.solve();
    cout << line_break << endl << puzzle;
    ASSERT_TRUE(puzzle.is_solved());
}

// TEST(Euler096, FinalSolution) {
    // std::ifstream input(INPUT, std::ifstream::in);
    // int corner_sum = 0;
    // int grid_num = 0;
    // std::vector<int> failed;

    // int cnt = 0;
    // while (input) {
        // Sudoku puzzle;
        // grid_num++;
        // std::string grid_line;
        // std::getline(input, grid_line);
        // cout << grid_line << endl;

        // input >> puzzle;
        // cout << puzzle << endl;
        // puzzle.init_checkers();
        // puzzle.solve();
        // if (!puzzle.solve()) {
            // failed.push_back(grid_num);
        // }
        // cout << line_break << endl << puzzle << endl;

        // See second puzzle.
        // if (++cnt == 2) {
            // puzzle.print_details();
            // break;
        // }
        // corner_sum += puzzle.top_cells();
    // }

    // if (failed.size()) {
        // cout << "Failed to solve " << failed.size() << " grids: ";
        // for (auto num : failed) {
            // cout << num << ", ";
        // }
        // cout << endl;
    // } else {
        // cout << "The sum of the top left cells of solved sudoku puzzles is: " << corner_sum << endl;
    // }
// }

    // Goes in Sudoku class
    // FIXME: Rewrite or gut. Was going for a slightly more efficient stategy than scanning all, involving triggering only scans for
    //        Checkeers where values changed under the cells.
    // bool check_possible() {
        // bool deduction_made = false;
        // for (auto &row : cells) {
            // for (auto &cell : row) {
                // if (cell.value != 0) {
                    // continue;
                // }

                // auto &row_check = row_checks.at(cell.row);
                // auto &col_check = col_checks.at(cell.col);;
                // auto &block_check = block_checks.at(cell.block);
                // // std::vector<num_t> result = cells_check_union(row_check, col_check, block_check);

                // // cout << "Found ";
                // // for (auto val : result) {
                   // // cout << val << " ";
                // // }
                // // cout << endl;

                // // if (result.size() == 1) {
                    // // deduction_made = true;
                    // // num_t value = result.back();
                    // // cell.set_value(value);
                    // // row_check.eliminate(value);
                    // // col_check.eliminate(value);
                    // // block_check.eliminate(value);
                // // } else {
                    // // cell.set_possible(result);
                // // }
            // }
        // }

        // return deduction_made;
    // }

