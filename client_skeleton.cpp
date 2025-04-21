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
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

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
    std::string byteString = "";
    
    char buffer[1]; // Need to read from `recv()` one byte at a time, so no point in having a buffer > 1 byte in size.
    while(true)
    {
                                             // 0 is passed as we have no use for flags.
        if(recv(sockfd, buffer, sizeof(buffer), 0) <= 0 ) // if no bytes received
        {
            std::cerr << "[ERROR] recv(): "<< errno << " - " <<  strerror(errno) << std::endl;
            break;
        }
        if(buffer[0] == '\n') // if '\n` received, break from loop.
            break;
        
        //if(byteString == "\n") removed this line as it's not necessary in my implementation.
        byteString += buffer[0]; // Add recieved data to byteString.
    }
    
    return byteString;
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
bool writeLine(int sockfd, const std::string& line) {
    // TODO: Implement using send() in a loop.

    std::string newLine = line; //technically I don't need to do this since I can just do another send() after the loop with '\n', but that feels kinda dumb.
    newLine += '\n';
    for(char c : newLine)
    {
        if(send(sockfd, &c, 1, 0) <= 0) // If some error occurs
        {
            std::cerr << "[ERROR] send(): "<< errno << " - " <<  strerror(errno) << std::endl;
            return false; //break not necessary since error detected.
        }    
    }
    return true;
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
    const char* hostname = argv[1];
    const char* service = argv[2]; //swapped this to be a char* because getaddrinfo() requires a const char*
    //int port = std::stoi(argv[2]);

    struct addrinfo* result, result_ptr;
    struct sockaddr_in server = {}; // Init so there's no garbage data.
    




    // ============================================
    // TODO: Step 1 - Create a socket:
    // Pseudo code:
    //   • Use socket() with:
    //         - Domain: AF_INET (IPv4)
    //         - Type: SOCK_STREAM (TCP)
    //         - Protocol: 0
    //   • Check that the socket descriptor is valid.
    // ============================================
    

    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //  Socket file descriptor. Returns -1 on Error & used later on in `connect()` and `bind()`
    if(sockfd == -1)
    {
        std::cerr << "[ERROR] socket(): " << strerror(errno) << std::endl;
        return errno; //close program on errno
    }
    std::cout << "[DEBUG] socket() successful." << std::endl;


    // ============================================
    // TODO: Step 2 - Resolve the hostname:
    // Pseudo code:
    //   • Use a name resolution function like gethostbyname() or getaddrinfo() with the given hostname.
    //   • Verify that the result is valid.
    //   • If resolution fails, print an error and exit.
    // ============================================

    // hints not necessary since sockfd          vvvv 
    int getaddrinfo_status = getaddrinfo(hostname, service, NULL, &result); // Success on `getaddrinfo()`. This will be used later on in conjunction with connect()
    //above returns a list of IPs in this case to the resolved host and port that will allow us to pick one if >1 are returned.

    if(getaddrinfo_status != 0) // 0 is success. Only handling failures to print & exit as required.
    {
        std::cerr << "[ERROR] getaddrinfo(): " << gai_strerror(getaddrinfo_status) << std::endl;
        return getaddrinfo_status; // Exits the program with the error code given by getaddrinfo_status.
    }
    std::cout << "[DEBUG] getaddrinfo() successful." << std::endl;

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


    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(service));

    struct sockaddr_in* resolved = (struct sockaddr_in*)(result->ai_addr);
    server.sin_addr = resolved->sin_addr;
    
    std::cout << "[DEBUG] Running connect()..." << std::endl;

    int connect_success = connect(sockfd, (struct sockaddr*)&server, sizeof(server));
    // connect(socketfd, address, address_length) returns 0 on success, -1 on failure.
    std::cout << "[DEBUG] connect_success = " << connect_success << std::endl;
    if( connect_success == -1)
    {
        std::cerr << "[ERROR] connect(): " << strerror(errno) << std::endl;
        // TODO: Implement close socket here.
        return errno; // Exits the program 
    }
    std::cout << "[DEBUG] connect() successful." << std::endl;

    std::cout << "Connected to " << hostname << " on port " << service << "\n"; //renamed from earlier.
    //std::cout << "Connected to " << hostname << " on port " << port << "\n";

    bool gameOver = false;
    while (!gameOver) {
        // ============================================
        // The server should first send the header "BOARD".
        // ============================================
        std::string header = readLine(sockfd);
        if (header != "BOARD") {
            std::cerr << "Protocol error: expected BOARD, got '" << header << "'\n";
            break;
        }

        // ============================================
        // Read the board data: Expect 6 lines from the server.
        // ============================================
        std::string boardData;
        for (int i = 0; i < 6; i++) {
            std::string line = readLine(sockfd);
            boardData += line + "\n";
        }

        // ============================================
        // Read the turn message from the server (e.g., "TURN CLIENT" or "GAMEOVER ...").
        // ============================================
        std::string turnMsg = readLine(sockfd);

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
                if (!writeLine(sockfd, oss.str())) {
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


    close(sockfd); // no check is necessary here because if the code reaches this section, the game is over and is notifying the server to terminate the session.
    return 0;
}
