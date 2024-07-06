#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") //���־󿡼� ���� �ϱ� ���� ��
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
		cout << "���� �޼��� : " << buffer << endl << endl;
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
		cout <<"���� ����Ϸ�" << endl<< endl;
		break;
		} 
	}
	char buffer[PACKET_SIZE] = { 0 };
	thread proc1(thr_recvs);
	char msg[PACKET_SIZE] = {};

	while (!WSAGetLastError()) {
		cin.getline(buffer, PACKET_SIZE, '\n');
		cout << "���� ������ �޽��� : " << buffer << endl << endl;
		send(soc, buffer, strlen(buffer), 0);
	}

	proc1.join();

	closesocket(soc);
	WSACleanup();
}