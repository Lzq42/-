#include"settings.h"
using namespace std;
int w1()
{
	cout << "���ֽ̳����·ŵ�һ�Ե� expre.out ����\n";
	cout << "ս����ʼ��\n";
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
				cout << "ս��ʤ����\n";
				return 1;
			}
			else
			{
				cout << "ս��ʧ�ܡ���\n";
				return 0;
			}
			
		}
		cout << "������������������������������������������������������������������������������������������������\n\n" << character_name(num) << "�Ļغ�\n";
		if(!a && b)
		{ 
			cout << character_name(num) << "�޷��ж������غ����Զ�������\n";
			que.push(num);
			continue;
		}
		if (!b)
		{
			cout << character_name(num) << "������������\n";
			info.erase(num);
			continue;
		}
		que.push(num);
		cout << "��ʼ�ж��ɡ�\n";
		bool flag = true;
		while (flag)
		{
			cout << "a.��Ϣ\n    1.��ͼ\n    2.��ɫ\nb.�ж�\n    3.����\n    4.�ƶ�\n    5.���\n    6.����\nc.����\n    7.�鿨\n";
			int act;
			cin >> act;
			switch (act)
			{
			case 1:
				show_map();
				cout << endl;
				cout << "1Ϊ����\n2Ϊ����\n";
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