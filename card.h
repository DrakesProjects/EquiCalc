#ifndef CARD_H
#define CARD_H

#include <string>
#include <iostream>
#include <unordered_map>

// 0 -> 2; 1 -> 3; 2 -> 4; 3 -> 5; 4 -> 6; 5 -> 7; 6 -> 8
// 7 -> 9; 8 -> 10; 9 -> J; 10 -> Q; 11 -> K; 12 -> A

// 0 -> Clubs; 1 -> Diamonds; 2 -> Hearts; 3 -> Spades

class card {
	private:
		const int CARDS_PER_SUIT = 13;
		int card_value;

		// for converting string-argument constructors to a usable value
		int string_to_value(const std::string& card_string) const {
			if (card_string.size() != 2) {
				std::cout << "Error, incorrect input for card string\n";
				return -1;
			}

			static const std::unordered_map<char, int> rank_map = {
	            {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3}, {'6', 4},
	            {'7', 5}, {'8', 6}, {'9', 7}, {'T', 8}, {'J', 9},
	            {'Q', 10}, {'K', 11}, {'A', 12}
	        };

	        static const std::unordered_map<char, int> suit_map = {
	            {'c', 0}, {'d', 1}, {'h', 2}, {'s', 3}
	        };

	        char rank_char = card_string[0];
	        char suit_char = card_string[1];

	        if (rank_map.find(rank_char) == rank_map.end() || suit_map.find(suit_char) == suit_map.end()) {
	            std::cout << "Error, invalid card string format.\n";
	            return -1;
	        }

	        int rank_value = rank_map.at(rank_char);
	        int suit_value = suit_map.at(suit_char);
	        return suit_value * 13 + rank_value;
		}

	public:
		// constructors ************************************************************
		explicit card(int card_in) {
			if (card_in >= 0 && card_in < 52) {
				this->card_value = card_in;
			} else {
				std::cout << "Error, card value must be between 0 and 51, inclusive\n";
				this->card_value = -1;
			}
		}

		explicit card(const std::string& card_string) {
			int card_in = string_to_value(card_string);
			// everything else is the same as the constuctor which takes an int
			if (card_in >= 0 && card_in < 52) {
				this->card_value = card_in;
			} else {
				std::cout << "Error, card value must be between 0 and 51, inclusive\n";
				this->card_value = -1;
			}
		}
		// Very self explanatory getter methods ************************************
		int get_suit() const {
			return static_cast<int>(this->get_value()/CARDS_PER_SUIT);
		}

		int get_rank() const {
			return this->get_value()%CARDS_PER_SUIT;
		}
		// *************************************************************************
		// get internal value of the card
		int get_value() const {
			return this->card_value;
		}

		// for user readablilty
		std::string to_string() const {
			static const std::string ranks = "23456789TJQKA";
        	static const std::string suits = "cdhs";
        
        	return std::string(1, ranks[this->get_rank()]) + suits[this->get_suit()];
		}

		// overload comparison, assingment, stream insertion operator **************
		// overload comparison operators
	    bool operator<(const card& other) const {
	        return this->get_value() < other.get_value();
	    }

	    bool operator>(const card& other) const {
	        return this->get_value() > other.get_value();
	    }

	    bool operator<=(const card& other) const {
	        return this->get_value() <= other.get_value();
	    }

	    bool operator>=(const card& other) const {
	        return this->get_value() >= other.get_value();
	    }

	    bool operator==(const card& other) const {
	        return this->get_value() == other.get_value();
	    }

	    bool operator!=(const card& other) const {
	        return this->get_value() != other.get_value();
	    }

	    // Overload assignment operator
	    card& operator=(const card& other) {
	    	if (this != &other) {
	    		this->card_value = other.get_value();
	    	}
	    	return *this;
	    }

	    // Overload stream insertion operator for easy printing
	    friend std::ostream& operator<<(std::ostream& os, const card& c) {
	        os << c.to_string();
	        return os;
	    }
	    // *************************************************************************
};

#endif