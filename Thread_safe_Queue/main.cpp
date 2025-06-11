#include "qtype.h"
#include "queue.h"
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

#define REQUEST_PER_CLINET 1000
#define MAX_THREADS 32
#define MAX_KEY 10000000
#define MAX_VALUE_SIZE sizeof(int) //4바이트로 고정

#define SET 0
#define GET 1

typedef struct {
    int op;
    Item item;
} Request;

atomic<long long> sum_key(0);
atomic<long long> sum_value(0);

//클라이언트 스레드 함수
void client_func(Queue* queue, Request* requests, int request_count) {
    for (int i = 0; i < request_count; i++) {
        if (requests[i].op == SET) {
            Reply reply = enqueue(queue, requests[i].item);
            if (reply.success && reply.item.value != nullptr) {
                sum_key += reply.item.key;
                sum_value += *(int*)reply.item.value; //역참조 전 널 체크
            }
        }
        else if (requests[i].op == GET) {
            Reply reply = dequeue(queue);
            if (reply.success && reply.item.value != nullptr) {
                sum_key += reply.item.key;
                sum_value += *(int*)reply.item.value; //역참조 전 널 체크
                free(reply.item.value);
            }
        }
    }
}

//동시성 성능 테스트용 함수
void stress_test(Queue* queue, int iterations) {
    for (int i = 0; i < iterations; i++) {
        Item item;
        item.key = rand() % MAX_KEY;
        item.value_size = MAX_VALUE_SIZE;
        item.value = malloc(item.value_size);
        if (item.value != nullptr) {
            *(int*)item.value = rand() % 1000000;
        }
        Reply reply = enqueue(queue, item);
        if (reply.success && reply.item.value != nullptr) {
            sum_key += reply.item.key;
            sum_value += *(int*)reply.item.value;
        }
        free(item.value);
        if (rand() % 2 == 0) {
            Reply reply = dequeue(queue);
            if (reply.success && reply.item.value != nullptr) {
                sum_key += reply.item.key;
                sum_value += *(int*)reply.item.value;
                free(reply.item.value);
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    //스택 오버플로 해결: 동적 할당
    Request* requests = new Request[REQUEST_PER_CLINET]; //힙 할당

    //워크로드 생성
    for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
        requests[i].op = SET;
        requests[i].item.key = i;
        requests[i].item.value_size = MAX_VALUE_SIZE;
        requests[i].item.value = malloc(MAX_VALUE_SIZE);
        if (requests[i].item.value != nullptr) {
            *(int*)requests[i].item.value = rand() % 1000000; // 실제 데이터 저장
        }
        else {
            requests[i].item.value = nullptr; //명시적 초기화
        }
    }
    for (int i = REQUEST_PER_CLINET / 2; i < REQUEST_PER_CLINET; i++) {
        requests[i].op = GET;
        requests[i].item.value = nullptr;
        requests[i].item.value_size = 0;
    }

    Queue* queue = init();
    if (queue == nullptr) {
        for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
            if (requests[i].item.value != nullptr) {
                free(requests[i].item.value);
            }
        }
        delete[] requests;
        return -1;
    }

    //동시성 성능 테스트
    //기본 워크로드 테스트
    vector<thread> workers;
    for (int i = 0; i < MAX_THREADS; i++) {
        workers.emplace_back(client_func, queue, requests, REQUEST_PER_CLINET);
    }
    for (auto& t : workers) t.join();

    // 추가: 스트레스 테스트 (동시성 성능 강화)
    /*
    vector<thread> stress_workers;
    for (int i = 0; i < MAX_THREADS; i++) {
        stress_workers.emplace_back(stress_test, queue, 1000);
    }
    for (auto& t : stress_workers) t.join();
    */

    //잔여 아이템 처리 (널 체크 강화)
    while (true) {
        Reply reply = dequeue(queue);
        if (!reply.success) break;
        if (reply.item.value != nullptr) {
            free(reply.item.value);
        }
    }

    //SET에서 사용한 메모리 해제
    for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
        if (requests[i].item.value != nullptr) {
            free(requests[i].item.value);
        }
    }
    delete[] requests; //힙 메모리 해제
    release(queue);

    cout << "sum of returned keys = " << sum_key << endl;
    cout << "sum of returned values = " << sum_value << endl;
    return 0;
}
