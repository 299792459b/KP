# KP
Kuhn Poker is a simplified poker game with two players and three cards, for example J, Q, K represented as 1, 2, 3 in the program and only one betting round.  Through self-play, the optimal playing strategy is realized in the limit.  The optimal strategy is determined through counterfactual regret minimization algorithm that approximates the Nash equilibrium. 
Regret is a number that describes how much one regrets a particular decision. Some examples are: folding a good hand (wrong thing to do, high regret), folding a bad hand (right thing to do, low regret).  The strategy in a particular position is averaged over the number of iterations based on this regret and therefore gives the optimal strategy in any particular position.  This theory can be applied to more complex poker games like NLH, LH, etc.

Rules:

Basically limit hold'em (bet of 1) with one betting street and ante of 1.  E.g.:
- Both players ante 1 into pot and are each dealt one of the three cards
- Player 1 has option to check or bet 1
  - If Player 1 bet:
    - Player 2 can call or fold
  - If Player 1 check:
    - Player 2 can bet, or check
      - If Player 2 bet:
        - Player 1 can call or fold
