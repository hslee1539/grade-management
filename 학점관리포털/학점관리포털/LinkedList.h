#pragma once
#include "Array1.h"
#include "MemoryTool.h"

template < typename keyT, typename valueT >
class Node {
public:
	keyT key;
	valueT value;
	Node * next;
	Node(keyT &key, valueT &value) : key(key), value(value), next(nullptr) {
	}
	Node(keyT &key, valueT &value, Node * next) : key(key), value(value), next(next) {
	}
};
/*Ŀ�� ������ �ִ� ����Ʈ�Դϴ�. keyT Ÿ���� �ݵ��� == ������ ���ǰ� �־�� �մϴ�.*/
template < typename keyT , typename valueT >
class LinkedList{
	Node<keyT,valueT> *head;
	Node<keyT, valueT> *cursor;
public:
	const size_t len;
	const size_t index;
protected:
	void init(Array<keyT> &keys, Array<valueT> &value) {
		size_t keyIndex = keys.getByte() / sizeof(keyT);
		size_t valueIndex = value.getByte() / sizeof(valueT);
	}
public: // �����ڵ�
	LinkedList() : len(0),index(0){
		this->cursor = nullptr;
		this->head = nullptr;
	}
	// ����� �����մϴ� (���� ����) Ŀ���� �ʱ�ȭ �մϴ�.
	LinkedList(LinkedList &list) : len(list.len), index(0) {
		this->cursor = list.head;
		this->head = list.head;
	}
	~LinkedList() {
		this->removeAll();
	}
public:
	//�� Ŀ�� ������ ����Ʈ�� �߰��մϴ�. Ŀ���� �߰��� ��带 ����ŵ�ϴ�.
	void append(keyT &key, valueT &value) {
		Node<keyT,valueT> * newNode = new Node< keyT ,valueT>( key, value);
		if (this->cursor) {
			Node <keyT, valueT > *tmp = this->cursor->next;
			this->cursor->next = newNode;
			newNode->next = tmp;
			this->cursor = newNode;
			(*(int*)(&this->index))++;
		}
		else {
			this->head = this->cursor = newNode;
		}
		(*(int*)(&this->len))++;
	}
	//�� Ŀ�� ���� ��带 �����մϴ�.
	bool remove() {
		Node<keyT, valueT> *tmp = nullptr;
		if (this->cursor) {
			if (this->cursor->next) {
				if (this->cursor->next->next)
					tmp = this->cursor->next->next;
				delete this->cursor->next;
				this->cursor->next = tmp;
				(*(int*)(&this->len))--;
				return true;
			}
		}
		return false;
	}
	// ��� ��带 �����մϴ�.
	void removeAll() {
		Node<keyT, valueT> *tmp = this->head;
		while (tmp) {
			this->head = tmp;
			tmp = tmp->next;
			delete this->head;
		}
		this->head = nullptr;
		this->cursor = nullptr;
		(*(int*)(&this->len)) = 0;
		(*(int*)(&this->index)) = 0;
	}
	// Ŀ���� Ű������ �̵��մϴ�. mode�� true�̸� �� Ŀ������ ã��, false�̸� ó������ ã���ϴ�.
	bool moveKey(keyT &key, bool mode) {
		Node<keyT, valueT> *tmp = this->head;
		size_t tmp_index = 0;
		if (mode) {
			tmp = this->cursor;
			tmp_index = this->index;
			return true;
		}
		for (; tmp; tmp = tmp->next , tmp_index++)
			if (tmp->key == key) {
				this->cursor = tmp;
				(*(int*)(&this->index)) = tmp_index;
				return true;
			}
		return false;
	}
	// Ŀ���� �ش� �ε����� �̵��մϴ�.
	bool moveIndex(size_t index) {
		if (index < this->len) {
			// Ŀ�� ��ġ�� �̵��ϰ��� �ϴ� ��ġ���� ���̸� ó������ ã��. �� �ܿ��� �̾ �̵�
			if (index < this->index) {
				this->cursor = this->head;
				(*(int*)(&this->index)) = 0;
			}
			while (index != this->index) {
				(*(int*)(&this->index))++;
				this->cursor = this->cursor->next;
			}
			return true;
		}
		return false;
	}
	valueT& getValue() {return this->cursor->value;}

};
