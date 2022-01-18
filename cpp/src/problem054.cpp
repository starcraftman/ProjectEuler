/**
In the card game poker, a hand consists of five cards and are ranked, from lowest to highest, in the following way:

    High Card: Highest value card.
    One Pair: Two cards of the same value.
    Two Pairs: Two different pairs.
    Three of a Kind: Three cards of the same value.
    Straight: All cards are consecutive values.
    Flush: All cards of the same suit.
    Full House: Three of a kind and a pair.
    Four of a Kind: Four cards of the same value.
    Straight Flush: All cards are consecutive values of same suit.
    Royal Flush: Ten, Jack, Queen, King, Ace, in same suit.

The cards are valued in the order:
2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace.

If two players have the same ranked hands then the rank made up of the highest value wins; for example,
a pair of eights beats a pair of fives (see example 1 below).
But if two ranks tie, for example, both players have a pair of queens,
then highest cards in each hand are compared (see example 4 below);
if the highest cards tie then the next highest cards are compared, and so on.

The file, poker.txt, contains one-thousand random hands dealt to two players.
Each line of the file contains ten cards (separated by a single space):
the first five are Player 1's cards and the last five are Player 2's cards.
You can assume that all hands are valid (no invalid characters or repeated cards),
each player's hand is in no specific order, and in each hand there is a clear winner.

How many hands does Player 1 win?
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream>
#include <sstream>
#include <exception>
#include <map>
#include <set>
#include <algorithm>
#include <initializer_list>

#include "gtest/gtest.h"
#include "util.hpp"

/**************** Namespace Declarations ******************/
using std::cout;
using std::endl;

/************** Global Vars & Functions *******************/
typedef std::int8_t num_t;
static const std::string INPUT = "./src/input_e054.txt";
static const std::string FIRST_DEAL = "8C TS KC 9H 4S 7D 2S 5D 3S AC";

enum Suits {
    Hearts,
    Diamonds,
    Clubs,
    Spades,
};
enum HandTypes {
    HighCard = 1,
    OnePair,
    TwoPair,
    ThreeKind,
    Straight,
    Flush,
    FullHouse,
    FourKind,
    StraightFlush,
    RoyalFlush,
};
static std::map<std::string, Suits> text_to_suit = {
    {"H", Suits::Hearts},
    {"D", Suits::Diamonds},
    {"C", Suits::Clubs},
    {"S", Suits::Spades},
};
static std::map<Suits, std::string> suit_to_text = {
    {Suits::Hearts, "H"},
    {Suits::Diamonds, "D"},
    {Suits::Clubs, "C"},
    {Suits::Spades, "S"},
};
static std::map<std::string, num_t> face_to_value = {
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
    {"T", 10},
    {"J", 11},
    {"Q", 12},
    {"K", 13},
    {"A", 14},
};
static std::map<num_t, std::string> value_to_face = {
    {2, "2"},
    {3, "3"},
    {4, "4"},
    {5, "5"},
    {6, "6"},
    {7, "7"},
    {8, "8"},
    {9, "9"},
    {10, "T"},
    {11, "J"},
    {12, "Q"},
    {13, "K"},
    {14, "A"},
};
static std::map<HandTypes, std::string> hand_type_to_text = {
    {HandTypes::HighCard, "Highest Card"},
    {HandTypes::OnePair, "One Pair"},
    {HandTypes::TwoPair, "Two Pairs"},
    {HandTypes::ThreeKind, "Three of a kind"},
    {HandTypes::Straight, "Straight"},
    {HandTypes::Flush, "Flush"},
    {HandTypes::FullHouse, "Full House"},
    {HandTypes::FourKind, "Four of a kind"},
    {HandTypes::StraightFlush, "Straight Flush"},
    {HandTypes::RoyalFlush, "Royal Flush"},
};

static const std::string HAND_HIGH_CARD = "2H 2D 4S 5C KC";
static const std::string HAND_ONE_PAIR = "2H 2D 4S 5C 9C";
static const std::string HAND_TWO_PAIR = "2H 2D 5C 5S 9C";
static const std::string HAND_THREE_KIND = "2H 2D 2C 5C 9C";
static const std::string HAND_STRAIGHT = "2H 3D 4S 5C 6C";
static const std::string HAND_FLUSH = "2H 6H 9H QH AH";
static const std::string HAND_FULL_HOUSE = "JH JD JC QH QD";
static const std::string HAND_FOUR_KIND = "2H 2D 2C 2S 9C";
static const std::string HAND_STRAIGHT_FLUSH = "5H 6H 7H 8H 9H";
static const std::string HAND_ROYAL_FLUSH = "TH JH QH KH AH";

class Card
{
public:
    Card () : value(2), suit(Suits::Hearts) {};
    explicit Card(const std::string &text) {
        this->change(text);
    }
    Card(const Card &other) {
        this->value = other.value;
        this->suit = other.suit;
    }
    // Constraint: Text is valid. I won't validate. Input guaranteed no dupes nor invalid.
    void change(const std::string &text) {
        this->value = face_to_value[text.substr(0, 1)];
        this->suit = text_to_suit[text.substr(1)];
        this->selected = false;
    }

    // Mainly for debugging/output
    inline std::string text_value() const { return value_to_face[this->value]; };
    inline std::string text_suit() const { return suit_to_text[this->suit]; };
    inline std::string to_text() const { return this->text_value() + this->text_suit(); };

    bool operator==(const Card &other) const {
        return this->value == other.value &&
                this->suit == other.suit;
    }
    bool operator!=(const Card &other) const {
        return !(*this == other);
    }
    // Cards are sortable by FIRST value, then by their suit if they are equal
    bool operator<=(const Card &other) const {
        if (*this == other) {
            return true;
        } else if (this->value == other.value) {
            return this->suit < other.suit;
        } else {
            return this->value < other.value;
        }
    }
    bool operator<(const Card &other) const {
        return *this <= other && *this != other;
    }
    bool operator>(const Card &other) const {
        return !(*this <= other);
    }
    bool operator>=(const Card &other) const {
        return !(*this < other);
    }
    friend std::ostream& operator<<(std::ostream &os, const Card &card);
    friend std::istream& operator>>(std::istream &is, Card &hand);

    // Data
    num_t value = 1;
    Suits suit = Suits::Hearts;
    bool selected = false;
};

class CardGroup {
public:
    CardGroup() : type(HandTypes::HighCard) {}
    explicit CardGroup(const Card &card) : type(HandTypes::HighCard) {
        this->cards.push_back(card);
    }
    // Returns true only if card taken by this group
    bool check_card(const Card &next_card) {
        if (this->cards.size() != 0) {
            Card last = this->cards.back();
            if (last.value == next_card.value) {
                this->cards.push_back(next_card);
                this->set_type();
                return true;
            }
        }

        return false;
    }
    void add_card(const Card &next_card) {
        this->cards.push_back(next_card);
    }
    void set_type() {
        switch (cards.size()) {
            case 1:
                this->type = HandTypes::HighCard;
                break;
            case 2:
                this->type = HandTypes::OnePair;
                break;
            case 3:
                this->type = HandTypes::ThreeKind;
                break;
            case 4:
                this->type = HandTypes::FourKind;
                break;
        }
    }
    int value() const {
        return cards.back().value;
    }

    bool operator==(const CardGroup &other) const {
        return this->type == other.type && this->value() == other.value();
    }
    bool operator!=(const CardGroup &other) const {
        return !(*this == other);
    }
    // CardGroups are ordered by type first, then value of set of cards
    bool operator<=(const CardGroup &other) const {
        if (this->type == other.type) {
            return this->value() <= other.value();
        } else {
            return this->type <= other.type;
        }
    }
    bool operator<(const CardGroup &other) const {
        return *this <= other && *this != other;
    }
    bool operator>(const CardGroup &other) const {
        return !(*this <= other);
    }
    bool operator>=(const CardGroup &other) const {
        return !(*this < other);
    }
    friend std::ostream& operator<<(std::ostream &os, const CardGroup &group);

    // Data
    HandTypes type;
    std::vector<Card> cards;
};

// Defines the composite type of a hand, reduces everything to a single value.
// A HandValue can be made of sub groupings of cards (like a pair + one triple = full house).
// Able to be compared to another HandValue via operators for sorting.
class HandValue {
public:
    HandValue(HandTypes type = HandTypes::HighCard, int value = 2) :
        type(type), type_value(value) {};

    friend std::ostream& operator<<(std::ostream &os, const Card &card);

    HandTypes type;  // Highest type of hand
    int type_value;  // Highest card in the grouping (i.e. if StraightFlush of 5, 6, 7, 8, 9
    std::vector<CardGroup> groups;
};

class Hand {
public:
    Hand(int player_num = 1): player(player_num) {};
    std::istream& read_cards(std::istream &is);
    void sort() {
        std::stable_sort(this->cards.begin(), this->cards.end());
    }

    bool operator==(const Hand &other) const {
        if (this->cards.size() != other.cards.size()) {
            return false;
        }

        // Assume cards are sorted
        for (int i = 0; i < 5; ++i) {
            if (this->cards[i] != other.cards[i]) {
                return false;
            }
        }

        return true;
    }
    bool operator!=(const Hand &other) const {
        return !(*this == other);
    }
    friend std::ostream& operator<<(std::ostream &os, const Hand &hand);
    friend std::istream& operator>>(std::istream &is, Hand &hand);

    const int player = 1;
    std::vector<Card> cards;
    HandValue value;
};

// Hands must be 5 cards in size, simply update hand once at size.
std::istream& Hand::read_cards(std::istream &is) {
    while (cards.size() != 5) {
        if (cards.size() < 5) {
            cards.push_back(Card());
        } else {
            cards.pop_back();
        }
    }

    auto iter = cards.begin();
    int cnt = 5;
    while (iter != cards.end() && is.good() && cnt-- != 0) {
        is >> *iter;
        iter++;
    }
    this->sort();

    return is;
}

std::ostream& operator<<(std::ostream &os, const Card &card) {
    os << card.to_text();
    return os;
}

std::ostream& operator<<(std::ostream &os, const Hand &hand) {
    os << "Player " << hand.player << " has: ";
    for (auto card : hand.cards) {
        os << card << " ";
    }

    return os;
}

std::ostream& operator<<(std::ostream &os, const CardGroup &group) {
    os << "Card Group: " << hand_type_to_text[group.type] << " of "
        << group.cards.back().value << "s.";
}

std::istream& operator>>(std::istream &is, Card &card) {
    std::string text;
    is >> text;
    card.change(text);

    return is;
}

std::istream& operator>>(std::istream &is, Hand &hand) {
    hand.read_cards(is);

    return is;
}

// All cards assumed sorted during detection.
bool detect_royal_flush(Hand &hand) {
    auto iter = hand.cards.cbegin();
    Card expect(*iter);

    if (iter->value != 10) {
        return false;
    }
    while (++iter != hand.cards.cend()) {
        expect.value += 1;
        if (*iter != expect) {
            return false;
        }
    }

    hand.value = HandValue(HandTypes::RoyalFlush, iter->value);
    return true;
}

bool detect_straight_flush(Hand &hand) {
    auto iter = hand.cards.cbegin();
    Card expect(*iter);

    while (++iter != hand.cards.cend()) {
        expect.value += 1;
        if (*iter != expect) {
            return false;
        }
    }

    hand.value = HandValue(HandTypes::StraightFlush, iter->value);
    return true;
}

bool detect_straight(Hand &hand) {
    auto iter = hand.cards.cbegin();
    Card expect(*iter);

    while (++iter != hand.cards.cend()) {
        expect.value += 1;
        if (iter->value != expect.value) {
            return false;
        }
    }

    hand.value = HandValue(HandTypes::Straight, iter->value);
    return true;
}

bool detect_flush(Hand &hand) {
    auto iter = hand.cards.cbegin();
    Card expect(*iter);

    while (++iter != hand.cards.cend()) {
        if (iter->suit != expect.suit) {
            return false;
        }
    }

    hand.value = HandValue(HandTypes::Flush, iter->value);
    return true;
}

// TODO: Integrate with above dtection, assign n hands a value, highest value wins.
//       Value should take into account ties.
void assign_hand_value(Hand &hand) {

}


int number_won_hands() {
    Suits suit = Suits::Hearts;
    cout << suit << endl;


    return 0;
}

TEST(E054_Card, CardDefault) {
    Card deuce;
    ASSERT_EQ(deuce.value, 2);
    ASSERT_EQ(deuce.suit, Suits::Hearts);
}

TEST(E054_Card, CardConstructor) {
    Card ace("AC");
    ASSERT_EQ(ace.value, 14);
    ASSERT_EQ(ace.suit, Suits::Clubs);
}

TEST(E054_Card, CardChange) {
    Card ace("AC");
    ace.change("TD");
    ASSERT_EQ(ace.value, 10);
    ASSERT_EQ(ace.suit, Suits::Diamonds);
}

TEST(E054_Card, CardInputOperator) {
    Card ace("AC");
    std::stringstream ss("TD");
    ss >> ace;
    ASSERT_EQ(ace.value, 10);
    ASSERT_EQ(ace.suit, Suits::Diamonds);
}

TEST(E054_Card, CardOutputOperator) {
    Card ace("TD");
    std::stringstream ss;
    ss << ace;
    ASSERT_EQ(ss.str(), std::string("TD"));
}

TEST(E054_Card, CardEqualityOperator) {
    const Card ace("TD");
    const Card also_ace("TD");
    ASSERT_EQ(ace, also_ace);
}

TEST(E054_Card, CardNotEqualyOperator) {
    const Card ace("TD");
    const Card not_ace("QH");
    ASSERT_TRUE(ace != not_ace);
}

TEST(E054_Card, CardLessOperator) {
    const Card nine("9D");
    const Card ten("TD");
    ASSERT_TRUE(nine < ten);
    const Card nine_hearts("9H");
    ASSERT_TRUE(nine_hearts < nine);
}

TEST(E054_Card, CardTextValue) {
    Card card("TD");
    ASSERT_EQ(card.text_value(), std::string("T"));
}

TEST(E054_Card, CardTextSuit) {
    Card card("TD");
    ASSERT_EQ(card.text_suit(), std::string("D"));
}

TEST(E054_Card, CardToText) {
    Card card("TD");
    ASSERT_EQ(card.to_text(), std::string("TD"));
}

TEST(E054_Hand, HandCreation) {
    Hand hand1;
    ASSERT_EQ(hand1.player, 1);
    ASSERT_EQ(hand1.cards.size(), 0);
}

TEST(E054_Hand, HandSortValues) {
    Hand hand1;
    std::stringstream(FIRST_DEAL) >> hand1;
    hand1.sort();
    std::stringstream ss;
    ss << hand1;
    std::string expect("Player 1 has: 4S 8C 9H TS KC ");
    ASSERT_EQ(ss.str(), expect);
}

TEST(E054_Hand, HandSortValueAndSuit) {
    Hand hand1;
    std::stringstream("KD JS KS JH JD") >> hand1;
    hand1.sort();
    std::stringstream ss;
    ss << hand1;
    std::string expect("Player 1 has: JH JD JS KD KS ");
    ASSERT_EQ(ss.str(), expect);
}

TEST(E054_Hand, HandInputOperator) {
    Hand hand1;
    std::stringstream(FIRST_DEAL) >> hand1;
    ASSERT_EQ(hand1.player, 1);
    ASSERT_EQ(hand1.cards[0], Card("4S"));
}

TEST(E054_Hand, HandOutputOperator) {
    Hand hand1;
    std::stringstream(FIRST_DEAL) >> hand1;
    std::stringstream ss;
    ss << hand1;
    std::string expect("Player 1 has: 4S 8C 9H TS KC ");
    ASSERT_EQ(ss.str(), expect);
}

TEST(E054_CardGroup, AddCard) {
    CardGroup group;
    group.add_card(Card("8H"));
    ASSERT_EQ(group.cards.size(), 1);
    ASSERT_EQ(group.type, HandTypes::HighCard);
}

TEST(E054_CardGroup, CheckCardValid) {
    CardGroup group;
    group.add_card(Card("8H"));
    ASSERT_TRUE(group.check_card(Card("8D")));
    ASSERT_EQ(group.type, HandTypes::OnePair);
    ASSERT_EQ(group.cards.back(), Card("8D"));

    ASSERT_TRUE(group.check_card(Card("8C")));
    ASSERT_EQ(group.type, HandTypes::ThreeKind);
    ASSERT_EQ(group.cards.back(), Card("8C"));

    ASSERT_TRUE(group.check_card(Card("8S")));
    ASSERT_EQ(group.type, HandTypes::FourKind);
    ASSERT_EQ(group.cards.back(), Card("8S"));
}

TEST(E054_CardGroup, CheckCardInvalid) {
    CardGroup group;
    group.add_card(Card("8H"));
    ASSERT_FALSE(group.check_card(Card("7D")));
    ASSERT_EQ(group.type, HandTypes::HighCard);
    ASSERT_EQ(group.cards.back(), Card("8H"));
}

TEST(E054_CardGroup, SetType) {
    CardGroup group;
    group.add_card(Card("8H"));
    group.add_card(Card("8D"));
    group.set_type();
    ASSERT_EQ(group.type, HandTypes::OnePair);

    group.add_card(Card("8C"));
    group.set_type();
    ASSERT_EQ(group.type, HandTypes::ThreeKind);

    group.add_card(Card("8S"));
    group.set_type();
    ASSERT_EQ(group.type, HandTypes::FourKind);
}

// To debug detectors ....
//cout << "Read hand" << endl << hand << endl << hand2 << endl;
TEST(E054_HandValue, RoyalFlush) {
    Hand hand(1);
    Hand hand2(2);
    std::stringstream ss(HAND_ROYAL_FLUSH + " " + HAND_STRAIGHT);
    ss >> hand >> hand2;
    ASSERT_TRUE(detect_royal_flush(hand));
    ASSERT_FALSE(detect_royal_flush(hand2));
}

TEST(E054_HandValue, StraightFlush) {
    Hand hand(1);
    Hand hand2(2);
    std::stringstream ss(HAND_STRAIGHT_FLUSH + " " + HAND_STRAIGHT);
    ss >> hand >> hand2;
    ASSERT_TRUE(detect_straight_flush(hand));
    ASSERT_FALSE(detect_straight_flush(hand2));
}

TEST(E054_HandValue, Straight) {
    Hand hand(1);
    Hand hand2(2);
    std::stringstream ss(HAND_STRAIGHT + " " + HAND_FLUSH);
    ss >> hand >> hand2;
    ASSERT_TRUE(detect_straight(hand));
    ASSERT_FALSE(detect_straight(hand2));
}

TEST(E054_HandValue, Flush) {
    Hand hand(1);
    Hand hand2(2);
    std::stringstream ss(HAND_FLUSH + " " + HAND_STRAIGHT);
    ss >> hand >> hand2;
    ASSERT_TRUE(detect_flush(hand));
    ASSERT_FALSE(detect_flush(hand2));
}

// TODO: Solution loop, use at end
// TEST(E054_Hand, ReadPokerHands) {
    // cout << "Read hand" << endl;
    // std::ifstream input(INPUT, std::ifstream::in);
    // Hand hand1(1);
    // Hand hand2(2);
    // try {
        // while (input.good()) {
            // input >> hand1 >> hand2;
            // cout << hand1 << endl << hand2 << endl;
        // }
    // } catch (std::exception &e) {
        // // Input gone bad, ignore.
    // }
// }

// TEST(Euler054, PokerHandsWon) {
    // int result = number_won_hands();
    // cout << "The number of hands player 1 won is: " << result << endl;
// }
