#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <WinSock2.h>
#include <iostream>

using namespace std;
void Err_handling(char *message) {
  fputs(message, stderr); // �޽��� ���
  fputc('\n', stderr);
  exit(1); //����
}
void main() {
  WSADATA wsaData;
  SOCKET ServSock, ClntSock;
  SOCKADDR_IN servAddr, clntAddr;

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // WSAStartup�Լ��� ȣ���Ͽ� ���Ϲ����� �˸��� ���̺귯�� �ʱ�ȭ �۾�(������ 0 ���н� 0�ƴ� �����ڵ�)
    Err_handling("WSAStartup ERR");
  }

  ServSock = socket(PF_INET, SOCK_STREAM, 0); // ���� ���ϻ��� ipv4��������, ����������, ??(������ �����ڵ�, ���н� INVALID_SOCKET ��ȯ)
  if (ServSock == INVALID_SOCKET) {
    Err_handling("INVALID_SOCKET");
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET; // ipv4
  servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  // long���� host to network long������ �ٲ��. INADDR_ANY  = �����̵����ϴ��ּ�  little endian > big endian
  servAddr.sin_port = htons(9000); // host to network  short���� �Է¹����� atoi�� ��Ƽ�� ����ȯ
  
  if (SOCKET_ERROR == bind(ServSock, (SOCKADDR*)&servAddr, sizeof(servAddr))) { // ���Ͽ� ip�� ��Ʈ��ȣ �Ҵ�
    Err_handling("Bind ERR");
  }

  if (SOCKET_ERROR == listen(ServSock, 5)) { // ������ Ŭ���̾�Ʈ ������ �޾Ƶ��̱� ���� ���� ���� backlog(�ִ� �����û ���ť�� ��)
    Err_handling("Listen ERR");
  }
  int clntAddr_size = sizeof(clntAddr); // accept(Socket, sockaddr*, int *addrlen) addrlen�� �����������̹Ƿ� bind�ʹ� �ٸ��� �����ϳ� ����������
  ClntSock = accept(ServSock, (SOCKADDR*)&clntAddr, &clntAddr_size); // Ŭ���̾�Ʈ�� �����û ����(������ �����ڵ�, ���н� invalid_socket)
  if (ClntSock == INVALID_SOCKET) {
    Err_handling("Client socket ERR");
  }

  send(ClntSock, "HI im server", 15, 0);
  //recv()
  closesocket(ClntSock);
  closesocket(ServSock);
  WSACleanup();


  // ��ü�� �帧
  // wsastartup���� ���Ϲ���, ���̺귯�� �ʱ�ȭ
  // �������ϻ���
  // �������� family, �ּ�, ��Ʈ ����
  // �ּҸ� ���Ͽ� bind
  // �������� listen
  // Ŭ���̾�Ʈ���ϻ��� (accept)
  // send, recv �� �۾�
  // closesocket, wsacleanup���� ������
}