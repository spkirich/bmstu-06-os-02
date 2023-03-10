#include <stdio.h>
#include <netinet/in.h>
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

  int val = 1;

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
  {
    perror("Failed to set a socket option");
    exit(1);
  }

  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
  {
    perror("Failed to bind the socket");
    exit(1);
  }

  while (1)
  {
    if (listen(sock, 5) == -1)
    {
      perror("Failed to listen on the socket");
      exit(1);
    }

    int conn;

    if ((conn = accept(sock, NULL, NULL)) == -1)
    {
      perror("Failed to accept a connection");
      exit(1);
    }

    const size_t len = 1024;
    char req[len], res[len];

    if (recv(conn, req, len, 0) == -1)
    {
      perror("Failed to receive a message");
      exit(1);
    }

    printf("Server %d got: %s\n", getpid(), req);
    sprintf(res, "%d+%s", getpid(), req);

    if (send(conn, res, sizeof(res), 0) == -1)
    {
      perror("Failed to send a message");
      exit(1);
    }

    printf("Server %d put: %s\n", getpid(), res);
    close(conn);
  }

  return 0;
}
