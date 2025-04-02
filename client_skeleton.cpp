/*
 *  client_skeleton.cpp
 *
 *  -------------------------------------------------------------------
 *  This skeleton code provides the basic structure for the Connect 4
 *  client. You will implement the networking parts to:
 *   1. Create a TCP socket.
 *   2. Resolve the server's hostname.
 *   3. Connect to the server.
 *   4. Receive board updates.
 *   5. Send your moves back to the server.
 *   6. Close the connection when the game ends.
 *  -------------------------------------------------------------------
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
// TODO: Include necessary headers for network calls (e.g., unistd.h, sys/types.h, sys/socket.h, netdb.h)

 /*
  * Function: readLine (TO BE IMPLEMENTED)
  *
  * Pseudo code:
  *   • Initialize an empty string.
  *   • Repeatedly call recv() to read one byte at a time.
  *   • Stop if a newline character is read or on error.
  *   • Return the accumulated string.
  */
std::string readLine(int sockfd) {
    // TODO: Implement using recv() in a loop.
    return "";
}

/*
 * Function: writeLine (TO BE IMPLEMENTED)
 *
 * Pseudo code:
 *   • Append a newline character to your string.
 *   • Use a loop with send() to ensure all data is transmitted.
 *   • If send() fails, return false.
 *   • Return true if successful.
 */
bool writeLine(int sockfd, const std::string &line) {
    // TODO: Implement using send() in a loop.
    return false;
}

void displayBoard(const std::string &boardData) {
    std::istringstream iss(boardData);
    std::string line;
    std::cout << " 1 2 3 4 5 6 7" << std::endl;
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        std::cout << line << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <hostname> <port>\n";
        return 1;
    }
    const char *hostname = argv[1];
    int port = std::stoi(argv[2]);

    // ============================================
    // TODO: Step 1 - Create a socket:
    // Pseudo code:
    //   • Use socket() with:
    //         - Domain: AF_INET (IPv4)
    //         - Type: SOCK_STREAM (TCP)
    //         - Protocol: 0
    //   • Check that the socket descriptor is valid.
    // ============================================

    // ============================================
    // TODO: Step 2 - Resolve the hostname:
    // Pseudo code:
    //   • Use a name resolution function like gethostbyname() or getaddrinfo() with the given hostname.
    //   • Verify that the result is valid.
    //   • If resolution fails, print an error and exit.
    // ============================================

    // ============================================
    // TODO: Step 3 - Connect to the server:
    // Pseudo code:
    //   • Fill a sockaddr_in structure with:
    //         - Family: AF_INET
    //         - Port: Convert port to network byte order using htons()
    //         - IP: Copy the resolved IP address from Step 2.
    //   • Call connect() with the server's address.
    //   • If connect() fails, close the socket and exit.
    // ============================================

    std::cout << "Connected to " << hostname << " on port " << port << "\n";

    bool gameOver = false;
    while (!gameOver) {
        // ============================================
        // The server should first send the header "BOARD".
        // ============================================
        std::string header = readLine(/* client socket descriptor */);
        if (header != "BOARD") {
            std::cerr << "Protocol error: expected BOARD, got '" << header << "'\n";
            break;
        }

        // ============================================
        // Read the board data: Expect 6 lines from the server.
        // ============================================
        std::string boardData;
        for (int i = 0; i < 6; i++) {
            std::string line = readLine(/* client socket descriptor */);
            boardData += line + "\n";
        }

        // ============================================
        // Read the turn message from the server (e.g., "TURN CLIENT" or "GAMEOVER ...").
        // ============================================
        std::string turnMsg = readLine(/* client socket descriptor */);

        displayBoard(boardData);

        if (turnMsg.rfind("TURN", 0) == 0) {
            if (turnMsg == "TURN CLIENT") {
                std::cout << "Enter your move (1-7): ";
                int col;
                std::cin >> col;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "Invalid input, try again.\n";
                    continue;
                }
                std::ostringstream oss;
                oss << "MOVE " << col;
                if (!writeLine(/* client socket descriptor */, oss.str())) {
                    std::cerr << "Failed to send move.\n";
                    break;
                }
            } else {
                std::cout << "Waiting for server's move...\n";
            }
        }
        else if (turnMsg.rfind("GAMEOVER", 0) == 0) {
            std::cout << turnMsg << std::endl;
            gameOver = true;
        }
        else {
            std::cerr << "Protocol error: unknown command '" << turnMsg << "'\n";
            break;
        }
    } // end while

    // ============================================
    // TODO: Close the socket:
    // Pseudo code:
    //   • Call close() on the client socket when the game is finished.
    // ============================================

    return 0;
}
