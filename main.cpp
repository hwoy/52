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
				if(group[index]->live && group[index]->score<=SCORE) break;
			
			for(unsigned int i=index+1;i<group.size();i++)
				if( group[i]->live && group[i]->score<=SCORE && t(group[i]->score , group[index]->score)) index=i;
			
			return index;
		}

	
	static std::mt19937 gen;
	
	
	computer(unsigned int id,const char *name,unsigned int money)
	:player_t(id,name,money)
	{
		A = random(_A-2,_A+2);
		B = random(_B-5,_B+5);
		C = random(_C-5,_C+20);
	}
	virtual char bid(const group_t &group) const 
	{
		char ch;
		

		if( group[find(group,[](unsigned int a,unsigned int b)->bool{return a>b;} )]->id==id)
			{
				if(std::count_if(group.begin(),group.end(),[](const player_ptr &player)->bool { return player->live && player->canbid;  } )==1 )
					return IDNO;
				
				else{
					std::uniform_int_distribution<unsigned int> dis(1,100);
		
		
					if((SCORE-score)<=2) ch=(dis(gen)<=A?IDYES:IDNO);
					else if((SCORE-score)<=5) ch=(dis(gen)<=B?IDYES:IDNO);
					else if((SCORE-score)<=9) ch=(dis(gen)<=C?IDYES:IDNO);
					else ch=IDYES;					
				}
			}
			
		else ch=IDYES;
	
		
		return ch;
	}
	
	private:
	static unsigned int random(unsigned int a,unsigned int b)
	{
		std::uniform_int_distribution<> dis;
		
		return a+(dis(gen)%(b-a+1));
	}

};

std::mt19937 computer::gen(system_clock::to_time_t(system_clock::now()));

struct human final : public player_t
{
	human(unsigned int id,const char *name,unsigned int money):player_t(id,name,money){}
	virtual char bid(const group_t &deck) const
	{
		
		std::string str;
		
		do{
			std::cout << name << "<"  <<'[' << IDYES << ']' << "Yes," << '[' << IDNO << ']' << "No," << '[' << IDQUIT << ']' << "Quit"<< ">:";
			std::getline(std::cin,str);
			
			if(!str.size()) str=IDDEFAULT;
			
		}while((str[0]!=IDYES && str[0]!=IDNO && str[0]!=IDQUIT) || str.size()>1);
		
		return str[0];
	}
};


static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck);

static void showwinner(const player_t &player,unsigned int money);

static void showwinnerofthematch(const player_t &player);

template <std::size_t M,std::size_t N>
static deck_t constructdeck(const Card::Rank (&rank)[M],const Card::Suit (&suit)[N]);


static const Card::Rank rank[]={ {0,"A",1} , {1,"2",2} , {2,"3",3} ,\
					  {3,"4",4} , {4,"5",5} , {5,"6",6} ,\
					  {6,"7",7} , {7,"8",8} , {8,"9",9} ,\
					  {9,"10",10} , {10,"K",10} , {11,"Q",10} , {12,"K",10} };
					  
static const Card::Suit suit[]={{0,"Spade",0} , {1,"Heart",1} , {2,"Diamon",2}, {3,"Clover",3}};


int main()
{
	unsigned int bid=BID,money=MONEY;
	
	bool iswin,nonoecanwin,idquit=false;
	unsigned int winindex;
	
	std::vector<unsigned int> vec;
	
	game52_t game52(bid);
	
	deck_t deck=constructdeck(rank,suit);
	
	group_t group;
	
	group.push_back(std::shared_ptr<player_t>(new computer(0,"Hwoy",money)));
	group.push_back(std::shared_ptr<player_t>(new computer(1,"View",money)));
	group.push_back(std::shared_ptr<player_t>(new computer(2,"Kung",money)));
	group.push_back(std::shared_ptr<player_t>(new computer(3,"Ding",money)));
	group.push_back(std::shared_ptr<player_t>(new computer(4,"Lekk",money)));


	
	do{
		
		bool outloop=false;
		
		game52.shufflephase(deck);
	
		game52.drawphase(group,deck,DRAW);
			
		showinfo(game52,group,deck);
	
		do
		{
	
		for(auto &playerPtr:group)
			{
				if(playerPtr->live && playerPtr->canbid && playerPtr->money>=bid)
				{
					char ch=playerPtr->bid(group);
			
					std::cout << playerPtr->name << ": " << ch << std::endl;
			
					if(ch==IDYES)
					{
						game52.draw(*playerPtr,deck);
						std::cout << playerPtr->name << " ===> " << playerPtr->deck.back() << std::endl;
				
					}
					
					else if(ch==IDQUIT) { idquit=true;break; }
				
					else
					{
						playerPtr->canbid=false;
					}

					showinfo(game52,group,deck);
			
					std::tie(nonoecanwin,vec)=game52.gameover(group);
			
					if(!(!nonoecanwin && !vec.size())) { outloop=true;break; }
				}
			}

		} while(!outloop && !idquit);
		
	if(idquit) break;
	
	if(!nonoecanwin && group[vec.front()]->score<=SCORE)
	{
		auto &winner=*group[vec.front()];
		showwinner(winner,game52.money);
		winner.money+=game52.money;
	}
	else{
		std::cout << "\n========= All fuckin win!! =========\n";
		for(auto &playerPtr:group)
			if(playerPtr->live)
			{
				showwinner(*playerPtr,playerPtr->deck.size()*bid);
				playerPtr->money+=(playerPtr->deck.size()*bid);
			}
		}
		
	game52.update(group);
	game52.endphase(group,deck);
	
	std::tie(iswin,winindex)=game52.matchover(group);
	}while(!iswin && !idquit);
	
	if(!idquit) showwinnerofthematch(*group[winindex]);
	
	
	return 0;
}

static void showwinner(const player_t &player,unsigned int money)
{
	std::cout << "\n========= Winner is " << player.name << "[+" << money << "] =========\n";
}

static void showwinnerofthematch(const player_t &player)
{
	std::cout << "\n\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Winner of the match is " << player.name << "[" << player.money << "] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
}

static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck)
{
	std::cout << "\nMain Deck: <" << deck.size() << ">\n";
	
	std::cout << "Money: <" << game52.money << ">\n";
	
	std::cout << "BID: <" << game52.bid << ">\n\n";
	
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



