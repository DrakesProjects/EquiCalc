#ifndef PLAYER_H
#define PLAYER_H

#include <array>
#include "card_pool.h"

class player {
	private:
		card_pool my_card_pool;
		std::vector<card> hole_cards;
		std::vector<card> community_cards;

		std::string id;
		bool button = false;

		// remove hole cards from card_pool and player classes
		void clear_hole_cards() {
			for (auto v_it = hole_cards.begin(); v_it != hole_cards.end(); ++v_it) {
				my_card_pool.remove_card(*v_it);
			}
			hole_cards.clear();
		}

		// remove community cards from card_pool and player classes
		void clear_community_cards() {
			for (auto v_it = community_cards.begin(); v_it != community_cards.end(); ++v_it) {
				my_card_pool.remove_card(*v_it);
			}
			community_cards.clear();
		}

	public:
		explicit player(std::string id) {
			this->id = id;
		}

		// print statements for testing ***************************************
		void print_card_pool() {
			this->my_card_pool.print_card_pool();
		}

		void print_hand() {
			this->my_card_pool.print_hand();
		}

		void print_hand_type() {
			this->my_card_pool.print_hand_type();
		}

		void print_hole_cards() {
			std::cout << "[" << hole_cards[0];
			std::cout << " ";
			std::cout << hole_cards[1] << "]";
		} 
		
		// getters and setters ***********************************************
		void set_community_cards(std::vector<card> community_cards) {
			this->clear_community_cards();
			if (community_cards.size() <= 5) {
				this->community_cards = community_cards;
				for (auto card : community_cards) {
					my_card_pool.add_card(card);
				}
			} else {
				std::cout << "Error: too many community cards : " << community_cards.size() << "\n";
			}
			return;
		}

		void set_hole_cards(std::vector<card> hole_cards) {
			this->clear_hole_cards();
			if (hole_cards.size() <= 2) {
				this->hole_cards = hole_cards;
				for (auto card : hole_cards) {
					my_card_pool.add_card(card);
				}
			} else {
				std::cout << "Error: Too many hole cards dealt.\n";
			}
			return;
		}

		const std::vector<card>& get_community_cards() {
			return this->community_cards;
		}

		const std::string& get_id() const {
			return this->id;
		}

		const std::vector<card>& get_hand() const {
			return this->my_card_pool.get_hand();
		}

		int get_hand_type() const {
			return this->get_card_pool().get_hand_type();
		}

		const std::vector<card>& get_hole_cards() const {
			return this->hole_cards;
		}

		const card_pool& get_card_pool() const {
			return my_card_pool;
		}

		// compare hands with another player **************************
		int compare(const player villan) const {
			return my_card_pool.compare(villan.get_card_pool());
		}
};

#endif