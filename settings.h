#include<iostream>
#include<fstream>
#include<Windows.h>
#include<string>
#include <iomanip>
#include<vector>
#include<random>
#include<set>
#include<queue>
#include<map>
#define equal 0
#define follow 1
#define stay 2
#define clever 3
#define crazy 4
#define sprawn 5
#define dying 6
extern int mp[20][20];
extern std::fstream fi;
class character
{
	int type;
	int id;
	int x, y;
	int hp, atk, sp;
	int mv, atr;
	int team;
	
	/* 燃烧：每回合减少20-50点血量
眩晕：角色该回合无法操作
生命恢复：每回合恢复20-30点血量
顽强：承伤系数减少0.25
脆弱：承伤系数增加0.25
锋利：攻击系数增加0.25
虚弱：攻击系数减少0.25
心神不定：每回合恢复的能量减少一点
粘滞：角色无法移动
*/
	int effect[9];
	std::set<int> card;
	double hurtnum;
	double atknum;
public:
	character(int _id = 0, int _x = 19, int _y = 19, int team_ = 0);
	void move(int move_step = -1);
	void gethurt(int hurt);
	int attack(int range = -1);
	void rest();
	void special();
	bool reflash();
	bool dead();
	void card_select();
	bool incircle(int xo,int yo,int r);
	void print();
	int Getteam();
};
class slot
{
public:
	int id;
	int achievement[10];
	int& operator[] (int i);
};
extern std::map<int, character> info;
extern int cur;
extern int achievement[10];
extern std::set<int> checkpoints;
extern int slots[10];
char sleep(int t);
void show_map();
int randint(int a, int b);
std::string character_name(int id);
std::set<int> explode(int x, int y, int harm,int range);
void description(int num);
inline void saving(int id);
int w1();
void c1(int x);
void c2_1();
void c2_2();
void c2(int x);