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

/*
    this is an error since this is a UNIX header. windows headers are:
    - winsock2.h
    - ws2tcpip.h
    - ws2spi.h
    prob means for simplicity sake, I need to test everything on a UNIX-based OS
        (prob linprog which kms)
*/



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
    while(recv(sockfd) > 1)
    {
        byteString = "";
        
        //do something

        if(byteString == "\n")
            //stop and return string?
    }

    return ""; // am I allowed to replace with byteString?
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


    if(send()) // need to implement send()?
        return true;
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
    const char* hostname = argv[1];
    const char* service = argv[2]; //swapped this to be a char* because getaddrinfo() requires a const char*
    //int port = std::stoi(argv[2]);

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
    if(sockfd = -1)
    {
        std::cerr << "[ERROR] socket(): " << strerror(errno);
        return errno; //close program on errno
    }



    // ============================================
    // TODO: Step 2 - Resolve the hostname:
    // Pseudo code:
    //   • Use a name resolution function like gethostbyname() or getaddrinfo() with the given hostname.
    //   • Verify that the result is valid.
    //   • If resolution fails, print an error and exit.
    // ============================================

    struct addrinfo* result;
    // hints not necessary since sockfd          vvvv 
    int success = getaddrinfo(hostname, service, NULL, &result); // Success on `getaddrinfo()`. This will be used later on in conjunction with connect() and bind(). 
    //above returns a list of IPs in this case to the resolved host and port that will allow us to pick one if >1 are returned.


    if(success != 0) // 0 is success. Only handling failures to print & exit as required.
    {
        std::cerr << "[ERROR] getaddrinfo(): " << gai_strerror(success) << std::endl;
        return success; // Exits the program with the error code given by success.
    }

    /* 
    Since getaddrinfo() returns a Linked List of valid addresses, we can try and
    connect to each one by navigating the list until we connect to one.
    */
    
    bool connected = false;
    
    //  A pointer to `result` from `getaddrinfo()`. Used for head preservation.
    for(addrinfo* result_ptr = result; result_ptr != null; list->next)
    {
        if(bind(socketfd, result_ptr->ai_addr, result_ptr->ai_addrlen) == 0)
            break; // bind is successful since bind() returns 0 on success. So we can break from the loop now.
    }
    /*
    Below copied from getaddrinfo() man page:

        for (rp = result; rp != NULL; rp = rp->ai_next) {
               sfd = socket(rp->ai_family, rp->ai_socktype,
                       rp->ai_protocol);
               if (sfd == -1)
                   continue;

               if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
                   break; //success             

                   close(sfd);
        }

    */

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

    struct sockaddr_in whatshouldicallthis = { AF_INET, service, /*<address of site> (I think it might be something like result->ai_addr)*/ }

    // connect(socketfd, address, address_length) returns 0 on success, -1 on failure.
    if(connect() == -1)
    {
        std::cerr << "[ERROR] connect(): " << strerror(errno) << std::endl;
        // TODO: Implement close socket here.
        return errno; // Exits the program 
    }

    std::cout << "Connected to " << hostname << " on port " << service << "\n"; //renamed from earlier.
    //std::cout << "Connected to " << hostname << " on port " << port << "\n";

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


    close(sockfd) // no check is necessary here because if the code reaches this section, the game is over and is notifying the server to terminate the session.
    return 0;
}
