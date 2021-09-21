#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib,"ws2_32.lib")

int main() {

	std::string ipAddress = "127.0.0.1";		// esta es la direccion ip del server 
	int port = 54000;		// direccion puerto del server

	// inicializar el winsock
	WSADATA wsdata;		// este tiene la informacion que necesitamos para los servicios del socket 
	DWORD ver = MAKEWORD(2, 2);

	int resultServiceWinsock = WSAStartup(ver, &wsdata);
	if (resultServiceWinsock != 0) {
		std::cerr << "error en el winwsock" << std::endl;
		return 1;
	}

	// esta es la creacion del socket 
	SOCKET initializeSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (initializeSocket == INVALID_SOCKET) {
		std::cerr << "error en el socket" << std::endl;
		return 1;
	}

	// creacion de la estructura que va a almacenar el pueto y la ip
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// nos conectamos al socket 
	int connectSocket = connect(initializeSocket, (sockaddr*)& hint,sizeof(hint));
	if (connectSocket == SOCKET_ERROR) {
		std::cerr << "Error al conectarase al servidor: "<< WSAGetLastError() << std::endl;
		closesocket(initializeSocket);
		WSACleanup();
		return 1;
	}

	// este es el loop en que se van a recibir y mandar los datos 
	char buff[4096];
	std::string inputUser;
	do {
		std::getline(std::cin, inputUser);

		if (inputUser.size() > 0) {
			// enviamos el texto
			int sendResult = send(initializeSocket, inputUser.c_str(), inputUser.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				// esperamos una respuesta 
				int bytesReceived = recv(initializeSocket, buff, 4096, 0);
				if (bytesReceived > 0) {
					// esperamos una respuesta
					std::cout << " < SERVER > " << std::string(buff, 0, bytesReceived) << std::endl;
				}
			}
		}

	} while (inputUser.size() > 0);

	closesocket(initializeSocket);
	WSACleanup();

	std::cin.get();
}




