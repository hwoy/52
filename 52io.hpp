#ifndef __52IO_H__
#define __52IO_H__

#include <iostream>

#include "card.hpp"
#include "player.hpp"


template <typename T,typename U>
std::basic_ostream<T,U> & operator<< (std::basic_ostream<T,U> &os, const Card &card)
{
	os << card.suit.name << "[" << card.rank.name << "] ";
	
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
	os << player.name << " ===> " << player.deck;

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