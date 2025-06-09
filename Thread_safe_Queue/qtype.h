#pragma once
#include <mutex>

typedef int Key;
typedef void* Value;

typedef struct { //Item 구조체: 우선순위 항목
	Key key;
	Value value;
	int value_size; //byte
} Item;

typedef struct { //Reply 구조체: 연산 결과
	bool success;   // true: 성공, false: 실패
	Item item;
	int error_code; // 필드 추가
	} Reply;

typedef struct node_t { //Node 구조체: 연결 리스트 노드
	Item item;
	struct node_t* next;
	// 필드추가가능
} Node;

typedef struct { //Queue 구조체: 큐 메타데이터
	Node* head, * tail;
	std::mutex lock; // 뮤텍스 추가, 스레드 안전을 위한 뮤텍스
	size_t count; //수 추적
} Queue;
//더미 헤드 노드 추가: head ->  next를 첫 노드로 사용해 경합 조건 감소