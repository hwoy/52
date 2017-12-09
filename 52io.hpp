#ifndef __52IO_H__
#define __52IO_H__

#include <iostream>

#include "card.hpp"
#include "player.hpp"



template <typename T,typename U>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const Card &card)
{
	os << ((card.visible)?card.suit.name:"X") << "[" << ((card.visible)?card.rank.name:"X") << "] ";
	
	return os;
}

template <typename T,typename U,typename C,typename G>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const Deck<C,G> &deck)
{
	for(const auto &i:deck)
		os << i;
	return os;
}

template <typename T,typename U,typename D>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const Player<D> &player)
{
	os << player.name << "["<< player.money <<"]";
	if(player.live)
	{
		os << " ===> " << player.deck << " <==== " << player.score << (!player.canbid?"*\n":"\n");
	}
	else{
		os << "*\n";
	}

	return os;
}

template <typename T,typename U,typename P>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const Group<P> &group)
{
	for(const auto &i:group)
		os << i << std::endl;
	return os;
}



#endif