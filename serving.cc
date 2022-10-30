#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool ellenoriz(std::string query_path) {
  std::istringstream iss{query_path};
  int melyseg = 0;
  std::string komponens;
  while (std::getline(iss, komponens, '/')) {
    if (komponens == "..") {
      --melyseg;
    } else if (komponens != ".") {
      ++melyseg;
    }
    if (melyseg < 0) {
      return false;
    }
  }
  return true;
}

std::string get_mime_type(std::string path) {
  if (path.ends_with(".png")) {
    return "image/png";
  }
  if (path.ends_with(".html")) {
    return "text/html";
  }
  return "text/plain";
}

int kiszolgal(int fd, std::string path) {
  std::ifstream input_file(path);

  std::string mime_type = get_mime_type(path);

  std::ostringstream header;
  header << "HTTP/1.1 200 OK\r\n"
            "Content-Type: "
         << mime_type
         << "\r\n"
            "\r\n";

  std::string header_str = header.str();

  int rc = write(fd, header_str.data(), header_str.size());
  if (rc == -1) {
    perror("Header irasa");
    return 1;
  }
  int byte_count = 0;
  while (input_file) {
    char buf[4096];

    input_file.read(buf, 4096);
    int rc = write(fd, buf, input_file.gcount());
    if (rc == -1) {
      perror("file irasa");
      return 1;
    }
    byte_count += rc;
  }
  std::cerr << "byte_count: " << byte_count << std::endl;
  return 0;
}

std::string get_query_path(std::string request) {
  std::istringstream iss{request};
  std::string szo;
  std::getline(iss, szo, ' ');
  std::getline(iss, szo, ' ');
  return szo;
}

bool serve(int csckt, std::string www_dir) {
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

  if (!ellenoriz(query_path)) {
    std::cerr << "Tamado!: " << query_path << std::endl;
  } else {
    std::string path = www_dir + query_path;

    kiszolgal(csckt, path);
  }

  int rc = close(csckt);
  if (rc == -1) {
    perror("close");
    return false;
  }
  return true;
}
