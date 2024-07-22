#ifndef CARD_POOL_H
#define CARD_POOL_H

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "card.h"

// 0 -> 2; 1 -> 3; 2 -> 4; 3 -> 5; 4 -> 6; 5 -> 7; 6 -> 8
// 7 -> 9; 8 -> 10; 9 -> J; 10 -> Q; 11 -> K; 12 -> A

// 0 -> Clubs; 1 -> Diamonds; 2 -> Hearts; 3 -> Spades



class card_pool {
    private:
      	// const int CARDS_PER_SUIT = 13;
      	const int HAND_SIZE = 5;
      	const int MAX_CARDS = 7;

        std::vector<card> my_card_pool;
        std::map<int, std::vector<card>> rank_map;
        std::map<int, std::vector<card>> suit_map;

        std::vector<card> hand;
        int hand_type = -1; // 0 -> high card; 1 -> pair; 2 -> two pair; 3 -> trips; 
        // 4 -> straight; 5 -> flush; 6 -> full house; 7 -> quads; 8 -> straight flush;

        void sorted_insert(std::vector<card> &vec, card card_in) {
            // find where card fits in the vector in order from highest to lowest value
            auto it = vec.begin();
            while (it != vec.end() && card_in < (*it)) {
                ++it;
            }
            // insert card into the vector
            vec.insert(it, card_in);
        }


        // adds card to the suit map and rank map and increments respective size arrays.
        void add_to_maps(card card_in) {
            this->check_card_range(card_in);

            // check if rank is already in rank_map. If not, add it.
        	  if (this->rank_map.find(card_in.get_rank()) == this->rank_map.end()) {
        		    this->rank_map[card_in.get_rank()] = std::vector<card>();
        	  }

            // check if suit is already in suit_map. If not, add it.
        	  if (this->suit_map.find(card_in.get_suit()) == this->suit_map.end()) {
        		    this->suit_map[card_in.get_suit()] = std::vector<card>();
        	  }

            // add card to correct vector in map.
            this->sorted_insert(this->rank_map[card_in.get_rank()], card_in);
            this->sorted_insert(this->suit_map[card_in.get_suit()], card_in);
        }

        // remove a card from maps
        void remove_from_maps(card card_in) {
            this->check_card_range(card_in);

            // remove from rank vector
            std::vector<card>& rank_vector = rank_map[card_in.get_rank()];
            for (auto v_it = rank_vector.begin(); v_it != rank_vector.end(); ++v_it) {
                if ((*v_it) == card_in) {
                    rank_vector.erase(v_it);
                    break;
                }
            }

            // remove from suit vector
            std::vector<card>& suit_vector = suit_map[card_in.get_suit()];
            for (auto v_it = suit_vector.begin(); v_it != suit_vector.end(); ++v_it) {
                if ((*v_it) == card_in) {
                    suit_vector.erase(v_it);
                    break;
                }
            }
        }

        void check_card_range(card card_in) const {
            if (card_in.get_value() < 0 || card_in.get_value() >= 52) {  // check card is valid
                throw std::out_of_range("Card value is out of range.\n");
            }
        }

        bool card_is_unique(card card_in) const {
            // make sure there is not an exact replica card in card_pool
            for (card c : this->my_card_pool) {
              	if (card_in == c) {
              	   return false;
              	}
            }
            return true;
        }
    
    // Make_hand-specific methods **********************************************

        int find_flush_and_straight_flush() {
            // check for flush and straight flush in hand using suit_map
            int hand_found = 0; 
            for (auto m_it = this->suit_map.begin(); m_it != this->suit_map.end(); ++m_it) {
                const std::vector<card> &cards_in_suit = m_it->second;
                if (cards_in_suit.size() >= HAND_SIZE) {  // flush found
                    hand_found = 5;
                    int flush_count = 0;               // flush count
                    int straight_flush_count = 1;      // straight flush count

                    // prepare for loop
                    card prev = *cards_in_suit.begin();
                    this->hand.push_back(prev);
                    ++flush_count;

                    for (auto card_it = cards_in_suit.begin()+1; card_it != cards_in_suit.end(); ++card_it) {
                        // add five highest suited cards to hand
                        if (flush_count < HAND_SIZE) {
                            this->hand.push_back(*card_it);
                            ++flush_count;
                        }

                        // increment for straight flush
                        if ((*card_it).get_rank() == prev.get_rank()-1) {
                            ++straight_flush_count;
                        } else {
                            straight_flush_count = 1;
                        }

                        // check for straight flush
                        if (straight_flush_count == HAND_SIZE) {
                            this->hand.clear();
                            for(int j=0; j<HAND_SIZE; ++j) {
                                this->hand.push_back(card((*card_it).get_value() + j));
                            }
                            hand_found = 8;
                            return hand_found;
                        }

                        // update prev
                        prev = (*card_it);
                      }

                      // check for 5-high straight flush
                      if (straight_flush_count == 4 && (*cards_in_suit.begin()).get_rank() == 0 && (*cards_in_suit.rbegin()).get_rank() == 12) {
                          ++straight_flush_count;
                          this->hand.clear();
                          this->hand.push_back(*cards_in_suit.rbegin());
                          auto sf_it = cards_in_suit.begin();
                          for (int j=0; j<4; ++j) {
                              this->hand.push_back(*sf_it);
                              ++sf_it;
                          }
                          hand_found = 8;
                          return hand_found;
                    }
                }
            } 
            return hand_found;
        }


        int find_straight() {
            // check for straight using rank_map
            int hand_found = 0;
            int straight_count = 1;
            auto m_it = this->rank_map.rbegin();
            int prev = m_it->first;
            ++m_it;
            for (; m_it != this->rank_map.rend(); ++m_it) {
                const int &rank = m_it->first;

                // straight incrementor
                if (rank == prev-1) {
                    ++straight_count;
                } else {
                    straight_count = 1;
                }
                
                // check if straight found
                if (straight_count == HAND_SIZE) {
                    auto straight_it = m_it.base();
                    --straight_it;
                    for (int k=0; k<HAND_SIZE; ++k) {
                        this->hand.push_back(*(straight_it->second.begin()));
                        ++straight_it;
                    }
                    hand_found = 4;
                    return hand_found;
                }
                prev = rank;
            }

            // check for 5-high straight
            if (straight_count == 4 && this->rank_map.begin()->first == 0 && this->rank_map.rbegin()->first == 12) {  // A-5 straight
                this->hand.push_back(*(this->rank_map.rbegin()->second).begin());
                auto straight_it = rank_map.begin();
                for (int k=0; k < 4; ++k, ++straight_it) {
                    if (straight_it == rank_map.end()) {
                        std::cout << "Error at 5-high straight.\n";
                        break;
                    }
                    this->hand.push_back(*(straight_it->second.begin()));
                }
                hand_found = 4;
            }
            return hand_found;
        }


        void add_kickers(std::set<int> ranks_in_hand) {  // adds n kickers to the hand
            auto rank_it = rank_map.rbegin();
            // outer loop counts how many high cards have been added
            for (int i=hand.size(); i<HAND_SIZE; ++i) {
                if (rank_it == rank_map.rend()) {
                    std::cout << "Error in add_kickers\n";
                    return;
                }
                // skip over ranks already in our hand
                while (ranks_in_hand.find(rank_it->first) != ranks_in_hand.end()) {
                    ++rank_it;
                    if (rank_it == rank_map.rend()) {
                        std::cout << "Error in add_kickers\n";
                        return;
                    }
                }
                // add card to hand
                hand.push_back(*(rank_it->second.begin()));
                ++rank_it;
            }
        }

        void add_n_of_a_kind(int n, std::set<int>& ranks_in_hand) {
            // searches for best rank which occurs n times and adds it to hand, returning its rank as well
            if (n + hand.size() > HAND_SIZE) {
                std::cout << "Error in add_n_of_a_kind.\n";
                return;
            }

            // 
            for (auto m_it = rank_map.rbegin(); m_it != rank_map.rend(); ++m_it) {
                if (ranks_in_hand.find(m_it->first) != ranks_in_hand.end()) continue;  // make sure hasn't already been added to hand
                if (m_it->second.size() >= n) {  // if number of cards with this rank > n, we are ready to add it to our hand
                    auto v_it = m_it->second.begin();
                    for (int i=0; i<n; ++i) {
                        hand.push_back(*v_it);
                        ++v_it;
                    }
                    ranks_in_hand.insert(m_it->first);
                    return;
              }
          }

          std::cout << "Error, no n-duplicate rank found\n";
          return;
        }

    //***********************************************************************

    // Compare-specific methods ************************************************
        int remove_n_of_a_kind(std::vector<int>& ranks, int n) const {  // *********Double check this guy....
            // remove the value with n occurrences in the vector "vec".
            for (auto v_it = ranks.begin(); v_it != ranks.end(); ++v_it) {
                int k = 1;
                // check if correct value
                auto v2_it = v_it + 1;
                while (v2_it != ranks.end() && (*v2_it) == (*v_it)) {
                    ++k;
                    ++v2_it;
                }
                // remove occurences
                if (k >= n) {
                    int rank_removed = (*v_it);
                    ranks.erase(v_it, v_it + n);
                    return rank_removed;
                }
            }
            return -1;
        }

        // The following methods do two things: 1. They remove all cards that make up the pair/trips/quads from hand_ranks
        // 2. They compare the ranks that they removed from each hand and use that to determine which hand is better.
        // this allows us to compare the hand while removing cards we have already looked at to make future comparisons easier.
        // I wonder if using pointers instead of deletion might be faster though?
        // THESE METHODS ASSUME THE hand_ranks VECTORS ARE SORTED. THIS MUST BE TRUE FOR THEM TO WORK...
        int compare_pairs(std::vector<int>& hero_hand_ranks, std::vector<int>& villan_hand_ranks) const {
            int hero_pair = remove_n_of_a_kind(hero_hand_ranks, 2);
            int villan_pair = remove_n_of_a_kind(villan_hand_ranks, 2);

            if (hero_pair > villan_pair) {
                return 1;
            } else if (hero_pair < villan_pair) {
                return -1;
            } else {
                return 0;
            }
        }

        int compare_trips(std::vector<int>& hero_hand_ranks, std::vector<int>& villan_hand_ranks) const {
          int hero_trips = remove_n_of_a_kind(hero_hand_ranks, 3);
          int villan_trips = remove_n_of_a_kind(villan_hand_ranks, 3);

          if (hero_trips > villan_trips) {
            return 1;
          } else if (hero_trips < villan_trips) {
            return -1;
          } else {
            return 0;
          }
        }

        int compare_quads(std::vector<int>& hero_hand_ranks, std::vector<int>& villan_hand_ranks) const {
          int hero_quads = remove_n_of_a_kind(hero_hand_ranks, 4);
          int villan_quads = remove_n_of_a_kind(villan_hand_ranks, 4);

          if (hero_quads > villan_quads) {
            return 1;
          } else if (hero_quads < villan_quads) {
            return -1;
          } else {
            return 0;
          }
        }

        // This method loops though ranks in hand_ranks vectors and compares, them. if end is reached and all adjacent
        // (same index) cards are equal, then the arrays are equal.
        int compare_high_cards(std::vector<int>& hero_hand_ranks, std::vector<int>& villan_hand_ranks) const {
          // loop through remaining cards for high card
            for (int i=0; i<hero_hand_ranks.size(); ++i) {  // loop through hand and compare
              if (hero_hand_ranks[i] > villan_hand_ranks[i]) {
                return 1;
              } else if (hero_hand_ranks[i] < villan_hand_ranks[i]) {
                return -1;
              }
            }
            return 0;
        }
  //***********************************************************************

  //***********************************************************************
    public:
      // constructor
      card_pool() = default;

      explicit card_pool(const std::vector<card> &card_pool_init) {
          // should check length of input vector at some point

          for (auto card : card_pool_init) {
              this->add_card(card);
          }
      }


      // add a card to the pool
      void add_card(card card_in) {
          // make sure card_pool is not bigger than 7, which is impossible
          if (this->my_card_pool.size() == MAX_CARDS) {
            	std::cout << "Error, card pool is full\n";
            	return;
          }

          // check input card
          this->check_card_range(card_in);       // ensure it is within the correct range
          if (!this->card_is_unique(card_in)) {   // ensure it is unique
              std:: cout << "Error, card is already in pool\n";
              return;
          }
          // do the stuff
          this->my_card_pool.push_back(card_in);
          this->add_to_maps(card_in);

          if (this->my_card_pool.size() == MAX_CARDS) {
          	 this->make_hand();
          }
      }

      // remove a card from the pool
      void remove_card(card card_in) {
          if (this->my_card_pool.empty()) {return;}

          // find card to be erased
          auto v_it = std::find(this->my_card_pool.begin(), this->my_card_pool.end(), card_in);

          // erase card if found
          if (v_it != this->my_card_pool.end()) {
              this->my_card_pool.erase(v_it);
              this->remove_from_maps(card_in);
          }
      }

  	//***********************************************************************
      // rank-related getters
      const std::map<int, std::vector<card>>& get_rank_map() const {
          return this->rank_map;
      }


      // suit-related getters
      const std::map<int, std::vector<card>>& get_suit_map() const {
          return this->suit_map;
      }

      // other getters
      const std::vector<card>& get_my_card_pool() const {
      	  return this->my_card_pool;
      }

      const std::vector<card>& get_hand() const {
      	  return this->hand;
      }

      const int& get_hand_type() const {
      	  return this->hand_type;
      }
      //***********************************************************************
      // some print statements for testing
      void print_card_pool() {
        	for (auto v_it = this->my_card_pool.begin(); v_it != this->my_card_pool.end(); ++v_it) {
          		std::cout << (*v_it) << " ";
        	}
        	std::cout << "\n";
      }

      void print_hand() {
  		    for (auto v_it = this->hand.begin(); v_it != this->hand.end(); ++v_it) {
        		std::cout << (*v_it) << " ";
      	  }
      	  std::cout << "\n";
      }

      void print_hand_type() {
        	switch (hand_type) {
          		case 0:
            			std::cout << "High Card\n";
            			break;
          		case 1:
            			std::cout << "Pair\n";
            			break;
          		case 2:
            			std::cout << "Two pair\n";
            			break;
          		case 3:
            			std::cout << "Trips\n";
            			break;
          		case 4:
            			std::cout << "Straight\n";
            			break;
          		case 5:
            			std::cout << "Flush\n";
            			break;
          		case 6:
            			std::cout << "Full House\n";
            			break;
          		case 7:
            			std::cout << "Quads\n";
            			break;
          		case 8:
            			std::cout << "Straight Flush\n";
            			break;
        	}
      }
      // **********************************************************************
      // Returns the best hand for the current card pool
      void make_hand() {
          // this method utilizes rank_map and suit_map to find the best hand more efficiently
          // for any given suit in the card pool, we can loop through all cards in the suit map,
          // in order of rank, just as we can loop though all cards in the card pool based off of rank
          // This allows us to easily find flushes, straights, and duplicate rank hands.

        	this->hand.clear();
          int best_hand = 0;

          // we only want to make a hand if our card pool is full
        	if (this->my_card_pool.size() != 7) {
          		std::cout << "Error, must have a card pool of length 7\n";
          		return;
        	}

        	// 0 -> high card; 1 -> pair; 2 -> two pair; 3 -> trips; 
        	// 4 -> straight; 5 -> flush; 6 -> full house; 7 -> quads; 8 -> straight flush;

          // check for flush and straight flush, if none found, set best_hand to 0
          best_hand = this->find_flush_and_straight_flush();

        	// check for straight if no better hand has been found

          if (best_hand == 0) {
              best_hand = this->find_straight();
          }

        	// Duplicate-rank and high-card hands
        	// What we're going to do it make the best duplicate-rank/high card hand we can make
        	// and if it is better than what has already been found, we store it...
        	if (best_hand < 8) {
          		// Use duplicate-ranks array to find the best duplicate rank hand and then add high cards
          		// at the end depending on how many we need.

          		// count how many quads, trips, pairs, high cards in card_pool
          		int duplicate_ranks[4] = {0, 0, 0, 0};
          		for (auto m_it = this->rank_map.begin(); m_it != this->rank_map.end(); ++m_it) {
          			  ++duplicate_ranks[m_it->second.size()-1];
          		}

          		// determine and make hand
              std::set<int> ranks_in_hand;
          		if (duplicate_ranks[3] > 0) {											// quads //
                  this->hand.clear();
                  // add quads to hand
            			this->add_n_of_a_kind(4, ranks_in_hand);
                  // update best_hand
            			best_hand = 7;
          		} else if ((duplicate_ranks[2] > 0) && (duplicate_ranks[1] > 0 || duplicate_ranks[2] > 1)) {			// full house //
            			this->hand.clear();
                  // add trips to hand
                  this->add_n_of_a_kind(3, ranks_in_hand);
                  // add pair to hand
                  this->add_n_of_a_kind(2, ranks_in_hand);
                  // update best hand
            			best_hand = 6;
          		} else if (best_hand <= 3 && duplicate_ranks[2] > 0) {					// trips //
                  this->hand.clear();
            			// add trips to hand
                  this->add_n_of_a_kind(3, ranks_in_hand);
                  // update best hand
            			best_hand = 3;
          		} else if (best_hand <= 2 && duplicate_ranks[1] > 1) {					// two pair //
                  this->hand.clear();
            			// add first pair to hand
                  this->add_n_of_a_kind(2, ranks_in_hand);
                  // add second pair to hand
                  this->add_n_of_a_kind(2, ranks_in_hand);
                  // update best hand
            			best_hand = 2;
          		} else if (best_hand <= 1 && duplicate_ranks[1] > 0) {					// pair //
                  this->hand.clear();
            			// add pair to hand
                  this->add_n_of_a_kind(2, ranks_in_hand);
                  // update best hand
        				  best_hand = 1;
          		}

              // add the remainign high cards to the hand, exluding the duplicate ranks already selected.
              if (best_hand != 6) this->add_kickers(ranks_in_hand);
        	}

          if (hand.size() != HAND_SIZE) {
              std::cout << "Error, hand size is [" << hand.size() << "] after make_hand\n";
          }

        	this->hand_type = best_hand;
      }

      // ***********************************************************************************************************
      int compare (const card_pool& villan_card_pool) const {
          if (this->get_hand_type()  == -1) {
              std::cout << "Error, hero does not have a hand.\n";
              return -2;
          } else if (villan_card_pool.get_hand_type() == -1) {
              std::cout << "Error, villan does not have a hand.\n";
              return -2;
          }
          
        	// Compare hands
        	// my hand is: -1 -> worse, 0 -> equal, 1 -> better
        	if (this->get_hand_type() < villan_card_pool.get_hand_type()) {
        		  return -1;
        	} else if (this->get_hand_type() > villan_card_pool.get_hand_type()) {
        		  return 1;
        	} else {  // same hand type...

          		// copy ranks of cards in each hand
          		std::vector<int> villan_hand_ranks;
              for (auto card : villan_card_pool.get_hand()) {
                  villan_hand_ranks.push_back(card.get_rank());
              }
          		std::vector<int> hero_hand_ranks;
              for (auto card : this->get_hand()) {
                  hero_hand_ranks.push_back(card.get_rank());
              }

          		// descending insertion sort the hands by rank
          		std::sort(hero_hand_ranks.begin(), hero_hand_ranks.end(), std::greater<int>());
              std::sort(villan_hand_ranks.begin(), villan_hand_ranks.end(), std::greater<int>());

          		// start by analyzing duplicate hands. We check for equality between the duplicates in each hand, 
          		// then remove them from each hand if they have the same rank. After this step, we compare the high 
          		// cards that are left over. if at any time a comparison is not equal, we return the higher hard. 
          		// If all cards compared are equal, the two hands are equal.
              int cmp1;
              int cmp2;
          		switch (this->get_hand_type()) {
                  case 1:  // pair
                      cmp1 = this->compare_pairs(hero_hand_ranks, villan_hand_ranks);
                      if (cmp1 != 0) return cmp1;
                      break;
                  case 2:  // two pair
                      cmp1 = this->compare_pairs(hero_hand_ranks, villan_hand_ranks);
                      if (cmp1 != 0) return cmp1;
                      cmp2 = this->compare_pairs(hero_hand_ranks, villan_hand_ranks);
                      if (cmp2 != 0) return cmp2;
                      break;
                  case 3:  // trips
                      cmp1 = this->compare_trips(hero_hand_ranks, villan_hand_ranks);
                      if (cmp1 != 0) return cmp1;
                      break;
            			case 6:  // full house 
                      cmp1 = this->compare_trips(hero_hand_ranks, villan_hand_ranks);
                      if (cmp1 != 0) return cmp1;
                      cmp2 = this->compare_pairs(hero_hand_ranks, villan_hand_ranks);
                      if (cmp2 != 0) return cmp2;
                      break;
        	    		case 7:  // quads
                      cmp1 = this->compare_quads(hero_hand_ranks, villan_hand_ranks);
                      if (cmp1 != 0) return cmp1;
                      break;
        		  }
              // this covers the kickers of duplicate hands and any non-duplicate hands
              return compare_high_cards(hero_hand_ranks, villan_hand_ranks);
        	}
      }
};



#endif