#ifndef EQUITY_CALCULATOR_H
#define EQUITY_CALCULATOR_H

#include "player.h"
#include <set>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>



class equity_calculator {
	private:
		const int DECK_SIZE = 52;
		const int HAND_SIZE = 5;
		int iterations = 100;

		// take a card off the deck
		card pop_card(std::vector<card>& deck) {
		    if (deck.empty()) {
		        throw std::runtime_error("Error: trying to pop a card off an empty deck");
		    }
		    card popped_card = deck.back();
		    deck.pop_back();
		    return popped_card;
		}

		// for getting random ints
		unsigned int generate_random_seed() {
		    std::random_device rd; // Non-deterministic random number generator
		    unsigned int seed = rd();
		    seed ^= std::chrono::system_clock::now().time_since_epoch().count(); // Mix with current time
		    seed ^= reinterpret_cast<std::uintptr_t>(&rd); // Mix with address of random_device
		    return seed;
		}



	public:
		equity_calculator() = default;

		// how many simulations do we want to run?
		explicit equity_calculator(int iterations) {
			this->iterations = iterations;
		}

		// another method that could be interesting is the frequency of winning hands. But idk how that would really be helpful
		// for users... We'll see.

		// returns the equity of each player in an array
		std::vector<double> get_equity(std::vector<player> players, std::vector<card> community_cards) {			
			if (players.size() == 0 || players.size() == 1) {return {1.0};}
			
			// record cards in play
			std::set<card> cards_in_play(community_cards.begin(), community_cards.end());
		    for (auto v_it = players.begin(); v_it != players.end(); ++v_it) {
		        const std::vector<card>& hole_cards = v_it->get_hole_cards();
		        cards_in_play.insert(hole_cards.begin(), hole_cards.end());
		    }
		    
			// create full deck 
			std::vector<card> deck;
			deck.reserve(DECK_SIZE - cards_in_play.size());
			for (int n = 0; n < DECK_SIZE; ++n) {
			    if (cards_in_play.find(card(n)) == cards_in_play.end()) {
			        deck.push_back(card(n));
			    }
			}

			// hand type counter
			std::vector<int> hand_count(9, 0);

			// we have two options here: run many randomized iterations and get the average, or run every possible outcome

			// create random int generator
	        std::mt19937 gen(this->generate_random_seed());

	        std::vector<int> scores(players.size(), 0);
	        int unknown_community_cards = 5 - community_cards.size();
	        for (int i=0; i<iterations; ++i) {
	        	// std::cout << i << "\n";
	        	// create copies for safety
	        	std::vector<card> community_cards_cpy = community_cards;
	        	std::vector<player> players_cpy = players;
	        	std::vector<card> deck_cpy = deck;

	        	// shuffle deck
	        	std::shuffle(deck_cpy.begin(), deck_cpy.end(), gen);

	        	// fill in unknown player cards
	        	for (auto v_it = players_cpy.begin(); v_it != players_cpy.end(); ++v_it) {
	        		std::vector<card> tmp_hole = (*v_it).get_hole_cards();
	        		if (tmp_hole.size() < 2) {
	        			for (int k=0; k<2-tmp_hole.size(); ++k){
	        				tmp_hole.push_back(this->pop_card(deck_cpy));
	        			}
	        			(*v_it).set_hole_cards(tmp_hole);
	        		}
	        	}

	        	// fill in unknown community cards
	        	for (int j=0; j<unknown_community_cards; ++j) {
	        		community_cards_cpy.push_back(this->pop_card(deck_cpy));
	        	}

	        	// add community cards to player card pools
	        	for (auto v_it = players_cpy.begin(); v_it != players_cpy.end(); ++v_it) {
	        		(*v_it).set_community_cards(community_cards_cpy);
	        	}

	        	// determine winner(s)
	        	std::vector<player> winners = {players_cpy[0]};
	        	for (int i=1; i<players_cpy.size(); ++i) {
	        		int cmp = players_cpy[i].compare(winners[0]);
	        		if (cmp == 1) {
	        			winners.clear();
	        			winners.push_back(players_cpy[i]);
	        		} else if (cmp == 0) {
	        			winners.push_back(players_cpy[i]);
	        		} else if (cmp != -1) {
	        			std::cout << "Error with compare output [" << cmp << "]\n";
	        			return {};
	        		}
	        	}

	        	// increment score of winner
	        	if (winners.size() != 1) {
	        		continue;
	        	} else {
	        		for (int j=0; j<players.size(); ++j) {
	        			if (players[j].get_id() == (*winners.begin()).get_id()) {
	        				++scores[j];
	        				break;
	        			}
	        		}
	        	}
	      
	        	// increment hand_count
	        	++hand_count[winners[0].get_hand_type()];
	    	}


	    	// calculate equities
	    	std::vector<double> final_equities;
	    	for (const auto& score : scores) {
	    		final_equities.push_back(score*1.0 / iterations*1.0);
	    	}

	    	return final_equities;
		}
};

#endif