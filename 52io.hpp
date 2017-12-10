#ifndef __52IO_H__
#define __52IO_H__

#include <iostream>

#include "card.hpp"
#include "player.hpp"
#include "52.hpp"

#include "52type.hpp"



template <typename T,typename U>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const card_t &card)
{
	os << ((card.visible)?card.suit.name:"X") << "[" << ((card.visible)?card.rank.name:"X") << "] ";
	
	return os;
}

template <typename T,typename U>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const deck_t &deck)
{
	for(const auto &i:deck)
		os << i;
	return os;
}

template <typename T,typename U>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const player_t &player)
{
	os << player.name << "(";
	
	if(!player.A && !player.B && !player.C)
		 os << "Human";
	else os << player.A << "," << player.B << "," << player.C;
	
	os << ")" << "["<< player.money <<"]";
	if(player.live)
	{
		os << " ===> " << player.deck << " <==== " << player.score << (!player.canbid?"*\n":"\n");
	}
	else{
		os << "*\n";
	}

	return os;
}

template <typename T,typename U>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const group_t &group)
{
	for(const auto &i:group)
		os << *i << std::endl;
	return os;
}



#endif