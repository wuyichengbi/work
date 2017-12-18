#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<cstring>
using namespace std;

const int maxn = 110;
FILE *infp;
int num[10];
int cnt;
int w[maxn], head[maxn];

struct Course
{
	char num[maxn];
	char name[maxn];
	int period;
	int term;
	int indeg;
}course[maxn];

struct edge
{
	int to, next;
}e[maxn];

void readfile()
{
	if ((infp = fopen("course_inf.txt", "rb")) == NULL)
	{
		cout << "Can not open file:" << endl;
		exit(1);
	}
	char ch;
	char s[maxn];
	rewind(infp);
	ch = fgetc(infp);
	while (ch<'0' || ch>'9')
		ch = fgetc(infp);
	num[1] = ch - '0';
	cnt = num[1];
	for (int i = 2; i <= 8; i++)
	{
		fscanf(infp, "%d", &num[i]);
		cnt += num[i];
	}
	for (int i = 0; i <= cnt; i++)
	{
		course[i].indeg = 0;
	}
	ch = fgetc(infp);
	while (ch!='c')
		ch = fgetc(infp);
	ch = fgetc(infp);
	while (ch != 'c')
		ch = fgetc(infp);
	fscanf(infp, "%s", s);
	course[1].num[0] = 'c';
	course[1].num[1] = '0';
	course[1].num[2] = '1';
	course[1].num[3] = '\0';
	fscanf(infp, "%s", course[1].name);
	fscanf(infp, "%s", s);
	course[1].period = s[0] - '0';
	fscanf(infp, "%s", s);
	course[1].term = s[0] - '0';
	for (int i = 0; i < maxn; i++)
		head[i] = -1;
	int t = 0;
	for (int i = 2; i <= cnt; i++)
	{
		fscanf(infp, "%s", course[i].num);
		fscanf(infp, "%s", course[i].name);
		fscanf(infp, "%s", s);
		course[i].period = s[0] - '0';
		fscanf(infp, "%s", s);
		course[i].term = s[0] - '0';
		ch = fgetc(infp);
		int x = 0, y = 0;
		while (ch != '\n')
		{
			if (ch >= '0'&&ch <= '9')
			{
				x = x * 10 + ch - '0';
				y++;
			}
			if (y == 2)
			{
				y = 0;
				course[i].indeg++;
				e[t].to = i;
				e[t].next = head[x];
				head[x] = t++;
				x = 0;
			}
			ch = fgetc(infp);
		}
	}
	fclose(infp);
}

void topsort()
{
	int count = 0;
	for (int i = 1; i <= cnt; i++)
		if (course[i].indeg == 0 && course[i].term == 0)
			w[count++] = i;
	cout << "1学期:";
	for (int i = 1; i <= cnt; i++)
		if (course[i].term == 1)
		{
			cout << ' ' << course[i].name;
			num[1]--;
		}
	int x = 1;
	for (int i = 0; i < count; i++)
	{
		int t = w[i];
		for (int j = head[t]; ~j; j = e[j].next)
		{
			int to = e[j].to;
			course[to].indeg--;
			if (course[to].indeg == 0)
				w[count++] = to;
		}
		if (num[x])
		{
			cout << ' ' << course[t].name;
			num[x]--;
		}
		else
		{
			x++;
			cout << endl;
			cout << x << "学期:";
			for (int i = 1; i <= cnt; i++)
				if (course[i].term == x)
				{
					cout << ' ' << course[i].name;
					num[x]--;
				}
			cout << ' ' << course[t].name;
			num[x]--;
		}
	}
	bool flag = true;
	for (int i = x + 1; i <= 8; i++)
	{
		for (int j = 1; j <= cnt; j++)
			if (course[j].term == i)
			{
				cout << endl;
				cout << i << "学期:";
				flag = false;
				break;
			}
		if (flag)
			break;
		for (int j = 1; j <= cnt; j++)
			if (course[j].term == i)
				cout << ' ' << course[j].name;
	}
	cout << endl;
}

int main()
{
	cout << "选课顺序：" << endl;
	readfile();
	topsort();
	return 0;
}