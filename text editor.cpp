#include<cstdio>
#include<cstdlib>
#include<cctype>
#include<iostream>
#include<fstream>
using namespace std;

const int maxn = 1010;

int slen(char *s)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return i;
}

void scpy(char *s, char *t)
{
	int len = slen(t);
	for (int i = 0; i <= len; i++)
		s[i] = t[i];
}

struct node
{
	char *ch;
	int len;
	int index;
	node *lLink, *rLink;
	node()
	{
		ch = NULL;
		index = len = 0;
	}
}*Listnode;

class List
{
private:
	node *first, *last;
public:
	List()
	{
		first = NULL;
		last = NULL;
	}
	void changefirst(node *p)
	{
		first = p;
	}
	void changelast(node *p)
	{
		last = p;
	}
	node *First()
	{
		return first;
	}
	node *Last()
	{
		return last;
	}
	void insert(char *s, int x)
	{
		Listnode = new node;
		Listnode->index = x;
		last = Listnode;
		int len = slen(s);
		if (len == 0)
			Listnode->ch = NULL;
		else
		{
			Listnode->ch = (char *)malloc(len * sizeof(char));
			for (int i = 0; i <= len; i++)
				Listnode->ch[i] = s[i];
		}
		Listnode->len = len;
		Listnode->rLink = NULL;
		if (first == NULL)
		{
			first = Listnode;
			first->lLink = NULL;
		}
		else
		{
			node *cur;
			cur = first;
			while (cur->rLink)
				cur = cur->rLink;
			cur->rLink = Listnode;
			Listnode->lLink = cur;
		}
	}
};

class txt_editor
{
private:
	List A;
	node *current;						//denote the current line
	ifstream I;
	ofstream O;
	bool flag;
	char iname[maxn], oname[maxn];

	void readfile();
	void writefile();
	void print();
	void gotoline();
	void deleteline();
	void insertline();
	void substituteline();
	void changestring();
	void findstring();
public:
	txt_editor(char *in, char *out);
	void run();
};

txt_editor::txt_editor(char *in, char *out)
{
	flag = false;
	scpy(iname, in);
	scpy(oname, out);
}

void txt_editor::readfile()
{
	I.open(iname, ios::in);
	O.open(oname, ios::out);
	if (!I.is_open())
	{
		cout << "Error" << endl;
		exit(1);
	}
	char s[maxn];
	int num = 0;
	while (!I.eof())
	{
		I.getline(s, 1000);
		A.insert(s, num);
		num++;
	}
}

void txt_editor::writefile()
{
	node *cur;
	cur = A.First();
	while (cur->rLink != NULL)
	{
		O << cur->ch << endl;
		cur = cur->rLink;
	}
	O << cur->ch << endl;
}

void txt_editor::print()
{
	current = A.First();
	while (current->rLink != NULL)
	{
		cout << current->ch << endl;
		current = current->rLink;
	}
	cout << current->ch << endl;
}

void txt_editor::gotoline()
{
	int t;
	scanf("%d", &t);
	if (t > A.Last()->index)
		cout << " Warning: No such line" << endl;
	else
	{
		current = A.First();
		while (current->index != t)
			current = current->rLink;
	}
}

void txt_editor::insertline()
{
	int x;
	scanf("%d", &x);
	if (x<0 || x - 1>A.Last()->index)
		cout << " Error: Insertion failed!" << endl;
	else
	{
		cout << " What is the new line to insert? ";
		char s[maxn];
		scanf("%s", s);
		if (x == A.Last()->index + 1)
		{
			A.insert(s, x);
			current = A.Last();
		}
		else if (x == 0)
		{
			node *cur;
			cur = new node;
			int len = slen(s);
			cur->ch = (char *)malloc(len * sizeof(char));
			scpy(cur->ch, s);
			cur->index = x;
			cur->rLink = A.First();
			A.First()->lLink = cur;
			cur->lLink = NULL;
			A.changefirst(cur);
			current = cur;
			while (cur->rLink != NULL)
			{
				cur = cur->rLink;
				cur->index++;
			}
		}
		else
		{
			current = A.First();
			while (current->index != x)
				current = current->rLink;
			node *cur;
			cur = new node;
			int len = slen(s);
			cur->ch = (char *)malloc((len + 10) * sizeof(char));
			scpy(cur->ch, s);
			cur->index = x;
			current->lLink->rLink = cur;
			cur->lLink = current->lLink;
			cur->rLink = current;
			current->lLink = cur;
			current = cur;
			while (cur->rLink != NULL)
			{
				cur = cur->rLink;
				cur->index++;
			}
		}
	}
}

void txt_editor::deleteline()
{
	node *cur;
	cur = current;
	if (current == NULL)
		cout << " Error: Deletion failed!" << endl;
	if (A.First() == A.Last())
	{
		A.changefirst(NULL);
		A.changelast(NULL);
		delete(cur);
		current = NULL;
		return;
	}
	else if (current == A.First())
	{
		current = current->rLink;
		current->lLink = NULL;
		A.changefirst(current);
		while (current != NULL)
		{
			current->index--;
			current = current->rLink;
		}
		current = A.First();
	}
	else if (current == A.Last())
	{
		current = current->lLink;
		current->rLink = NULL;
		A.changelast(current);
	}
	else
	{
		current = current->lLink;
		current->rLink = current->rLink->rLink;
		current->rLink->rLink->lLink = current;
		node *p;
		p = current->rLink;
		while (p != NULL)
		{
			p->index--;
			p = p->rLink;
		}
	}
	delete(cur);
}

void txt_editor::substituteline()
{
	int x;
	scanf("%d", &x);
	if (x<A.First()->index || x>A.Last()->index)
		cout << " Error: Substitution failed!" << endl;
	else
	{
		current = A.First();
		while (current->index != x)
			current = current->rLink;
		cout << " What is the new line to substitute? ";
		char s[maxn];
		scanf("%s", s);
		int len = slen(s);
		current->ch = (char *)malloc(len * sizeof(char));
		scpy(current->ch, s);
	}
}

void txt_editor::findstring()
{
	char s[maxn], t[maxn];
	scanf("%s", s);
	int len1 = slen(current->ch), len2 = slen(s);
	for (int i = 0; i < len1; i++)
		t[i] = ' ';
	t[len1] = '\0';
	bool f;
	for (int i = 0; i < len1; i++)
	{
		f = false;
		int j;
		for (j = 0; j < len2; j++)
		{
			if (i + j < len1&&current->ch[i + j] != s[j])
				break;
		}
		if (j == len2)
		{
			f = true;
			for (j = i; j < i + len2; j++)
				t[j] = '^';
			break;
		}
	}
	if (f)
	{
		cout << current->ch << endl;
		cout << t << endl;
	}
	else
		cout << "String was not found." << endl;
}

void txt_editor::changestring()
{
	char s[maxn];
	scanf("%s", s);
	bool f;
	int len1 = slen(current->ch), len2 = slen(s);
	int x;
	for (int i = 0; i < len1; i++)
	{
		f = false;
		int j;
		for (j = 0; j < len2; j++)
		{
			if (i + j < len1&&current->ch[i + j] != s[j])
				break;
		}
		if (j == len2)
		{
			f = true;
			x = i;
			break;
		}
	}
	if (f)
	{
		cout << " What new text segment do you want to add in? ";
		char t[maxn];
		scanf("%s", t);
		int len3 = slen(t), l = x + len2;
		char p[maxn];
		scpy(p, current->ch);
		for (int i = x + len3; i < len1 - len2 + len3;i++)
			current->ch[i] = p[l++];
		current->ch[len1 - len2 + len3] = '\0';
		for (int i = x; i < x + len3; i++)
			current->ch[i] = t[i - x];
	}
	else
		cout << " Error: Substitution failed!" << endl;
}

void txt_editor::run()
{
	char command[100];
	while (1)
	{
		scanf("%s", command);																//ignore the redundant letters
		command[0] = tolower(command[0]);				
		if (!flag&&command[0] != 'r'&&command[0] != 'q')
		{
			cout << " Warning: empty buffer" << endl;
			cout << "File buffer is empty(please use command r to read file to buffer)." << endl;
			continue;
		}
		switch (command[0])
		{
		case 'b':																			//jump to the first line of the file
			current = A.First();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'e':																			//jump to the last line of the file
			current = A.Last();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'n':																			//jump to the next line of the file
			if (current->rLink != NULL)
				current = current->rLink;
			else
				cout << " Warning: at end of buffer" << endl;
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'p':																			//jump to the previous line of the file
			if (current->lLink != NULL)
				current = current->lLink;
			else
				cout << " Warning: at start of buffer" << endl;
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'g':																			//jump to the line specified by the user
			cout << " Goto what line number? ";
			gotoline();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'i':																			//insert a new line at the place specified by the user
			cout << " Insert what line number? ";
			insertline();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'd':																			//delete the current line
			deleteline();
			if (current != NULL)
				cout << current->index << " : " << current->ch << endl;
			else
				cout << "NULL!" << endl;
			break;
		case 'f':																			//find the string specified by the user in the current line
			cout << "Enter string to search for: ";
			findstring();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'c':																			//change one string to another string specified by the user in the current line
			cout << " What text segment do you want to replace? ";
			changestring();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 's':																			//change one line to another new line specified by the user
			cout << " Substitute what line number? ";
			substituteline();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'l':																			//print the number of lines in the file and the length of the current line
			cout << "There are " << A.Last()->index + 1 << " lines in the file." << endl;
			cout << "Current line length is " << slen(current->ch) << endl;
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'h':																			//help
			cout << "Valid commands are: b(egin) c(hange) d(el) e(nd)" << endl;
			cout << "f(ind) g(o) h(elp) i(nsert) l(ength) n(ext) p(rior)" << endl;
			cout << "q(uit) r(ead) s(ubstitute) v(iew) w(rite)" << endl;
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'r':																			//read the file
			flag = true;
			readfile();
			current = A.Last();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'w':																			//preserve the file
			writefile();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'v':																			//print the file
			print();
			cout << current->index << " : " << current->ch << endl;
			break;
		case 'q':																			//exit
			I.close();
			O.close();
			break;
		default:
			cout << "Press h or ? for help or enter a valid command" << endl;
			cout << current->index << " : " << current->ch << endl;
		}
		if (command[0] == 'q')
			break;
	}
}

int main()
{
	char s1[maxn], s2[maxn];
	cout << "Please input inputfile name(eg. file_in.txt): ";
	scanf("%s", s1);
	cout << "Please input outputfile name(eg. file_out.txt): ";
	scanf("%s", s2);
	cout << "File buffer is empty(please use command r to read file to buffer)." << endl;
	txt_editor	T(s1, s2);
	T.run();
	return 0;
}