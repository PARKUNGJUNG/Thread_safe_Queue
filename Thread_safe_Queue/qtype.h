#pragma once
#include <mutex>

typedef int Key;
typedef void* Value;


//Item 구조체: 우선순위 항목
typedef struct { //목적: 큐에 저장될 각 항목의 키(우선순위)와 값을 저장
	Key key; //우선순위 기준
	Value value; //실제 데이터
	int value_size; //byte
} Item;


//Reply 구조체: 연산 결과
typedef struct {
	bool success;   //연산 성공 여부
	Item item; //반환된 아이템
	int error_code; //실패 원인
} Reply;


//노드 구조체: 연결 리스트 노드
typedef struct node_t { //Node 구조체: 연결 리스트 노드
	Item item;
	struct node_t* next;
	struct node_t* prev; //역방향 탐색 지원 //초기는 단방향 연결만 지원했음, 우선순위 검색 비효율적.
	int priorty; //키 캐싱(성능 향상)
} Node;


//Queue 구조체: 큐 메타데이터
typedef struct {
	Node* head, * tail;
	std::mutex lock; // 뮤텍스 추가, 스레드 안전을 위한 뮤텍스
	size_t count; //현재 아이템 수(성능 모니터링)
} Queue;
//더미 헤드 노드 추가: head ->  next를 첫 노드로 사용해 경합 조건 감소