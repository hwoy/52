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

#include "Copt.hpp"

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
	
	
	computer(unsigned int id,const std::string &name,unsigned int money)
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
	human(unsigned int id,const std::string &name,unsigned int money):player_t(id,name,money){}
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


static const char* option[] = { "-b:", "-m:", "-h:", "-c:", "-H" };

static const char* option_des[] = { "-b:number <Bid money>", "-m:money <Money>", "-h:name <Human>", "-c:name <Computer>", "Help" };

enum optid : unsigned int 
{ 	opt_b,
    opt_m,
    opt_h,
    opt_c,
    opt_H };
	
template <std::size_t M>
static void showhelp(const char* (&opt)[M],const char* (&opt_des)[M]);
	
	
enum errid : unsigned int 
{ 	err_nan,
    err_invalidopt,
    err_bid,
    err_money,
    err_player,
	err_bidmoney,
	err_up2player,
	err_empty};
	
static const char* err_des[] = { "Not a number", "Invalid option", "Less than BID", "Less than MONEY", "Less than MAXPLAYER","bid more than money","up to 2 players","Empty name" };

static int showerr(const char** err_des,unsigned int id,const std::string &str);

static bool isdigital(const std::string &str)
{
	if(str.empty()) return false;
		
	for(const auto &i:str)
		if(!(i>='0' && i<='9'))
			return false;
		
	return true;
}


int main(int argc,const char *argv[])
{
	
	bool iswin,nonoecanwin,idquit=false;
	
	unsigned int winindex;

	
	/*********************************  Option  *********************************/
	unsigned int bid=BID,money=MONEY;
	unsigned int id,maxplayer=0;
	std::string str;
	group_t group;
	
	Copt opt(argc, argv, option);
	
	if (opt.argc == 1) {
		showhelp(option,option_des);
        return 0;
    }
	
    while (std::tie(id, str) = opt.action(), id != Copt::ID::END) {
        switch (id) {
        case optid::opt_b:
			
			if(!isdigital(str))
				return showerr(err_des,errid::err_nan, option[opt_b]+str);
			
            bid=std::stoul(str);
            break;

        case optid::opt_m:
		
			if(!isdigital(str))
				return showerr(err_des, errid::err_nan, option[opt_m]+str);
			
			money=std::stoul(str);
            break;

        case optid::opt_h:
			if(str.empty()) return showerr(err_des,errid::err_empty, option[opt_h]+str);
			group.push_back(std::shared_ptr<player_t>(new human(maxplayer++,str,MONEY)));
	
            break;

        case optid::opt_c:
			if(str.empty()) return showerr(err_des,errid::err_empty, option[opt_c]+str);
			group.push_back(std::shared_ptr<player_t>(new computer(maxplayer++,str,MONEY)));
			
            break;

        case optid::opt_H:
            showhelp(option,option_des);
            return 0;
            break;

        default:
		showhelp(option,option_des);
		return showerr(err_des,errid::err_invalidopt, str);
        }
    }
	
	if(bid<BID)
		return showerr(err_des,errid::err_bid, std::to_string(bid) +" < " + std::to_string(BID));
	if(money<MONEY)
		return showerr(err_des,errid::err_money, std::to_string(money)+" < " +std::to_string(MONEY));
	if(bid>money)
		return showerr(err_des,errid::err_bidmoney, std::to_string(bid) + " > " +std::to_string(money));
	
	if(maxplayer<2)
		return showerr(err_des,errid::err_up2player, std::to_string(maxplayer));
	if(maxplayer>MAXPLAYER)
		return showerr(err_des,errid::err_player, std::to_string(maxplayer) + " > " +std::to_string(MAXPLAYER));
		
	
	for(auto &playerPtr:group)
		playerPtr->money=money;
	
	game52_t game52(bid);
	/*********************************  Option  *********************************/
	
	std::vector<unsigned int> vec;
	
	deck_t deck=constructdeck(rank,suit);

	
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

static int showerr(const char** err_des,unsigned int id,const std::string &str)
{
	std::cerr << std::endl << str << std::endl;
	std::cerr << "Error ID:" << id << " ====> " << err_des[id] << std::endl;
	return id+1;
}

template <std::size_t M>
static void showhelp(const char* (&opt)[M],const char* (&opt_des)[M])
{
	std::cerr << "\n\n";
	for(unsigned int i=0;i<M;i++)
		std::cerr << opt[i] << " =====> " << opt_des[i] << std::endl;
	
	std::cerr << "\nMinimum bid(BID) = " << BID << std::endl;
	std::cerr << "Minimum money(MONEY) = " << MONEY << std::endl;
	std::cerr << "Maximum player(MAXPLAYER) = " << MAXPLAYER << std::endl;
}



