#pragma once
#include<iostream>
#include<queue>
using namespace std;

template<typename T>
struct Node
{
	bool col;		//true for black, false for red//��ɫ��ζ�Ÿýڵ�����ĸ��ڵ���һ����or�Ľڵ�
	T key;     //Ԥ����4�ڵ���м�̬
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
class RBTree    //indexes must be different, ��ͬԪ�صĲ�����δ���
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
	bool delete_index(T k);       //δ�����
	bool insert(T k);
	void see();//�������
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
inline void RBTree<T>::insert_adjust(Node<T>* cur)//�����мٽڵ㶼������߷�����
{
	//������е����ı�Ҫ���о������Ϊ��
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

	//����������
	// 
	// ��μ��cur��2�ڵ��һ���֣�cur->l==black or null, cur->r==black or null, cur=black 
	// ��μ��cur��3�ڵ��һ���֣�
	//		��ȷ������������£�����֤��(cur==red&&cur->p==black&&cur->l==black)||(cur->l==red&&cur->l->l==black&&cur==black)
	//		������״̬�£�ÿ�������νڵ㶼�������������Ҫ����ȷ����һ֧���е����
	// ��μ��cur��4�ڵ��һ���֣�
	//		ͬ3

	if (cur = cur->p->ri)
		cur = rotateToleft(cur->p)->le;//����ȷ��������������cur�������µ��о�����;������������֤����λ�ò��䣬���ص��Ǹ�λ��
	//����Ϊ2�ڵ�
	if ((cur->le==nullptr||cur->le->col==true)&&cur->p->col==true)
	{
		return;
	}
	//����Ϊ3�ڵ�
	
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
