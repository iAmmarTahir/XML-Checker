#ifndef NODE_H
#define NODE_H

template <class T>
class Node
{

public:
	T data;
	Node<T> *next;
	Node();
	Node(T);
};

template <class T>
Node<T>::Node()
{
	data = 0;
	next = 0;
}

template <class T>
Node<T>::Node(T _data)
{
	data = _data;
	next = 0;
}

#endif // !NODE_H