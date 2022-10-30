#include <fstream>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>

int kepet_kuld(int fd) {
  std::ifstream input_file("dirtblock.png");

  const std::string kep_header =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: image/png\r\n"
  "\r\n";

  int rc = write(fd, kep_header.data(), kep_header.size());
  if (rc == -1) {
    perror("Header irasa a kephez");
    return 1;
  }

  while (input_file) {
    char buf[4096];
    input_file.read(buf, 4096);
    int rc = write(fd, buf, input_file.gcount());
    if (rc == -1) {
      perror("kep irasa");
      return 1;
    }
  }

  return 0;
}

std::string get_query_path(std::string request) {
  std::istringstream iss{request};
  std::string szo;
  std::getline(iss, szo, ' ');
  std::getline(iss, szo, ' ');
  return szo;
}

bool serve(int csckt) {
    char buffer[4096];
  int n_read = read(csckt, buffer, 4095);
  if (n_read == -1) {
    perror("read");
    return false;
  }

  buffer[n_read] = 0;
  std::string kaptuk{buffer};
  std::cout << kaptuk << std::endl;

  std::string query_path = get_query_path(kaptuk);
  std::cout << "query path: " << query_path << std::endl;


  if (query_path == "/kep.png") {
    return kepet_kuld(csckt) == 0;
  } else {
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
      return false;
    }

    if (n_written != (int)valasz.size()) {
      std::cerr << "csak " << n_written << " karaktert tudtunk irni " << valasz.size() << " helyett." << std::endl;
      return false;
    }
  }
  return true;
}

