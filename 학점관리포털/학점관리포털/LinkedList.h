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
/*커서 정보가 있는 리스트입니다. keyT 타입은 반듯이 == 연산자 정의가 있어야 합니다.*/
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
public: // 생성자들
	LinkedList() : len(0),index(0){
		this->cursor = nullptr;
		this->head = nullptr;
	}
	// 대상을 복사합니다 (앝은 복사) 커서는 초기화 합니다.
	LinkedList(LinkedList &list) : len(list.len), index(0) {
		this->cursor = list.head;
		this->head = list.head;
	}
	~LinkedList() {
		this->removeAll();
	}
public:
	//현 커서 다음에 리스트를 추가합니다. 커서는 추가된 노드를 가르킵니다.
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
	//현 커서 다음 노드를 삭제합니다.
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
	// 모든 노드를 삭제합니다.
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
	// 커서를 키값으로 이동합니다. mode가 true이면 현 커서부터 찾고, false이면 처음부터 찾습니다.
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
	// 커서를 해당 인덱스로 이동합니다.
	bool moveIndex(size_t index) {
		if (index < this->len) {
			// 커서 위치가 이동하고자 하는 위치보다 앞이면 처음부터 찾음. 그 외에는 이어서 이동
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
