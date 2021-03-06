#ifndef __52TYPE_H__
#define __52TYPE_H__

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif


#include "card.hpp"
#include "player.hpp"
#include "52.hpp"

typedef std::mt19937 gen_t;

typedef Rank rank_t;

typedef Suit suit_t;

typedef Card<rank_t,suit_t> card_t;

typedef Deck<card_t> deck_t;

typedef Player<deck_t,gen_t> player_t;

typedef Group<player_t> group_t;

typedef group_t::Player_ptr player_ptr;


typedef Game52<group_t,deck_t> game52_t;

#if __cplusplus > 201402L
	#include <string_view>
	typedef std::string_view string_t;
#else
	#include <string>
typedef std::string string_t;
#endif



#endif