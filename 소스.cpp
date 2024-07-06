#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") //비주얼에서 소켓 하기 위한 것
using namespace std;

#define PACKET_SIZE 1024

SOCKET soc;

void thr_recvs() {
	char buffer[PACKET_SIZE] = {0};
	string cmd;
	while (!WSAGetLastError()) {
		memset(&buffer, 0, sizeof(buffer));
		recv(soc, buffer, PACKET_SIZE, 0);
		cmd = buffer;
		if (cmd == "hi") break;
		cout << "받은 메세지 : " << buffer << endl << endl;
	}
}

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

	while (1) {
		if (!connect(soc, (SOCKADDR*)&addr, sizeof(addr))) {
		cout <<"서버 연결완료" << endl<< endl;
		break;
		} 
	}
	char buffer[PACKET_SIZE] = { 0 };
	thread proc1(thr_recvs);
	char msg[PACKET_SIZE] = {};

	while (!WSAGetLastError()) {
		cin.getline(buffer, PACKET_SIZE, '\n');
		cout << "내가 전달한 메시지 : " << buffer << endl << endl;
		send(soc, buffer, strlen(buffer), 0);
	}

	proc1.join();

	closesocket(soc);
	WSACleanup();
}