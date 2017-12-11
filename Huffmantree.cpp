#include<cstdio>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cctype>
using namespace std;

const int maxn = 1010;
int a[26];
char r[1000010], w[1000010];
int q;
int cnt, n;
FILE *infp, *outfp;

class HuffmanNode
{
public:
	HuffmanNode(int k, HuffmanNode *l, HuffmanNode *r) :key(k), lc(l), rc(r), par(NULL) {}
	HuffmanNode() :lc(NULL), rc(NULL), par(NULL), len(0), value('\0'), is_leaf(false) {}
	HuffmanNode(int k, char v) :key(k), value(v), lc(NULL), rc(NULL), par(NULL), len(0) {}
	int key, len;
	bool is_leaf;
	char value;
	char code[50];
	HuffmanNode *lc, *rc, *par;
};

void bytetobit()
{
	char ch = 0;
	
	for (int i = 0; i < q; i++)
	{
		ch = (ch << 1) + w[i] - '0';
		if ((i + 1) % 8 == 0)
		{
			fputc(ch, outfp);
			ch = 0;
		}
	}
}

void swap(HuffmanNode* &a, HuffmanNode* &b)
{
	HuffmanNode* c = a;
	a = b;
	b = c;
}

int slen(char *s)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return i;
}

class heap			//We construct a minheap
{
private:
	int n;
	HuffmanNode *Heap[30];
	void siftdown(int pos);
public:
	heap();
	int size();
	bool isLeaf(int pos);
	int leftchild(int pos);
	int rightchild(int pos);
	int parent(int pos);
	void buildheap();
	HuffmanNode* removefirst();
	void insert(HuffmanNode* it);
};

void heap::siftdown(int pos)
{
	while (!isLeaf(pos))
	{
		int a = leftchild(pos), b = rightchild(pos);
		if (b<n&&Heap[a]->key>Heap[b]->key)
			a = b;
		if (Heap[a]->key < Heap[pos]->key)
			swap(Heap[a], Heap[pos]);
		pos = a;
	}
}

heap::heap()
{
	int t = 0;
	for (int i = 0; i < 26; i++)
		if (a[i])
		{
			HuffmanNode* p = new HuffmanNode(a[i], i + 'a');
			Heap[t++] = p;
		}
	n = t;
	buildheap();
}

int heap::size()
{
	return n;
}

bool heap::isLeaf(int pos)
{
	return pos >= n / 2;
}

int heap::leftchild(int pos)
{
	return 2 * pos + 1;
}

int heap::rightchild(int pos)
{
	return 2 * pos + 2;
}

int heap::parent(int pos)
{
	return (pos - 1) / 2;
}

void heap::buildheap()
{
	for (int i = n / 2 - 1; i >= 0; i--)
		siftdown(i);
}

HuffmanNode* heap::removefirst()
{
	swap(Heap[0], Heap[--n]);
	if (n != 0)
		siftdown(0);
	return Heap[n];
}

void heap::insert(HuffmanNode* it)
{
	int cur = n++;
	Heap[cur] = it;
	while (cur != 0 && Heap[parent(cur)]->key > Heap[cur]->key)
	{
		swap(Heap[cur], Heap[parent(cur)]);
		cur = parent(cur);
	}
}

class HuffmanTree
{
public:
	void create(heap A);
	void inorder(HuffmanNode* p);
	void print();
	void set();
	void convert();
private:
	HuffmanNode *root;
	char s[26][50];
};

void HuffmanTree::convert()
{
	char t[maxn];
	fscanf(infp, "%s", t);
	HuffmanNode* cur = root;
	fprintf(outfp, "%s", r);
	for (int i = 0; i < slen(t); i++)
	{
		if (t[i] == '0')
			cur = cur->lc;
		else if (t[i] == '1')
			cur = cur->rc;
		if (cur->is_leaf)
		{
			fprintf(outfp, "%c", cur->value);
			cur = root;
		}
	}
}

void HuffmanTree::set()
{
	root = new HuffmanNode;
	char t[maxn];
	for (int i = 0; i < n; i++)
	{
		HuffmanNode* cur = root;
		fscanf(infp, "%s", t);
		for (int j = 2; j < slen(t); j++)
		{
			if (t[j] == '0')
			{
				if (cur->lc==NULL)
					cur->lc = new HuffmanNode;
				cur = cur->lc;
			}
			else if (t[j] == '1')
			{
				if (cur->rc==NULL)
					cur->rc = new HuffmanNode;
				cur = cur->rc;
			}
		}
		cur->value = t[0];
		cur->is_leaf = true;
	}
	fscanf(infp, "%s", t);
}

void HuffmanTree::inorder(HuffmanNode* p)
{
	int cnt = 0;
	if (p != NULL&&p->lc == NULL&&p->rc == NULL)
	{
		HuffmanNode* cur = p;
		while (cur != root)
		{
			if (cur->par->lc == cur)
				p->code[cnt++] = '0';
			else
				p->code[cnt++] = '1';
			cur = cur->par;
		}
		p->code[cnt] = '\0';
		p->len = cnt;
		fprintf(outfp, "%c:", p->value);
		cnt = 0;
		for (int i = p->len - 1; i >= 0; i--)
		{
			fprintf(outfp, "%c", p->code[i]);
			s[p->value - 'a'][cnt++] = p->code[i];
		}
		fprintf(outfp, "\n");
		s[p->value - 'a'][cnt] = '\0';
	}
	if (p != NULL)
	{
		inorder(p->lc);
		inorder(p->rc);
	}
}

void HuffmanTree::print()
{
	int len = slen(r);
	cout << endl;
	for (int i = 0; i < len; i++)
	{
		printf("%s", s[r[i] - 'a']);
		for (int j = 0; j < slen(s[r[i] - 'a']); j++)
			w[q++] = s[r[i] - 'a'][j];
	}
	int x = q % 8;
	if (x != 0)
	{
		x = 8 - x;
		for (int i = q; i < q + x; i++)
			w[i] = '0';
		q = q + x;
	}
	w[q] = '\0';
	fprintf(outfp, "%d\n", x);
	bytetobit();
	cout << endl << endl;
	fclose(outfp);
}

void HuffmanTree::create(heap A)
{
	if (A.size() == 1)
	{
		HuffmanNode* a = A.removefirst();
		HuffmanNode* p = new HuffmanNode(a->key, a, NULL);
		a->par = p;
		A.insert(p);
	}
	else
	{
		while (A.size() >= 2)
		{
			HuffmanNode* a = A.removefirst();
			HuffmanNode* b = A.removefirst();
			HuffmanNode* p = new HuffmanNode(a->key + b->key, a, b);
			a->par = p;
			b->par = p;
			A.insert(p);
		}
	}
	root = A.removefirst();
	inorder(root);
}

void readfromsource(char *iname, char *oname)
{
	if ((infp = fopen(iname, "rb")) == NULL)
	{
		cout << "Can not open file:" << iname << endl;
		exit(1);
	}
	if (feof(infp) != 0) {
		cout << "Empty code file:" << iname << endl;
		exit(1);
	}
	if ((outfp = fopen(oname, "wb")) == NULL) {
		cout << "Can not open file:" << oname << endl;
		exit(1);
	}
	for (int i = 0; i < 26; i++)
		a[i] = 0;
	char t[maxn];
	while (!feof(infp))
	{
		fscanf(infp, "%s", t);
		int len = slen(t);
		for (int i = 0; i < len; i++)
		{
			t[i] = tolower(t[i]);
			a[t[i] - 'a']++;
			r[cnt++] = t[i];
		}
	}
	r[cnt] = '\0';
	int x = 0;
	q = 0;
	for (int i = 0; i < 26; i++)
		if (a[i])
		{
			x++;
			fprintf(outfp, "%c:%d,", i + 'a', a[i]);
		}
	fprintf(outfp, "\n%d\n", x);
	fclose(infp);
}

void readfromcode(char *iname,char *oname)
{
	if ((infp = fopen(iname, "rb")) == NULL) 
	{
		cout << "Can not open file:" << iname << endl;
		exit(1);
	}
	if (feof(infp) != 0) {
		cout << "Empty code file:" << iname << endl;
		exit(1);
	}
	if ((outfp = fopen(oname, "wb")) == NULL) {
		cout << "Can not open file:" << oname << endl;
		exit(1);
	}
	char t[maxn];
	fscanf(infp, "%s", t);
	fscanf(infp, "%s", t);
	n = 0;
	for (int i = 0; i < slen(t); i++)
		n = n * 10 + t[i] - '0';
	HuffmanTree huff;
	huff.set();
	huff.convert();
	fclose(infp);
	fclose(outfp);
}

int main()
{
	while (1)
	{
		cnt = 0;
		puts("1.Huffman compress.");
		puts("2.Huffman decompress.");
		puts("3.Exit.");
		printf("Please select:");
		int t;
		scanf("%d", &t);
		char iname[maxn], oname[maxn];
		if (t == 3)
			break;
		else if (t == 1)
		{
			printf("Please input source file name(size less than 4GB):");
			scanf("%s", iname);
			printf("Please input code file name:");
			scanf("%s", oname);
			readfromsource(iname,oname);
			heap A;
			HuffmanTree huff;
			huff.create(A);
			huff.print();
		}
		else
		{
			printf("Please input code file name:");
			scanf("%s", iname);
			printf("Please input target file name:");
			scanf("%s", oname);
			readfromcode(iname, oname);
		}
	}
	return 0;
}
