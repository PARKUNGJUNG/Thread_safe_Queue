#pragma once
#include "qtype.h"

// 큐 초기화, 해제
Queue* init(void); //큐 초기화
void release(Queue* queue); //큐 메모리 해제

//노드 생성&초기화, 해제, 복제
Node* nalloc(Item item);
void nfree(Node* node);
Node* nclone(Node* node);

//핵심 큐 연산
Reply enqueue(Queue* queue, Item item); //아이템 삽입
Reply dequeue(Queue* queue); //아이템 제거
Queue* range(Queue* queue, Key start, Key end); //특정 범위의 아이템 반환
//문제점: 스레드 세이프티를 보장하는 로직 전혀 없음.
//해결 방향: 뮤텍스 락/언락을 이용한 동시성 제어 및 우선순위 큐 로직 구현.

