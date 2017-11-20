#include<cstdio>
#include<iostream>
using namespace std;

const int maxn = 10010;
bool flag;

template<class T>
class Stack
{
public:
	Stack();
	void push(T x);
	void pop();
	T top();
	bool is_empty();
private:
	T s[maxn];
	int Top;
};

template<class T>
Stack<T>::Stack()
{
	Top = -1;
}

template<class T>
void Stack<T>::push(T x)
{
	Top++;
	s[Top] = x;
}

template<class T>
void Stack<T>::pop()
{
	Top--;
}

template<class T>
T Stack<T>::top()
{
	return s[Top];
}

template<class T>
bool Stack<T>::is_empty()
{
	return Top == -1;
}

int slen(char *s)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return i;
}

int calc(int a, int b, char c)
{
	if (c == '+')
		return a + b;
	if (c == '-')
		return a - b;
	if (c == '*')
		return a * b;
	if (c == '/')
	{
		if (b != 0)
			return a / b;
		else
			flag = false;
		return 0;
	}
}

int main()
{
	char s[maxn];
	cout << "Please input an expression that contains no spaces:" << endl;
	while (scanf("%s", s) == 1)
	{
		Stack<int> opnd;
		Stack<char> optr;
		int len = slen(s);
		flag = true;
		for (int i = 0; i < len; i++)
		{
			if (s[i] >= '0'&&s[i] <= '9')
			{
				int a = 0;
				a = a + s[i] - '0';
				while (s[i + 1] >= '0'&&s[i + 1] <= '9')
				{
					a = a * 10 + s[i + 1] - '0';
					i++;
				}
				opnd.push(a);
			}
			else
			{
				if (s[i]=='(')
				{
					optr.push(s[i]);
					continue;
				}
				if (s[i] != '+'&&s[i] != '-'&&s[i] != '*'&&s[i] != '/'&&s[i] != '('&&s[i] != ')')
				{
					flag = false;
					cout << "Input Error!" << endl;
					break;
				}
				if ((i == 0 && s[i] != '(') || (i>0&&s[i-1]!=')'&&(s[i - 1]<'0' || s[i - 1]>'9')))
				{
					flag = false;
					cout << "Input Error!" << endl;
					break;
				}
				if (optr.is_empty())
					optr.push(s[i]);
				else
				{
					char x = optr.top();
					if (s[i] == '+' || s[i] == '-')
					{
						while (!optr.is_empty() && x != '(')
						{
							int a = opnd.top();
							opnd.pop();
							int b = opnd.top();
							opnd.pop();
							int c = calc(b, a, x);
							opnd.push(c);
							optr.pop();
							if (!optr.is_empty())
								x = optr.top();
						}
						optr.push(s[i]);
					}
					else if (s[i] == '*' || s[i] == '/')
					{
						if (x == '+' || x == '-')
							optr.push(s[i]);
						else
						{
							while (!optr.is_empty() && x != '(')
							{
								int a = opnd.top();
								opnd.pop();
								int b = opnd.top();
								opnd.pop();
								int c = calc(b, a, x);
								if (flag == false)
									break;
								opnd.push(c);
								optr.pop();
								if (!optr.is_empty())
									x = optr.top();
							}
							if (flag == false)
							{
								cout << "Input Error!" << endl;
								break;
							}
							optr.push(s[i]);
						}
					}
					else if (s[i] == '(')
						optr.push(s[i]);
					else if (s[i] == ')')
					{
						while (x != '(')
						{
							int a = opnd.top();
							opnd.pop();
							int b = opnd.top();
							opnd.pop();
							int c = calc(b, a, x);
							opnd.push(c);
							optr.pop();
							if (optr.is_empty())
							{
								flag=false;
								cout << "Input Error!" << endl;
								break;
							}
							if (!optr.is_empty())
								x = optr.top();
						}
						optr.pop();
					}
					else
					{
						flag = false;
						cout << "Input Error!" << endl;
						break;
					}
				}
			}
		}
		if (flag)
		{
			while (!optr.is_empty())
			{
				char x = optr.top();
				if (x=='('||x==')')
				{
					flag=false;
					break;
				}
				optr.pop();
				if (opnd.is_empty())
				{
					flag=false;
					break;
				}
				int a = opnd.top();
				opnd.pop();
				if (opnd.is_empty())
				{
					flag=false;
					break;
				}
				int b = opnd.top();
				opnd.pop();
				int c = calc(b, a, x);
				if (flag == false)
					break;
				opnd.push(c);
				if (!optr.is_empty())
					x = optr.top();
			}
			if (flag)
				printf("%d\n", opnd.top());
			else
				cout << "Input Error!" << endl;
		}
		cout << "Please input an expression that contains no spaces:" << endl;
	}
	return 0;
}
