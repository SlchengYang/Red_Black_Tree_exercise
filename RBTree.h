#pragma once
#include<iostream>
#include<queue>
using namespace std;

template<typename T>
struct Node
{
	bool col;		//true for black, false for red//红色意味着该节点和他的父节点是一个三or四节点
	T key;     //预留出4节点的中间态
	Node<T>* p;
	Node<T>* le;
	Node<T>* ri;
	void seenode();
	Node(T k, bool c=false, Node<T>* l = nullptr, Node<T>* r = nullptr, Node<T>* p_=nullptr):col(c),key(k),p(p_),le(l),ri(r){}
};
template<typename T>
inline void Node<T>::seenode()
{
	if (col == true)
		cout << "black " << key;
	else
		cout << "red " << key;
	cout << "   ";
}

template<typename T>
class RBTree    //indexes must be different, 相同元素的查找尚未解决
{
private:
	Node<T>* root;  //if key==root, k belong to left tree. 
	void deepcpy(Node<T>*& to, Node<T>* from);
	void clear(Node<T>* root);
	void insert_adjust(Node<T>* cur);
	   
public:
	RBTree();
	RBTree(Node<T>* root);
	~RBTree();
	Node<T>* get_root() { return root; }
	bool delete_index(T k);       //未完待续
	bool insert(T k);
	void see();//层序遍历
	Node<T>* rotateToleft(Node<T>* h);
	Node<T>* rotateToright(Node<T>* h);
	void raiseMidvalue(Node<T>* m);
};

template<typename T>
inline void RBTree<T>::deepcpy(Node<T>*& to, Node<T>* from)
{
	if (from == nullptr)
		return; 
	to = new Node<T>(from->col, from->key, from->le, from->ri, from->p);
	deepcpy(to->le, from->le);
	deepcpy(to->ri, from->ri);
}

template<typename T>
inline void RBTree<T>::clear(Node<T>* root)
{
	if (root == nullptr)
		return;
	clear(root->le);
	clear(root->ri);
	delete root;
}

template<typename T>
inline void RBTree<T>::insert_adjust(Node<T>* cur)//把所有假节点都翻到左边方便检查
{
	//如果还有调整的必要，研究对象必为红
	if (cur->col == true)
		return;
	if (cur->p == root)
	{
		if (root->ri == cur)
		{
			if (root->le == nullptr || root->le->col == true)
			{
				root->col = false;
				root->p=cur;
				root->ri = cur->le;
				if (cur->le != nullptr)
					cur->le->p = root;
				cur->le = root;
				cur->p = nullptr;
				cur->col = true;
				root = cur;
				return;
			}
			raiseMidvalue(root);
			root->col = true;
		}
		return;
	}

	//impossible

	//上升的情形
	// 
	// 如何检查cur是2节点的一部分？cur->l==black or null, cur->r==black or null, cur=black 
	// 如何检查cur是3节点的一部分？
	//		在确保左链的情况下，仅需证：(cur==red&&cur->p==black&&cur->l==black)||(cur->l==red&&cur->l->l==black&&cur==black)
	//		在自由状态下：每个上下游节点都有两种情况并且要是先确认哪一支，有点糟糕
	// 如何检查cur是4节点的一部分？
	//		同3

	if (cur = cur->p->ri)
		cur = rotateToleft(cur->p)->le;//左旋确保仅需检查左链，cur是我们新的研究对象;另：我们左旋保证的是位置不变，返回的是根位置
	//当下为2节点
	if ((cur->le==nullptr||cur->le->col==true)&&cur->p->col==true)
	{
		return;
	}
	//当下为3节点
	
	if (cur->le != nullptr && cur->le->col == false)
	{
		Node<T>* m = rotateToright(cur->p);
		raiseMidvalue(m);
		insert_adjust(m);
		return;
	}
	
	if (cur->p->col == false)
	{
		Node<T>* m = rotateToright(cur->p->p);
		raiseMidvalue(m);
		insert_adjust(m);
		return;
	}
}

template<typename T>
inline RBTree<T>::RBTree()
{
	root = nullptr;
}

template<typename T>
inline RBTree<T>::RBTree(Node<T>* ro)
{
	deepcpy(root, ro);
	root->p = nullptr;
}

template<typename T>
inline RBTree<T>::~RBTree()
{
	clear(root);
}

template<typename T>
inline bool RBTree<T>::insert(T k)
{
	if (root == nullptr)
	{
		root = new Node<T>(k, true);
		return true;
	}

	Node<T>* tem = root;
	Node<T>* pre = tem;
	while (tem != nullptr)
	{
		if (k > tem->key)
		{
			pre = tem;
			tem = tem->ri;
		}
			
		else if (k < tem->key)
		{
			pre = tem;
			tem = tem->le;
		}
			
		else
			return false;
	}
	if (k > pre->key)
	{
		pre->ri = new Node<T>(k, false,nullptr,nullptr,pre);
		insert_adjust(pre->ri);
		return true;
	}
	else if(k < pre->key)
	{
		pre->le = new Node<T>(k, false, nullptr, nullptr, pre);
		insert_adjust(pre->le);
		return true;
	}
	return false;
}

template<typename T>
inline void RBTree<T>::see()
{
	if (root == nullptr)
		return;
	queue<Node<T>*> walk;
	walk.push(root);
	walk.push(nullptr);  //means \n
	while (!walk.empty())
	{
		while (walk.front() != nullptr)
		{
			if (walk.front()->le != nullptr)
			{
				walk.push(walk.front()->le);
			}
			if (walk.front()->ri != nullptr)
			{
				walk.push(walk.front()->ri);
			}
			walk.front()->seenode();
			walk.pop();
		}
		cout << '\n';
		walk.pop();
		if (!walk.empty())
			walk.push(nullptr);
	}
}

template<typename T>
inline Node<T>* RBTree<T>::rotateToleft(Node<T>* h)
{
	Node<T>* x = h->ri;
	Node<T>* y = h->p;
	x->p = y;
	if (y->le == h)
		y->le = x;
	else
		y->ri = x;
	h->p = x;
	h->ri = x->le;
	if(x->le!=nullptr)
	x->le->p = h;
	x->le = h;
	x->col = h->col;
	h->col = false;
	return x;
}

template<typename T>
inline Node<T>* RBTree<T>::rotateToright(Node<T>* h)
{
	Node<T>* x = h->le;
	Node<T>* y = h->p;
	x->p = y;
	if (y->le == h)
		y->le = x;
	else
		y->ri = x;

	h->p = x;
	h->le = x->ri;
	if (x->ri != nullptr)
	x->ri->p = h;
	x->ri = h;
	x->col = h->col;
	h->col = false;
	return x;
}

template<typename T>
inline void RBTree<T>::raiseMidvalue(Node<T>* m)
{
	m->col = false;
	m->le->col = true;
	m->ri->col = true;
}
