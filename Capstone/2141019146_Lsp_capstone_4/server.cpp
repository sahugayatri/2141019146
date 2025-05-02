#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_DIR "server_files"
#define AUTH_USER "admin"
#define AUTH_PASS "1234"

void xor_encrypt(char* data, size_t size, char key = 'K') {
    for (size_t i = 0; i < size; ++i) {
        data[i] ^= key;
    }
}

std::string list_files() {
    std::ostringstream oss;
    for (const auto& entry : std::filesystem::directory_iterator(SERVER_DIR)) {
        if (entry.is_regular_file()) {
            oss << entry.path().filename().string() << "\n";
        }
    }
    return oss.str();
}

bool handle_auth(int client_sock) {
    char buffer[1024] = {0};
    int bytes = read(client_sock, buffer, sizeof(buffer) - 1);
    if (bytes <= 0) return false;
    buffer[bytes] = '\0';

    std::string creds(buffer);
    size_t sep = creds.find(':');
    if (sep == std::string::npos) return false;

    std::string user = creds.substr(0, sep);
    std::string pass = creds.substr(sep + 1);

    if (user == AUTH_USER && pass == AUTH_PASS) {
        send(client_sock, "AUTH_SUCCESS", strlen("AUTH_SUCCESS"), 0);
        return true;
    } else {
        send(client_sock, "AUTH_FAILED", strlen("AUTH_FAILED"), 0);
        return false;
    }
}

void send_file(int client_sock, const std::string& filename) {
    std::ifstream infile(SERVER_DIR + std::string("/") + filename, std::ios::binary);
    if (!infile) {
        std::cerr << "File not found: " << filename << "\n";
        return;
    }

    char buffer[1024];
    while (!infile.eof()) {
        infile.read(buffer, sizeof(buffer));
        std::streamsize count = infile.gcount();
        xor_encrypt(buffer, count);
        send(client_sock, buffer, count, 0);
    }
    infile.close();
}

int main() {
    std::filesystem::create_directory(SERVER_DIR);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        socklen_t addrlen = sizeof(address);
        int client_sock = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "Client connected.\n";

        if (!handle_auth(client_sock)) {
            std::cout << "Authentication failed.\n";
            close(client_sock);
            continue;
        }

        std::cout << "Client authenticated.\n";

        std::string file_list = list_files();
        send(client_sock, file_list.c_str(), file_list.size(), 0);

        char filename[256] = {0};
        int name_bytes = read(client_sock, filename, sizeof(filename) - 1);
        if (name_bytes > 0) {
            filename[name_bytes] = '\0';
            std::cout << "Client requested file: " << filename << "\n";
            send_file(client_sock, filename);
        }

        close(client_sock);
    }

    close(server_fd);
    return 0;
}

