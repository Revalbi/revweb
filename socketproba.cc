#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

int main(int argc, char* argv[]) {
  int sckt = socket(AF_INET, SOCK_STREAM, 0);
  if (sckt == -1) {
    perror("socket krealas");
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(14555);
  addr.sin_addr.s_addr = 0;

  int rc = bind(sckt, (sockaddr*)&addr, sizeof(addr));
  if (rc == -1) {
    perror("bind");
    return 1;
  }

  rc = listen(sckt, SOMAXCONN);
  if (rc == -1) {
    perror("listen");
    return 1;
  }

  struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);
  int csckt = accept(sckt, (sockaddr*)&peer_addr, &peer_addr_len);
  if (rc == -1) {
    perror("accept");
    return 1;
  }

  char buffer[4096];
  int n_read = read(csckt, buffer, 4095);
  if (n_read == -1) {
    perror("read");
    return 1;
  }

  buffer[n_read] = 0;
  std::string kaptuk{buffer};
  std::cout << kaptuk << std::endl;

  std::string valasz = "HTTP/1.1 200 OK\r\n"
  "\r\n"
  "<html>\r\n"
  "<body>\r\n"
  "<h1>Elso weblapom</h1>\r\n"
  "abrakadabra\r\n"
  "</body\r\n>"
  "</html>\r\n";

  int n_written = write(csckt, valasz.c_str(), valasz.size());
  if (n_written == -1) {
    perror("write");
    return 1;
  }

  if (n_written != (int)valasz.size()) {
    std::cerr << "csak " << n_written << " karaktert tudtunk irni " << valasz.size() << " helyett." << std::endl;
    return 1;
  }

  rc = close(csckt);
  if (rc == -1) {
    perror("close");
    return 1;
  }

  return 0;
}