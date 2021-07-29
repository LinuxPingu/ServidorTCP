#include <iostream>
#include <ws2tcpip.h>
#include <sstream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

void main() {

	// 1) Initializae Winsock *///
		
	WSADATA wsDATA;
	WORD ver = MAKEWORD(2, 2);
	int wsOK = WSAStartup(ver, &wsDATA);
	if (wsOK) {
		cerr << "Can't Initialize winsock! Quitting..." << endl;
		return;
	}

	// 2) Create a Socket *//
	SOCKET listening = socket(AF_INET,SOCK_STREAM,0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create main Socket! Quitting..." << endl;
		return;
	}

	// 3) Bind a Socket to an Ip Adress and Port *//
	 sockaddr_in hint;
	 hint.sin_family = AF_INET;
	 hint.sin_port = htons(54000);
	 hint.sin_addr.S_un.S_addr = INADDR_ANY;
	 bind(listening,(sockaddr*)&hint,sizeof(hint));

	// 4) Tell Winsock the socket is for listening *//
	
	 listen(listening,SOMAXCONN);


     #pragma region Solo 1 cliente
	 //// 5) Wait for Connection *//
	// sockaddr_in client;
	// int clientsize = sizeof(client);
	// SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientsize);
	// if (clientsocket == INVALID_SOCKET)
	// {
	//	 cerr << "Can't create client Socket! Quitting..." << endl;
	//	 return;
	// }

	// char host[NI_MAXHOST]; // Nombre del cliente *//
	// char service[NI_MAXHOST]; // Puerto en el que el cliente esta conectado*//

	// ZeroMemory(host,NI_MAXHOST);
	// ZeroMemory(service, NI_MAXHOST);

	// if (getnameinfo( (sockaddr*)&client,sizeof(client),host,NI_MAXHOST,service,NI_MAXSERV,0) == 0) {

	//	 cout << host << " conected to port " << service << endl;
	// }
	// else
	// {
	//	 inet_ntop(AF_INET, &client.sin_addr,host, NI_MAXHOST);
	//	 cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	// }

	//// 6) Close Listening  socket *//
	// closesocket(listening);

	//// 7) While loop: accept and echo message back to client *//
	// 
	// char buf[4096];

	// while (true)
	// {
	//	 ZeroMemory(buf,4069);

	//	 // Espera por un cliente a enviar informacion *//
	//	 int bytesRecived = recv(clientsocket, buf , 4096,0);
	//	 if (bytesRecived == SOCKET_ERROR)
	//	 {
	//		 cerr << "Error in recv(). Quitting..." << endl;
	//		 break;
	//	 }

	//	 if (bytesRecived == 0)
	//	 {
	//		 cout << " Client disconnected " << endl;
	//		 break;
	//	 }

	//	 // Imprime el mensaje del cliente
	//	 send(clientsocket, buf, bytesRecived + 1, 0);
	// }

	//// 8) Close Socket *//
	// closesocket(clientsocket);
#pragma endregion

	#pragma region Varios Clientes 
	 /*5) Declaring the socket sets*/
	 fd_set master;
	 /* Se limpia el set*/
	 FD_ZERO(&master);
	 /* Agregamos al set el socket para escuchar*/
	 FD_SET(listening,&master);

	 while (true)
	 {
		/*Para aceptar a mas de un usuario debemos hacer un loop while que copie
		  el set master, ya que cada recorrido del loop este se borraría */

		 fd_set copy = master;
		 int socketCount = select(0,&copy,nullptr,nullptr,nullptr);

		 for (int i = 0; i < socketCount; i++)
		 {
			 SOCKET sock = copy.fd_array[i];

			 /* Si es el socket 1 es aceptar una nueva conexion */
			 if (sock == listening)
			 {

				 /* Aceptar una nueva conexion*/
				 SOCKET client = accept(listening,nullptr,nullptr);

				 /*Agregar la conexion a la lista de clientes*/
				 FD_SET(client,&master);

				 /*Mensaje de Welcome*/
				 string welcomeMsg = "Te has conectado al chat, disfruta tu tiempo \r\n";
				 send(client,welcomeMsg.c_str(),welcomeMsg.size()+1, 0);

			 }
			 else {
				 /* De lo contrario es un mensaje*/
				 char buf[4096];
				 ZeroMemory(buf,4096);
				 int bytesIn = recv(sock, buf, 4096, 0);
				 if (bytesIn <= 0)
				 {
					 /* Dropear el cliente*/
					 closesocket(sock);
					 FD_CLR(sock, &master);
				 }	
				 else {
					 /*Mandar mensaje*/
					 for (int i = 0; i < master.fd_count;i++) {

						 SOCKET outSock = master.fd_array[i];
						 if (outSock != listening && outSock != sock) {
							 ostringstream ss;

							 ss << "Socket #" << sock << " : " << buf << "\r\n";
							 string strOut = ss.str();
							 send(outSock,strOut.c_str(),strOut.size()+1,0);
						 }
					 }
				 }

			 }
		 }
	 }

	#pragma endregion
	
	
	// 9) Shutdown Winsock
	 WSACleanup();

	 system("pause");
}


