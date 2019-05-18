#pragma once
#include "Node.h"

template <class T>
class Stack
{
private:
	Node<T> * head;
public:
	Stack();
	~Stack();
	bool isEmpty();
	bool push(const T &_data);
	bool pop(T &_data);
	void clear();
	T top() const;
	void print() const;
};


using namespace std;

template <class T>
Stack<T>::Stack()
{
	head = 0;
}

template <class T>
Stack<T>::~Stack()
{
	clear();
}


template <class T>
bool Stack<T>::isEmpty()
{
	return head == 0;
}

template <class T>
bool Stack<T>::push(const T & _data)
{
	if (head == 0)
	{
		head = new Node<T>(_data);
		return true;
	}
	else
	{
		Node<T> *temp = new Node<T>(_data);
		temp->next = head;
		head = temp;
		return true;
	}
}

template <class T>
void Stack<T>::print() const
{
	Node<T> *current = head;
	while (current != 0)
	{
		cout << current->data;
		if (current->next != 0)
		{
			cout << ' ';
		}
		current = current->next;
	}
	cout << endl;
}

template <class T>
void Stack<T>::clear()
{
	Node<T> *temp;
	if (!isEmpty())
	{
		while (head != 0)
		{

			temp = head;
			head = head->next;
			delete temp;
		}
	}
}

template <class T>
bool Stack<T>::pop(T & _data)
{
	Node<T> *temp;
	if (!isEmpty())
	{
		temp = head;
		_data = temp->data;
		head = head->next;
		delete temp;
		return true;
	}
	else
	{
		cout << "Stack is empty\n";
		return false;
	}
}

template <class T>
T Stack<T>::top() const
{
	return head->data;
}

