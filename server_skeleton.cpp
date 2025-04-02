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

const int ROWS = 6;
const int COLS = 7;
const int BACKLOG = 1; // Maximum pending connections

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
std::string readLine(int sockfd) {
    // TODO: Implement using recv() in a loop.
    return "";
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
bool writeLine(int sockfd, const std::string &line) {
    // TODO: Implement using send() in a loop.
    return false;
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
bool sendBoardAndTurn(int sockfd, const char board[ROWS][COLS], const std::string &turnMsg) {
    std::string msg = "BOARD\n" + boardToString(board) + turnMsg;
    return writeLine(sockfd, msg);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
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

    // ============================================
    // TODO: Step 3 - Put the socket into listening mode:
    // Pseudo code:
    //   • Call listen() on your socket with a backlog value (e.g., BACKLOG).
    //   • If listen() fails, print an error and exit.
    //   • Optionally, print a message that the server is waiting for connections.
    // ============================================

    while (true) {
        // ============================================
        // TODO: Step 4 - Accept a connection:
        // Pseudo code:
        //   • Call accept() to wait for an incoming connection.
        //   • This returns a new socket dedicated to this client.
        //   • Check for errors; if accept() fails, print an error and continue to the next iteration.
        //   • Optionally, print a message that a client has connected.
        // ============================================

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

        // Game loop
        while (!gameOver) {
            if (clientTurn) {
                // Wait for the client to send a move command.
                std::string clientMsg = readLine(/* client socket descriptor */);
                if (clientMsg.empty()) {
                    std::cerr << "Client disconnected or error occurred.\n";
                    break;
                }
                std::istringstream iss(clientMsg);
                std::string command;
                int col;
                iss >> command >> col;
                if (command != "MOVE" || col < 1 || col > 7) {
                    writeLine(/* client socket descriptor */, "INVALID_MOVE");
                    sendBoardAndTurn(/* client socket descriptor */, board, "TURN CLIENT");
                    continue;
                }
                int dropRow = dropPiece(board, col - 1, 'C');
                if (dropRow == -1) {
                    writeLine(/* client socket descriptor */, "INVALID_MOVE");
                    sendBoardAndTurn(/* client socket descriptor */, board, "TURN CLIENT");
                    continue;
                }
                std::cout << "Client dropped a piece in column " << col << ".\n";
                if (checkWin(board, dropRow, col - 1, 'C')) {
                    sendBoardAndTurn(/* client socket descriptor */, board, "GAMEOVER CLIENT_WIN");
                    std::cout << "Client wins!\n";
                    gameOver = true;
                } else if (checkTie(board)) {
                    sendBoardAndTurn(/* client socket descriptor */, board, "GAMEOVER TIE");
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
                    sendBoardAndTurn(/* client socket descriptor */, board, "GAMEOVER SERVER_WIN");
                    std::cout << "Server wins!\n";
                    gameOver = true;
                } else if (checkTie(board)) {
                    sendBoardAndTurn(/* client socket descriptor */, board, "GAMEOVER TIE");
                    std::cout << "Tie!\n";
                    gameOver = true;
                } else {
                    clientTurn = true;
                    sendBoardAndTurn(/* client socket descriptor */, board, "TURN CLIENT");
                }
            }
        } // end game loop

        // ============================================
        // TODO: Step 6 - Close the client socket:
        // Pseudo code:
        //   • Call close() on the socket used for the current client.
        // ============================================

        std::cout << "Game ended. Waiting for next client...\n";
    } // end while true

    // ============================================
    // TODO: Step 7 - Close the listening socket:
    // Pseudo code:
    //   • Call close() on the listening socket.
    // ============================================

    return 0;
}
