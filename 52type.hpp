#ifndef __52TYPE_H__
#define __52TYPE_H__

#include "card.hpp"
#include "player.hpp"
#include "52.hpp"



typedef Card card_t;
typedef Deck<> deck_t;

typedef Player<> player_t;

typedef Group<> group_t;

typedef group_t::Player_t player_ptr;

typedef Game52<> game52_t;



#endif