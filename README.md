# Bank Management System

## Overview

This Bank Management System is a console application that allows users to manage client accounts and user accounts. It includes functionalities for adding, updating, deleting, and viewing client and user information, as well as handling transactions.

## Features

- User authentication
- Client management (add, update, delete, view)
- User management (add, update, delete, view)
- Transaction management (deposit, withdraw, view total balances)
- Permission-based access control

## Code Structure

The code is organized into several key components, each responsible for specific functionalities.

### Part 1: Client Management

- **`ShowAllClients()`**: Displays a list of all clients.
- **`AddNewClient()`**: Prompts for and adds a new client.
- **`DeleteClientData()`**: Deletes a client based on username.
- **`UpdateClientData()`**: Updates existing client information.
- **`FindClient()`**: Searches for a client by username and displays their information.

### Part 2: Transaction Management

- **`Deposit()`**: Handles deposit transactions for clients.
- **`Withdraw()`**: Handles withdrawal transactions for clients.
- **`ShowTotalBalances()`**: Displays the total balances of all clients.

### Part 3: User Management

- **`ReadPermissions()`**: Manages user permissions for various actions.
- **`ListUsers()`**: Displays a list of all users.
- **`AddNewUser ()`**: Prompts for and adds a new user.
- **`DeleteUser ()`**: Deletes a user based on username.
- **`UpdateUser Data()`**: Updates existing user information.
- **`FindUser ()`**: Searches for a user by username and displays their information.

### Part 4: User Authentication

- **`Login()`**: Manages user login and authentication.
- **`MainMenuWindow()`**: Displays the main menu and redirects to appropriate functionalities.
- **`TransactionsMenuWindow()`**: Displays the transactions menu and handles transaction-related actions.
- **`ManageUsersMenuWindow()`**: Displays the manage users menu and handles user-related actions.

### Part 5: Data Management

- **`ReadUser DataFromFile()`**: Reads user data from the file.
- **`AddNewUser DataToFile(string UserData)`**: Appends new user data to the user database file.
- **`UpdateUsersFileContent(vector<stUser >& vFileContent)`**: Updates the user database file with current user data.
- **`IsUsernameExists(string Username)`**: Checks if a username already exists in the database.
- **`IsAuthenticated(string Username, string Password)`**: Validates user credentials.

## Usage

1. Compile the code using a C++ compiler.
2. Run the executable.
3. Follow the prompts to log in and manage clients and users.
4. Username: Admin  Password: 1234

## Requirements

- C++11 or later
- Standard library

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thank you to all contributors and libraries that made this project possible.
