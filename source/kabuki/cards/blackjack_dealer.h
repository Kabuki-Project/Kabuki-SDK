/** kabuki::cards
    @file    ~/source/kabuki/cards/dealer.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef HEADER_FOR_KABUKI_CARDS_BLACKJACKDEADER
#define HEADER_FOR_KABUKI_CARDS_BLACKJACKDEADER

#include "dealer.h"
#include "blackjack_player.h"

namespace kabuki { namespace cards {

/** A dealer in a card game.
    A dealer has the Deck of cards, but is not necessarily a player. A
    dealer needs to keep track of all of the Players. A BlackjackDealer is both
    a Player and a Dealer at the same time. */
class BlackjackDealer : public Dealer, public BlackjackPlayer {
    public:
    
    enum {
        kNumDecks           = 1,      //< Number of decks for this game.
        kDeckSize           = 52,     //< Number of cards in the Deck.
        kStartingPoints     = 999999, //< Dealers starting points.
        kStartingAnte       = 5,      //< Starting ante.
        kMinBet             = 1,      //< Min bet.
        kMinNumCardsPerHand = 2,      //< Min cards in a hand.
        kMaxNumPlayer       = 2,      //< Max number of players.
        kMaxNumCardsPerHand = Deck::kDefaultDeckSize, //< Max cards in a hand.
        kKeyboardBufferSize = 79,     //< Size of the keyboard input buffer.
    };

    /** Default constructor. */
    BlackjackDealer ();

    /** Resets the number of wins to 0. */
    virtual void DealHand (CardStack& stock);

    /** Processes beginning of round logic. */
    virtual void BeginRound ();

    /** Processes beginning of round logic. */
    virtual void EndRound ();

};      //< class BlackjackDealer
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_CARDS_BLACKJACKDEADER
