#include "equity_calculator.h"
#include "player.h"
#include <vector>
#include <iostream>
#include <chrono>

int main(void) {
	int iterations = 1000000;

	std::cout << "Start\n";

	// set up players
	player hero("1");
	player villan("2");
	player villan2("3");

	hero.set_hole_cards({card("2s"), card("3s")});
	villan.set_hole_cards({card("As"), card("Ac")});
	villan2.set_hole_cards({card("5d"), card("5c")});

	std::vector<player> v_in = {hero, villan, villan2};

	// set up equity calculator instance
	equity_calculator ec(iterations);

	// set up community cards
	std::vector<card> community_cards = {card("4s"), card("5s"), card("6d")};

	// print community cards
	std::cout << "Preset Community Cards: ";
	if (community_cards.size() == 0) {
		std::cout << "none\n";
	} else {
		for (auto card : community_cards) {
			std::cout << card << " ";
		}
		std::cout << "\n";
	}
	std::cout << "---------------------\n";

	// get equities
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<double> v_out = ec.get_equity(v_in, community_cards);

	auto end = std::chrono::high_resolution_clock::now();

	for (int i=0; i<v_in.size(); ++i) {
		std::cout << "id: " << v_in[i].get_id() << "\n";
		v_in[i].print_hole_cards();
		std::cout << "\nEquity: " << v_out[i] << std::endl;
		std::cout << "---------------------\n";
	}

	std::chrono::duration<double> duration = end - start;
	std::cout << "Duration: " << duration.count() << " seconds" << std::endl;
	std::cout << "Iterations: " << iterations << std::endl;
}