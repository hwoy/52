#include <iostream>
#include <string>
#include <iterator>
#include <random>
#include <chrono>
#include <algorithm>



#include "card.hpp"
#include "player.hpp"
#include "52.hpp"
#include "52io.hpp"

#include "52config.hpp"
#include "52type.hpp"

using namespace std::chrono;

struct computer final : public player_t
{

		template <typename T>
		static unsigned int find (const group_t &group, const T &t)
		{
			unsigned int index;
			for(index=0;index<group.size();index++)
				if(group[index]->live) break;
			
			for(unsigned int i=index+1;i<group.size();i++)
				if( group[i]->live && t(group[i]->score , group[index]->score)) index=i;
			
			return index;
		}

	
	static std::mt19937 gen;
	
	
	computer(unsigned int id,const char *name):
	player_t(id,name)
	{
		A = random(_A-10,_A+5);
		B = random(_B-10,_A+20);
		C = random(_C-10,_C+20);
	}
	virtual char bid(const group_t &group) const 
	{
		char ch;
		

		if( group[find(group,[](unsigned int a,unsigned int b)->bool{return a>b;} )]->id==id &&
			std::count_if(group.begin(),group.end(),[](const player_ptr &player)->bool { return player->live && player->canbid;  } )==1 )
				return 'n';

		std::uniform_int_distribution<unsigned int> dis(1,100);
		
		
		if((SCORE-score)<=2) ch=(dis(gen)<=A?'y':'n');
		else if((SCORE-score)<=5) ch=(dis(gen)<=B?'y':'n');
		else if((SCORE-score)<=9) ch=(dis(gen)<=C?'y':'n');
		
		else ch='y';
	
		
		return ch;
	}
	
	private:
	unsigned int random(unsigned int a,unsigned int b)
	{
		std::uniform_int_distribution<> dis;
		
		return a+(dis(gen)%(b-a+1));
	}

};

std::mt19937 computer::gen(system_clock::to_time_t(system_clock::now()));

struct human final : public player_t
{
	human(unsigned int id,const char *name):player_t(id,name){}
	virtual char bid(const group_t &deck) const
	{
		
		return 'y';
	}
};


static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck);

static void showwinner(const player_t &player,unsigned int money);

template <std::size_t M,std::size_t N>
static deck_t constructdeck(const Card::Rank (&rank)[M],const Card::Suit (&suit)[N]);


static const Card::Rank rank[]={ {0,"A",1} , {1,"2",2} , {2,"3",3} ,\
					  {3,"4",4} , {4,"5",5} , {5,"6",6} ,\
					  {6,"7",7} , {7,"8",8} , {8,"9",9} ,\
					  {9,"10",10} , {10,"K",10} , {11,"Q",10} , {12,"K",10} };
					  
static const Card::Suit suit[]={{0,"Spade",0} , {1,"Heart",1} , {2,"Diamon",2}, {3,"Clover",3}};


int main()
{
	deck_t deck=constructdeck(rank,suit);
	
	group_t group;
	
	group.push_back(std::shared_ptr<computer>(new computer(0,"Hwoy")));
	group.push_back(std::shared_ptr<computer>(new computer(1,"View")));
	group.push_back(std::shared_ptr<computer>(new computer(2,"Kung")));
	group.push_back(std::shared_ptr<computer>(new computer(3,"Ding")));



	game52_t game52;
	

	game52.shufflephase(deck);
	
	game52.drawphase(group,deck);
	
	std::vector<unsigned int> vec;
	do
	{
	
	for(auto &player:group)
	{
		if(player->live && player->canbid)
		{
			char ch=player->bid(group);
			
			std::cout << player->name << ": " << ch << std::endl;
			
			if(ch=='y')
			{
				game52.draw(player,deck);
				std::cout << player->name << " ===> " << player->deck.back() << std::endl;
				
			}
				
			else
			{
				player->canbid=false;
			}
			
			showinfo(game52,group,deck);
		}
	}
	
	vec=game52.gameover(group);
	
	} while(!vec.size());
	
	auto &winner=*group[vec.front()];
	showwinner(winner,game52.money);
	winner.money+=game52.money;
	
	
	game52.endphase(group,deck);
	
	
	return 0;
}

static void showwinner(const player_t &player,unsigned int money)
{
	std::cout << "========= Winner is " << player.name << "[+" << money << "] =========\n";
}

static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck)
{
	std::cout << "\nMain Deck: <" << deck.size() << ">\n";
	
	std::cout << "Money: <" << game52.money << ">\n";
	
	std::cout << "BID: <" << BID << ">\n\n";
	
	std::cout << group;
	
}


template <std::size_t M,std::size_t N>
static deck_t constructdeck(const Card::Rank (&rank)[M],const Card::Suit (&suit)[N])
{
	deck_t deck;
	
	for(unsigned int i=0,c=0;i<N;i++)
	{
		for(unsigned int j=0;j<M;j++)
		{
			deck.push_back({c++,rank[j],suit[i]});
		}
	}
		
		return deck;
}



