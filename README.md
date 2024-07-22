EquiCalc Holdem

Written By Drake Adams, 07/21/2024


Summary: This is a simple library which calculates your odds of winning in a Texas Holdem poker hand based off the information you provide. It runs as randomized simulations of the hand as you want to give you accurate estimates.


Header Files:
  
  card.h – This is the underlying representation of any given card in the program. Internal values of card range from integers in the range 0 to 51, inclusive. Each integer in that range represents a unique card.
 
  card_pool.h – This is essentially the rules of poker. It holds all of the cards a given user has access to at any given time and creats the hand, as well as compares that hand to other hands.
 
  equity_calculator.h – This quite literally has one method which calculates the equities.
 
  player.h – identifies the player, their hole cards, and their card pool. Each player has a unique id as well.
