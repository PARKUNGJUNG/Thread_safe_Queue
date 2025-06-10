#include <iostream>
#include <mutex>
#include "queue.h"


///초기화/해제 함수
//문제점: 메모리 할당 및 락(lock) 초기화가 이루어지지 않음.
//해결 방향: Queue 구조체 할당 후, head/tail 초기화 및 뮤텍스 설정 필요.
//Queue 구조체 할당 + head/tail 초기화 + 뮤텍스 생성
Queue* init(void) {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->head = nalloc({ 0, NULL }); //더미 노드
	queue->tail = queue->head;
	new (&queue->lock) std::mutex(); //뮤텍스 초기화
	return queue;
}


//모든 노드 메모리 해제 + 큐 구조체 삭제
void release(Queue* queue) {
	Node* current = queue->head;
	while (current) {
		Node* temp = current;
		current = current->next;
		nfree(temp);
	}
	queue->lock.~mutex(); //뮤텍스 파괴
	free(queue);
}


///노드 관리 함수(메모리 관리)
//문제점: 노드 생성/삭제/복사 기능이 구현되지 않음.
//해결 방향: malloc/free를 이용한 메모리 관리 및 아이템 복사 로직 추가.
///아이템을 저장할 노드 생성
Node* nalloc(Item item) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->item = item;
	new_node->next = NULL;
	return new_node;
}

///노드 메모리 해제
void nfree(Node* node) {
	return;
}

///노드 복제(깊은 복사)
Node* nclone(Node* node) {
	return NULL;
}


///우선순위 큐 로직
//문제점: 스레드 세이프티를 보장하는 로직 전혀 없음 - 이걸 만들어야되는구나
//해결 방향: 뮤텍스 락/언락을 이용한 동시성 제어 및 우선순위 큐 로직 구현
///삽입 시 정렬 유지
Node* current = queue->head;
while (current->next && current->next->item.key < item.key) {
	current = current->next;
}
new_node->next = current->next;
current->next = new_node;

///우선순위 추출
Node* target = queue->head->next; //head는 더미 노드 가정
queue->head->next = target->next;
return target->item;

///핵심 큐 연산 (스레드 세이프)
Reply enqueue(Queue* queue, Item item) {
	std::lock_guard<std::mutex> lock(queue->lock);
	//새 노드 생성
	Node* new_node = nalloc(item);

	//우선순위 탐색: (Key 오름차순)
	Node* prev = queue->head;
	while (prev->next && prev->next->item.key < item.key) {
		prev = prev->next;
	}

	//노드 연결
	new_node->next = prev->next;
	prev->next = new_node;
	//tail 업데이트
	if (!new_node->next) queue->tail = new_node;

	return { true,item };
}

Reply dequeue(Queue* queue) {
	std::lock_guard<std::mutex> lock(queue->lock);
	if (queue->head->next == nullptr) {
		return{ false, {0, NULL} }; //큐 빔
	}

	Node* target = queue->head->next;
	Reply reply = { true, target->item };
	//노드 제거
	queue->head->next = target->next;
	if (!target->next) queue->tail = queue->head;

	nfree(target);
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	//미구현
}





///메모장
/// 주제: 멀티스레드 환경에서 안전하게 동작하는(스레드 세이프) 우선순위 큐(priority queue)를 직접 구현
//여러 스레드가 동시에 접근해도 데이터가 손상되지 않는 thread-safe 우선순위 큐를 구현.
//키 범위 0 <= key < 10,000,000으로 설정되어 대용량 데이터 처리 능력 평가.
//아이템 크기: 1바이트 ~ 1KB까지 변동하여 메모리 관리 효율성 테스트.

//qtype.h: 자료구조 정의, 수정 및 제출
//queue.cpp: 큐의 실제 동작(함수)구현, 제출
//queue.h: 함수 선언만 포함, 수정 및 제출 불가
//main.c: 테스트용 코드, 제출 불필요

//구현 단계 요약
//qtype.h 수정: Queue 구조체에 뮤텍스 필드 추가
//init / release 구현 : 메모리 할당 / 해제 및 락 초기화 / 파괴
//노드 관리 함수 완성 : nalloc, nfree, nclone
//enqueue / dequeue 로직 작성 : 우선순위 정렬 + 락 보호
//range 함수 구현 : 키 범위 검색(추가 락 필요)
//성능 테스트 : 다양한 스레드 수와 워크로드로 검증

///qtype.h
//Item, Reply, Node, Queue 구조체 qtype.h 에서 정의 및 수정 가능.

///main.c
//멀티스레드 환경에서 우선순위 큐의 동작을 검증하기 위한 기본 프레임워크.
//


