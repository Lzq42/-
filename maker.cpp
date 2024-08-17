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
// TODO ��Ҫ���Ǹ�������������
std::string character_name(int id)
{
	switch (id)
	{
	case 1:
		return "����";
	case 2:
		return "����";
	}
}
int randint(int a, int b)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> distrib(a, b);
	return distrib(gen);
}
// TODO ��Ҫ���Ǹ�������������
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
		atknum = 1.25;
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
		
		cout << "��ɫ��ǰλ�� (" << x << ',' << y << ")�������ƶ�������" << move_step << "����\n�������ƶ����ꣻ";
		cout << "x����:";
		cin >> cx;
		cout << "y����:";
		cin >> cy;
		if (cx > 15 || cy > 15 || cx < 0 || cy < 0)
			cout << "�ƶ���Խ��ͼ�߽磡\n";
		else if (abs(cx - x) + abs(cy - y) > move_step)
			cout << "�ƶ��������࣡�ƶ���" << abs(cx - x) + abs(cy - y) << "��������ƶ�" << move_step << "����\n";
		else if (mp[cx][cy])
			cout << "ʵ���ص�������ͬ��һ��\n";
		else
		{
			cout << "ȷ��Ҫ��ô�ƶ���(1(ͬ��)/0(��ͬ��))\n";
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
		cout << "���һ�������ж����ᣡ\n��ѡ���ж���\n1.�ƶ�\n2.����\n3.��Ϣ\n";
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
	cout << "�ɹ��������У�\n";
	for (auto i:list)
		cout << i << "." << character_name(i) << '\n';
	int ans;
	cin >> ans;
	if (!list.count(ans) && ans != -1)
	{
		cout << "���鲻ͨ����\n";
		return 0;
	}
	cout << "�����" << atk * atknum * (effect[5] ? 1.25 : 1) * (effect[6] ? 0.75 : 1) << "���˺���";
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
	cout << character_name(id) << "�ָ���" << sp - past << "������\n\n\n";
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
		cout << "1.��������-3��ʹһ����ɫ���2�غϡ���ǿ��\n";
		cout << "2.ƽ����-5: ʹһ����ɫ���3�غϡ�������\n";
		cout << "3.�������-7����Ծ��3�����ڵĵط�������Χ2�����ʩ��2�غϡ�ѣ�Ρ���������˺�\n";
		if (sp >= 3)
			able.insert("1.��������");
		if (sp >= 5)
			able.insert("2.ƽ����");
		if (sp >= 7 && hp > 100)
			able.insert("3.�������");
		cout << "Ŀǰ������" << sp << "�㣬��ѡ����У�";
		for (auto i : able)
		{
			cout << i << endl;
		}
		cout << "ѡ��ʲô?  :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			if (sp < 3)
			{
				cout << "���Ϸ�Ŷ~��" << "\n";
				return;
			}
			for (auto i : info)
				if (i.second.team == team)
					current.insert(i.second.id);
			cout << "�ɸ�������Ľ�ɫ�У�\n";
			for (auto i : current)
				cout << i << '.' << character_name(i);
			cout << "ѡ��";
			cin >> cur;
			info[cur].effect[3] += 2;
			break;
		case 2:
			if (sp < 5)
			{
				cout << "���Ϸ�Ŷ~��" << "\n";
				return;
			}
			for (auto i : info)
				if (i.second.team == team)
					current.insert(i.second.id);
			cout << "�ɸ�������Ľ�ɫ�У�\n";
			for (auto i : current)
				cout << i << '.' << character_name(i);
			cout << "ѡ��";
			cin >> cur;
			info[cur].effect[5] += 3;
			break;
		case 3:
			if (sp < 7)
			{
				cout << "ע�����������з��ܡ�" << "\n";
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
		cout << "1.���-3�������������5��\n";
		cout << "2.̩ɽѹ��-5: �����˵��뾶1�����ڵ�һ�����ˣ�����ʩ��2�غ�ѣ��\n";
		cout << "3.��ȭ-7��������з����һ��������ը����ȭ\n";
		if (sp >= 3)
			able.insert("1.���");
		if (sp >= 5)
			able.insert("2.̩ɽѹ��");
		if (sp >= 7 && hp > 100)
			able.insert("3.��ȭ");
		cout << "Ŀǰ������" << sp << "�㣬��ѡ����У�\n";
		for (auto i : able)
		{
			cout << i << endl;
		}
		cout << "ѡ��ʲô?  :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			if (sp < 3)
			{
				cout << "���Ϸ�Ŷ~��" << "\n";
				return;
			}
			sp -= 3;
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
				cout << "���Ϸ�Ŷ~��" << "\n";
				return;
			}
			sp -= 3;
			cur = attack();
			info[cur].effect[1] += 2;
			if (card.count(12))
				info[cur].effect[1]++;
			break;
		case 3:
			if (sp < 7)
			{
				cout << "��ϧ��������������ע�����������з��ܡ�" << "\n";
				return;
			}
			sp -= 7;
			cur = attack();
			info[cur].effect[1] += 1;
			info[cur].gethurt(atk * atknum * 2 * (card.count(13) ? 1.5 : 0.5));
			break;
		}
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
		cout << "�����ѵ����ޣ����������㣡��鿴ԭ��\n";
		return;
	}
	cout << "�ѿ۳�5������\n";
	sp -= 5;
	cout << "��ʼ��ȡ����" << sleep(3);
	set<int> got;
	fi.open("����.in", ios::in);
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
	cout << "Ѫ��:" << hp;
	cout << "\n����:" << sp;
	cout << "\n����:" << atk;
	cout << "\n�˺�ϵ����" << atknum;
	cout << "\n����ϵ����" << hurtnum;
	cout << "\n��ǰ״̬��\n";
	for (auto i : effect)
	{
		if (!effect[i])
			continue;
		switch(i)
		{
		case 0:
			cout << "ȼ��: " << effect[i] << "�غ�\n";
			break;
		case 1:
			cout << "ѣ��: " << effect[i] << "�غ�\n";
			break;
		case 2:
			cout << "�����ָ�: " << effect[i] << "�غ�\n";
			break;
		case 3:
			cout << "��ǿ: " << effect[i] << "�غ�\n";
			break;
		case 4:
			cout << "����: " << effect[i] << "�غ�\n";
			break;
		case 5:
			cout << "����: " << effect[i] << "�غ�\n";
			break;
		case 6:
			cout << "����: " << effect[i] << "�غ�\n";
			break;
		case 7:
			cout << "���񲻶�: " << effect[i] << "�غ�\n";
			break;
		case 8:
			cout << "ճ��: " << effect[i] << "�غ�\n";
			break;
		}
	}
	cout << "���ƣ�\n";
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
		cout << "������ʯ������ϵ������0.25������ϵ���½�0.1\n";
		break;
	case 1:
		cout << "�޼᲻�ݣ�����ϵ������0.2��������ϵ������0.25\n";
		break;
	case 2:
		cout << "¡��лĻ����ɫ����ʱ��ɰ뾶Ϊ2���˺�Ϊ (����ʱ���� * 10 + ����������) �ı�ը\n";
		break;
	case 3:
		cout << "�����棺��ɫ�ƶ�ʱ��50%���ʶ����ƶ�һ��\n";
		break;
	case 4:
		cout << "�������ԣ���ɫ���ܹ���ʱ��50%���ʻظ�1������\n";
		break;
	case 5:
		cout << "���������ɫ���һ�θ�����ᣬ����󽫻��1��Ѫ�������غϵġ������ָ���(���ᴥ������)\n";
		break;
	case 6:
		cout << "��װ���󣺽�ɫÿ�غ϶����ƶ�һ����������ϵ������50%\n";
		break;
	case 7:
		cout << "��װ���󣺽�ɫ�ĳ���ϵ������0.5����ÿ�غ��ƶ�������һ(��Ͳ�����һ��)\n";
		break;
	case 8:
		cout << "���ĺ��ࣺ��������ʱ������ɫ���⣬Ϊ����ʩ��3�غϵ�[��ǿ][����][�����ָ�]\n";
		break;
	case 9:
		cout << "̫ƽذ����̫ƽذ�������壬����ϵ������1.0����ÿ�غ϶���ظ�2������\n";
		break;
	case 10:
		cout << "��Ȼ�������ܵ�����ʱ���������㹻��������[����/10]��������������������������������ʹ�ܵ��Ĺ�������[��������*10]\n";
		break;
	case 11:
		cout << "��֮��̣�����[���]��Ϊ�����Ľ�ɫ(��������)ʩ��һ�غ�[ѣ��]\n";
		break;
	case 12:
		cout << "��֮����������[����]�Ľ�ɫ������ܵ�һ�غϵ�ѣ��\n";
		break;
	case 13:
		cout << "��֮��ŭ������[��ȭ]��ɵ��˺�����50%\n";
		break;
	case 14:
		cout << "�񵲣���ɫ�и��ʸ񵲣�����70%�˺����񵲸���Ϊ(��ǰ����*0.1)\n";
		break;
	case 15:
		cout << "�ػ�����ɫ�ܵ�����ʱ����50%�ĸ����������һ���ж��Ļ��ᡣ\n";
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