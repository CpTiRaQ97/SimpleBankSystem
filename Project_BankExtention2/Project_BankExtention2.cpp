#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>

using namespace std;

// File paths for storing client and user data
string BankDB = "BankDataBase.txt";
string UsersDB = "Users.txt";

// Structure representing a bank client
struct stClient {
    string AccountNumber;  // Unique account number
    string PinCode;        // Client's PIN code for account access
    string Name;           // Client's full name
    string PhoneNumber;    // Contact number
    double AccountBalance; // Account balance
    bool MarkToDelete = false; // Flag to mark client for deletion
};

// Structure representing a system user (e.g., bank staff)
struct stUser {
    string Username, Password; // Login credentials
    int Permission;            // Permission level for actions
    bool MarkToDelete = false; // Flag to mark user for deletion
};

stUser LoggedInUser;
// Enum to define access permissions (bitwise flags)
enum HasAccess {
    ToAllAccess = -1,         // Full access
    ShowClientListAccess = 1, // Access to view client list
    AddNewClientAccess = 2,   // Access to add new clients
    DeleteClientAccess = 4,   // Access to delete clients
    UpdateClientAccess = 8,   // Access to update client info
    FindClinetAccess = 16,    // Access to find a client
    TransactionsAccess = 32,  // Access to transaction-related operations
    ManageUsersAccess = 64    // Access to user management operations
};

// Forward declarations for menu screens and functionalities
void MainMenuWindow();
void TransactionsMenuWindow();
void ManageUsersMenuWindow();
void Login();

// Check if a user has the required permissions
bool HasPermission(stUser User, HasAccess Access) {
    // Grant access if user has all permissions or specific permission
    if (User.Permission == HasAccess::ToAllAccess || (User.Permission & Access) != 0)
        return true;
    return false;
}

// Display a screen separator for UI consistency
void ScreenSeperator() {
    char designChar = '*';
    cout << string(50, designChar) << endl;
}

// Display an "Access Denied" message when a user lacks permissions
void AccessDenied() {
    system("cls"); // Clear the screen
    ScreenSeperator();
    cout << "Access Denied,\nYou Don't Have Permission To Do This,\nContact Your Admin." << endl;
    ScreenSeperator();
}

// Display the title of the current screen
void ScreenTitle(string Title) {
    system("cls"); // Clear the screen
    ScreenSeperator();
    cout << "\t\t" << Title << endl;
    ScreenSeperator();
}

// Display the main menu options
void MainMenuScreen() {
    ScreenTitle("Main Menu Screen");
    cout << "\t(1) Show Client List.\n";
    cout << "\t(2) Add New Client.\n";
    cout << "\t(3) Delete Client.\n";
    cout << "\t(4) Update Client Info.\n";
    cout << "\t(5) Find Clinet.\n";
    cout << "\t(6) Transactions.\n";
    cout << "\t(7) Manage Users.\n";
    cout << "\t(8) Logout.\n";
    ScreenSeperator();
}

// Display the transactions menu options
void TransactionsMenuScreen() {
    ScreenTitle("Transactions Menu Screen");
    cout << "\t(1) Deposit.\n";
    cout << "\t(2) Withdraw.\n";
    cout << "\t(3) Total Balances.\n";
    cout << "\t(4) Main Menu.\n";
    ScreenSeperator();
}

// Display the login screen
void LoginScreen() {
    ScreenTitle("Login Screen");
}

// Display the manage users menu options
void ManageUsersMenuScreen() {
    ScreenTitle("Manage Users Menu");
    cout << "\t(1) List Users.\n";
    cout << "\t(2) Add New User.\n";
    cout << "\t(3) Delete User.\n";
    cout << "\t(4) Update User.\n";
    cout << "\t(5) Find User.\n";
    cout << "\t(6) Main Menu.\n";
    ScreenSeperator();
}

// Validate and read user input within a specific range
int ReadUserInput(string Text, int From, int To) {
    int Number;
    do {
        cout << Text;
        cin >> Number;
        while (cin.fail()) { // Handle invalid input
            cin.clear(); // Clear error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            cout << "Wrong Input Type, Enter a valid one:" << endl;
            cin >> Number;
        }
        if (Number < From || Number > To)
            cout << "\nInsert a valid response number within the range:\n";
    } while (Number < From || Number > To);
    return Number;
}

// Prompt user to enter an account number
string ReadAccountNumber() {
    string AccountNumber = "";
    cout << "Insert the Account Number: ";
    getline(cin >> ws, AccountNumber); // Read input while skipping leading whitespace
    return AccountNumber;
}

// Collect updated data for a client
void ReadNewDataForClient(stClient& Client) {
    cout << "Please Update Client Data: \n\n";
    cout << "PIN Code? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Name? ";
    getline(cin, Client.Name);
    cout << "Phone? ";
    getline(cin, Client.PhoneNumber);
    cout << "Account Balance? ";
    cin >> Client.AccountBalance;
    cout << endl;
}

// Display the header for the client list
void ShowHeader(int NumberOfClients) {
    for (short i = 0; i < 6; i++)
        cout << "\t";
    cout << "Client List " << NumberOfClients << " Client(s)." << endl;
    cout << string(120, '-') << "\n" << endl;
    cout << "| Account Number " << setw(20);
    cout << "| Pin Code " << setw(20);
    cout << "| Client Name " << setw(40);
    cout << "| Phone " << setw(20);
    cout << "| Balance " << setw(20);
    cout << "\n" << string(120, '-') << "\n" << endl;
}

// Display the header for the balances list
void ShowHeaderBalances(int NumberOfClients) {
    for (short i = 0; i < 6; i++)
        cout << "\t";
    cout << "Client List " << NumberOfClients << " Client(s)." << endl;
    cout << string(120, '-') << "\n" << endl;
    cout << left;
    cout << "| " << setw(25) << "Account Number";
    cout << "| " << setw(40) << "Client Name";
    cout << "| " << setw(25) << "Balance";
    cout << "\n" << string(120, '-') << "\n" << endl;
}

// Display the header for the users list
void UsersListHeader(short NumberOfUsers) {
    for (short i = 0; i < 6; i++)
        cout << "\t";
    cout << "Users List " << NumberOfUsers << " User(s)." << endl;
    cout << string(120, '-') << "\n" << endl;
    cout << left;
    cout << "| " << setw(25) << "Username";
    cout << "| " << setw(40) << "Password";
    cout << "| " << setw(25) << "Permissions";
    cout << "\n" << string(120, '-') << "\n" << endl;
}

// Utility to split a string into parts based on a delimiter
vector<string> SplitFunction(string Text, string Delim) {
    vector<string> SplitWords;
    int Pos;
    string TempWord;
    while ((Pos = Text.find(Delim)) != std::string::npos) {
        TempWord = Text.substr(0, Pos);
        if (TempWord != "") SplitWords.push_back(TempWord);
        Text.erase(0, Pos + Delim.length());
    }
    if (Text != "") SplitWords.push_back(Text); // Push remaining part
    return SplitWords;
}

// Convert a line from the file into a stClient object
stClient ConvertClientLinetoRecord(string Line, string Delim) {
    stClient Client;
    vector<string> vClientData = SplitFunction(Line, Delim);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.PhoneNumber = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]); // Convert balance to double
    return Client;
}

// Convert a stClient object into a string for saving to the file
string ClientDataToLine(stClient Client, string Delim) {
    string DataLine = "";
    DataLine += Client.AccountNumber + Delim;
    DataLine += Client.PinCode + Delim;
    DataLine += Client.Name + Delim;
    DataLine += Client.PhoneNumber + Delim;
    DataLine += to_string(Client.AccountBalance); // Convert balance to string
    return DataLine;
}

// Read all client data from the file
vector<stClient> ReadClientDataFromFile() {
    vector<stClient> vFileContent;
    fstream ClientFile;
    ClientFile.open(BankDB, ios::in);
    if (ClientFile.is_open()) {
        string FileLine;
        stClient Client;
        while (getline(ClientFile, FileLine)) {
            Client = ConvertClientLinetoRecord(FileLine, "##");
            vFileContent.push_back(Client);
        }
        ClientFile.close();
    }
    return vFileContent;
}

// Check if an account number already exists in the database
bool isAccountNumberExists(string UserAccountNumber) {
    vector<stClient> Clients = ReadClientDataFromFile();
    for (stClient& Client : Clients)
        if (Client.AccountNumber == UserAccountNumber) return true;
    return false;
}

// Print information about a single client
void PrintClientInfo(stClient Client) {
    cout << left;
    cout << "| " << setw(24) << Client.AccountNumber;
    cout << "| " << setw(15) << Client.PinCode;
    cout << "| " << setw(44) << Client.Name;
    cout << "| " << setw(16) << Client.PhoneNumber;
    cout << "| " << setw(12) << Client.AccountBalance;
    cout << endl;
}

// Print the balance details of a single client
void PrintClientBalance(stClient Client) {
    cout << left;
    cout << "| " << setw(25) << Client.AccountNumber;
    cout << "| " << setw(40) << Client.Name;
    cout << "| " << setw(25) << Client.AccountBalance;
    cout << endl;
}

// Print information about a single user
void PrintUserInfo(stUser User) {
    cout << left;
    cout << "| " << setw(25) << User.Username;
    cout << "| " << setw(40) << User.Password;
    cout << "| " << setw(25) << to_string(User.Permission);
    cout << endl;
}

// Display all clients from the database
void ShowAllClients() {
    system("cls");
    if (HasPermission(LoggedInUser, HasAccess::ShowClientListAccess)) {
        vector<stClient> vFileContent = ReadClientDataFromFile();
        ShowHeader(vFileContent.size());
        for (stClient& Client : vFileContent)
            PrintClientInfo(Client);
        cout << string(120, '-') << "\n" << endl;
    }
    else
        AccessDenied();
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    MainMenuWindow();
}

// Read data for a new client from user input
void ReadClientData(stClient& Client) {
    cout << "Adding New Client: \n\n";
    cout << "Account Number? ";
    getline(cin >> ws, Client.AccountNumber);
    cout << "PIN Code? ";
    getline(cin, Client.PinCode);
    cout << "Name? ";
    getline(cin, Client.Name);
    cout << "Phone? ";
    getline(cin, Client.PhoneNumber);
    cout << "Account Balance? ";
    cin >> Client.AccountBalance;
    cout << endl;
}

// Append new client data to the file
void AddNewClientDataToFile(string ClientData) {
    fstream ClientFile;
    ClientFile.open(BankDB, ios::out | ios::app); // Open file in append mode
    if (ClientFile.is_open()) {
        ClientFile << ClientData << endl;
        ClientFile.close();
    }
}

void AddNewClient()
{
    // Checks if the user has permission to add new clients.
    if (HasPermission(LoggedInUser, HasAccess::AddNewClientAccess))
    {
        ScreenTitle("Add New Client");
        bool WantToContinue = false;
        string UserAnswer;

        // Loop to allow adding multiple clients in one session.
        do
        {
            stClient Client;
            ReadClientData(Client);

            // Validates if the account number is unique.
            if (!isAccountNumberExists(Client.AccountNumber))
            {
                string ClientData = ClientDataToLine(Client, "##");
                AddNewClientDataToFile(ClientData); // Saves client data to file.
                cout << "Client Added Successfully :)\n";
            }
            else
            {
                cout << "This Account Number already exists, choose another account number.\n";
            }

            // Prompt user to decide whether to add another client.
            cout << "Do you want to add a new Client? (Yes or No)" << endl;
            cin >> UserAnswer;

            // Determine whether to continue based on user input.
            (UserAnswer == "Yes" || UserAnswer == "yes") ? WantToContinue = true : WantToContinue = false;
            cout << endl;

        } while (WantToContinue); // Repeat until the user chooses to stop.
    }
    else
        AccessDenied(); // Display access denied if the user lacks permission.

    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    MainMenuWindow(); // Redirect to the main menu.
}

void PrintClientData(stClient Client)
{
    // Displays client details in a readable format.
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PIN Code       : " << Client.PinCode << endl; // Consider masking PIN for security.
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.PhoneNumber << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
}

void ShowClientByAccountNumber(string UserAccountNumber)
{
    // Reads all clients from the file.
    vector <stClient> vFileContent = ReadClientDataFromFile();

    // Checks if the account number exists.
    if (isAccountNumberExists(UserAccountNumber))
    {
        // Loops through all clients to find a match.
        for (stClient& Client : vFileContent)
            if (Client.AccountNumber == UserAccountNumber)
                PrintClientData(Client); // Displays client data if found.
    }
    else
    {
        // Displays a message if the client is not found.
        cout << "Client with Account Number <" << UserAccountNumber << "> Not Found.";
    }
}

void UpdateClientsFileContent(vector <stClient>& vFileContent)
{
    // Opens the file for writing to update client records.
    fstream ClientFile;
    ClientFile.open(BankDB, ios::out);

    if (ClientFile.is_open())
    {
        // Writes all client data to the file, excluding those marked for deletion.
        for (stClient Client : vFileContent)
        {
            if (Client.MarkToDelete == false)
            {
                string Line = ClientDataToLine(Client, "##");
                ClientFile << Line << endl;
            }
        }
        ClientFile.close();
    }
}

void DeleteClientData()
{
    // Checks if the user has permission to delete clients.
    if (HasPermission(LoggedInUser, HasAccess::DeleteClientAccess))
    {
        ScreenTitle("Delete Client Account");
        vector <stClient> vFileContent = ReadClientDataFromFile();
        string UserAccountNumber = ReadAccountNumber(); // Reads the account number to delete.

        ShowClientByAccountNumber(UserAccountNumber); // Displays client details for confirmation.

        // Confirms if the account exists.
        if (isAccountNumberExists(UserAccountNumber))
        {
            char UserDeleteResponse;
            cout << "Do you want to delete this Account? (Y/N)\n";
            cin >> UserDeleteResponse;

            // If the user confirms deletion.
            if (tolower(UserDeleteResponse) == 'y')
            {
                // Marks the account for deletion and updates the file.
                for (stClient& Client : vFileContent)
                    if (Client.AccountNumber == UserAccountNumber)
                    {
                        Client.MarkToDelete = true;
                        UpdateClientsFileContent(vFileContent);
                        cout << "Account with account number " << UserAccountNumber << " has been deleted successfully :)" << endl;
                    }
            }
        }
    }
    else
        AccessDenied(); // Display access denied if the user lacks permission.

    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    MainMenuWindow(); // Redirect to the main menu.
}

void UpdaeClientData()
{
    // Checks if the user has permission to update client information.
    if (HasPermission(LoggedInUser, HasAccess::UpdateClientAccess))
    {
        ScreenTitle("Update Client Info");
        vector <stClient> vFileContent = ReadClientDataFromFile();
        string UserAccountNumber = ReadAccountNumber(); // Reads the account number to update.

        ShowClientByAccountNumber(UserAccountNumber); // Displays client details for confirmation.

        // Confirms if the account exists.
        if (isAccountNumberExists(UserAccountNumber))
        {
            char UserEditResponse;
            cout << "Do you want to update this Account? (Y/N)\n";
            cin >> UserEditResponse;

            // If the user confirms the update.
            if (tolower(UserEditResponse) == 'y')
            {
                for (stClient& Client : vFileContent)
                    if (Client.AccountNumber == UserAccountNumber)
                    {
                        ReadNewDataForClient(Client); // Reads updated data for the client.
                        UpdateClientsFileContent(vFileContent); // Updates the client data in the file.
                        cout << "Account with account number " << UserAccountNumber << " has been updated successfully :)" << endl;
                    }
            }
        }
    }
    else
        AccessDenied(); // Display access denied if the user lacks permission.

    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    MainMenuWindow(); // Redirect to the main menu.
}

void FindClient()
{
    // Checks if the user has permission to find clients.
    if (HasPermission(LoggedInUser, HasAccess::FindClinetAccess))
    {
        ScreenTitle("Find Client");
        string UserAccountNumber = ReadAccountNumber(); // Reads the account number to find.
        ShowClientByAccountNumber(UserAccountNumber); // Displays client data if found.
    }
    else
        AccessDenied(); // Display access denied if the user lacks permission.

    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    MainMenuWindow(); // Redirect to the main menu.
}

bool EditBalance(string AccountNumber, double Amount)
{
    // Reads all client data from the file.
    vector<stClient> vFileContent = ReadClientDataFromFile();

    // Loops through the clients to find the matching account number.
    for (stClient& Client : vFileContent)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            // Checks if the balance after the transaction remains valid.
            if ((Client.AccountBalance + Amount) >= 0)
            {
                Client.AccountBalance += Amount; // Updates the account balance.
                UpdateClientsFileContent(vFileContent); // Saves updated data back to the file.
                return true; // Transaction successful.
            }
            else
            {
                // Insufficient balance for the transaction.
                cout << "Not sufficient amount, transaction aborted, please retry again later.\n";
                cout << "\nPlease Note:\n1) There are no limits for deposit amount.\n2) Max amount for withdrawal is ";
                cout << Client.AccountBalance; // Displays the current balance as the withdrawal limit.
                return false; // Transaction failed.
            }
        }
    }
}

void Deposit()
{
    ScreenTitle("Deposit"); // Displays the deposit screen title.
    string UserAccountNumber = ReadAccountNumber(); // Reads the account number.
    ShowClientByAccountNumber(UserAccountNumber); // Shows the client details for verification.

    double DepositAmount;
    cout << "\nPlease enter deposit amount? ";
    cin >> DepositAmount; // Reads the deposit amount from the user.

    char UserAnswer;
    cout << "Are you sure you want to perform this transaction? (Y/N) ";
    cin >> UserAnswer;

    // Processes the deposit if the user confirms.
    if (tolower(UserAnswer) == 'y')
    {
        if (EditBalance(UserAccountNumber, DepositAmount))
            cout << "Amount was added successfully.\n"; // Confirms successful deposit.
    }

    // Returns to the transactions menu.
    cout << "\n\nPress any key to go back to Transactions Menu...";
    system("pause>0");
    TransactionsMenuWindow();
}

void Withdraw()
{
    ScreenTitle("Withdraw"); // Displays the withdrawal screen title.
    string UserAccountNumber = ReadAccountNumber(); // Reads the account number.
    ShowClientByAccountNumber(UserAccountNumber); // Shows the client details for verification.

    double WithdrawAmount;
    cout << "\nPlease enter withdrawal amount? ";
    cin >> WithdrawAmount;
    WithdrawAmount *= -1; // Converts the amount to a negative value for withdrawal.

    char UserAnswer;
    cout << "Are you sure you want to perform this transaction? (Y/N) ";
    cin >> UserAnswer;

    // Processes the withdrawal if the user confirms.
    if (tolower(UserAnswer) == 'y')
    {
        if (EditBalance(UserAccountNumber, WithdrawAmount))
            cout << "Amount was withdrawn successfully.\n"; // Confirms successful withdrawal.
    }

    // Returns to the transactions menu.
    cout << "\n\nPress any key to go back to Transactions Menu...";
    system("pause>0");
    TransactionsMenuWindow();
}

void ShowTotalBalances()
{
    system("cls"); // Clears the screen.
    double TotalBalances = 0; // Initializes total balance accumulator.

    vector<stClient> vFileContent = ReadClientDataFromFile(); // Reads all client data from the file.
    ShowHeaderBalances(vFileContent.size()); // Displays a header showing the number of clients.

    // Loops through all clients and prints their balance.
    for (stClient& Client : vFileContent)
    {
        PrintClientBalance(Client); // Prints client balance details.
        TotalBalances += Client.AccountBalance; // Adds the balance to the total.
    }

    // Displays the total balance for all clients.
    cout << string(120, '-') << "\n" << endl;
    for (short i = 0; i < 6; i++)
        cout << "\t";
    cout << "Balances Total: " << TotalBalances;

    // Returns to the main menu.
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    TransactionsMenuWindow();
}

int ReadPermissions()
{
    char UserInput = ' ';
    short Accesses = 0;

    // Checks if full access should be granted.
    cout << "\nDo you want to give full access? y/n?";
    cin >> UserInput;

    if (tolower(UserInput) == 'y')
        return -1; // Full access granted.
    else
    {
        // Asks the user to enable individual permissions.
        cout << "\nDo you want to:";
        cout << "\nShow Client List? y/n?";
        cin >> UserInput;
        if (tolower(UserInput) == 'y')
            Accesses += HasAccess::ShowClientListAccess;

        cout << "\nAdd New Client? y/n?";
        cin >> UserInput;
        if (tolower(UserInput) == 'y')
            Accesses += HasAccess::AddNewClientAccess;

        cout << "\nDelete Client? y/n?";
        cin >> UserInput;
        if (tolower(UserInput) == 'y')
            Accesses += HasAccess::DeleteClientAccess;

        cout << "\nUpdate Client Info? y/n?";
        cin >> UserInput;
        if (tolower(UserInput) == 'y')
            Accesses += HasAccess::UpdateClientAccess;

        cout << "\nFind Client? y/n?";
        cin >> UserInput;
        if (tolower(UserInput) == 'y')
            Accesses += HasAccess::FindClinetAccess;

        cout << "\nTransactions? y/n?";
        cin >> UserInput;
        if (tolower(UserInput) == 'y')
            Accesses += HasAccess::TransactionsAccess;

        cout << "\nManage Users? y/n?";
        cin >> UserInput;
        if (tolower(UserInput) == 'y')
            Accesses += HasAccess::ManageUsersAccess;
    }

    return Accesses; // Returns the permissions as a combined value.
}

stUser ConvertUserLinetoRecord(string Line, string Delim)
{
    stUser User;
    vector<string> vUserData;

    // Splits the user data line into individual fields.
    vUserData = SplitFunction(Line, Delim);

    // Populates the user structure with parsed data.
    User.Username = vUserData[0];
    User.Password = vUserData[1];
    User.Permission = stoi(vUserData[2]);

    return User;
}

vector<stUser> ReadUserDataFromFile()
{
    vector<stUser> vFileContent;
    fstream UserFile;

    UserFile.open(UsersDB, ios::in); // Opens the user data file in read mode.

    if (UserFile.is_open())
    {
        string FileLine;
        stUser User;

        // Reads each line and converts it to a user record.
        while (getline(UserFile, FileLine))
        {
            User = ConvertUserLinetoRecord(FileLine, "##");
            vFileContent.push_back(User); // Adds the user to the vector.
        }

        UserFile.close(); // Closes the file after reading.
    }

    return vFileContent; // Returns all user records as a vector.
}

bool IsUsernameExists(string Username)
{
    // Reads all users from the file.
    vector<stUser> Users = ReadUserDataFromFile();

    // Checks if the given username matches any existing username.
    for (stUser& User : Users)
        if (User.Username == Username)
            return true; // Username exists.
    return false; // Username does not exist.
}

bool IsAuthenticated(string Username, string Password)
{
    // Reads all users from the file.
    vector<stUser> Users = ReadUserDataFromFile();

    // Checks if both username and password match any user.
    for (stUser& User : Users)
        if (User.Username == Username && User.Password == Password)
            return true; // Authentication successful.
    return false; // Authentication failed.
}

void ListUsers()
{
    system("cls"); // Clears the screen.

    // Reads all users from the file.
    vector<stUser> vFileContent = ReadUserDataFromFile();

    // Displays the header with the number of users.
    UsersListHeader(vFileContent.size());

    // Loops through all users and prints their information.
    for (stUser& User : vFileContent)
        PrintUserInfo(User);

    cout << string(120, '-') << "\n" << endl;

    // Waits for user input to return to the Manage Users menu.
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ManageUsersMenuWindow();
}

void ReadUserData(stUser& User)
{
    cout << "Adding New User: \n\n";

    // Prompts the user for input and reads the new user's data.
    cout << "Username? ";
    getline(cin >> ws, User.Username);
    cout << "Password? ";
    getline(cin, User.Password);
    cout << "Permission? ";
    User.Permission = ReadPermissions(); // Assigns permissions.
    cout << endl;
}

void ReadNewDataForUser(stUser& User)
{
    cout << "Please Update Client Data: \n\n";

    // Prompts the user for input and reads the updated user's data.
    cout << "Password? ";
    getline(cin >> ws, User.Password);
    cout << "Permission? ";
    User.Permission = ReadPermissions(); // Updates permissions.
    cout << endl;
}

string UserDataToLine(stUser User, string Delim)
{
    // Converts user data to a single delimited line for file storage.
    string DataLine = "";
    DataLine += User.Username + Delim;
    DataLine += User.Password + Delim;
    DataLine += to_string(User.Permission);
    return DataLine;
}

void AddNewUserDataToFile(string UserData)
{
    fstream UserFile;

    // Opens the file in append mode and writes the new user data.
    UserFile.open(UsersDB, ios::out | ios::app);
    if (UserFile.is_open())
    {
        UserFile << UserData << endl;
        UserFile.close();
    }
}

void AddNewUser()
{
    ScreenTitle("Add New User"); // Displays the title for adding a new user.
    bool WantToContinue = false;
    string UserAnswer;

    do
    {
        stUser User;
        ReadUserData(User); // Reads user input for new user data.

        // Checks if the username already exists.
        if (!IsUsernameExists(User.Username))
        {
            string UserData = UserDataToLine(User, "##");
            AddNewUserDataToFile(UserData); // Adds the new user to the file.
            cout << "User Added Successfully :)\n";
        }
        else
        {
            // Username already exists; prompts for a different one.
            cout << "This Username already exists, choose another Username.\n";
        }

        // Asks if the user wants to add another user.
        cout << "Do you want to add a new User? (Yes or No)" << endl;
        cin >> UserAnswer;
        WantToContinue = (UserAnswer == "Yes" || UserAnswer == "yes");
        cout << endl;

    } while (WantToContinue);

    // Waits for user input to return to the Manage Users menu.
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ManageUsersMenuWindow();
}

string ReadUsername()
{
    string Username = "";

    // Prompts the user for a username.
    cout << "Insert Username: ";
    getline(cin >> ws, Username);

    return Username; // Returns the entered username.
}

void PrintUserData(stUser User)
{
    // Displays user data in a readable format.
    cout << "Username  : " << User.Username << endl;
    cout << "Password  : " << User.Password << endl;
    cout << "Permission: " << to_string(User.Permission) << endl;
}

void ShowUserByUsername(string Username)
{
    // Reads all users from the file.
    vector<stUser> vFileContent = ReadUserDataFromFile();

    // Checks if the username exists.
    if (IsUsernameExists(Username))
    {
        // Finds the user and prints their data.
        for (stUser& User : vFileContent)
            if (User.Username == Username)
                PrintUserData(User);
    }
    else
    {
        // Informs the user if the username was not found.
        cout << "User with Username <" << Username << "> Not Found.";
    }
}

stUser GetUserByUsername(string Username)
{
    // Reads all users from the file.
    vector<stUser> vFileContent = ReadUserDataFromFile();

    // Checks if the username exists.
    if (IsUsernameExists(Username))
    {
        // Finds and returns the user data.
        for (stUser& User : vFileContent)
            if (User.Username == Username)
                return User;
    }
    else
    {
        // Informs the user if the username was not found.
        cout << "User with Username <" << Username << "> Not Found.";
    }
}

void UpdateUsersFileContent(vector<stUser>& vFileContent)
{
    fstream UserFile;

    // Opens the file in write mode to overwrite existing content.
    UserFile.open(UsersDB, ios::out);
    if (UserFile.is_open())
    {
        // Writes all users to the file, skipping those marked for deletion.
        for (stUser User : vFileContent)
        {
            if (User.MarkToDelete == false)
            {
                string Line = UserDataToLine(User, "##");
                UserFile << Line << endl;
            }
        }

        UserFile.close(); // Closes the file.
    }
}
void DeleteUser()
{
    ScreenTitle("Delete User"); // Display the delete user screen title.

    // Read all users from the file.
    vector<stUser> vFileContent = ReadUserDataFromFile();

    // Prompt the admin to input the username of the user to delete.
    string Username = ReadUsername();

    // Prevent deletion of the "Admin" account.
    if (Username == "Admin")
        cout << "You can not delete this User" << endl;
    else
    {
        ShowUserByUsername(Username); // Display the details of the selected user.
        char UserDeleteResponse;

        // Confirm if the admin wants to delete the account.
        cout << "Do you want to delete this Account? (Y/N)\n";
        cin >> UserDeleteResponse;

        if (tolower(UserDeleteResponse) == 'y')
        {
            // Mark the user for deletion and update the file content.
            for (stUser& User : vFileContent)
                if (User.Username == Username)
                {
                    User.MarkToDelete = true;
                    UpdateUsersFileContent(vFileContent); // Save changes to the file.
                    cout << "User with Username " << Username << " has been deleted successfully :)" << endl;
                }
        }
    }

    // Wait for user input to return to the Manage Users menu.
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ManageUsersMenuWindow();
}

void UpdaeUserData()
{
    ScreenTitle("Update User Info"); // Display the update user info screen title.

    // Read all users from the file.
    vector<stUser> vFileContent = ReadUserDataFromFile();

    // Prompt the admin to input the username of the user to update.
    string Username = ReadUsername();

    ShowUserByUsername(Username); // Display the details of the selected user.
    char UserEditResponse;

    // Confirm if the admin wants to update the account.
    cout << "Do you want to update this Account? (Y/N)\n";
    cin >> UserEditResponse;

    if (tolower(UserEditResponse) == 'y')
    {
        // Update user information and save changes to the file.
        for (stUser& User : vFileContent)
            if (User.Username == Username)
            {
                ReadNewDataForUser(User); // Prompt for new user data.
                UpdateUsersFileContent(vFileContent); // Save changes to the file.
                cout << "User With username " << Username << " has been updated successfully :)" << endl;
            }
    }

    // Wait for user input to return to the Manage Users menu.
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ManageUsersMenuWindow();
}

void FindUser()
{
    ScreenTitle("Find User"); // Display the find user screen title.

    // Prompt the admin to input the username of the user to find.
    string Username = ReadUsername();

    // Display the details of the found user or indicate not found.
    ShowUserByUsername(Username);

    // Wait for user input to return to the Manage Users menu.
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ManageUsersMenuWindow();
}

int main()
{
    Login(); // Start the application with the login function.
    return 0;
}

void Login()
{
    LoginScreen(); // Display the login screen.
    string Username, Password;

    do
    {
        // Prompt the user for username and password.
        cout << "Enter username: ";
        getline(cin >> ws, Username);
        cout << "Enter password: ";
        getline(cin, Password);

        // Authenticate the user.
        if (IsAuthenticated(Username, Password))
        {
            LoggedInUser = GetUserByUsername(Username); // Retrieve the authenticated user's data.
            MainMenuWindow(); // Redirect to the main menu.
        }
        else
        {
            // Clear the screen and inform the user of an invalid login.
            system("cls");
            LoginScreen();
            cout << "Invalid username or password. Please try again." << endl;
        }
    } while (!IsAuthenticated(Username, Password)); // Repeat until login is successful.
}

void MainMenuWindow()
{
    MainMenuScreen(); // Display the main menu screen.

    // Prompt the user to select an option from the main menu.
    int UserInputChoice = ReadUserInput("Choose what do you want to do? [1 to 8]? ", 1, 8);

    // Redirect to the appropriate menu or action based on the choice.
    switch (UserInputChoice)
    {
    case 1: ShowAllClients(); break;
    case 2: AddNewClient(); break;
    case 3: DeleteClientData(); break;
    case 4: UpdaeClientData(); break;
    case 5: FindClient(); break;
    case 6: TransactionsMenuWindow(); break;
    case 7: ManageUsersMenuWindow(); break;
    case 8: Login(); break; // Log out and return to the login screen.
    }
}

void TransactionsMenuWindow()
{
    // Check if the user has permissions to access the transactions menu.
    if (HasPermission(LoggedInUser, HasAccess::TransactionsAccess))
    {
        TransactionsMenuScreen(); // Display the transactions menu screen.

        // Prompt the user to select an option from the transactions menu.
        int UserInputChoice = ReadUserInput("Choose what do you want to do? [1 to 4]? ", 1, 4);

        // Redirect to the appropriate transaction action based on the choice.
        switch (UserInputChoice)
        {
        case 1: Deposit(); break;
        case 2: Withdraw(); break;
        case 3: ShowTotalBalances(); break;
        case 4: MainMenuWindow(); break; // Return to the main menu.
        }
    }
    else
        AccessDenied(); // Display access denied message if the user lacks permissions.
}

void ManageUsersMenuWindow()
{
    // Check if the user has permissions to access the manage users menu.
    if (HasPermission(LoggedInUser, HasAccess::ManageUsersAccess))
    {
        ManageUsersMenuScreen(); // Display the manage users menu screen.

        // Prompt the user to select an option from the manage users menu.
        int UserInputChoice = ReadUserInput("Choose what do you want to do? [1 to 6]? ", 1, 6);

        // Redirect to the appropriate user management action based on the choice.
        switch (UserInputChoice)
        {
        case 1: ListUsers(); break;
        case 2: AddNewUser(); break;
        case 3: DeleteUser(); break;
        case 4: UpdaeUserData(); break;
        case 5: FindUser(); break;
        case 6: MainMenuWindow(); break; // Return to the main menu.
        }
    }
    else
        AccessDenied(); // Display access denied message if the user lacks permissions.

    // Wait for user input to return to the main menu.
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    MainMenuWindow();
}
