#include "qtype.h"
#include "queue.h"
#include <stdio.h>
#include <iostream>
#include <time.h>

int main(void) {
	srand((unsigned int)time(NULL)); //데이터의 무작위 srand.

	//워크로드 생성(GETRANGE는 패스)
	Request requests[REQUEST_PER_CLINET]; //클라이언트 스레드가 수행할 작업을 저장. //requests 배열: 클라이언트 스레드가 수행할 작업(SET/GET)을 저장.
	//워크로드 분배
	for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
		requests[i].op = SET; //전체 요청의 절반을 차지
		requests[i].item.key = i; //key는 순차적
		requests[i].item.value = (void*)(rand() % 1000000); //value는 난수로 할당.
	}
	for (int i = REQUEST_PER_CLINET / 2; i < REQUEST_PER_CLINET; i++) {
		requests[i].op = GET; //나머지 절반을 차지하며, 큐에서 아이템 추출 테스트.
	}

	Queue* queue = init(); //큐 초기화 함수 추출.
	//if (queue == NULL) return 0;

	//일단 한 개 뿐인데, 그래도 multi client라고 가정하기
	thread client = thread(client_func, queue, requests, REQUEST_PER_CLINET); //실제 큐 연산(SET/GET)을 수행하는 함수. 별도 스레드에서 실행됨.
	client.join(); //스레드 완료 대기.

	//의미 없는 작업
	//누적된 결과 값. 데이터 무결성 검증용.
	cout << "sum of returned keys = " << sum_key << endl;
	cout << "sum of returned values = " << sum_value << endl;
}