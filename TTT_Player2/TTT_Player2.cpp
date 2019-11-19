#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <string>
#include <windows.h>

#pragma comment (lib, "ws2_32.lib") // This part will disable some warnings 

using namespace std;
string ip = "127.0.0.1";
void Recieve();
void Send(std::string);
char Board[9] =  { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
void printGame();
bool flag = 0;
int wincon = -1;
int Boardvalue = 0;
char YN=' ';
int WinCondition();
void Message();
void reset();
void Message() {
	string message;
	
	do {
		std::cout << "whats your pick >>> ";
		std::cin >> message;
		
		if (flag == 1) {
			Boardvalue = static_cast<int>(message[0] - 49);
			Board[Boardvalue] = 'O';
			printGame();
			wincon = WinCondition();
			if (wincon == 0 || wincon == 1) {
				std::cout << "Player  >>>" << wincon + 1 << " <<< win ";
				std::cout << "Do you want to play again : (y/n) : ";
				std::cin >> YN;
				switch (YN)
				{
				case 'y': case 'Y':
					reset;
					break;
				case 'n': case 'N':
					system("close");
					break;
				default:
					break;
				}
				
			}
			std::cout << "-------------" << Boardvalue << std::endl;
			Send(message);
			flag = !flag;
		}
		else {
			std::cout << "Opps!!! It's not your turn ... wait for player 1 to play ... !";
		}
		} while (message != "q");
	
}
// workin on reseting the game 
void reset() {
	for (int i = 49; i < 57; i++) {
		Board[i-49] = i;
	}
	flag = 0;
	Message();
}

void printGame() {
	system("CLS");
	for (int a = { 0 }; a < 3; a++)
	{
		std::cout << "  | ";
		for (int b = { 0 }; b < 3; b++)
		{
			std::cout << Board[b + (a * 3)] << " | ";
		}
		std::cout << "\n";
	}
}

/// Game win condition  // By Kritian

int playing = -1;
int WinCondition()
{
	if (((Board[0] == 'X') && (Board[1] == 'X') && (Board[2] == 'X')) || ((Board[0] == 'O') && (Board[1] == 'O') && (Board[2] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on top row!!\n";
		playing = 0;
		return 0;
	}
	// cheking vertical line 2
	if (((Board[3] == 'X') && (Board[4] == 'X') && (Board[5] == 'X')) || ((Board[3] == 'O') && (Board[4] == 'O') && (Board[5] == 'O')))
	{
		std::cout << "Player " << Board[3] << " is the winner on second row!!\n";
		playing = 0;
		return 0;
	}
	// cheking vertical line 3
	if (((Board[6] == 'X') && (Board[7] == 'X') && (Board[8] == 'X')) || ((Board[6] == 'O') && (Board[7] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[6] << " is the winner on third row!!\n";
		playing = 0;
		return 0;
	}

	// cheking horizontal line 1
	if (((Board[0] == 'X') && (Board[3] == 'X') && (Board[6] == 'X')) || ((Board[0] == 'O') && (Board[3] == 'O') && (Board[6] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on first collon!!\n";
		playing = 0;
		return 0;
	}
	// cheking horizontal line 2
	if (((Board[1] == 'X') && (Board[4] == 'X') && (Board[7] == 'X')) || ((Board[1] == 'O') && (Board[4] == 'O') && (Board[7] == 'O')))
	{
		std::cout << "Player " << Board[1] << " is the winner on second collon!!\n";
		playing = 0;
		return 0;
	}
	// cheking horizontal line 3
	if (((Board[2] == 'X') && (Board[5] == 'X') && (Board[8] == 'X')) || ((Board[2] == 'O') && (Board[5] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[2] << " is the winner on second collon!!\n";
		playing = 0;
		return 0;
	}
	// cheking diagonal line 1 from top left to bottom right
	if (((Board[0] == 'X') && (Board[4] == 'X') && (Board[8] == 'X')) || ((Board[0] == 'O') && (Board[4] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on the diagonal row1 !!\n";
		playing = 0;
		return 0;

	}
	// cheking diagonal line 2 from top right to bottom left
	if (((Board[2] == 'X') && (Board[4] == 'X') && (Board[6] == 'X')) || ((Board[2] == 'O') && (Board[4] == 'O') && (Board[6] == 2)))
	{
		std::cout << "Player " << Board[2] << " is the winner on the diagonal row2!!\n";
		playing = 0;
		return 0;

	}
	return -1;
}



////////////////////////////////////////////// Server Codes 




void Recieve() {

	WSADATA data;
	WORD version = MAKEWORD(3, 2); // we just make a version here, can be any number 

	int wsOk = WSAStartup(version, &data); // starting the socket 
	if (wsOk != 0) // if socket is not started , then quit 
	{
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);  // Creating the socket // Sock_DGRAM is UDP socket

	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // any IP would be okay 
	serverHint.sin_family = AF_INET; // IPv4 addresses 
	serverHint.sin_port = htons(54001); // selecting a port to open for the Assigned ip 
	// Convert from little to big endian // study more about Little and big endian


	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) // biding to the socket 
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}


	sockaddr_in client; // instance object of the scokaddr // hold address information 
	int clientLength = sizeof(client); // The size of the client information

	char buf[1024]; // buffer size 


	while (true)   // always listening to the port
	{
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buf, 1024); // Clear the receive buffer

		// recieve the message 
		int Msg = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (Msg == SOCKET_ERROR)
		{
			perror("Error receiving from client ");
			WSAGetLastError();
			continue;
		}

		
		char clientIp[256]; 
		ZeroMemory(clientIp, 256); 

		// Convert from byte array to char
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		//cout << "Message recv from " << clientIp << " : " << buf << endl;
		std::cout << buf;
		Boardvalue = static_cast<int>(buf[0] - 49);
		std::cout << "-------------" << Boardvalue << std::endl;
		Board[Boardvalue] = 'X';
		printGame();
		WinCondition();
		flag =!flag;
		std::cout << "Flag :" << flag;
	}
	// Close socket
	closesocket(in);

	// Shutdown winsock
	WSACleanup();
}

void Send(std::string s) {

	WSADATA data;

	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	// CONNECT TO THE SERVER

	sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(54000); 
	int ret = inet_pton(AF_INET, ip.c_str(), &server.sin_addr); 
	if (ret <= 0) {
		if (ret == 0)
			perror( "Not correct format");
		else
			perror("inet_pton error");
		exit(EXIT_FAILURE);
	}


	// Socket creation, note that the socket type is datagram
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
	// Write out to that socket
	//do {

	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		cout << "That didn't work! " << WSAGetLastError() << endl;
	}
	//	} while (s != "q");
		// Close the socket
	closesocket(out);

	// Close down Winsock
	WSACleanup();

}
void Print(string name) {

	std::cout << "        \t \t\t            Player 1  === > " << name << "\n";

}
int main() // We can pass in a command line option!! 
{
	
	std::cout << "Enter the oppomet IP: < Deafult : 127.0.0.1 > ";
	std::cin >> ip;
	//Print(name);
	printGame();
	thread T1(Message);
	thread T2(Recieve);
		T1.join();
	T2.join();
	return 0;
}