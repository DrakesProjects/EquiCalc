EquiCalc Holdem

Written By Drake Adams, 07/21/2024


Summary: This is a simple library which calculates your odds of winning in a Texas Holdem poker hand based off the information you provide. It runs as many randomized simulations of the hand as you want to give you accurate estimates.


Header Files:
  
  card.h – This is the underlying representation of any given card in the program. Internal values of card range from integers in the range 0 to 51, inclusive. Each integer in that range represents a unique card.
 
  card_pool.h – This is essentially the rules of poker. It holds all of the cards a given user has access to at any given time and creates the hand, as well as compares that hand to other hands.
 
  equity_calculator.h – This quite literally has one method which calculates the equities.
 
  player.h – identifies the player and stores their hole cards and their card pool.

  Side note:
   - I've realized that the method I use to compare hands is much too complex (takes too much time to be ran thousands of times in a row) and not really needed. The method is not completely useless though, as it accurately compares two 5-card hands perfectly, but I don't really need to be comparing the actual hands...
   - My plan is to use this method of hand comparison i've developed to create a series of maps which map hands to their 'rank,' or how many hands it is better than. This way if I need to compare two hands, instead of needing to label the type of hand it is, I can just get their hand ranks (extremely quickly, log time I believe) and compare that.
   - A concern I have about this is the size of a file needed to store all hand ranks, so I would also like to find some ways to reduce the amount of space by not storing every single hand rank which exists (52 choose 5).
