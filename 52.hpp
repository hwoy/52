#ifndef __52_H__
#define __52_H__


#include <vector>

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif

#include <initializer_list>


#include "card.hpp"
#include "player.hpp"
#include "52config.hpp"


template <typename G=Group<>,typename D=Deck<> >
struct Game52
{
	unsigned int money;
	
	Game52():money(0){}
	
	
	void shufflephase(D &deck,unsigned int loop=10240)
	{
		deck.shuffle(loop);
	}
	
	static void drawphase(G &group,D &deck,unsigned int n=1)
	{
		Game52<G,D>::take(group,deck,n);
	}
	
	static void draw(G &group,D &deck,unsigned int index,unsigned int n=1)
	{
		Game52<G,D>::take(group,deck,n);
	}
	
	void endphase(G &group,D &deck)
	{
		Game52<G,D>::giveall(group,deck);
	}

	static void giveall(G &group,D &deck)
	{
		for(auto &i:group)
		{
			for(auto &j:i.deck)
			{
				j.visible=false;
			}
			
			i.deck.giveall(deck);
		}
	}
	

	static void take(G &group,D &deck,unsigned int n)
	{
		for(unsigned int i=0;i<n;i++)
		{
			for(auto &i:group)
			{
				i.deck.take(deck,1);
				i.deck.back().visible=true;
			}
		}
	}
};

#endif