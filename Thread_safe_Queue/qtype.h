#pragma once

typedef struct {
	Key key;
	Value value;
} Item;

typedef struct {
	bool success;   // true: ����, false: ����
	Item item;
	// �ʵ��߰�����
	} Reply;

typedef struct node_t {
	Item item;
	struct node_t* next;
	// �ʵ��߰�����
} Node;

typedef struct {
	Node* head, tail;
	// �ʵ��߰�����
	} Queue;
