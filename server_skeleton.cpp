/*
 *  server_skeleton.cpp
 *
 *  -------------------------------------------------------------------
 *  This skeleton code provides the complete game logic for a Connect 4
 *  server. However, the networking parts (creating/binding/listening/
 *  accepting connections, sending/receiving data) have been removed.
 *  It is YOUR job to implement them based on the pseudo code provided.
 *
 *  You will need to:
 *   1. Create a TCP socket.
 *   2. Bind that socket to the port provided on the command line.
 *   3. Put the socket in listening mode.
 *   4. Accept an incoming client connection.
 *   5. Exchange messages with the client using socket reads and writes.
 *   6. Close the connection when the game is finished.
 *
 *  Read the pseudo code in the TODO sections carefully.
 *  -------------------------------------------------------------------
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
// TODO: Include necessary headers for socket programming (e.g., unistd.h, sys/types.h, sys/socket.h, netinet/in.h)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // for inet_ntoa(). Not necessary, but I want it.

const int ROWS = 6;
const int COLS = 7;
const int BACKLOG = 1; // Maximum pending connections. Set to 1 to allow a backlog, but we don't need >1 other than to show that it works.

void initBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = '.';
}

void printBoard(const char board[ROWS][COLS]) {
    std::cout << " 1 2 3 4 5 6 7" << std::endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::string boardToString(const char board[ROWS][COLS]) {
    std::ostringstream oss;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            oss << board[i][j];
            if (j < COLS - 1)
                oss << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

int dropPiece(char board[ROWS][COLS], int col, char piece) {
    if (col < 0 || col >= COLS)
        return -1;
    for (int row = ROWS - 1; row >= 0; row--) {
        if (board[row][col] == '.') {
            board[row][col] = piece;
            return row;
        }
    }
    return -1;
}

bool checkWin(const char board[ROWS][COLS], int row, int col, char piece) {
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    for (int i = 0; i < 4; i++) {
        int dr = directions[i][0], dc = directions[i][1];
        int count = 1;
        int r = row + dr, c = col + dc;
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == piece) {
            count++;
            r += dr;
            c += dc;
        }
        r = row - dr;
        c = col - dc;
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == piece) {
            count++;
            r -= dr;
            c -= dc;
        }
        if (count >= 4)
            return true;
    }
    return false;
}

bool checkTie(const char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j] == '.')
                return false;
    return true;
}

/*
 * Function: readLine (TO BE IMPLEMENTED)
 *
 * Pseudo code:
 *   - Initialize an empty string to hold the incoming characters.
 *   - Loop:
 *       • Use recv() to read one byte from the socket.
 *       • If recv() returns 0 or a negative value, break (error or disconnect).
 *       • If the character is '\n', stop reading.
 *       • Otherwise, append the character to your string.
 *   - Return the string.
 */
std::string readLine(int client) { 
    // TODO: Implement using recv() in a loop.
    std::string byteString = "";
    
    char buffer[1]; // Need to read from `recv()` one byte at a time, so no point in having a buffer >1 byte in size.
    while(true)
    {
                                                  // 0 is passed as we have no use for flags in here.
        int n = recv(client, buffer, sizeof(buffer), 0);
        if( n <= 0 ) // if no bytes received
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
 *   - Append a newline character to the provided string.
 *   - Using a loop, call send() until all bytes of the string are transmitted.
 *   - If send() fails at any point, return false.
 *   - Return true if the complete string is sent.
 */
bool writeLine(int client, const std::string &line) {
    // TODO: Implement using send() in a loop.
    std::string newLine = line; //technically I don't need to do this since I can just do another send() after the loop with '\n', but that feels kinda dumb.
    newLine += '\n';
    for(char c : newLine)
    {
        if(send(client, &c, 1, 0) <= 0) // If some error occurs
        {
            std::cerr << "[ERROR] send(): "<< errno << " - " <<  strerror(errno) << std::endl;
            return false; //break not necessary since error detected.
        }    
    }
    return true;
}

/*
 * Function: sendBoardAndTurn
 *
 * Constructs a message consisting of:
 *   - The header "BOARD"
 *   - The current board state (using boardToString)
 *   - A message indicating whose turn it is or the game outcome.
 * Sends this message to the client.
 */
bool sendBoardAndTurn(int client, const char board[ROWS][COLS], const std::string &turnMsg) {
    std::string msg = "BOARD\n" + boardToString(board) + turnMsg;
    return writeLine(client, msg);
}



int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }
    int port = std::stoi(argv[1]);

    // ============================================
    // TODO: Step 1 - Create a socket:
    // Pseudo code:
    //   • Call the socket() function with:
    //         - Domain: AF_INET (IPv4)
    //         - Type: SOCK_STREAM (TCP)
    //         - Protocol: 0 (default for TCP)
    //   • Check if the returned socket descriptor is valid (not negative).
    //   • If it fails, display an error and exit.
    // ============================================
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        std::cerr << "[ERROR] socket(): " << strerror(errno) << std::endl;
        return errno; //close program on errno
    }
//    std::cout << "[DEBUG] socket() successful." << std::endl;
    // ============================================
    // TODO: Step 2 - Bind the socket to the specified port:
    // Pseudo code:
    //   • Prepare a sockaddr_in structure:
    //         - Set the family to AF_INET.
    //         - Set the port using htons(port).
    //         - Set the IP address to INADDR_ANY.
    //   • Call bind() with your socket, the address structure, and its size.
    //   • Verify that bind() succeeds; otherwise, print an error and exit.
    // ============================================
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(sockfd,(struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
    { // If bind() errors
        std::cerr << "[ERROR] bind(): " << strerror(errno) << std::endl;
        return errno;
    }
//    std::cout << "[DEBUG] bind() successful." << std::endl;
    // ============================================
    // TODO: Step 3 - Put the socket into listening mode:
    // Pseudo code:
    //   • Call listen() on your socket with a backlog value (e.g., BACKLOG).
    //   • If listen() fails, print an error and exit.
    //   • Optionally, print a message that the server is waiting for connections.
    // ============================================



    struct sockaddr_in client_addr;
    socklen_t client_length = sizeof(client_addr);

    // Get information about the socket
    if (getsockname(sockfd, (struct sockaddr*)&client_addr, &client_length) == 0) 
    {
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
        int port = ntohs(client_addr.sin_port);
        
        std::cout << "[INFO] Server listening on IP: " << ip_str << ", Port: " << port << std::endl;
    } 
    else 
        std::cerr << "[ERROR] getsockname(): " << strerror(errno) << std::endl;

    const int BACKLOG = 1; 
//    std::cout << "[DEBUG] Running listen..." << std::endl;

    int listen_status = listen(sockfd, BACKLOG);

//    std::cout << "[DEBUG] listen_status = " << listen_status << std::endl;
    if(listen_status != 0)
    {
        std::cerr << "[ERROR] listen(): " << strerror(errno) << std::endl;
        return errno;
    }
//    std::cout << "[DEBUG] listen() successful." << std::endl;
    std::cout << "Listening and awaiting a connection..." << std::endl;


    while (true) {
//        std::cout << "[DEBUG] Start of loop." << std::endl;
        // ============================================
        // TODO: Step 4 - Accept a connection:
        // Pseudo code:
        //   • Call accept() to wait for an incoming connection.
        //   • This returns a new socket dedicated to this client.
        //   • Check for errors; if accept() fails, print an error and continue to the next iteration.
        //   • Optionally, print a message that a client has connected.
        // ============================================

        socklen_t client_length = sizeof(client_addr);
        
//        std::cout << "[DEBUG] Awaiting accept()..." << std::endl;
        int client = accept(sockfd, (struct sockaddr*)&client_addr, &client_length);

//        std::cout << "[DEBUG] client = " << client << std::endl;
        //std::cout << "[DEBUG] VALUES\n----------\nsockfd = " << sockfd << "" 
        if( client == -1)
        { // Error
            std::cerr << "[ERROR] accept(): " << strerror(errno) << std::endl; // Print error.
            continue; // Continue to next iter.
        }
//        std::cout << "[DEBUG] accept() successful." << std::endl;

        //If we get here, that means we accepted the connection.
        std::cout << "Connection accepted from: [" << inet_ntoa(client_addr.sin_addr) << ":"<< ntohs(client_addr.sin_port) << "]!" << std::endl;

        // Initialize game state
        char board[ROWS][COLS];
        initBoard(board);
        bool gameOver = false;
        bool clientTurn = true;  // client moves first

        // ============================================
        // TODO: Step 5 - Send the initial board to the client:
        // Pseudo code:
        //   • Use sendBoardAndTurn() with the message "TURN CLIENT" to prompt the client.
        // ============================================
        sendBoardAndTurn(client, board, "TURN CLIENT"); //unsure what to do with the bool value here tbh.
        // Game loop
        while (!gameOver) {
            if (clientTurn) {
                // Wait for the client to send a move command.
                std::string clientMsg = readLine(client);
                if (clientMsg.empty()) {
                    std::cerr << "Client disconnected or error occurred.\n";
                    break;
                }
                std::istringstream iss(clientMsg);
                std::string command;
                int col;
                iss >> command >> col;
                if (command != "MOVE" || col < 1 || col > 7) {
                    writeLine(client, "INVALID_MOVE");
                    sendBoardAndTurn(client, board, "TURN CLIENT");
                    continue;
                }
                int dropRow = dropPiece(board, col - 1, 'C');
                if (dropRow == -1) {
                    writeLine(client, "INVALID_MOVE");
                    sendBoardAndTurn(client, board, "TURN CLIENT");
                    continue;
                }
                std::cout << "Client dropped a piece in column " << col << ".\n";
                if (checkWin(board, dropRow, col - 1, 'C')) {
                    sendBoardAndTurn(client, board, "GAMEOVER CLIENT_WIN");
                    std::cout << "Client wins!\n";
                    gameOver = true;
                } else if (checkTie(board)) {
                    sendBoardAndTurn(client, board, "GAMEOVER TIE");
                    std::cout << "Tie!\n";
                    gameOver = true;
                } else {
                    clientTurn = false;
                }
            } else {
                printBoard(board);
                std::cout << "Your move (1-7): ";
                int col;
                std::cin >> col;
                if (std::cin.fail() || col < 1 || col > 7) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "Invalid input, try again.\n";
                    continue;
                }
                int dropRow = dropPiece(board, col - 1, 'S');
                if (dropRow == -1) {
                    std::cout << "Column full, pick another.\n";
                    continue;
                }
                std::cout << "Server dropped a piece in column " << col << ".\n";
                if (checkWin(board, dropRow, col - 1, 'S')) {
                    sendBoardAndTurn(client, board, "GAMEOVER SERVER_WIN");
                    std::cout << "Server wins!\n";
                    gameOver = true;
                } else if (checkTie(board)) {
                    sendBoardAndTurn(client, board, "GAMEOVER TIE");
                    std::cout << "Tie!\n";
                    gameOver = true;
                } else {
                    clientTurn = true;
                    sendBoardAndTurn(client, board, "TURN CLIENT");
                }
            }
        } // end game loop

        // ============================================
        // TODO: Step 6 - Close the client socket:
        // Pseudo code:
        //   • Call close() on the socket used for the current client.
        // ============================================
        close(client); // Close the socket for the CLIENT, NOT the actual listening socket.
        std::cout << "Game ended. Waiting for next client...\n----------------------------------------------------------------" << std::endl; // Added -- to make visual distinction on termination.
    } // end while true

    // ============================================
    // TODO: Step 7 - Close the listening socket:
    // Pseudo code:
    //   • Call close() on the listening socket.
    // ============================================


    close(sockfd); // Close the listening socket as the program no longer is going to run.
    return 0;
}
