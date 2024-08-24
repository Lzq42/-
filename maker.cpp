#include "settings.h"
using namespace std;
int mp[20][20];
std::fstream fi;
std::map<int, character> info;
std::set<int>checkpoints;
char sleep(int t)
{
	Sleep(t * 1000);
	return '\n';
}
// TODO 不要忘记更新它！！！；
std::string character_name(int id)
{
	switch (id)
	{
	case 1:
		return "黎雾";
	case 2:
		return "九章";
	case 3:
		return "忆源";

	case 10:
		return "近卫甲";
	case 11:
		return "近卫乙";
	}
}
int randint(int a, int b)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> distrib(a, b);
	return distrib(gen);
}
// TODO 不要忘记更新它！！！；
character::character(int _id,int _x,int _y,int _team)
{
	int datas[5];
	fstream fi;
	fi.open("data.in", std::ios::in);
	while(!fi.eof())
	{
		for (int i = 0; i < 4; i++)
		{
			fi >> datas[i];
		}
		if (datas[0] == _id)
		{
			id = _id;
			hp = datas[1];
			atk = datas[2];
			type = datas[3];
			break;
		}
	}
	fi.close();
	switch (_id)
	{
	case 1:
		atr = 2;
		mv = 2;
		sp = 0;
		break;
	case 2:
		atr = 1;
		mv = 1;
		sp = 0;
		break;
	case 3:
		atr = 2;
		mv = 2;
		sp = 0;
		break;
	case 10:
		atr = 2;
		mv = 1;
		sp = 0;
		break;
	case 11:
		atr = 1;
		mv = 1;
		sp = 0;
		break;
	}
	x = _x;
	y = _y;
	mp[x][y] = id;
	sp = 0;
	team = _team;
	memset(effect, 0, sizeof(effect));
	card = set<int>();
	atknum = 1.0;
	hurtnum = 1.0;
	if (type == follow)
		hurtnum = 1.25;
	if (type == crazy)
	{
		atknum = 1.25;
		hurtnum = 0.75;
	}
	if (type == clever)
		sp = 9;
}
void character::move(int move_step)
{
	if (move_step == -1)
		move_step = mv;
	int cx, cy, ans;
	if (card.count(3) && randint(0, 1))
		move_step++;
	if (effect[8])
		move_step = max(0, move_step - 1);
	do
	{
		
		cout << "角色当前位于 (" << x << ',' << y << ")处。可移动步数有" << move_step << "步。\n请输入移动坐标；";
		cout << "x坐标:";
		cin >> cx;
		cout << "y坐标:";
		cin >> cy;
		if (cx > 15 || cy > 15 || cx < 0 || cy < 0)
			cout << "移动超越地图边界！\n";
		else if (abs(cx - x) + abs(cy - y) > move_step)
			cout << "移动步数过多！移动了" << abs(cx - x) + abs(cy - y) << "步，最大移动" << move_step << "步。\n";
		else if (mp[cx][cy])
			cout << "实体重叠！与人同挤一格！\n";
		else
		{
			cout << "确定要这么移动吗？(1(同意)/0(不同意))\n";
			cin >> ans;
			if (ans)
			{
				mp[x][y] = 0;
				x = cx;
				y = cy;
				mp[x][y] = id;
				break;
			}
		}
	} while (true);
	if (type == follow)
		sp++;
	cout << sleep(1) << '\n';
}
void character::gethurt(int hurt)
{
	if (card.count(14) && sp * 10 <= randint(0, 100))
	{
		hurt *= 0.3;
	}
	if (effect[3])
		hurt *= 0.75;
	if (effect[4])
		hurt *= 1.25;
	hp -= hurt * hurtnum;
	if (type == dying)
		sp++;
	if (card.count(4) && randint(0, 1))
		sp++;
	if (card.count(10))
	{
		if (sp >= ceil(hurt * hurtnum / 10))
		{
			sp -= ceil(hurt * hurtnum / 10);
			hp += hurt * hurtnum;
		}
		else
		{
			hp += sp * 10;
			sp = 0;
		}
	}
	if (card.count(15) && randint(0, 1))
	{
		cout << "获得一次重新行动机会！\n请选择行动：\n1.移动\n2.攻击\n3.休息\n";
		int aa;
		cin >> aa;
		switch (aa)
		{
		case 1:
			move();
			break;
		case 2:
			attack();
			break;
		case 3:
			rest();
			break;
		}
	}
}
int character::attack(int range)
{
	set<int> list;
	if (range == -1)
		range = atr;
	for (int i = 0; i < 10; i++)
	{
		if (info[i].incircle(x,y,range) && info[i].team != team)
			list.insert(i);
	}
	cout << "可攻击对象有：\n";
	for (auto i:list)
		cout << i << "." << character_name(i) << '\n';
	int ans;
	cin >> ans;
	if (!list.count(ans) && ans != -1)
	{
		cout << "核验不通过！\n";
		return 0;
	}
	cout << "造成了" << atk * atknum * (effect[5] ? 1.25 : 1) * (effect[6] ? 0.75 : 1) << "点伤害。";
	info[ans].gethurt(atk * atknum * (effect[5] ? 1.25 : 1) * (effect[6] ? 0.75 : 1));
	cout << sleep(1) << '\n';
	return ans;
}
void character::rest()
{
	int past = sp;
	int ran = randint(1, 3);
	if (effect[7])
		ran--;
	switch (type)
	{
	case equal:
	case sprawn:
	case dying:
		sp += ran;
		break;
	case clever:
	case follow:
	case crazy:
		sp += max(0,ran - 1);
		break;
	case stay:
		sp += ran + 1;
	}
	if (card.count(9))
		sp += 2;
	cout << character_name(id) << "恢复了" << sp - past << "能量。\n\n\n";
}
bool character::reflash()
{
	bool returning = true;
	for (int i = 0; i < 10; i++)
	{
		if (effect[i])
		{
			effect[i] -= 1;
			switch (i)
			{
			case 0:
				hp -= randint(20, 50);
				break;
			case 1:
				returning = false;
				break;
			case 2:
				hp += randint(20, 30);
				break;
			}
		}
	}
	return returning;
}
void character::special()
{
	std::set<string> able;
	int choice;
	int cur;
	set<int> current;
	switch (id)
	{
	case 1:
		cout << "1.文气护盾-3：使一个角色获得2回合【顽强】\n";
		cout << "2.平天下-5: 使一个角色获得3回合【锋利】\n";
		cout << "3.上天入地-7：跳跃到3格以内的地方，对周围2格敌人施加2回合【眩晕】，并造成伤害\n";
		if (sp >= 3)
			able.insert("1.文气护盾");
		if (sp >= 5)
			able.insert("2.平天下");
		if (sp >= 7 && hp > 100)
			able.insert("3.上天入地");
		cout << "目前能量有" << sp << "点，可选择的有：";
		for (auto i : able)
		{
			cout << i << endl;
		}
		cout << "选择什么?  :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			if (sp < 3)
			{
				cout << "不合法哦~。" << "\n";
				return;
			}
			for (auto i : info)
				if (i.second.team == team)
					current.insert(i.second.id);
			cout << "可附加增益的角色有：\n";
			for (auto i : current)
				cout << i << '.' << character_name(i);
			cout << "选择：";
			cin >> cur;
			info[cur].effect[3] += 2;
			break;
		case 2:
			if (sp < 5)
			{
				cout << "不合法哦~。" << "\n";
				return;
			}
			for (auto i : info)
				if (i.second.team == team)
					current.insert(i.second.id);
			cout << "可附加增益的角色有：\n";
			for (auto i : current)
				cout << i << '.' << character_name(i);
			cout << "选择：";
			cin >> cur;
			info[cur].effect[5] += 3;
			break;
		case 3:
			if (sp < 7)
			{
				cout << "注意能量，大招费能。" << "\n";
				return;
			}
			move(3);
			current = explode(x, y, atk, 2);
			for (auto i : current)
			{
				info[i].effect[1] += 2;
			}
			break;
		}
		break;
	case 2:
		cout << "1.冲锋-3：九章驱散【粘滞】并立即冲锋5格\n";
		cout << "2.泰山压顶-5: 九章扑倒半径1格以内的一个敌人，对其施加2回合眩晕\n";
		cout << "3.重拳-7：九章向敌方打出一记威力爆炸的重拳\n";
		if (sp >= 3)
			able.insert("1.冲锋");
		if (sp >= 5)
			able.insert("2.泰山压顶");
		if (sp >= 7 && hp > 100)
			able.insert("3.重拳");
		cout << "目前能量有" << sp << "点，可选择的有：\n";
		for (auto i : able)
		{
			cout << i << endl;
		}
		cout << "选择什么?  :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			if (sp < 3)
			{
				cout << "不合法哦~。" << "\n";
				return;
			}
			sp -= 3;
			effect[8] = 0;
			move(5);
			if (card.count(11))
			{
				current = explode(x, y, 0, 1);
				for (auto i : current)
					info[i].effect[1]++;
			}
			break;
		case 2:
			if (sp < 5)
			{
				cout << "不合法哦~。" << "\n";
				return;
			}
			sp -= 5;
			cur = attack();
			info[cur].effect[1] += 2;
			if (card.count(12))
				info[cur].effect[1]++;
			break;
		case 3:
			if (sp < 7)
			{
				cout << "爱惜生命，你我有责。注意能量，大招费能。" << "\n";
				return;
			}
			sp -= 7;
			cur = attack();
			info[cur].effect[1] += 1;
			info[cur].gethurt(atk * atknum * 2 * (card.count(13) ? 1.5 : 0.5));
			break;
		}
	case 3:
		cout << "1.地瓜补给-3：向队友和敌人投掷特殊地瓜(爆炸地瓜、发芽地瓜、熟地瓜、甜地瓜)，分别造成爆炸伤害、【虚弱】【脆弱】、【生命恢复】、【锋利】\n";
		cout << "2.地瓜泥-5: 形成一片半径为2的地瓜泥，对打击范围内的|所有人|造成2回合【粘滞】状态\n";
		cout << "3.巨瓜天降-7：扔出一个巨大的爆炸地瓜\n";
		if (sp >= 3)
			able.insert("1.地瓜补给");
		if (sp >= 5)
			able.insert("2.地瓜泥");
		if (sp >= 7)
			able.insert("3.巨瓜天降");
		cout << "目前能量有" << sp << "点，可选择的有：\n";
		for (auto i : able)
		{
			cout << i << endl;
		}
		cout << "选择什么?  :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			if (sp < 3)
			{
				cout << "不合法哦~。" << "\n";
				return;
			}
			sp -= 3;
			cur = attack(3);
			if(randint(0,1))
				if(info[cur].Getteam() != team)
					explod(info[cur].x,info[cur].y,atk*(card.count(17) ? atknum + 0.5 : atknum),1);
				else
					info[cur].effect[2]+= (card.count(17) ? 3 : 2);
			else
				if(info[cur].Getteam() != team)
					info[cur].effect[4]+= 2,info[cur].effect[6]+= (card.count(17) ? 3 : 2);
				else
					info[cur].effect[5]+= (card.count(17) ? 3 : 2);
			break;
		case 2:
			if (sp < 5)
			{
				cout << "不合法哦~。" << "\n";
				return;
			}
			sp -= 5;
			cout << "请输入地瓜泥的圆心坐标:(x y)\n";
			int x,y;
			cin>>x>>y;
			auto g = explode(x,y,0,2);
			for(auto i:g)
				info[i].effect[8]+=(card.count(16) ? 3 : 2);
			break;
		case 3:
			if (sp < 7)
			{
				cout << "爱惜生命，你我有责。注意能量，大招费能。" << "\n";
				return;
			}
			sp -= 7;
			cout << "请输入巨型爆炸地瓜的坐标:(x y)\n";
			int x,y;
			cin>>x>>y;
			auto g = explode(x,y,atk * (card.count(18) ? atknum + 0.5 : atknum) + 25,2);
			break;
		}

	case 10:
		if(sp>=2)
		{
			cout << "急速拉弓-2：立刻进行三次普攻";
			sp -= 2;
			attack();
			attack();
			attack();
		}
		else
			return;
	case 11:
		if(sp>=3)
		{
			cout << "缩头乌龟-3：获得三回合的【顽强】";
			sp -= 3;
			effect[3]+=3;
		}
		else
			return;
	}
	
	cout << sleep(1) << '\n';
}
bool character::dead()
{
	if (hp <= 0)
	{
		if (card.count(5))
		{
			hp = 1;
			effect[3] += 2;
			return true;
			card.erase(5);
			card.insert(5);
		}
		if (card.count(2))
			explode(x, y, sp * 10 + atknum, 2);
		if (card.count(8))
		{
			for (auto i : info)
			{
				if (i.second.team == team)
				{
					i.second.effect[2] += 3;
					i.second.effect[3] += 3;
					i.second.effect[5] += 3;
				}
			}
		}
		mp[x][y] = 0;
		return false;
	}
	return true;
}
void character::card_select()
{
	if (card.size() > 6 || sp < 5)
	{
		cout << "卡组已到上限！或能量不足！请查看原因\n";
		return;
	}
	cout << "已扣除5点能量\n";
	sp -= 5;
	cout << "开始抽取……" << sleep(3);
	set<int> got;
	fi.open("卡库.in", ios::in);
	int ty, num;
	string na;
	while (!fi.eof())
	{
		fi >> ty >> num >> na;
		if (ty == 0 || ty == id)
			got.insert(num);
	}
	while (true)
	{
		num = randint(0, 15);
		if (got.count(num) && !card.count(num) && !card.count(-num))
			break;
	}
	card.insert(num);
	description(num);
	switch (num)
	{
	case 0:
		atknum -= 0.1;
		hurtnum -= 0.25;
		break;
	case 1:
		atknum += 0.2;
		hurtnum += 0.25;
		break;
	case 6:
		mv++;
		hurtnum += 0.5;
		break;
	case 7:
		mv--;
		hurtnum -= 0.5;
		mv = mv - 1 > 0 ? mv - 1 : 0;
		break;
	case 9:
		hurtnum += 1;
		break;
	case 13:
		atr++;
		break;
	}
	fi.close();
	cout << sleep(1) << '\n';
}
bool character::incircle(int ox, int oy, int r)
{
	if (ceil(sqrt(pow(x - ox, 2) + pow(y - oy, 2))) <= r)
		return true;
	return false;
}
void character::print()
{
	cout << '\n';
	cout << character_name(id) << ":\n";
	cout << "血量:" << hp;
	cout << "\n能量:" << sp;
	cout << "\n攻击:" << atk;
	cout << "\n伤害系数：" << atknum;
	cout << "\n承伤系数：" << hurtnum;
	cout << "\n当前状态：\n";
	for (auto i : effect)
	{
		if (!effect[i])
			continue;
		switch(i)
		{
		case 0:
			cout << "燃烧: " << effect[i] << "回合\n";
			break;
		case 1:
			cout << "眩晕: " << effect[i] << "回合\n";
			break;
		case 2:
			cout << "生命恢复: " << effect[i] << "回合\n";
			break;
		case 3:
			cout << "顽强: " << effect[i] << "回合\n";
			break;
		case 4:
			cout << "脆弱: " << effect[i] << "回合\n";
			break;
		case 5:
			cout << "锋利: " << effect[i] << "回合\n";
			break;
		case 6:
			cout << "虚弱: " << effect[i] << "回合\n";
			break;
		case 7:
			cout << "心神不定: " << effect[i] << "回合\n";
			break;
		case 8:
			cout << "粘滞: " << effect[i] << "回合\n";
			break;
		}
	}
	cout << "卡牌：\n";
	for (auto i : card)
	{
		description(i);
	}
	cout << sleep(1) << '\n';
}
void show_map()
{
	cout << "   ";
	for (int i = 0; i < 16; i++)
		cout << setw(3) << left << i;
	cout << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << setw(3) << left << i;
		for (int j = 0; j < 16; j++)
		{
			if (mp[i][j])
				cout << setw(3) << left << mp[i][j] << ' ';
			else
				cout << "   ";
		}
		cout << endl;
	}
}
void description(int num)
{
	switch (num)
	{
	case 0:
		cout << "安如磐石：承伤系数降低0.25，攻击系数下降0.1\n";
		break;
	case 1:
		cout << "无坚不摧：攻击系数上升0.2，但承伤系数增加0.25\n";
		break;
	case 2:
		cout << "隆重谢幕：角色死亡时造成半径为2的伤害为 (死亡时能量 * 10 + 基础攻击力) 的爆炸\n";
		break;
	case 3:
		cout << "清风拂面：角色移动时有50%概率额外移动一格\n";
		break;
	case 4:
		cout << "动心忍性：角色承受攻击时有50%概率回复1点能量\n";
		break;
	case 5:
		cout << "护身符：角色获得一次复活机会，复活后将获得1点血量及二回合的【生命恢复】(不会触发亡语)\n";
		break;
	case 6:
		cout << "轻装上阵：角色每回合额外移动一步，但承伤系数提升50%\n";
		break;
	case 7:
		cout << "重装上阵：角色的承伤系数降低0.5，但每回合移动步数减一(最低不低于一步)\n";
		break;
	case 8:
		cout << "丹心汗青：黎雾死亡时绽出青色流光，为队友施加3回合的[顽强][锋利][生命恢复]\n";
		break;
	case 9:
		cout << "太平匕：将太平匕插入身体，承伤系数提升1.0，但每回合额外回复2点能量\n";
		break;
	case 10:
		cout << "浩然正气：受到攻击时，若能量足够，则消耗[攻击/10]点能量抵御攻击；否则消耗所有能量使受到的攻击减少[所有能量*10]\n";
		break;
	case 11:
		cout << "熊之冲刺：九章[冲锋]后为附近的角色(包括队友)施加一回合[眩晕]\n";
		break;
	case 12:
		cout << "熊之巨力：九章[击倒]的角色会额外受到一回合的眩晕\n";
		break;
	case 13:
		cout << "熊之愤怒：九章[重拳]造成的伤害提升50%\n";
		break;
	case 14:
		cout << "格挡：角色有概率格挡，降低70%伤害，格挡概率为(当前能量*0.1)\n";
		break;
	case 15:
		cout << "回击：角色受到攻击时，有50%的概率立即获得一次行动的机会。\n";
		break;
	case 16:
		cout << "注胶地瓜：地瓜泥将额外叠加一次迟滞。\n";
		break;
	case 17:
		cout << "优质地瓜：强化地瓜补给的效果。\n";
		break;
	case 18:
		cout << "品种改良：巨瓜天降的攻击系数增加0.5\n";
		break;
	}
	cout << sleep(1) << "\n";
}
set<int> explode(int x, int y, int harm,int range)
{
	set<int> ans;
	for (int i = 0; i < 10; i++)
	{
		if (info[i].incircle(x, y, range))
		{
			ans.insert(i);
			info[i].gethurt(harm);
		}
	}
	return ans;
}
int character::Getteam()
{
	return team;
}
inline void saving(int x)
{
	checkpoints.insert(x);
}
