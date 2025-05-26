#pragma once

typedef struct {
	Key key;
	Value value;
} Item;

typedef struct {
	bool success;   // true: 성공, false: 실패
	Item item;
	// 필드추가가능
	} Reply;

typedef struct node_t {
	Item item;
	struct node_t* next;
	// 필드추가가능
} Node;

typedef struct {
	Node* head, tail;
	// 필드추가가능
	} Queue;
