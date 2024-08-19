#include"RBTree.h"
#include<iostream>
using namespace std;
int main()
{
	int n;
	cin >> n;
	RBTree<int> tree;
	while (n--)
	{
		int tem;
		cin >> tem;
		tree.insert(tem);
	}
	tree.see();
}