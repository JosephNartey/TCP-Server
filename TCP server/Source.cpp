#include<iostream>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void main()
{
	// initialize winsock

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) 
	{
		cerr << "can't Initialize winsock! Quitting" << endl;
		return;

	}
	// create a socket its just a number but we define the type as shown below
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{

		cerr << "can't create a socket! Quitting" << endl;
		return;

	}
	//bind an ip address and port to the socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;// could also use inet_pton...
	bind(listening,(sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);// maximum connections
    // wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	char host[NI_MAXHOST]; // client's remote name
	char service[NI_MAXSERV]; // service( i.e port) the client is connect on
	ZeroMemory(host, NI_MAXHOST);// same as memset(host, 0, NI_MAXHOST)
	ZeroMemory(service, NI_MAXSERV);
	// getting host name and ip of client 
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " Connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " Connected on port " << ntohs(client.sin_port) << endl;
}

	// clsoe listening socket
	closesocket(listening);

		// while loop : to accept and echo message back to the client
	char buf[4096];
	while (true)
	{

		ZeroMemory(buf, 4096);
		// wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Err in recv()" << endl;
			break;
		}

		if (bytesReceived == 0) {
			cout << "client disconnected" << endl;

			break;
		}
		cout << buf << endl;
		send(clientSocket, buf, bytesReceived + 1, 0);


	}
	// close socket
	closesocket(clientSocket);
	// clean up winsock
	WSACleanup();




}