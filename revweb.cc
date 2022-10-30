#include <netinet/ip.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "serving.h"

int main(int argc, char *argv[]) {
  std::string www_dir = ".";
  if (argc > 1) {
    www_dir = argv[1];
  }

  int sckt = socket(AF_INET, SOCK_STREAM, 0);
  if (sckt == -1) {
    perror("socket krealas");
    return 1;
  }

  const int enable = 1;
  int rc = setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
  if (rc == -1) {
    perror("setsockopt");
    return 1;
  }
 
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(14555);
  addr.sin_addr.s_addr = 0;

  rc = bind(sckt, (sockaddr *)&addr, sizeof(addr));
  if (rc == -1) {
    perror("bind");
    return 1;
  }

  rc = listen(sckt, SOMAXCONN);
  if (rc == -1) {
    perror("listen");
    return 1;
  }

  while (true) {
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    int csckt = accept(sckt, (sockaddr *)&peer_addr, &peer_addr_len);
    if (rc == -1) {
      perror("accept");
      return 1;
    }

    if (!serve(csckt, www_dir)) {
      std::cerr << "nem sikerult kiszolgalni" << std::endl;
      return 1;
    }
  }

  rc = close(sckt);
  if (rc == -1) {
    perror("closing original socket");
    return 1;
  }

  return 0;
}