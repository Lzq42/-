#define _CRT_SECURE_NO_WARNINGS
#include"settings.h"
using namespace std;
int slots[10] = {0,1,4,5,6,7,8,9,10,11};
int main()
{
	fi.open("save.out", ios::in);
	int aa;
	string ans;
	cout << "����ת�ڵ��У�";
	checkpoints.insert(1);
	while (!fi.eof())
	{
		fi >> aa;
		checkpoints.insert(aa);
		cout << aa << ' ';
	}
	cout << "\n��ѡ����ת���㡣" << endl;
	while (true)
	{
		cin >> aa;
		cout << "�⽫��ת����" << upper_bound(slots, slots + 10, aa) - slots - 1 << "�½ڵĵ�" << aa - *(upper_bound(slots, slots + 10, aa) - 1) << "�ڣ��밴tȷ����\n";
		cin >> ans;
		int g = aa - *(upper_bound(slots, slots + 10, aa) - 1);
		if (ans == "t")
		{
			switch (upper_bound(slots, slots + 10, aa) - slots)
			{
			case 1:
				c1(g);
			case 2:
				c2(g);
			case 3:
				c3(g);
			case 4:
				c4(g);
			}
		}
	}
}