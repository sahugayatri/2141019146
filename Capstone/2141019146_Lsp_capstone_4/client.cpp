#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT 8080
#define OUTPUT_DIR "capstone_4/"

void xor_decrypt(char* data, size_t size, char key = 'K') {
    for (size_t i = 0; i < size; ++i) {
        data[i] ^= key;
    }
}

int main() {
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported\n";
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return 1;
    }

    // Authenticate
    std::string user, pass;
    std::cout << "Username: ";
    std::getline(std::cin, user);
    std::cout << "Password: ";
    std::getline(std::cin, pass);
    std::string creds = user + ":" + pass;
    send(sock, creds.c_str(), creds.size(), 0);

    char response[1024] = {0};
    int resp_bytes = read(sock, response, sizeof(response) - 1);
    if (resp_bytes <= 0) {
        std::cerr << "Failed to receive authentication response.\n";
        close(sock);
        return 1;
    }
    response[resp_bytes] = '\0';

    if (std::string(response) != "AUTH_SUCCESS") {
        std::cout << "Auth failed.\n";
        close(sock);
        return 1;
    }
    std::cout << "Authenticated!\n";

    // Receive file list
    char filelist[2048] = {0};
    int list_bytes = read(sock, filelist, sizeof(filelist) - 1);
    filelist[list_bytes] = '\0';
    std::cout << "\nAvailable Files:\n" << filelist;

    // Request file
    std::string fname;
    std::cout << "\nEnter filename to download: ";
    std::getline(std::cin, fname);
    send(sock, fname.c_str(), fname.size(), 0);

    // Create output directory if it doesn't exist
    std::filesystem::create_directory(OUTPUT_DIR);

    std::ofstream outfile(OUTPUT_DIR + fname, std::ios::binary);
    if (!outfile) {
        std::cerr << "Failed to create file: " << OUTPUT_DIR + fname << "\n";
        close(sock);
        return 1;
    }

    // Read and decrypt file content
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = read(sock, buffer, sizeof(buffer))) > 0) {
        xor_decrypt(buffer, bytesRead);
        outfile.write(buffer, bytesRead);
    }

    std::cout << "File downloaded as: " << OUTPUT_DIR + fname << "\n";

    outfile.close();
    close(sock);
    return 0;
}

