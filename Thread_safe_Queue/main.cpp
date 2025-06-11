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
#define MAX_VALUE_SIZE sizeof(int) //4����Ʈ�� ����

#define SET 0
#define GET 1

typedef struct {
    int op;
    Item item;
} Request;

atomic<long long> sum_key(0);
atomic<long long> sum_value(0);

//Ŭ���̾�Ʈ ������ �Լ�
void client_func(Queue* queue, Request* requests, int request_count) {
    for (int i = 0; i < request_count; i++) {
        if (requests[i].op == SET) {
            Reply reply = enqueue(queue, requests[i].item);
            if (reply.success && reply.item.value != nullptr) {
                sum_key += reply.item.key;
                sum_value += *(int*)reply.item.value; //������ �� �� üũ
            }
        }
        else if (requests[i].op == GET) {
            Reply reply = dequeue(queue);
            if (reply.success && reply.item.value != nullptr) {
                sum_key += reply.item.key;
                sum_value += *(int*)reply.item.value; //������ �� �� üũ
                free(reply.item.value);
            }
        }
    }
}

//���ü� ���� �׽�Ʈ�� �Լ�
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

    //���� �����÷� �ذ�: ���� �Ҵ�
    Request* requests = new Request[REQUEST_PER_CLINET]; //�� �Ҵ�

    //��ũ�ε� ����
    for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
        requests[i].op = SET;
        requests[i].item.key = i;
        requests[i].item.value_size = MAX_VALUE_SIZE;
        requests[i].item.value = malloc(MAX_VALUE_SIZE);
        if (requests[i].item.value != nullptr) {
            *(int*)requests[i].item.value = rand() % 1000000; // ���� ������ ����
        }
        else {
            requests[i].item.value = nullptr; //����� �ʱ�ȭ
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

    //���ü� ���� �׽�Ʈ
    //�⺻ ��ũ�ε� �׽�Ʈ
    vector<thread> workers;
    for (int i = 0; i < MAX_THREADS; i++) {
        workers.emplace_back(client_func, queue, requests, REQUEST_PER_CLINET);
    }
    for (auto& t : workers) t.join();

    // �߰�: ��Ʈ���� �׽�Ʈ (���ü� ���� ��ȭ)
    /*
    vector<thread> stress_workers;
    for (int i = 0; i < MAX_THREADS; i++) {
        stress_workers.emplace_back(stress_test, queue, 1000);
    }
    for (auto& t : stress_workers) t.join();
    */

    //�ܿ� ������ ó�� (�� üũ ��ȭ)
    while (true) {
        Reply reply = dequeue(queue);
        if (!reply.success) break;
        if (reply.item.value != nullptr) {
            free(reply.item.value);
        }
    }

    //SET���� ����� �޸� ����
    for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
        if (requests[i].item.value != nullptr) {
            free(requests[i].item.value);
        }
    }
    delete[] requests; //�� �޸� ����
    release(queue);

    cout << "sum of returned keys = " << sum_key << endl;
    cout << "sum of returned values = " << sum_value << endl;
    return 0;
}
