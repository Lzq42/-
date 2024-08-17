#include"settings.h"
using namespace std;
int w1()
{
	cout << "新手教程已下放到一旁的 expre.out 中了\n";
	cout << "战斗开始！\n";
	memset(mp, 0, sizeof(mp));
	queue<int> que;
	que.push(1);
	que.push(2);
	info[1] = character(1, 5, 5, 1);
	info[2] = character(2, 4, 5, 2);
	set<int>teamcount;
	while (true)
	{
		int num = que.front();
		que.pop();
		int a = info[num].reflash(), b = info[num].dead();
		teamcount.clear();
		for(auto i:info)
		{
			if (i.second.Getteam())
				teamcount.insert(i.second.Getteam());
		}
		if (teamcount.size() == 1)
		{
			if (teamcount.count(1))
			{
				cout << "战斗胜利！\n";
				return 1;
			}
			else
			{
				cout << "战斗失败……\n";
				return 0;
			}
			
		}
		cout << "————————————————————————————————————————————————\n\n" << character_name(num) << "的回合\n";
		if(!a && b)
		{ 
			cout << character_name(num) << "无法行动，本回合已自动跳过。\n";
			que.push(num);
			continue;
		}
		if (!b)
		{
			cout << character_name(num) << "已死亡！！！\n";
			info.erase(num);
			continue;
		}
		que.push(num);
		cout << "开始行动吧。\n";
		bool flag = true;
		while (flag)
		{
			cout << "a.信息\n    1.地图\n    2.角色\nb.行动\n    3.攻击\n    4.移动\n    5.休憩\n    6.技能\nc.卡牌\n    7.抽卡\n";
			int act;
			cin >> act;
			switch (act)
			{
			case 1:
				show_map();
				cout << endl;
				cout << "1为黎雾\n2为九章\n";
				cout << sleep(1) << '\n';
				break;
			case 2:
				info[num].print();
				break;
			case 3:
				info[num].attack();
				flag = false;
				break;
			case 4:
				info[num].move();
				flag = false;
				break;
			case 5:
				info[num].rest();
				flag = false;
				break;
			case 6:
				info[num].special();
				flag = false;
				break;
			case 7:
				info[num].card_select();
				flag = false;
				break;
			}
		}
	}
}