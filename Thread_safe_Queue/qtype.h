#pragma once
#include <mutex>

typedef int Key;
typedef void* Value;

typedef struct {
	Key key;
	Value value;
} Item;

typedef struct {
	bool success;   // true: 성공, false: 실패
	Item item;
	int error_code; // 필드 추가
	} Reply;

typedef struct node_t {
	Item item;
	struct node_t* next;
	// 필드추가가능
} Node;

typedef struct {
	Node* head, * tail;
	std::mutex lock; // 뮤텍스 추가
	size_t count; //수 추적
} Queue;
//더미 헤드 노드 추가: head ->  next를 첫 노드로 사용해 경합 조건 감소