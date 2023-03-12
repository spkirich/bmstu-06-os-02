#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8484

int main()
{
  int sock;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Failed to create a socket");
    exit(1);
  }

  struct sockaddr_in addr;

  addr.sin_family = AF_INET;

  if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) == -1)
  {
    perror("Failed to convert an address");
    exit(1);
  }

  addr.sin_port = htons(PORT);

  if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
  {
    perror("Failed to create a connection");
    exit(1);
  }

  const size_t len = 1024;
  char req[len], res[len];

  sprintf(req, "%d", getpid());

  if (send(sock, req, sizeof(req), 0) == -1)
  {
    perror("Failed to send a message");
    exit(1);
  }

  printf("Client %d put: %s\n", getpid(), req);

  if (recv(sock, res, sizeof(res), 0) == -1)
  {
    perror("Failed to receive a message");
    exit(1);
  }

  printf("Client %d got: %s\n", getpid(), res);
  close(sock);

  return 0;
}
