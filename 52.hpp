#ifndef __52_H__
#define __52_H__


#include <utility>

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif

#include <algorithm>

#include "card.hpp"
#include "player.hpp"


template <typename G=Group<>,typename D=Deck<> >
struct Game52
{
	typedef typename G::Player_ptr Player_ptr;
	typedef typename G::Player_t Player_t;
	
	unsigned int bid;
	unsigned int money;
	
	Game52(unsigned int bid):bid(bid),money(0){}
	
	
	void shufflephase(D &deck,unsigned int loop=10240)
	{
		deck.shuffle(loop);
	}
	
	void drawphase(G &group,D &deck,unsigned int n=1)
	{
		Game52<G,D>::take(group,deck,n);
		Game52<G,D>::score(group);

	}
	
	void draw(G &group,D &deck,unsigned int index,unsigned int n=1)
	{
		Game52<G,D>::draw(*group[index],deck,n);
	}
	
	void draw(Player_t &player,D &deck,unsigned int n=1)
	{
		Game52<G,D>::takep(player,deck,n);
		Game52<G,D>::score(player);

	}
	
	static void score(G &group)
	{
		for(auto &playerPtr:group)
			Game52<G,D>::score(*playerPtr);
	}
	
	static unsigned score(Player_t &player)
	{
		unsigned int sum=0;
		for(const auto &i:player.deck)
			sum+=i.rank.value;
		
		player.score=sum;
		
		if(player.score>SCORE) player.canbid=false;
		
		return sum;
	}
	
	void endphase(G &group,D &deck)
	{
		Game52<G,D>::giveall(group,deck);
		
		Game52<G,D>::money=0;
		
		for(auto &playerPtr:group)
		{
			playerPtr->score=0;
			playerPtr->canbid=true;
		}
		
	}

	static void giveall(G &group,D &deck)
	{
		for(auto &playerPtr:group)
		{
			for(auto &j:playerPtr->deck)
			{
				j.visible=false;
			}
			
			playerPtr->deck.giveall(deck);
		}
	}
	

	void take(G &group,D &deck,unsigned int n)
	{
		for(unsigned int i=0;i<n;i++)
		{
			for(auto &playerPtr:group)
			{
				takep(*playerPtr,deck,1);
			}
		}
	}
	

	void takep(Player_t &player,D &deck,unsigned int n)
	{
			for(unsigned int i=0;i<n;i++)
		{
				if(player.canbid && player.live)
				{			
					player.deck.take(deck,1);
					player.deck.back().visible=true;
					
					
					player.money-=Game52<G,D>::bid;
					money+=Game52<G,D>::bid;
					

					if(player.money<Game52<G,D>::bid) 
					{
						player.canbid=false;break;
					}

				}
		}
	}
	
		
	static std::pair<bool,std::vector<unsigned int>> gameover(const G &group)
	{
		std::vector<unsigned int> vec;
		
		for(unsigned int i=0;i<group.size();i++)
			if(group[i]->live && group[i]->score==SCORE)
			{
				vec.push_back(i);
				return std::pair<int,std::vector<unsigned int>>(false,vec);
			}
		
		
		if(std::any_of(group.begin(),group.end(),[](const Player_ptr &playerPtr)->bool{return playerPtr->live && playerPtr->canbid;}))
			return std::pair<int,std::vector<unsigned int>>(false,vec);
		
		for(unsigned int i=0;i<group.size();i++)
			if(group[i]->live && group[i]->score<=SCORE) vec.push_back(i);
		
		if(!vec.size())
			return std::pair<bool,std::vector<unsigned int>>(true,vec);
		
		
		std::sort(vec.begin(),vec.end(),[&group](unsigned int a,unsigned int b)->bool{
			return group[a]->score > group[b]->score;
		});
		
		
		while(group[vec.front()]->score>group[vec.back()]->score) vec.pop_back();
		
		return std::pair<bool,std::vector<unsigned int>>(false,vec);
	}
	
	
	static std::pair<bool,unsigned int> matchover(const G &group)
	{
		unsigned int j=0,id=0;
		
		for(const auto &player:group)
		{
			if(player->live)
			{
				j++;
				id=player->id;
			}
		}
		
		return std::make_pair(j==1,id);
	}
	
	
	void update(G &group)
	{
		for(auto &playerPtr:group)
			update(*playerPtr);
	}
	
	void update(Player_t &player)
	{
		if(player.money < Game52<G,D>::bid) player.live=false;
	}
};

#endif