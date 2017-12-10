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


template <typename G=Group<>,typename D=Deck<> >
struct Game52
{
	unsigned int money;
	
	Game52():money(0){}
	
	
	void shufflephase(D &deck,unsigned int loop=10240)
	{
		deck.shuffle(loop);
	}
	
	void drawphase(G &group,D &deck,unsigned int n=1)
	{
		Game52<G,D>::take(group,deck,n);

	}
	
	void draw(G &group,D &deck,unsigned int index,unsigned int n=1)
	{
		draw(group[index],deck,n);
	}
	
	template <typename P>
	void draw(P &player,D &deck,unsigned int n=1)
	{
		takep(player,deck,n);

	}
	
	static void score(G &group)
	{
		for(auto &i:group)
			score(i);
	}
	
	template <typename P>
	static unsigned score(P &player)
	{
		unsigned int sum=0;
		for(const auto &i:player.deck)
			sum+=i.rank.value;
		
		return player.score=sum;
	}
	
	void endphase(G &group,D &deck)
	{
		Game52<G,D>::giveall(group,deck);
		
		Game52<G,D>::money=0;
		
		for(auto &i:group)
		{
			i.score=0;
			i.canbid=true;
		}
		
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
	

	void take(G &group,D &deck,unsigned int n)
	{
		for(unsigned int i=0;i<n;i++)
		{
			for(auto &i:group)
			{
				takep(i,deck,1);
			}
		}
	}
	
	template <typename P>
	void takep(P &player,D &deck,unsigned int n)
	{
			for(unsigned int i=0;i<n;i++)
		{
				if(player.canbid && player.live)
				{
					player.deck.take(deck,1);
					player.deck.back().visible=true;
					
					player.money-=BID;
					money+=BID;
				}
		}
	}
};

#endif