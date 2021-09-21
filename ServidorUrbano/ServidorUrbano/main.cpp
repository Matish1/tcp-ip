#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")

bool initializerWinsock();

int main() {

	if (!initializerWinsock()) {
		std::cerr << "Winsock not initializer" << std::endl;
	}

	//*******crear el socket****************************************
	// un socket es un numero de los sistamas unix, es solo un punto que define puerto e ip
	
	int newSocket = socket(AF_INET, SOCK_STREAM, 0);

	//*******generando un listado de socket******************************* 
	SOCKET listening = newSocket;
		
	if (listening == INVALID_SOCKET) {
		std::cerr << "socket invalido" << std::endl;
		return 1;
	}

	// ******enlazar el socket con la direccion de ip y el puerto************

	sockaddr_in addrsocket;	// esta es una estructura para manejar direcciones de internet
	addrsocket.sin_family = AF_INET;
	addrsocket.sin_port = htons(54000);
	
	// si solo quiero enlazarlo a la direccion de host usamos inet_addr("127.0.0.1");
	addrsocket.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)& addrsocket, sizeof(addrsocket));

	// decir si el winsock se ha conectado para escuchar 
	int listenSocket = listen(listening, SOMAXCONN);

	//*********** esperar una conexion de parte del cliente***************************** 
	// si queremos especificar esl escucha de un cliente debemos especificar que existe in cliente con direccion 
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)& client, &clientSize);

	// para informaciond de la conexion de los datos 
	char host[NI_MAXHOST];	// el nombre del ciente remoto 

	char service[NI_MAXSERV];	// nombre del puerto de servico


	// para lograr llenar de 0 una direccion de memoria 
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);


	if (getnameinfo((sockaddr*)& client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV,0) == 0) {
		std::cout << host << " conectado al pueto de servicio de : " << service << std::endl;
	}
	else {
		// si no se puede de esa forma podemos obtener la IPV4 en una cadena y separarla 
		inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
		
		std::cout << host << "conectado al puerto de servicio de :" << 
			ntohs(client.sin_port) << std::endl;
	}

	// cerrar de escuchar el socket 

	closesocket(listening);
	
	// while loop: acepta y envia mensaje de vuelta al cliente 
	
	char buf[4096];

	while (true){
		// esperar al cliente para enviar datos 
		ZeroMemory(buf,sizeof(buf));
		int byteReceived = recv(clientSocket, buf, sizeof(buf), 0);
		if (byteReceived == SOCKET_ERROR) {
			std::cout << "error en recv()" << std::endl;
			break;
		}
		if (byteReceived == 0) {
			std::cout << "CLiente desconectado " << std::endl;
			break;
		}

		// se envia un mensaje de eco de nuevo al cliente  
		send(clientSocket, buf, sizeof(buf), 0);
		if (byteReceived > 0) {
			// esperamos una respuesta
			std::cout << std::string(buf, 0, byteReceived) << std::endl;
		}
	}


	// close the sock 
	closesocket(clientSocket);
	// apagar el winsock
	WSACleanup();

	std::cin.get();
}

// para no olvidar
//WORD se definió como una estructura de datos de 16 bits se dejo por compatibilidad 
// representa un tamño de datos  y es un tipo de datos de 2 bytes

bool initializerWinsock() {
	// inicializar el winsock
	WSADATA wsaData;  // contiene informacion sobre la implementacion de os socket 
	WORD versionWinsock = MAKEWORD(2, 2);

	int resutlWinsock = WSAStartup(versionWinsock, &wsaData);	// la inicializacion de Winsock si es 0 se inicializo con exito
	return (resutlWinsock == 0);
}