#include <netinet/in.h>
#define BUFSIZE 1024

class Server
{
public:
	Server(int port = 9000, int maxConnections = 100);
	int serverListen();
private:
	void error(const char*);
	const char *readTextTCP(char *text, int length, int inFromServer);
	void writeTextTCP(int outToServer, char *line);
	int serverSocket_;
	const int port_;
	const int maxConnections_;
	struct sockaddr_in serverAddr_, clientAddr_;
	char serverBuff_[BUFSIZE];
};