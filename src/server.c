#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define CMAX 8
#define PORT 8484

int main()
{
  int epfd;

  if ((epfd = epoll_create(CMAX)) == -1)
  {
    perror("Failed to create an event poll");
    exit(1);
  }

  int sock;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Failed to create a socket");
    exit(1);
  }

  int sopt = 1;

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sopt, sizeof(sopt)) == -1)
  {
    perror("Failed to configure the socket");
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

  if (listen(sock, CMAX) == -1)
  {
    perror("Failed to listen on the socket");
    exit(1);
  }

  struct epoll_event epev;

  epev.events = EPOLLIN;
  epev.data.fd = sock;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &epev) == -1)
  {
    perror("Failed to add the socket to the event poll");
    exit(1);
  }

  while (1)
  {
    struct epoll_event epev[CMAX + 1];
    int nfds;

    if ((nfds = epoll_wait(epfd, &epev, CMAX + 1, -1)) == -1)
    {
      perror("Failed to wait for an event");
      exit(1);
    }

    for (int i = 0; i < nfds; i++)
    {
      if (epev[i].data.fd == sock)
      {
        int conn;

        if ((conn = accept(sock, NULL, NULL)) == -1)
        {
          perror("Failed to accept a connection");
          exit(1);
        }

        struct epoll_event epev;

        epev.events = EPOLLIN;
        epev.data.fd = conn;

        if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn, &epev) == -1)
        {
          perror("Failed to add the connection to the event poll");
          exit(1);
        }
      }

      else
      {
        int conn = epev[i].data.fd;

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
    }
  }

  return 0;
}
