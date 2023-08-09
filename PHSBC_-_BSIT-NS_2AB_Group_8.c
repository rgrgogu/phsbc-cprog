/**     PROGRAM NAME:           Philippine Social Bank and Commerce [ATM SYSTEM]
 *
 *      Description:            An ATM System that is built for different transaction involved in Automated Teller Machine.
 *                              Program Options:
 *                                  - Balance Inquiry
 *                                  - Cash Deposit
 *                                  - Cash Withdrawal
 *                                  - Transaction History
 *                                  - Change PIN
 *
 *      Data Structure Used:    Binary Search Tree Operations:
 *                              o	getNewNode (creating Nodes by scanning Account Numbers)
 *                              o	insert (inserting nodes into binary tree)
 *                              o	search (searching the nodes that we create from insert)
 *                              o	checkNewNode (if Account Number is existing or not)
 *                              Linked List Operations:
 *                              o	saveHistory (if Deposit or Withdraw has been successfully transacted, it will save).
 *                              o	showHistory (printing the history of user’s transaction).
 *
 *      Programmed by:  ESPINOSA, John Rollyver Q.
 *                      NUGUID, Andrei V.
 *                      OBSEQUIO, Russell A.
 *                      ZEPEDA, Micklare Angelo C.
 *
 *      Section:        BSIT-NS-2AB
 */

/**< PREPROCESSOR DIRECTIVES */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>

// A structure type for User Details
typedef struct userDetailsATM{
    long int accountNumber;
    char firstname[50];
    char middlename[50];
    char lastname[50];
    char birthday[50];
    char address[100];
    int pinNum;
    double amount;
}userDetailsATM;

// A Binary Tree type for searching Account Numbers
typedef struct node{
    long int key;
    struct node *left, *right;
}node;

// A Linked List type for Transaction History (Singly-Linked List)
typedef struct transac{
    char transaction[50];
    char date[50];
    char time[50];
    double balance;
    double transacAmount;
    double totalBalance;
    struct transac *next;
}transac;

/// GLOBAL VARIABLE DECLARATIONS
userDetailsATM user;            // declare the structure variable user to access the userDetailsATM structure tag
char fullFileName[MAX_PATH];    // string variable that holds the full File Path of the created project folder in this program.
char fpRAN[50];                 // string variable that holds the file path of "Registered Account Numbers.txt" located at ATM Database Folder.
int total = 0;                  // variable for total amount of user's deposit.

/// FUNCTION DECLARATIONS

/**< DESIGN Functions */
void gotoxy(int x, int y);      // function that places cursor at a desired location on screen
void intro();                   // function for moving borders in front face program
void frontface();               // function for moving text in front face program
void loading();                 // function for loading bar screen
void maxBorder(int x, int y);   // function for border in console
void colorCode(int ForeGroundColor,int BackGroundColor);    // function for coloring the text

/**< UTILITY Functions (Flash Drives, Loading Data, and Folder) */
int scanFlashDrive();           // function for scanning flash drive and the .txt file if it is exist or not.
void loadDataFromFile();        // function for checking the flash drive (.txt file) its contents.
void createFolder();            // function for creating the Folder in designated project program namely "ATM Database"

/**< MENU Function */
void mainMenuATM(node *valid, node *root);  // function for Menu Options in ATM System

/**< BALANCE INQUIRY Function */
void balanceInquiry();          // function for checking the balance of the user's ATM.

/**< DEPOSIT Functions */
bool depPin();                  // function for asking the user's PIN before it deposits.
void depositMenu();             // function for cash denominations and user's desired input to enter from the bills displayed [1000,500,200,100,50,20]
void deposit(transac **head);   // function for checking the user's deposit amount if it is considered in the limits in the ATM system.
void depositReceipt();          // function for printing the receipt of user's deposit transaction.

/**< WITHDRAWAL Functions */
void withdrawal();                                      // function for asking's the user's amount to withdraw
void pinPanel(int withdraw, transac **head);            // function for asking the user's PIN before it withdraws.
void saveTransaction(int withdraw, transac **head);     // function if the user did not choose the print receipt in withdraw transaction.
void printReceipt(int withdraw, transac **head);        // function if the user choose the print receipt in withdraw transaction.

/**< TRANSACTION HISTORY Functions */
void saveHistory(transac **head, int number, char *str, double oldBalance, double newBalance);  // function for saving the User's transaction today
void showHistory(transac **head);     // function for showing the user's transaction today in ATM.

/**< CHANGE PIN Function */
void changePass();      // function for changing the PIN of user's ATM.

/**< STRUCTURE Functions */

/** NAME:           *getNewNode
 *  DESCRIPTION:    A utility function to create a new BST node
 *  PARAMETER:      @val = Account Number Value from (Registered Accounts.txt)
 */

node *getNewNode(long int val){
    struct node *newNode = malloc(sizeof(node));
    newNode->key = val;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
};

/** NAME:           *insert
 *  DESCRIPTION:    A utility function to insert a new node with given val in BST
 *  PARAMETER:      @val = Account Number Value from (Registered Accounts.txt)
 *                  @root = a pointer node variable.
 */

node *insert(node *root, long int val){
    // If the tree is empty, return a new node
    if(root == NULL)
        return getNewNode(val);

    // Otherwise, recur down the tree
    if(root->key < val){
        root->right = insert(root->right,val);
    }
    else if(root->key > val){
        root->left = insert(root->left,val);
    }

    // return the (unchanged) node pointer //
    return root;
};

/** NAME:           *search
 *  DESCRIPTION:    A utility function to search a given key in a given BST
 *  PARAMETER:      @key = Account Number Value from (Registered Accounts.txt)
 *                  @root = a pointer node variable.
 */

node *search(node *root, long int key){
    // Base Cases: root is null or key is present at root
    if(root == NULL || root->key == key)
        return root;

    // Key is greater than root's key
    if(root->key < key)
        return search(root->right, key);

    // Key is smaller than root's key
    return search(root->left, key);
};

/** NAME:           *checkAccountNumber
 *  DESCRIPTION:    A utility function to check the account Number if it is exist or not in a given BST
 *  PARAMETER:      @root = a pointer node variable.
 */

node *checkAccountNumber(node* root){
    FILE* accNo;
    FILE* userAcc;
    long int value;
    long int userAccNo;

    accNo = fopen(fpRAN, "r");                          // opening file path of "Registered Account Numbers.txt"
    userAcc = fopen("E:\\UserAccount.txt", "r");        // opening file path of "UserAccount.txt"

    if(accNo == NULL){                          // If accNo file is empty
        Sleep(200);
        colorCode(11,0); printf("PHSBC_BOT:");
        colorCode(14,0); printf("~$");
        colorCode(12,0); printf(" Your Account Details is Empty on your ATM Card! Please report to your nearest PHSBC Bank!");
        Sleep(1000);
        gotoxy(0,1);
        colorCode(11,0); printf("PHSBC_BOT:");
        colorCode(14,0); printf("~$");
        colorCode(12,0); printf(" Please eject your Flash Drive!");

        Sleep(5000);
    }
    else{
        while(!feof(accNo)){                    // if file doesn't meet the end of file of accNo
            fscanf(accNo, "%ld", &value);       // it scans and pass into the value variable
            root = insert(root,value);          // then it calls the insert function and the return will received by root.
        }
    }

    fscanf(userAcc, "%ld", &userAccNo);         // Then, it will scan again in the file and save into userAccNo variable.

    if(!(search(root, userAccNo))){             // If the user's account number isn't find in the created BST of account Numbers, error will apply and return NULL
        Sleep(200);
        colorCode(11,0); printf("PHSBC_BOT:");
        colorCode(14,0); printf("~$");
        colorCode(12,0); printf(" Your Account Number is not registered in our database. Please report to your nearest PHSBC Bank!");
        Sleep(1000);
        gotoxy(0,1);
        colorCode(11,0); printf("PHSBC_BOT:");
        colorCode(14,0); printf("~$");
        colorCode(12,0); printf(" Please eject your Flash Drive!");

        Sleep(5000);

        fclose(accNo);
        fclose(userAcc);
        return NULL;
    }
    else{                                       // Else, it will return root.
        fclose(accNo);
        fclose(userAcc);
        return root;
    }
};

int main(){

    node *valid;            // declaring valid as a pointer in node structure.
    node *root = NULL;      // root pointer in node structure as value NULL.
    int access;

    //frontface();            // calling front face program   [SPLASH SCREEN]
    createFolder();         // calling create folder program [DATABASE]

    start:
        access = scanFlashDrive();
        if(access == 1){
            valid = checkAccountNumber(root);   // call checkAccountNumber structure function with parameter root as null
            if(valid){      // if the return value of checkAccountNumber is exists then it will proceed to the statements
                loadDataFromFile(); // load the data from the flash drive
                mainMenuATM(valid, root);   // going to ATM Menu
            }
        }
        else{
            goto start; // go back to start label
        }

    return 0;
}

/** FUNCTION NAME:  scanFlashDrive
 *  DESCRIPTION:    Function for scanning flash drive and the .txt file if it is exist or not.
 *                  This function tells that if flash drive E:\\ doesn't exists, it will find again until it is inserted.
 *  PARAMETER:      NONE
 */

int scanFlashDrive(){
    FILE *check;
    int i = 38, j = 1;      // i and j are iterators; i is for moving flash drive graphic and j is for incrementing variable.

    if(access("E:\\UserAccount.txt", F_OK) == -1){          // accessing flash Drive E
        system("cls");
        maxBorder(0,0);
        do{
            gotoxy(45,6);    printf("Please insert Flash Drive...\n\n");
            gotoxy(35,8);    printf("--");
            gotoxy(35,9);    printf("  |");
            gotoxy(35,10);   printf("  |");
            gotoxy(35,11);   printf("  |");
            gotoxy(35,12);   printf("  |");
            gotoxy(35,13);   printf("  |");
            gotoxy(35,14);   printf("  |");
            gotoxy(35,15);   printf("  |");
            gotoxy(35,16);   printf("  |");
            gotoxy(35,17);   printf("--");

            gotoxy(i,8);    printf("            __________________________ ");
            gotoxy(i,9);    printf(" __________/                          |");
            gotoxy(i,10);   printf("|     _    |                          |");
            gotoxy(i,11);   printf("|    [ ]   |                          |");
            gotoxy(i,12);   printf("|    [_]   |                          |");
            gotoxy(i,13);   printf("|     _    |                          |");
            gotoxy(i,14);   printf("|    [ ]   |                          |");
            gotoxy(i,15);   printf("|    [_]   |                          |");
            gotoxy(i,16);   printf("|__________|                          |");
            gotoxy(i,17);   printf("           \\__________________________|");

            gotoxy(58,8);   printf(" \\       /");
            gotoxy(58,9);   printf("  \\     /");
            gotoxy(58,10);  printf("   \\   /");
            gotoxy(58,11);  printf("    \\_/");
            gotoxy(58,12);  printf("    | |");
            gotoxy(58,13);  printf("    |_|");
            gotoxy(58,14);  printf("    / \\");
            gotoxy(58,15);  printf("   /   \\");
            gotoxy(58,16);  printf("  /     \\");
            gotoxy(58,17);  printf(" /       \\");
            gotoxy(58,18);  printf("/         \\");

            if(i == 38)
                i += 4;
            else
                i -= 4;

            j++;
            Sleep(500);
            system("cls");
            maxBorder(0,0);
        }while(j <= 5);

        return 0;
    }
    else{
        system("cls");
        check = fopen("E:\\UserAccount.txt", "r");  // If FLASH DRIVE is empty, it will exit.
        if(check == NULL){
            colorCode(11,0); gotoxy(19,14); printf("PHSBC_BOT:");
            colorCode(14,0); printf("~$");
            colorCode(12,0); printf(" Your Account Details is Empty on your ATM Card! Please report to your nearest PHSBC Bank!");
            Sleep(1000);
            colorCode(11,0); gotoxy(30,17); printf("PHSBC_BOT:");
            colorCode(14,0); printf("~$");
            colorCode(12,0); printf(" Please eject your Flash Drive!");
            fclose(check);
            Sleep(1000);
            exit(1);
        }
        else{
            fclose(check);
            return 1;
        }
    }
}

/** FUNCTION NAME:  loadDataFromFile
 *  DESCRIPTION:    Function for loading the data from the UserAccount.txt and save it into members of structure tag userATMDetails under the variable user.
 *  PARAMETER:      NONE
 */

void loadDataFromFile(){
    int result;                     // variable for successful conversions
    FILE *read;

    read = fopen("E:\\UserAccount.txt","r");

    if(read != NULL){
        while(!feof(read)){ // end of file
            fscanf(read, "%ld ", &user.accountNumber);
            fgets(user.firstname, 50,read);
            user.firstname[strcspn(user.firstname, "\n")] = 0;  // removing the trailing newline character
            fgets(user.middlename, 50,read);
            user.middlename[strcspn(user.middlename, "\n")] = 0;
            fgets(user.lastname, 50,read);
            user.lastname[strcspn(user.lastname, "\n")] = 0;
            fscanf(read, "%s ", user.birthday);
            fgets(user.address, 100,read);
            user.address[strcspn(user.address, "\n")] = 0;
            result = fscanf(read, "%06d %lf", &user.pinNum, &user.amount);  // successful conversion stored in result
        }

        if(result != 2){        // if result is not equal to 2 (which is the fscanf successful conversions must be valued in 2), proceed to error and exit
            maxBorder(0,0);
            colorCode(11,0); gotoxy(14,12); printf("PHSBC_BOT:");
            colorCode(14,0); printf("~$");
            colorCode(12,0); printf(" There's a problem on PIN in your ATM Card! Please report to your nearest PHSBC Bank!");
            Sleep(2000);
            colorCode(11,0); gotoxy(38,15); printf("PHSBC_BOT:");
            colorCode(14,0); printf("~$");
            colorCode(12,0); printf(" Please eject your Flash Drive ....");
            Sleep(5000);
            gotoxy(0,30);
            exit(2);
        }
    }

    fclose(read);
}

/** FUNCTION NAME:  createFolder
 *  DESCRIPTION:    Function for creating the folder in the designated project program. File Name is "ATM Database"
 *  PARAMETER:      NONE
 */

void createFolder(){
    char *NewDir = "ATM Database";
    CreateDirectory(NewDir,NULL);       //Creating ATM Database in the directory
    GetFullPathName(NewDir, MAX_PATH, fullFileName, NULL); //Getting the File Path of the created folder

    char RAN[50] = "\\Registered Account Numbers.txt";  // Creating string

    strcpy(fpRAN, fullFileName);    // copying the fullFileName (of your designated file path project of this code) to fpRAN
    strcat(fpRAN, RAN);             // adding the string RAN ("Registered Account Numbers.txt") to fpRAN (file path of folder)

    FILE *accNo = fopen(fpRAN,"w");

    // This code will assume that the Account Numbers are saved in the text files and registered in the PHSBC ATM System.
    // Since this program does not have ADMIN Panel to create and add the registered account numbers of every user,
    // We will imply that the ATM System have its own checker of account numbers through the text file, "Registered Account Numbers.txt".
    for(int i = 1365000001; i <= 1365000100; i++){
        fprintf(accNo, "%10d\n", i);
    }

    fclose(accNo);
}

/** -------- MENU FUNCTION ---------------
 *
 *  FUNCTION NAME:  mainMenuATM
 *  DESCRIPTION:    Function that is for ATM Menu
 *  PARAMETER:      @valid = node that holds the value of return in node * checkAccountNumber
 *                  @root = node that holds the value of parameter in node * checkAccountNumber
 */

void mainMenuATM(node *valid, node *root){

    transac *head = NULL;       // head is NULL
    int option;                 // variable for option value

    do{
        system("cls");

        maxBorder(0,0);

        colorCode(11,0);
        gotoxy(12,3);    printf(" .----------------. .----------------.");
        gotoxy(12,4);    printf("| .--------------. | .--------------. ");
        gotoxy(12,5);    printf("| |   ______     | | |  ____  ____  | ");
        gotoxy(12,6);    printf("| |  |_   __ \\   | | | |_   ||   _| | ");
        gotoxy(12,7);    printf("| |    | |__) |  | | |   | |__| |   | ");
        gotoxy(12,8);    printf("| |    |  ___/   | | |   |  __  |   | ");
        gotoxy(12,9);    printf("| |   _| |_      | | |  _| |  | |_  | ");
        gotoxy(12,10);   printf("| |  |_____|     | | | |____||____| | ");
        gotoxy(12,11);   printf("| |              | | |              | ");
        gotoxy(12,12);   printf("| '--------------' | '--------------' ");
        gotoxy(12,13);   printf(" '----------------' '----------------'");

        colorCode(15,0);
        gotoxy(50,3);    printf(" .----------------.");
        gotoxy(50,4);    printf("| .--------------. ");
        gotoxy(50,5);    printf("| |    _______   | ");
        gotoxy(50,6);    printf("| |   /  ___  |  | ");
        gotoxy(50,7);    printf("| |  |  (__ \\_|  | ");
        gotoxy(50,8);    printf("| |   '.___`-.   | ");
        gotoxy(50,9);    printf("| |  |`\\____) |  | ");
        gotoxy(50,10);   printf("| |  |_______.'  | ");
        gotoxy(50,11);   printf("| |              | ");
        gotoxy(50,12);   printf("| '--------------' ");
        gotoxy(50,13);   printf(" '----------------'");

        colorCode(6,0);
        gotoxy(69,3);    printf(" .----------------.");
        gotoxy(69,4);    printf("| .--------------. ");
        gotoxy(69,5);    printf("| |   ______     | ");
        gotoxy(69,6);    printf("| |  |_  _  \\    | ");
        gotoxy(69,7);    printf("| |   | |_) |    | ");
        gotoxy(69,8);    printf("| |   |  __'.    | ");
        gotoxy(69,9);    printf("| |  _| |__) |   | ");
        gotoxy(69,10);   printf("| | |_______/    | ");
        gotoxy(69,11);   printf("| |              | ");
        gotoxy(69,12);   printf("| '--------------' ");
        gotoxy(69,13);   printf(" '----------------'");

        colorCode(12,0);
        gotoxy(88,3);    printf(" .----------------. ");
        gotoxy(88,4);    printf("| .--------------. |");
        gotoxy(88,5);    printf("| |     ______   | |");
        gotoxy(88,6);    printf("| |   .' ___  |  | |");
        gotoxy(88,7);    printf("| |  / .'   \\_|  | |");
        gotoxy(88,8);    printf("| |  | |         | |");
        gotoxy(88,9);    printf("| |  \\ `.___.'\\  | |");
        gotoxy(88,10);   printf("| |   `._____.'  | |");
        gotoxy(88,11);   printf("| |              | |");
        gotoxy(88,12);   printf("| '--------------' |");
        gotoxy(88,13);   printf(" '----------------' ");

        colorCode(9,0);
        gotoxy(43,15);  printf("Philippine ");
        colorCode(14,0);
        gotoxy(54,15);  printf("Social Bank ");
        colorCode(12,0);
        gotoxy(66,15);  printf("and Commerce ");

        colorCode(9,0);
        gotoxy(46,17);   printf("[1] -> Balance Inquiry");
        colorCode(6,0);
        gotoxy(46,18);   printf("[2] -> Cash Withdrawal");
        colorCode(12,0);
        gotoxy(46,19);   printf("[3] -> Cash Deposit");
        colorCode(10,0);
        gotoxy(46,21);   printf("[4] -> Transaction History");
        colorCode(14,0);
        gotoxy(46,22);   printf("[5] -> Change Pin");
        colorCode(7,0);
        gotoxy(46,24);   printf("[6] -> Exit");

        colorCode(15,0);
        gotoxy(46,26);   printf("Enter transaction type: ");

        if(scanf("%d", &option) == 0){          // if the input is not successful as integer value, error!
            fflush(stdin);
            colorCode(11,0);
            Sleep(50);gotoxy(77,27); printf("--------------------------------------");
            colorCode(12,0);
            Sleep(50);gotoxy(77,26); printf("|                                    |");
            Sleep(50);gotoxy(77,25); printf("|                                    |");
            Sleep(50);gotoxy(77,24); printf("|                                    |");
            Sleep(50);gotoxy(77,23); printf("|                                    |");
            colorCode(12,0);
            Sleep(50);gotoxy(77,22); printf("|                                    |");
            colorCode(11,0);
            Sleep(50);gotoxy(77,21); printf("--------------------------------------");

            Sleep(100);colorCode(11,0); gotoxy(90,22);printf("PHSBC_BOT:");
            colorCode(14,0); gotoxy(100,22);printf("~$");
            Sleep(100);colorCode(12,0); gotoxy(84,23);printf("Please enter appropriate");
            Sleep(100);colorCode(12,0); gotoxy(87,24);printf("option! Try Again!");
            colorCode(11,0);
            Sleep(100);gotoxy(80,26); system("pause");
        }
        else{
            switch(option){
                case 1: // Balance Inquiry
                    loadDataFromFile();
                    balanceInquiry();
                    break;
                case 2: // Cash Withdrawal
                    loadDataFromFile();
                    withdrawal(&head);
                    break;
                case 3: // Cash Deposit
                    loadDataFromFile();
                    if(depPin()){
                        depositMenu();
                        deposit(&head);
                    }
                    break;
                case 4: // Transaction History
                    showHistory(&head);
                    break;
                case 5: // Change Pass
                    changePass();
                    break;
                case 6: // Exit
                    free(valid);
                    free(head);
                    gotoxy(0,30);
                    exit(0);
                    break;
                default: // Values from 0 below and 7 up
                    fflush(stdin);
                    colorCode(11,0);
                    Sleep(50);gotoxy(77,27); printf("--------------------------------------");
                    colorCode(12,0);
                    Sleep(50);gotoxy(77,26); printf("|                                    |");
                    Sleep(50);gotoxy(77,25); printf("|                                    |");
                    Sleep(50);gotoxy(77,24); printf("|                                    |");
                    Sleep(50);gotoxy(77,23); printf("|                                    |");
                    colorCode(12,0);
                    Sleep(50);gotoxy(77,22); printf("|                                    |");
                    colorCode(11,0);
                    Sleep(50);gotoxy(77,21); printf("--------------------------------------");

                    Sleep(100);colorCode(11,0); gotoxy(90,22);printf("PHSBC_BOT:");
                    colorCode(14,0); gotoxy(100,22);printf("~$");
                    Sleep(100);colorCode(12,0); gotoxy(84,23);printf("Please enter appropriate");
                    Sleep(100);colorCode(12,0); gotoxy(87,24);printf("option! Try Again!");
                    colorCode(11,0);
                    Sleep(100);gotoxy(80,26); system("pause");
                    break;
            }
        }
    }while(1); // infinite loop
}

/** -------- BALANCE INQUIRY FUNCTION ---------------
 *
 *  FUNCTION NAME:  balanceInquiry
 *  DESCRIPTION:    Function for checking the balance of user's account.
 *  PARAMETER:      NONE
 */

void balanceInquiry(){

    system("cls");

    maxBorder(0,0);

    colorCode(6,0);
    gotoxy(21,4);    printf("  ____        _                        _____                   _            ");
    gotoxy(21,5);    printf(" |  _ \\      | |                      |_   _|                 (_)           ");
    gotoxy(21,6);    printf(" | |_) | __ _| | __ _ _ __   ___ ___    | |  _ __   __ _ _   _ _ _ __ _   _ ");
    gotoxy(21,7);    printf(" |  _ < / _` | |/ _` | '_ \\ / __/ _ \\   | | | '_ \\ / _` | | | | | '__| | | |");
    gotoxy(21,8);    printf(" | |_) | (_| | | (_| | | | | (_|  __/  _| |_| | | | (_| | |_| | | |  | |_| |");
    gotoxy(21,9);    printf(" |____/ \\__,_|_|\\__,_|_| |_|\\___\\___| |_____|_| |_|\\__, |\\__,_|_|_|   \\__, |");
    gotoxy(21,10);   printf("                                                      | |              __/ |");
    gotoxy(21,11);   printf("                                                      |_|             |___/ ");

    for(int j=35; j<=85; j++){
        colorCode(6,0);
        gotoxy(j,14);    printf("%c",220);
        colorCode(6,0);
        gotoxy(j,24);    printf("%c",223);
    }
    for(int k=15; k<=23; k++){
        colorCode(6,0);
        gotoxy(35,k);    printf("%c",219);
        colorCode(6,0);
        gotoxy(85,k);    printf("%c",219);
    }

    colorCode(15,0);
    gotoxy(40,16);   printf("Account Number: %ld", user.accountNumber);
    gotoxy(40,17);   printf("Account Name: %s %s %s", user.firstname, user.middlename, user.lastname);
    gotoxy(40,18);   printf("Birthday: %s", user.birthday);
    gotoxy(40,19);   printf("Address: %s", user.address);
    gotoxy(40,21);   printf("Account Amount: %.2lf", user.amount);

    gotoxy(45,26);
    system("pause");
}

/** -------- DEPOSIT FUNCTIONS ---------------
 *
 *  FUNCTION NAME:  depPin
 *  DESCRIPTION:    Function for entering the user's PIN to deposit.
 *  PARAMETER:      NONE
 */

bool depPin(){
    int chance = 0, i = 0;          // chances are up to 3; i is for iterator.
    char a, checkPin[7];            // a stores the character entered by the user, checkPin stores the user's pinNum

    sprintf(checkPin,"%06d", user.pinNum);  // converts user's pin from integer to string and stored in checkPin.

    do{
        system("cls");
        maxBorder(0,0);
        char pin[7]="\0";    // string is NULL.

        colorCode(4,0);
        gotoxy (38, 3);     printf("              @@@@@@@@@@@@@@@@             ");
        gotoxy (38, 4);     printf("           @@&                @@&          ");
        gotoxy (38, 5);     printf("        @@    @@@&       @@@@    @@        ");
        gotoxy (38, 6);     printf("      @@    @&                @@   @&      ");
        gotoxy (38, 7);     printf("    @@  @@                      @@  @@     ");
        gotoxy (38, 8);     printf("   @@  @                          ,@  @@   ");
        gotoxy (38, 9);     printf("   @  @@                           @&  @   ");
        gotoxy (38, 10);    printf("@@&                                     &@@");
        gotoxy (38, 11);    printf("@                                         @");
        gotoxy (38, 12);    printf("@                                         @");
        gotoxy (38, 13);    printf("@                                         @");
        gotoxy (38, 14);    printf("@                                         @");
        gotoxy (38, 15);    printf("@    @                              @     @");
        gotoxy (38, 16);    printf("@    @  @@@@@@ @@@@@@ @@@@@@(@@@@@  @     @");
        gotoxy (38, 17);    printf("@    @  @@ @@  @@ @@  @@ @@  @@ @@  @     @");
        gotoxy (38, 18);    printf("@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     @");
        gotoxy (38, 19);    printf("@                                         @");
        gotoxy (38, 20);    printf("@                                         @");
        gotoxy (38, 21);    printf("@                                         @");
        gotoxy (38, 22);    printf("@                                         @");
        gotoxy (38, 23);    printf("  &@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ");

        colorCode(2,0);
        gotoxy (43, 14);    printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*");
        gotoxy (43, 15);    printf("@                              @");
        gotoxy (43, 16);    printf("@  @@@@@@ @@@@@@ @@@@@@(@@@@@  @");
        gotoxy (43, 17);    printf("@  @@ @@  @@ @@  @@ @@  @@ @@  @");
        gotoxy (43, 18);    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

        colorCode(15,0);
        gotoxy(48,20);      printf("Enter your 6-Digit PIN: ");
        gotoxy(56,21);      printf("_______");
        gotoxy(56,21);      printf(" ");

        for(i=0;;){
            a=getch();              // catching the character and store in a
            if(a==8&&i>0){          // if user's entered input is backspace (8 as the ASCII Hex Value) and i is less than 0
                printf("\b \b");    // backspace character transfers the cursor one character back with or without deleting on different compilers.
                pin[i] = ' ';       // index position of i in pin stores the whitespace
                i--;                // i is decrement
            }
            if(a!='\r'&& a != 8 && i < 6){  // if a is not carriage return character (\r) and a is not a backspace character and i is less than 6.
                pin[i] = a;         // a is passed to pin with index i
                ++i;                // Pre-increment i.
                printf("*");        // Then, print asterisk in the console to make it hide.
            }
            if(a=='\r' || i == 6){  // if a is equal to carriage return character (\r) or i is equal to 6 (as the limit of 6-DIGIT PIN)
                pin[i] = '\0';      // Last position of index[i] in pin is set as NULL character.
                if(strcmp(pin,checkPin)== 0){   //comparing the strings of pin and checkpin (if it is same)
                    colorCode(15,0); gotoxy(53,26); printf("Please Wait!!");    // redirecting
                    Sleep(900); // zzzz sleepwell
                    return 1;   // return 1 as the true value.
                }
                else{   // if it is not same of string compare
                    colorCode(11,0); gotoxy(48,25);printf("PHSBC_BOT:");
                    colorCode(14,0); gotoxy(58,25);printf("~$");
                    colorCode(12,0); gotoxy(59,25);printf(" WRONG PIN!");
                    colorCode(15,0); gotoxy(45,27);system("pause");
                    chance++; // increment chance
                    break;
                }
            } // end third if (line 691)
        } // end for loop (line 679)
    } while (chance != 3);// end of do while

    if(chance == 3){    // If chance is 3, error input and redirecting again to mainMenuATM function
        system("cls");
        maxBorder(0,0);
        colorCode(11,0); gotoxy(42,14);printf("PHSBC_BOT:");
        colorCode(14,0); gotoxy(52,14);printf("~$");
        colorCode(12,0); gotoxy(55,14);printf(" PLEASE TRY AGAIN LATER!");
        colorCode(15,0); gotoxy(45,26);system("pause");
    }

    return 0; // 0 as false
}

/** FUNCTION NAME:  depositMenu
 *  DESCRIPTION:    Function for Deposit Menu Bill Denomination.
 *  PARAMETER:      NONE
 */

void depositMenu(){
    int select, num, option[] = {1000, 500, 200, 100, 50, 20};      // select = option from payment, num is the pieces from select value, option is an array of cash bills.
    int d = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0;              // d to d5 is set to 0, as the holder of num value of pieces.
    int i=13;                                                       // y axis position
    char YN;                                                        // YN to confirm the deposit
    bool valid = false;                                             // valid indicator set to false

    do{
        system("cls");

        maxBorder(0,0);

        colorCode(4,0);
        gotoxy(20,4);   printf("________                             .__  __   ");
        gotoxy(20,5);   printf("\\______ \\   ____ ______   ____  _____|__|/  |_ ");
        gotoxy(20,6);   printf(" |    |  \\_/ __ \\\\____ \\ /  _ \\/  ___/  \\   __\\");
        gotoxy(20,7);   printf(" |    `   \\  ___/|  |_> >  <_> )___ \\|  ||  |  ");
        gotoxy(20,8);   printf("/_______  /\\___  >   __/ \\____/____  >__||__|  ");
        gotoxy(20,9);   printf("        \\/     \\/|__|              \\/          ");

        colorCode(11,0);
        gotoxy(73,5);  printf("----------------------------------");
        gotoxy(73,6);  printf(" NOTE: Cash Deposit must be range");
        gotoxy(73,7);  printf("       from PHP 100 to 500,000.");
        gotoxy(73,8);  printf("----------------------------------");

        gotoxy(20,11);  printf("TOTAL DEPOSIT: %d", total);
        gotoxy(20,13);  printf("[1] 1000\n");
        gotoxy(20,14);  printf("[2] 500\n");
        gotoxy(20,15);  printf("[3] 200\n");
        gotoxy(20,16);  printf("[4] 100\n");
        gotoxy(20,17);  printf("[5] 50\n");
        gotoxy(20,18);  printf("[6] 20\n");
        gotoxy(20,19);  printf("[7] Next\n");

        colorCode(2,0);
        gotoxy(70,11);  printf("        ,,,                 ,,,         ");
        gotoxy(70,12);  printf("      .,,,,,,.            .,,,,,,.      ");
        gotoxy(70,13);  printf("  ,,,,,,,,,,,,,,,.    ,,,,,,,,,,,,,,,.  ");
        gotoxy(70,14);  printf(" ,,,,,  *,*          ,,,,,  *,*         ");
        gotoxy(70,15);  printf("  ,,,,*  *,*          ,,,,*  *,*        ");
        gotoxy(70,16);  printf("  **,*,*,**           **,*,*,**         ");
        gotoxy(70,17);  printf("     .**********         .**********    ");
        gotoxy(70,18);  printf("        *** ,*****          *** ,*****  ");
        gotoxy(70,19);  printf("        ***   ****.         ***   ****. ");
        gotoxy(70,20);  printf(".****,. ***,******  .****,. ***,******  ");
        gotoxy(70,21);  printf(" **************.     **************.    ");
        gotoxy(70,22);  printf("        ***                 ***         ");
        gotoxy(70,23);  printf("        ***                 ***         ");

        colorCode(15,0);
        gotoxy(20,21);printf("Select amount of payment: ");

        if(scanf("%d", &select)== 0){   // if scanf returns unsuccessful conversion of integer
            fflush(stdin);       // clearing buffer of standard input
            gotoxy(20,23);
            printf("Error Input!");
            gotoxy(45,27);
            system("pause");
            depositMenu();
            fflush(stdin);
        }
        else{
            switch(select){
                case 1: // for 1000 bill
                    gotoxy(20,22);
                    printf("How many Php 1,000 do you have? : ");
                    if(scanf("%d", &num)== 0){ // if scanf of num returns unsuccessful conversion of integer
                        fflush(stdin);
                        gotoxy(20,23);
                        printf("Error Input!");
                        gotoxy(45,27);
                        system("pause");
                        fflush(stdin);
                        break;
                    }
                    else{ // if scanf of num returns successful conversion of integer
                        gotoxy(20,23);
                        printf("Total amount of Php 1,000 is %d \n", option[0] * num);
                        gotoxy(45,27);
                        d += num;   // num is added to d
                        total += option[0] * num; // total deposit is added by option[index] multiply to num
                        system("pause");
                        valid = false;  // set to false for indicating the looping process
                        fflush(stdin);
                        break;
                    }
                case 2: // for 500 bill
                    gotoxy(20,22);
                    printf("How many Php 500 do you have? : ");
                    if(scanf("%d", &num)== 0){
                        fflush(stdin);
                        gotoxy(20,23);
                        printf("Error Input!");
                        gotoxy(45,27);
                        system("pause");
                        break;
                        fflush(stdin);
                    }
                    else{
                        gotoxy(20,23);
                        printf("Total amount of Php 500 is %d \n", option[1] * num);
                        gotoxy(45,27);
                        d1 += num;
                        total += option[1] * num;
                        system("pause");
                        valid = false;
                        fflush(stdin);
                        break;
                    }
                case 3: // for 200 bill
                    gotoxy(20,22);
                    printf("How many Php 200 do you have? : ");
                    if(scanf("%d", &num)== 0){
                        fflush(stdin);
                        gotoxy(20,23);
                        printf("Error Input!");
                        gotoxy(45,27);
                        system("pause");
                        break;
                        fflush(stdin);
                    }
                    else{
                        gotoxy(20,23);
                        printf("Total amount of Php 200 is %d \n", option[2] * num);
                        gotoxy(45,27);
                        d2 += num;
                        total += option[2] * num;
                        system("pause");
                        valid = false;
                        fflush(stdin);
                        break;
                    }
                case 4: // for 100 bill
                    gotoxy(20,22);
                    printf("How many Php 100 do you have? : ");
                    if(scanf("%d", &num)== 0){
                        gotoxy(20,23);
                        printf("Error Input!");
                        gotoxy(45,27);
                        system("pause");
                        break;
                        fflush(stdin);
                    }
                    else{
                        gotoxy(20,23);
                        printf("Total amount of Php 100 is %d \n", option[3] * num);
                        gotoxy(45,27);
                        d3 += num;
                        total += option[3] * num;
                        system("pause");
                        valid = false;
                        fflush(stdin);
                        break;
                    }
                case 5: // for 50 bill
                    gotoxy(20,22);
                    printf("How many Php 50 do you have? : ");
                    if(scanf("%d", &num)== 0){
                        fflush(stdin);
                        gotoxy(20,23);
                        printf("Error Input!");
                        gotoxy(45,27);
                        system("pause");
                        break;
                        fflush(stdin);
                    }
                    else{
                        gotoxy(20,23);
                        printf("Total amount of Php 50 is %d \n", option[4] * num);
                        gotoxy(45,27);
                        d4 += num;
                        total += option[4] * num;
                        system("pause");
                        valid = false;
                        fflush(stdin);
                        break;
                    }
                case 6: // for 20 bill
                    gotoxy(20,22);
                    printf("How many Php 20 do you have? : ");
                    if(scanf("%d", &num)== 0){
                        fflush(stdin);
                        gotoxy(20,23);
                        printf("Error Input!");
                        gotoxy(45,27);
                        system("pause");
                        break;
                        fflush(stdin);
                    }
                    else{
                        gotoxy(20,23);
                        printf("Total amount of Php 20 is %d \n", option[5] * num);
                        gotoxy(45,27);
                        d5 += num;
                        total += option[5] * num;
                        system("pause");
                        valid = false;
                        fflush(stdin);
                        break;
                    }
                case 7: // for Next
                    gotoxy(20,22);
                    printf("Confirm[Y/N]: "); // confirm the deposit
                    scanf(" %c", &YN);

                    gotoxy(5,14);
                    if(YN == 'y' || YN == 'Y'){ // YN is Y or y
                        system("cls");
                        maxBorder(0,0);

                        colorCode(4,0);
                        gotoxy(30,4);   printf("________                             .__  __   ");
                        gotoxy(30,5);   printf("\\______ \\   ____ ______   ____  _____|__|/  |_ ");
                        gotoxy(30,6);   printf(" |    |  \\_/ __ \\\\____ \\ /  _ \\/  ___/  \\   __\\");
                        gotoxy(30,7);   printf(" |    `   \\  ___/|  |_> >  <_> )___ \\|  ||  |  ");
                        gotoxy(30,8);   printf("/_______  /\\___  >   __/ \\____/____  >__||__|  ");
                        gotoxy(30,9);   printf("        \\/     \\/|__|              \\/          ");

                        colorCode(2,0);
                        gotoxy(70,11);  printf("        ,,,                 ,,,         ");
                        gotoxy(70,12);  printf("      .,,,,,,.            .,,,,,,.      ");
                        gotoxy(70,13);  printf("  ,,,,,,,,,,,,,,,.    ,,,,,,,,,,,,,,,.  ");
                        gotoxy(70,14);  printf(" ,,,,,  *,*          ,,,,,  *,*         ");
                        gotoxy(70,15);  printf("  ,,,,*  *,*          ,,,,*  *,*        ");
                        gotoxy(70,16);  printf("  **,*,*,**           **,*,*,**         ");
                        gotoxy(70,17);  printf("     .**********         .**********    ");
                        gotoxy(70,18);  printf("        *** ,*****          *** ,*****  ");
                        gotoxy(70,19);  printf("        ***   ****.         ***   ****. ");
                        gotoxy(70,20);  printf(".****,. ***,******  .****,. ***,******  ");
                        gotoxy(70,21);  printf(" **************.     **************.    ");
                        gotoxy(70,22);  printf("        ***                 ***         ");
                        gotoxy(70,23);  printf("        ***                 ***         ");

                        colorCode(15,0);
                        gotoxy(20,12);  printf("CASH DENOMINATION");

                        // PRINTING THE CASH DENOMINATION
                        if(d != 0){
                            gotoxy(20,i);    printf("  %c Php 1,000 = %d ",254, d);
                            i++;
                        }
                        if(d1 != 0){
                            gotoxy(20,i);    printf("  %c Php 500 = %d ",254, d1);
                            i++;
                        }
                        if(d2 != 0){
                            gotoxy(20,i);    printf("  %c Php 200 = %d ",254, d2);
                            i++;
                        }
                        if(d3 != 0){
                            gotoxy(20,i);    printf("  %c Php 100 = %d ",254, d3);
                            i++;
                        }
                        if(d4 != 0){
                            gotoxy(20,i);    printf("  %c Php 50 = %d ",254, d4);
                            i++;
                        }
                        if(d5 != 0){
                            gotoxy(20,i);    printf("  %c Php 20 = %d ",254, d5);
                            i++;
                        }

                        gotoxy(20,23);
                        printf("Total amount to deposit is %d \n", total); // total deposit
                        gotoxy(45,26);
                        system("pause");
                        valid = true; // set to true
                        break;
                    }
                    else{
                        d1 = d2 = d3 = d4 = d5 = d = 0; // d to d5 value is 0
                        total = 0; // 0 again
                        valid = false;  // set to false
                        fflush(stdin); // clear buffer
                        break;
                    }
                default: // if select enters 0 below and 8 above
                    gotoxy(20,23);
                    colorCode(11,0); printf("PHSBC_BOT:");
                    colorCode(14,0); printf("~$");
                    colorCode(12,0); printf(" Error input!");
                    gotoxy(20,24);
                    colorCode(11,0); system("pause");
                    fflush(stdin);
                    system("cls");
                    valid = false;
                    break;
            }
        }
    }while(valid != true); // looping until it gets true in valid value.
}

/** FUNCTION NAME:  deposit
 *  DESCRIPTION:    Function for checking the deposit if it is set to limits in the program.
 *                  Minumum: 100
 *                  Maximum: 500,000
 *  PARAMETER:      @head = double pointer under transac node
 */

void deposit(transac **head){
    FILE *read, *transac, *tempTxt;     // File pointers we need to use in deposit

    double oldBalance, newBalance;      // oldBalance for old amount in user's ATM; newBalance for new amount stored in user's ATM.
    int number;                         // number stores the total that we did in depositMenu

    system("cls");
    loading(2);            // call loading with argument 2
    maxBorder(0,0);
    number = total;        // total value is stored in number

    if(number < 100){   // if number is less than 100 (minimum)
        system("cls");
        maxBorder(0,0);
        gotoxy(25,11);
        colorCode(11,0); printf("PHSBC_BOT:");
        colorCode(14,0); printf("~$");
        colorCode(12,0); printf(" Sorry, your transaction cannot be processed! Minimum deposit is 100.");
        gotoxy(50,12);
        colorCode(15,0); printf("Try again later.");
        total = 0;
        gotoxy(45,26);
        system("pause");
    }
    else if (number > 500000){ // if number is greater than 500000 (maximum)
        system("cls");
        maxBorder(0,0);
        gotoxy(25,11);
        colorCode(11,0); printf("PHSBC_BOT:");
        colorCode(14,0); printf("~$");
        colorCode(12,0); printf(" Sorry, your transaction cannot be processed! Maximum deposit is 500,000.");
        gotoxy(50,12);
        colorCode(15,0); printf("Try again later.");
        total = 0;
        gotoxy(45,26);
        system("pause");
    }
    else{
        // Opening text files.
        read = fopen("\\UserAccount.txt", "r");
        tempTxt = fopen("E:\\Temp.txt","w");
        transac = fopen("E:\\Transaction History.txt", "a+");

        system("cls");
        maxBorder(0,0);

        // printing the values in Temp.txt File
        fprintf(tempTxt,"%ld\n%s\n%s\n%s\n%s\n%s\n%d\n%.2lf", user.accountNumber, user.firstname, user.middlename, user.lastname, user.birthday, user.address, user.pinNum, user.amount + number);
        oldBalance = user.amount; // user amount is stored in oldBalance
        newBalance = user.amount + number; // answer will be stored in newBalance
        fprintf(transac,"DEPOSIT %.2lf %d %.2lf %s %s\n",user.amount, number, user.amount + number, __DATE__, __TIME__); // printing the values in Transaction History.txt
        gotoxy(35,12);
        printf("Money has been deposited to your Account %ld \n", user.accountNumber); // show text
        saveHistory(head, number, "DEPOSIT", oldBalance, newBalance); // calling saveHistory function

        // Closing text files
        fclose(read);
        fclose(transac);
        fclose(tempTxt);

        remove("E:\\UserAccount.txt");  // remove the old UserAccount details
        rename("E:\\Temp.txt","E:\\UserAccount.txt"); // the new content details in program is retained in flash drive.

        gotoxy(45,26);
        system("pause");

        depositReceipt(); // call depositReceipt function
    }
}

/** FUNCTION NAME:  depositReceipt
 *  DESCRIPTION:    Function for displaying the Receipt in "Deposit Receipt.txt"
 *  PARAMETER:      NONE
 */

void depositReceipt(){
    // For accessing time today
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // File pointer
    FILE *write;

    char fp[100]; // string

    strcpy(fp, fullFileName); // copying the fullFileName path and stored in fp
    strcat(fp, "\\Deposit Receipt.txt"); // Then add the "\\Deposit Receipt.txt" text.

    write = fopen(fp,"w"); // opening the "Deposit Receipt.txt" in ATM Database folder.

    fprintf(write,"***************************************************\n");
    fprintf(write,"          PHILIPPINE SOCIAL BANK COMMERCE          \n");
    fprintf(write,"                  Ermita, Manila\n");
    fprintf(write,"===================================================\n");
    fprintf(write,"   Date: %s           Time: %02d:%02d:%02d\n",__DATE__, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(write,"   Location: TUP - MANILA      Terminal ID: 0001\n");
    fprintf(write,"===================================================\n");
    fprintf(write,"\n");
    fprintf(write,"   Receipt No: %d%02d%02d%02d%02d%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(write,"\n");
    fprintf(write,"   Account Number: %ld\n", user.accountNumber);
    fprintf(write,"   Account Name: %s %s\n", user.firstname, user.lastname);
    fprintf(write,"   Amount: %.2lf\n", user.amount);
    fprintf(write,"   Amount Deposit: %d\n", total);
    fprintf(write,"   Current Balance: %.2lf\n", user.amount+total);
    fprintf(write,"\n");
    fprintf(write,"  !! ENJOY THE CONVENIENCE OF 24/7 BANKING !!\n");
    fprintf(write,"     PAY YOUR BILLS VIA BDO ATMS AND EARN\n");
    fprintf(write,"            PHSBC REWARDS POINTS!\n");
    fprintf(write,"     VISIT ANY PHSBC BRANCH TO KNOW MORE.\n");
    fprintf(write,"***************************************************\n");

    total = 0; // set the total to 0
    fclose(write); // close the file
}

/** -------- WITHDRAWAL FUNCTIONS ---------------
 *
 *  FUNCTION NAME:  withdrawal
 *  DESCRIPTION:    Function for withdrawing the cash in user's ATM.
 *                  Minimum: 100
 *                  Maximum: 500,000
 *  PARAMETER:      @head = double pointer under transac node
 */

void withdrawal(transac **head){
    int withdraw; // storing the withdraw amount
    int i = 0;    // iterator to check 3 times of error in withdrawal
    bool valid = false; // set valid to false.

    do{
        system("cls");

        maxBorder(0,0);

        colorCode(11,0);
        gotoxy(24,4);    printf(" __          _______ _______ _    _ _____  _____       __          __");
        gotoxy(24,5);    printf(" \\ \\        / /_   _|__   __| |  | |  __ \\|  __ \\     /\\ \\        / /");
        gotoxy(24,6);    printf("  \\ \\  /\\  / /  | |    | |  | |__| | |  | | |__) |   /  \\ \\  /\\  / / ");
        gotoxy(24,7);    printf("   \\ \\/  \\/ /   | |    | |  |  __  | |  | |  _  /   / /\\ \\ \\/  \\/ /  ");
        gotoxy(24,8);    printf("    \\  /\\  /   _| |_   | |  | |  | | |__| | | \\ \\  / ____ \\  /\\  /   ");
        gotoxy(24,9);    printf("     \\/  \\/   |_____|  |_|  |_|  |_|_____/|_|  \\_\\/_/    \\_\\/  \\/    ");
        gotoxy(24,10);   printf("                                                                     ");

        colorCode(15,0);
        gotoxy(40,17); printf("Enter amount to be withdrawn: ");
        gotoxy(70,17);

        if(scanf("%d", &withdraw) == 0){ // if scanf returns unsuccessful conversions of integer. Error
            fflush(stdin);
            gotoxy(49,21);
            colorCode(11,0); printf("PHSBC_BOT: Error Input!");
            gotoxy(45,23);
            colorCode(12,0); system("pause");
            fflush(stdin);
        }
        else{
            if(withdraw < 100){ // if withdraw is less than 100. Error
                gotoxy(19,17);
                colorCode(11,0); printf("PHSBC_BOT:");
                colorCode(14,0); printf("~$");
                colorCode(12,0); printf(" Sorry, your transaction cannot be processed! Minimum withdrawal is 100.");
                colorCode(15,0); gotoxy(45,26);system("pause");
                fflush(stdin);
                i++;
            }
            else if(withdraw > user.amount-100){ // if withdraw is higher than user.amount subtracted by 100
                gotoxy(33,17);
                colorCode(11,0); printf("PHSBC_BOT:");
                colorCode(14,0); printf("~$");
                colorCode(12,0); printf(" Sorry, your transaction cannot be processed.");
                colorCode(15,0);gotoxy(45,26);system("pause");
                fflush(stdin);
                i++;
            }
            else if(withdraw > 500000){ // if withdraw is greater than 500,000
                gotoxy(19,17);
                colorCode(11,0); printf("PHSBC_BOT:");
                colorCode(14,0); printf("~$");
                colorCode(12,0); printf(" Sorry, your transaction cannot be processed! Maximum withdrawal is 500,000.");
                colorCode(15,0); gotoxy(45,26); system("pause");
                fflush(stdin);
                i++;
            }
            // if withdraw have coins and not bills.
            else if(!((withdraw % 20 == 0) || (withdraw % 50 == 0) || (withdraw % 100 == 0) || (withdraw % 200 == 0) || (withdraw % 500 == 0) || (withdraw % 1000 == 0))){
                gotoxy(20,17);
                colorCode(11,0); printf("PHSBC_BOT:");
                colorCode(14,0); printf("~$");
                colorCode(12,0); printf(" Sorry, your transaction cannot be processed! Bills must be entered!");
                colorCode(15,0);gotoxy(45,26);system("pause");
                fflush(stdin);
                i++;
            }
            else{
                pinPanel(withdraw, head); // call pinPanel function
                valid = true;
            } // end of else

            // i is the chance and equal to 3
            if(i == 3){
                system("cls");
                maxBorder(0,0);
                colorCode(11,0); gotoxy(42,14);printf("PHSBC_BOT:");
                colorCode(14,0); gotoxy(52,14);printf("~$");
                colorCode(12,0); gotoxy(55,14);printf(" PLEASE TRY AGAIN LATER!");
                colorCode(15,0); gotoxy(45,26);system("pause");
                break;
            }
        }
    }while(valid != true); // looping until it gets to true.
}

/** FUNCTION NAME:  pinPanel
 *  DESCRIPTION:    Function for entering the user's PIN to withdraw.
 *  PARAMETER:      @withdraw = value of user's withdraw
 *                  @head = double pointer under transac node
 */

void pinPanel(int withdraw, transac **head){
    int chance = 0;                 // chances are 3
    int i;                          // iterator
    char a,checkPin[7], yn;         // a stores the character entered by the user, checkPin stores the user's pinNum, yn is for confirming the receipt
    bool valid = false;             // set valid to false

    sprintf(checkPin,"%06d", user.pinNum);  // converts user's pin from integer to string and stored in checkPin.

    do{
        system("cls");
        maxBorder(0,0);
        colorCode(11,0);
        gotoxy(24,4);    printf(" __          _______ _______ _    _ _____  _____       __          __");
        gotoxy(24,5);    printf(" \\ \\        / /_   _|__   __| |  | |  __ \\|  __ \\     /\\ \\        / /");
        gotoxy(24,6);    printf("  \\ \\  /\\  / /  | |    | |  | |__| | |  | | |__) |   /  \\ \\  /\\  / / ");
        gotoxy(24,7);    printf("   \\ \\/  \\/ /   | |    | |  |  __  | |  | |  _  /   / /\\ \\ \\/  \\/ /  ");
        gotoxy(24,8);    printf("    \\  /\\  /   _| |_   | |  | |  | | |__| | | \\ \\  / ____ \\  /\\  /   ");
        gotoxy(24,9);    printf("     \\/  \\/   |_____|  |_|  |_|  |_|_____/|_|  \\_\\/_/    \\_\\/  \\/    ");
        gotoxy(24,10);   printf("                                                                     ");

        colorCode(14,0);
        gotoxy(25,12); printf("--------------------------------------------------------------------");
        gotoxy(25,13); printf("|                    WITHDRAWN AMOUNT: %d                        |", withdraw);
        gotoxy(25,14); printf("--------------------------------------------------------------------");

        colorCode(12,0);
        char pin[7] = "\0"; // set string as NULL;
        gotoxy (40,17); printf("Enter your 6-Digit PIN: ");
        for(i=0;;){
            a=getch();
            if(a==8&&i>0){
                printf("\b \b");
                pin[i]=' ';
                i--;
            }
            if(a!='\r'&& a != 8 && i < 6){
                pin[i]=a;
                ++i;
                printf("*");
            }
            if(a=='\r' || i == 6){
                pin[i]='\0';
                if(strcmp(pin,checkPin)== 0){   // if it is same
                    loading(1);
                    valid = true;
                    system("cls");
                    maxBorder(0,0);
                    gotoxy (50,11);     printf("Transaction Completed!");
                    gotoxy (42,13);     printf("Do you need to Print Receipt? [Y/N]: ");
                    scanf(" %c", &yn);
                    if(yn == 'y' || yn == 'Y'){ // Print receipt
                        user.amount -= withdraw; // subtracting the withdraw to user.amount
                        printReceipt(withdraw, head);   // calling printReceipt
                        loading(1); // call loading
                        system("cls");
                        gotoxy(35,12);
                        printf("Money has been withdrew to your Account %ld \n", user.accountNumber); // show text
                        maxBorder(0,0);
                        gotoxy(45,26);system("pause");
                        break;
                    }
                    else{ // if it is false
                        user.amount -= withdraw;
                        saveTransaction(withdraw, head); // calling saveTransaction
                        loading(1); // call loading
                        gotoxy(35,12);
                        printf("Money has been withdrew to your Account %ld \n", user.accountNumber); // show text
                        maxBorder(0,0);
                        gotoxy(45,26);system("pause");
                        break;
                    }
                }
                else{ // if it is wrong pin
                    colorCode(11,0); gotoxy(40,19);printf("PHSBC_BOT:");
                    colorCode(14,0); gotoxy(50,19);printf("~$");
                    colorCode(12,0); gotoxy(53,19);printf(" WRONG PIN!");
                    colorCode(15,0); gotoxy(45,26);system("pause");
                    chance++;
                    break;
                }
            }
        }
        if(valid == true){ // valid is true
            break;
        }
    }while (chance != 3);// end of do while

    // if chances are 3, try again.
    if(chance == 3){
        system("cls");
        maxBorder(0,0);
        colorCode(11,0); gotoxy(42,14);printf("PHSBC_BOT:");
        colorCode(14,0); gotoxy(52,14);printf("~$");
        colorCode(12,0); gotoxy(55,14);printf(" PLEASE TRY AGAIN LATER!");
        colorCode(15,0); gotoxy(45,26);system("pause");
    }
}

/** FUNCTION NAME:  saveTransaction
 *  DESCRIPTION:    Function for entering the user's PIN to withdraw.
 *  PARAMETER:      @withdraw = value of user's withdraw
 *                  @head = double pointer under transac node
 */

void saveTransaction(int withdraw, transac **head){

    // Getting the TIME
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // File pointer
    FILE *write, *transac;

    char *str = "WITHDRAW";
    double oldBalance, newBalance;

    // opening the file
    write = fopen("E:\\UserAccount.txt","w");
    transac = fopen("E:\\Transaction History.txt", "a+");

    oldBalance = withdraw + user.amount;
    newBalance = user.amount;

    // print the transaction history in "Transaction History.txt"
    fprintf(transac,"WITHDRAW %.2f %d %.2lf %s %02d:%02d:%02d\n",oldBalance, withdraw, newBalance, __DATE__, tm.tm_hour, tm.tm_min, tm.tm_sec);

    // print the New Account Details in "UserAccount.txt"
    fprintf(write,"%ld\n%s\n%s\n%s\n%s\n%s\n%d\n%.2lf", user.accountNumber, user.firstname, user.middlename, user.lastname, user.birthday, user.address, user.pinNum, user.amount);

    // save the today's transaction.
    saveHistory(head, withdraw, str, oldBalance, newBalance);

    // closing the file
    fclose(write);
    fclose(transac);

    gotoxy(45,26);
    system("pause");
}

/** FUNCTION NAME:  printReceipt
 *  DESCRIPTION:    Function for displaying receipt of withdraw in "Withdraw Receipt.txt" in ATM Database Folder
 *  PARAMETER:      @withdraw = value of user's withdraw
 *                  @head = double pointer under transac node
 */

void printReceipt(int withdraw, transac **head){

    // Getting the TIME
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    FILE *write,*write1,*transac;
    char *str = "WITHDRAW";
    static int terminalID = 0001;       // static because it exists until the end of the program once initialized
    double oldBalance, newBalance;

    char fp[100];

    strcpy(fp, fullFileName);
    strcat(fp, "\\Withdraw Receipt.txt");   // getting the file path of Withdraw Receipt.txt

    write = fopen("E:\\UserAccount.txt","w");
    write1 = fopen(fp,"w"); //creating the file in ATM Database folder
    transac = fopen("E:\\Transaction History.txt", "a+");

    fprintf(write1,"***************************************************\n");
    fprintf(write1,"          PHILIPPINE SOCIAL BANK COMMERCE          \n");
    fprintf(write1,"                  Ermita, Manila\n");
    fprintf(write1,"===================================================\n");
    fprintf(write1,"   Date: %s           Time: %02d:%02d:%02d\n",__DATE__, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(write1,"   Location: TUP - MANILA      Terminal ID: %d", terminalID);
    fprintf(write1,"\n");
    fprintf(write1,"===================================================\n");
    fprintf(write1,"\n");
    fprintf(write1,"   Receipt No: %d%02d%02d%02d%02d%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(write1,"\n");
    fprintf(write1,"   Account Number: %ld\n", user.accountNumber);
    fprintf(write1,"   Account Name: %s %s\n", user.firstname, user.lastname);
    fprintf(write1,"   Amount: %d\n", withdraw);
    fprintf(write1,"   Current Balance: %.2lf\n", user.amount);
    fprintf(write1,"\n");
    fprintf(write1,"   !! ENJOY THE CONVENIENCE OF 24/7 BANKING !!\n");
    fprintf(write1,"      PAY YOUR BILLS VIA BDO ATMS AND EARN\n");
    fprintf(write1,"             PHSBC REWARDS POINTS!\n");
    fprintf(write1,"      VISIT ANY PHSBC BRANCH TO KNOW MORE.\n");
    fprintf(write1,"***************************************************\n\n");

    oldBalance = withdraw + user.amount;
    newBalance = user.amount;

    fprintf(transac,"WITHDRAW %.2f %d %.2lf %s %02d:%02d:%02d\n",oldBalance, withdraw, newBalance, __DATE__, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(write,"%ld\n%s\n%s\n%s\n%s\n%s\n%d\n%.2lf", user.accountNumber, user.firstname, user.middlename, user.lastname, user.birthday, user.address, user.pinNum, user.amount);

    saveHistory(head, withdraw, str, oldBalance, newBalance);
    terminalID++;   // increment Terminal ID

    // Closing the file
    fclose(write);
    fclose(transac);
    fclose(write1);
}

/** -------- TRANSACTION HISTORY FUNCTIONS ---------------
 *
 *  FUNCTION NAME:  saveHistory
 *  DESCRIPTION:    Function for saving transaction history
 *  PARAMETER:      @head = double pointer under transac node
 *                  @number = transaction payment
 *                  @str = type of transaction
 *                  @oldBalance = old user's amount
 *                  @newBalance = new user's amount
 */

void saveHistory(transac **head, int number, char *str, double oldBalance, double newBalance){
    transac *temp = malloc(sizeof(transac));      // allocating node

    // put data
    strcpy(temp->transaction, str);         // copying type of transaction
    sprintf(temp->date, "%s", __DATE__);    // printing string of date stored in temp->date
    sprintf(temp->time, "%s", __TIME__);    // printing string of time stored in temp->time
    temp->balance = oldBalance;             // oldBalance will be stored in temp->balance
    temp->transacAmount = number;           // number will be stored in temp->transacAmount
    temp->totalBalance = newBalance;        // newBalance will be stored in temp->total
    temp->next = NULL;                      // temp->next will be NULL

    if(NULL == *head){
        *head = temp; // move the head to point to the temp when linked list is empty
    }
    else{
        transac *p; // declare transac *p
        p = *head;  // p store the address of head
        while(NULL != p->next){
            p = p->next; // traverse the list until p is the last node. The last node always points to NULL.
        }
        p->next = temp; //Point the previous last node to the new node created.
    }
}

/** FUNCTION NAME:  showHistory
 *  DESCRIPTION:    Function for saving transaction history
 *  PARAMETER:      @head = double pointer under transac node
 */

void showHistory(transac **head){
    transac *temp;      // declaring transac *temp
    temp = *head;       // assigning head to temp
    int i = 17;         // y-axis position

    system("cls");

    loading(0);

    maxBorder(0,0);

    // If temp is NULL or transaction processed
    if(NULL == temp){
        system("cls");
        maxBorder(0,0);
        gotoxy(36,13);
        colorCode(11,0); printf("PHSBC_BOT:");
        colorCode(14,0); printf("~$");
        colorCode(12,0); printf(" No Transaction History for Today!");
    }
    else{

        system("cls");

        maxBorder(0,0);

        colorCode(15,0);
        gotoxy(14,4);   printf("  _______                             _   _               _    _ _     _                   ");
        gotoxy(14,5);   printf(" |__   __|                           | | (_)             | |  | (_)   | |                  ");
        gotoxy(14,6);   printf("    | |_ __ __ _ _ __  ___  __ _  ___| |_ _  ___  _ __   | |__| |_ ___| |_ ___  _ __ _   _ ");
        gotoxy(14,7);   printf("    | | '__/ _` | '_ \\/ __|/ _` |/ __| __| |/ _ \\| '_ \\  |  __  | / __| __/ _ \\| '__| | | |");
        gotoxy(14,8);   printf("    | | | | (_| | | | \\__ \\ (_| | (__| |_| | (_) | | | | | |  | | \\__ \\ || (_) | |  | |_| |");
        gotoxy(14,9);   printf("    |_|_|  \\__,_|_| |_|___/\\__,_|\\___|\\__|_|\\___/|_| |_| |_|  |_|_|___/\\__\\___/|_|   \\__, |");
        gotoxy(14,10);  printf("                                                                                      __/ |");
        gotoxy(14,11);  printf("                                                                                     |___/ ");

        gotoxy(16,14);  printf("|   TYPE   |  OLD  BALANCE  |    AMOUNT    |   NEW  BALANCE  |        DATE/TIME        |");
        gotoxy(16,15);  printf("|__________|________________|______________|_________________|_________________________|");

        // Traversing the list using temp node as a starting reference.
        while(NULL != temp){
            gotoxy(18,i);    printf("%-6s", temp->transaction);
            gotoxy(32,i);    printf("%.2f", temp->balance);
            gotoxy(49,i);    printf("%.2lf", temp->transacAmount);
            gotoxy(65,i);    printf("%.2lf", temp->totalBalance);
            gotoxy(80,i);    printf("%s / %s", temp->date, temp->time);

            temp = temp->next;
            i++; // increment i
        }

        // Creation of Inside Border
        for(int j=14; j<=105; j++){
            colorCode(9,0);
            gotoxy(j,13);    printf("%c",223);
        }
        for(int k=13; k<=28; k++){
            colorCode(9,0);
            gotoxy(14,k);    printf("%c",219);
            colorCode(9,0);
            gotoxy(105,k);    printf("%c",219);
        }
    }

    colorCode(15,0);
    gotoxy(45,26);
    system("pause");
}

/** -------- CHANGE PIN FUNCTION ---------------
 *
 *  FUNCTION NAME:  changePass
 *  DESCRIPTION:    Function for changing PIN of user's ATM
 *  PARAMETER:      NONE
 */

void changePass(){
    FILE *read, *temp;      // FILE Pointers

    int i;      // iterator
    int oldPin; // oldPin variable
    char a;     // a stores the character entered by the user
    bool valid = false; // set valid to false

    // Opening text files
    read = fopen("E:\\UserAccount.txt","r");
    temp = fopen("E:\\Temp.txt","w");

    do{
        system("cls");

        maxBorder(0,0);

        colorCode(4,0);
        gotoxy (38, 3);     printf("              @@@@@@@@@@@@@@@@             ");
        gotoxy (38, 4);     printf("           @@&                @@&          ");
        gotoxy (38, 5);     printf("        @@    @@@&       @@@@    @@        ");
        gotoxy (38, 6);     printf("      @@    @&                @@   @&      ");
        gotoxy (38, 7);     printf("    @@  @@                      @@  @@     ");
        gotoxy (38, 8);     printf("   @@  @                          ,@  @@   ");
        gotoxy (38, 9);     printf("   @  @@                           @&  @   ");
        gotoxy (38, 10);    printf("@@&                                     &@@");
        gotoxy (38, 11);    printf("@                                         @");
        gotoxy (38, 12);    printf("@                                         @");
        gotoxy (38, 13);    printf("@                                         @");
        gotoxy (38, 14);    printf("@                                         @");
        gotoxy (38, 15);    printf("@    @                              @     @");
        gotoxy (38, 16);    printf("@    @  @@@@@@ @@@@@@ @@@@@@(@@@@@  @     @");
        gotoxy (38, 17);    printf("@    @  @@ @@  @@ @@  @@ @@  @@ @@  @     @");
        gotoxy (38, 18);    printf("@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     @");
        gotoxy (38, 19);    printf("@                                         @");
        gotoxy (38, 20);    printf("@                                         @");
        gotoxy (38, 21);    printf("@                                         @");
        gotoxy (38, 22);    printf("@                                         @");
        gotoxy (38, 23);    printf("  &@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ");

        colorCode(2,0);
        gotoxy (43, 14);    printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*");
        gotoxy (43, 15);    printf("@                              @");
        gotoxy (43, 16);    printf("@  @@@@@@ @@@@@@ @@@@@@(@@@@@  @");
        gotoxy (43, 17);    printf("@  @@ @@  @@ @@  @@ @@  @@ @@  @");
        gotoxy (43, 18);    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

        gotoxy(45,20);   printf("Enter your Old 6-Digit Pin: ");
        gotoxy(56,21);   printf("______");
        gotoxy(56,21);   scanf("%06d", &oldPin);    // 6 - Digit Pin (Enter will be the escape character)

        if(oldPin == user.pinNum){  // if it is same
            do{
                char pin[7] = "\0";
                char cpin[7] = "\0";

                system("cls");
                maxBorder(0,0);

                colorCode(4,0);
                gotoxy (38, 3);     printf("              @@@@@@@@@@@@@@@@             ");
                gotoxy (38, 4);     printf("           @@&                @@&          ");
                gotoxy (38, 5);     printf("        @@    @@@&       @@@@    @@        ");
                gotoxy (38, 6);     printf("      @@    @&                @@   @&      ");
                gotoxy (38, 7);     printf("    @@  @@                      @@  @@     ");
                gotoxy (38, 8);     printf("   @@  @                          ,@  @@   ");
                gotoxy (38, 9);     printf("   @  @@                           @&  @   ");
                gotoxy (38, 10);    printf("@@&                                     &@@");
                gotoxy (38, 11);    printf("@                                         @");
                gotoxy (38, 12);    printf("@                                         @");
                gotoxy (38, 13);    printf("@                                         @");
                gotoxy (38, 14);    printf("@                                         @");
                gotoxy (38, 15);    printf("@    @                              @     @");
                gotoxy (38, 16);    printf("@    @  @@@@@@ @@@@@@ @@@@@@(@@@@@  @     @");
                gotoxy (38, 17);    printf("@    @  @@ @@  @@ @@  @@ @@  @@ @@  @     @");
                gotoxy (38, 18);    printf("@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     @");
                gotoxy (38, 19);    printf("@                                         @");
                gotoxy (38, 20);    printf("@                                         @");
                gotoxy (38, 21);    printf("@                                         @");
                gotoxy (38, 22);    printf("@                                         @");
                gotoxy (38, 23);    printf("  &@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ");

                colorCode(2,0);
                gotoxy (43, 14);    printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*");
                gotoxy (43, 15);    printf("@                              @");
                gotoxy (43, 16);    printf("@  @@@@@@ @@@@@@ @@@@@@(@@@@@  @");
                gotoxy (43, 17);    printf("@  @@ @@  @@ @@  @@ @@  @@ @@  @");
                gotoxy (43, 18);    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

                gotoxy(43,21);      printf("Enter your new 6-Digit Pin: ");

                // create new password
                for(i=0;;){
                    a = getch();
                    if(a == 8 && i > 0){
                        printf("\b \b");
                        pin[i] = ' ';
                        i--;
                    }
                    if(a != '\r' && a != 8 && i < 6){
                        pin[i] = a;
                        ++i;
                        printf("*");
                    }
                    if(a == '\r' || i == 6){
                        pin[i] = '\0';

                        for(int j = 0; pin[j] != '\0'; j++){    // check if the new password compose of all digits
                            if(isdigit(pin[j])){    // every character checked by isdigit function
                                valid = true;
                                continue;
                            }
                            else{ // if there's one character, it will loop again to new password.
                                gotoxy(26,25);
                                colorCode(11,0); printf("PHSBC_BOT:");
                                colorCode(14,0); printf("~$");
                                colorCode(12,0); printf(" PIN contains characters! Must have 6-DIGIT PIN! Try again!");
                                gotoxy(45,27);
                                system("pause");
                                valid = false;
                                break;
                            }
                        }

                        if(valid == true){
                            system("cls");
                            maxBorder(0,0);

                            colorCode(4,0);
                            gotoxy (38, 3);     printf("              @@@@@@@@@@@@@@@@             ");
                            gotoxy (38, 4);     printf("           @@&                @@&          ");
                            gotoxy (38, 5);     printf("        @@    @@@&       @@@@    @@        ");
                            gotoxy (38, 6);     printf("      @@    @&                @@   @&      ");
                            gotoxy (38, 7);     printf("    @@  @@                      @@  @@     ");
                            gotoxy (38, 8);     printf("   @@  @                          ,@  @@   ");
                            gotoxy (38, 9);     printf("   @  @@                           @&  @   ");
                            gotoxy (38, 10);    printf("@@&                                     &@@");
                            gotoxy (38, 11);    printf("@                                         @");
                            gotoxy (38, 12);    printf("@                                         @");
                            gotoxy (38, 13);    printf("@                                         @");
                            gotoxy (38, 14);    printf("@                                         @");
                            gotoxy (38, 15);    printf("@    @                              @     @");
                            gotoxy (38, 16);    printf("@    @  @@@@@@ @@@@@@ @@@@@@(@@@@@  @     @");
                            gotoxy (38, 17);    printf("@    @  @@ @@  @@ @@  @@ @@  @@ @@  @     @");
                            gotoxy (38, 18);    printf("@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     @");
                            gotoxy (38, 19);    printf("@                                         @");
                            gotoxy (38, 20);    printf("@                                         @");
                            gotoxy (38, 21);    printf("@                                         @");
                            gotoxy (38, 22);    printf("@                                         @");
                            gotoxy (38, 23);    printf("  &@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ");

                            colorCode(2,0);
                            gotoxy (43, 14);    printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*");
                            gotoxy (43, 15);    printf("@                              @");
                            gotoxy (43, 16);    printf("@  @@@@@@ @@@@@@ @@@@@@(@@@@@  @");
                            gotoxy (43, 17);    printf("@  @@ @@  @@ @@  @@ @@  @@ @@  @");
                            gotoxy (43, 18);    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

                            gotoxy(43,21);      printf("Retype your new 6-Digit Pin: ");

                            // retyping password
                            for(i=0;;){
                                a = getch();
                                if(a == 8 && i > 0){
                                    printf("\b \b");
                                    cpin[i] = ' ';
                                    i--;
                                }
                                if(a != '\r' && a != 8 && i < 6){
                                    cpin[i] = a;
                                    ++i;
                                    printf("*");
                                }
                                if(a == '\r' || i == 6){
                                    cpin[i] = '\0';
                                    if(strcmp(pin,cpin) == 0){  // compare pin and cpin
                                        valid = true;
                                        gotoxy(43,21);      printf("Pin has been successfully changed!!");
                                        gotoxy(45,26);
                                        system("pause");
                                        user.pinNum = atoi(pin);
                                        break;
                                    }
                                    else{
                                        gotoxy(45,23);
                                        colorCode(11,0); printf("PHSBC_BOT:");
                                        colorCode(14,0); printf("~$");
                                        colorCode(12,0); printf("Error in changing PIN! Try again!");

                                        gotoxy(45,26);
                                        system("pause");
                                        break;
                                    }
                                } // end if (line 1730)
                            } // end for loop (line 1718)
                        } // end if (line 1681)
                        break;
                    } // end if (line 1661)
                } // end for loop (line 1649)
            }while(valid != true); // end do while  (line 1609)
        } // end first if (line 1608)
        else{
            gotoxy(41,22);
            colorCode(11,0); printf("PHSBC_BOT:");
            colorCode(14,0); printf("~$");
            colorCode(12,0); printf("Error OLD PIN! Try again!");
            gotoxy(45,27);
            system("pause");
        } // end else
    }while(valid != true); // end do while loop

    // printing the details in UserAccount.txt
    fprintf(temp,"%ld\n", user.accountNumber);
    fprintf(temp,"%s\n", user.firstname);
    fprintf(temp,"%s\n", user.middlename);
    fprintf(temp,"%s\n", user.lastname);
    fprintf(temp,"%s\n", user.birthday);
    fprintf(temp,"%s\n", user.address);
    fprintf(temp,"%06d\n", user.pinNum);
    fprintf(temp,"%.2lf", user.amount);

    // closing the file
    fclose(temp);
    fclose(read);

    // remove and rename the file
    remove("E:\\UserAccount.txt");
    rename("E:\\temp.txt", "E:\\UserAccount.txt");
}


/** -------- DESIGN FUNCTIONS ---------------
 *
 *  - gotoxy Function       = positioning of text
 *  - intro Function        = moving borders
 *  - frontface Function    = splash screen
 *  - colorCode Function    = coloring of text
 *  - maxBorder Function    = border in console
 *  - loading Function      = loading screen
 */

void gotoxy(int x, int y){
    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

void intro(int x, int y){
    int j = 9, k = 12;
    int l = 117;

    for(int i=2; i<=117; i++){       //x axis
        Sleep(x);
        colorCode(j,0);
        gotoxy(i,1);     printf("%c",220);

        if (l != 2){
            Sleep(x);
            colorCode(k,0);
            gotoxy(l,28);    printf("%c",220);
        }

        if(j == 9 && k == 12){
            j = 14;
            k = 9;
        }
        else if (j == 14 && k == 9){
            j = 12;
            k = 14;
        }
        else if(j == 12 && k == 14){
            j = 9;
            k = 12;
        }

        l--;
    }

    j = 12, k = 9;
    l = 28;
    for(int i=2; i<=28; i++){        //y axis
        Sleep(y);
        colorCode(k,0);
        gotoxy(2,i);        printf("%c",219);

        if (l != 1){
            Sleep(y);
            colorCode(j,0);
            gotoxy(117,l);       printf("%c",219);
        }

        if(j == 12 && k == 9){
            j = 9;
            k = 14;
        }
        else if (j == 9 && k == 14){
            j = 14;
            k = 12;
        }
        else if(j == 14 && k == 12){
            j = 12;
            k = 9;
        }

        l--;
    }
}

void frontface(){
    char name[] = {'P','H','I','L','I','P','P','I','N','E',' ','S','O','C','I','A','L',' ','B','A','N','K',' ','A','N','D',' ','C','O','M','M','E','R','C','E'};

    intro(1,1);

        colorCode(11,0);
        Sleep(1);gotoxy(12,7);    printf(" .----------------. .----------------.");
        Sleep(1);gotoxy(12,8);    printf("| .--------------. | .--------------. ");
        Sleep(1);gotoxy(12,9);    printf("| |   ______     | | |  ____  ____  | ");
        Sleep(1);gotoxy(12,10);    printf("| |  |_   __ \\   | | | |_   ||   _| | ");
        Sleep(1);gotoxy(12,11);    printf("| |    | |__) |  | | |   | |__| |   | ");
        Sleep(1);gotoxy(12,12);    printf("| |    |  ___/   | | |   |  __  |   | ");
        Sleep(1);gotoxy(12,13);    printf("| |   _| |_      | | |  _| |  | |_  | ");
        Sleep(1);gotoxy(12,14);   printf("| |  |_____|     | | | |____||____| | ");
        Sleep(1);gotoxy(12,15);   printf("| |              | | |              | ");
        Sleep(1);gotoxy(12,16);   printf("| '--------------' | '--------------' ");
        Sleep(1);gotoxy(12,17);   printf(" '----------------' '----------------'");

        Sleep(10);
        colorCode(15,0);
        Sleep(1);gotoxy(50,7);    printf(" .----------------.");
        Sleep(1);gotoxy(50,8);    printf("| .--------------. ");
        Sleep(1);gotoxy(50,9);    printf("| |    _______   | ");
        Sleep(1);gotoxy(50,10);    printf("| |   /  ___  |  | ");
        Sleep(1);gotoxy(50,11);    printf("| |  |  (__ \\_|  | ");
        Sleep(1);gotoxy(50,12);    printf("| |   '.___`-.   | ");
        Sleep(1);gotoxy(50,13);    printf("| |  |`\\____) |  | ");
        Sleep(1);gotoxy(50,14);   printf("| |  |_______.'  | ");
        Sleep(1);gotoxy(50,15);   printf("| |              | ");
        Sleep(1);gotoxy(50,16);   printf("| '--------------' ");
        Sleep(1);gotoxy(50,17);   printf(" '----------------'");

        Sleep(20);
        colorCode(6,0);
        Sleep(1);gotoxy(69,7);    printf(" .----------------.");
        Sleep(1);gotoxy(69,8);    printf("| .--------------. ");
        Sleep(1);gotoxy(69,9);    printf("| |   ______     | ");
        Sleep(1);gotoxy(69,10);    printf("| |  |_  _  \\    | ");
        Sleep(1);gotoxy(69,11);    printf("| |   | |_) |    | ");
        Sleep(1);gotoxy(69,12);    printf("| |   |  __'.    | ");
        Sleep(1);gotoxy(69,13);    printf("| |  _| |__) |   | ");
        Sleep(1);gotoxy(69,14);    printf("| | |_______/    | ");
        Sleep(1);gotoxy(69,15);   printf("| |              | ");
        Sleep(1);gotoxy(69,16);   printf("| '--------------' ");
        Sleep(1);gotoxy(69,17);   printf(" '----------------'");

        Sleep(30);
        colorCode(12,0);
        Sleep(1);gotoxy(88,7);    printf(" .----------------. ");
        Sleep(1);gotoxy(88,8);    printf("| .--------------. |");
        Sleep(1);gotoxy(88,9);    printf("| |     ______   | |");
        Sleep(1);gotoxy(88,10);    printf("| |   .' ___  |  | |");
        Sleep(1);gotoxy(88,11);    printf("| |  / .'   \\_|  | |");
        Sleep(1);gotoxy(88,12);    printf("| |  | |         | |");
        Sleep(1);gotoxy(88,13);    printf("| |  \\ `.___.'\\  | |");
        Sleep(1);gotoxy(88,14);    printf("| |   `._____.'  | |");
        Sleep(1);gotoxy(88,15);   printf("| |              | |");
        Sleep(1);gotoxy(88,16);   printf("| '--------------' |");
        Sleep(1);gotoxy(88,17);   printf(" '----------------' ");


    gotoxy(45,20);
    int k = 9;
    for(int i = 0; i < strlen(name); i++){
        Sleep(50);

        colorCode(k,0);
        printf("%c", name[i]);
        if(i <= 9){
            k = 9;
        }
        else if (i <= 20){
            k = 14;
        }
        else if(i <= 21){
            k = 12;
        }
    }

    gotoxy(46,23);
    Sleep(100);system("pause");
}

void colorCode(int ForeGroundColor,int BackGroundColor){                                // colors the text and background
   int color=16*BackGroundColor+ForeGroundColor;
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

void maxBorder(int x, int y){
    int j = 9, k = 12;
    for(int i=2; i<=117; i++){       //x axis
        Sleep(x);
        colorCode(j,0);
        gotoxy(i,1);     printf("%c",220);
        colorCode(k,0);
        Sleep(x);
        gotoxy(i,28);    printf("%c",220);

        if(j == 9 && k == 12){
            j = 14;
            k = 9;
        }
        else if (j == 14 && k == 9){
            j = 12;
            k = 14;
        }
        else if(j == 12 && k == 14){
            j = 9;
            k = 12;
        }
    }

    j = 12, k = 9;
    for(int i=2; i<=28; i++){        //y axis
        Sleep(y);
        colorCode(k,0);
        gotoxy(2,i);        printf("%c",219);
        Sleep(y);
        colorCode(j,0);
        gotoxy(117,i);       printf("%c",219);

        if(j == 12 && k == 9){
            j = 9;
            k = 14;
        }
        else if (j == 9 && k == 14){
            j = 14;
            k = 12;
        }
        else if(j == 14 && k == 12){
            j = 12;
            k = 9;
        }
    }
}

void loading(int a){
    int r;

    system("cls");

    maxBorder(0,0);

    colorCode(11,0);
    gotoxy(12,7);       printf(" .----------------. .----------------.");
    gotoxy(12,8);       printf("| .--------------. | .--------------. ");
    gotoxy(12,9);       printf("| |   ______     | | |  ____  ____  | ");
    gotoxy(12,10);      printf("| |  |_   __ \\   | | | |_   ||   _| | ");
    gotoxy(12,11);      printf("| |    | |__) |  | | |   | |__| |   | ");
    gotoxy(12,12);      printf("| |    |  ___/   | | |   |  __  |   | ");
    gotoxy(12,13);      printf("| |   _| |_      | | |  _| |  | |_  | ");
    gotoxy(12,14);      printf("| |  |_____|     | | | |____||____| | ");
    gotoxy(12,15);      printf("| |              | | |              | ");
    gotoxy(12,16);      printf("| '--------------' | '--------------' ");
    gotoxy(12,17);      printf(" '----------------' '----------------'");

    colorCode(15,0);
    gotoxy(50,7);       printf(" .----------------.");
    gotoxy(50,8);       printf("| .--------------. ");
    gotoxy(50,9);       printf("| |    _______   | ");
    gotoxy(50,10);      printf("| |   /  ___  |  | ");
    gotoxy(50,11);      printf("| |  |  (__ \\_|  | ");
    gotoxy(50,12);      printf("| |   '.___`-.   | ");
    gotoxy(50,13);      printf("| |  |`\\____) |  | ");
    gotoxy(50,14);      printf("| |  |_______.'  | ");
    gotoxy(50,15);      printf("| |              | ");
    gotoxy(50,16);      printf("| '--------------' ");
    gotoxy(50,17);      printf(" '----------------'");

    colorCode(6,0);
    gotoxy(69,7);       printf(" .----------------.");
    gotoxy(69,8);       printf("| .--------------. ");
    gotoxy(69,9);       printf("| |   ______     | ");
    gotoxy(69,10);      printf("| |  |_  _  \\    | ");
    gotoxy(69,11);      printf("| |   | |_) |    | ");
    gotoxy(69,12);      printf("| |   |  __'.    | ");
    gotoxy(69,13);      printf("| |  _| |__) |   | ");
    gotoxy(69,14);      printf("| | |_______/    | ");
    gotoxy(69,15);      printf("| |              | ");
    gotoxy(69,16);      printf("| '--------------' ");
    gotoxy(69,17);      printf(" '----------------'");

    colorCode(12,0);
    gotoxy(88,7);       printf(" .----------------. ");
    gotoxy(88,8);       printf("| .--------------. |");
    gotoxy(88,9);       printf("| |     ______   | |");
    gotoxy(88,10);      printf("| |   .' ___  |  | |");
    gotoxy(88,11);      printf("| |  / .'   \\_|  | |");
    gotoxy(88,12);      printf("| |  | |         | |");
    gotoxy(88,13);      printf("| |  \\ `.___.'\\  | |");
    gotoxy(88,14);      printf("| |   `._____.'  | |");
    gotoxy(88,15);      printf("| |              | |");
    gotoxy(88,16);      printf("| '--------------' |");
    gotoxy(88,17);      printf(" '----------------' ");

    if (a == 1){
        colorCode(14,0);    gotoxy(24,19);  printf("Make your Cash Withdrawal easier with Philippine Social Bank and Commerce!");
        colorCode(15,0);    gotoxy(55,25);  printf("Please wait");
    }
    else if (a == 2){
        colorCode(14,0);    gotoxy(15,19);  printf("Your money is more secured when you Cash Deposit with Philippine Social Bank and Commerce!");
        colorCode(15,0);    gotoxy(55,25);  printf("Please wait");
    }
    else{
        colorCode(14,0);    gotoxy(20,19);  printf("ATM Problems? Go to the nearest Philippine Social Bank and Commerce for assistance!");
        colorCode(15,0);    gotoxy(55,25);  printf("Please wait");
    }

    colorCode(12,0);
    gotoxy(5,27);
    for(r=1; r<=110; r++){ //This loop is for how many times to print unicode on screen
        printf("%c",219);
    }

    colorCode(11,0);
    gotoxy(5,27);
    for(r=1; r<=110; r++){ //This loop is for how many times to print unicode on screen
        Sleep(50);
        printf("%c",219);
    }
}
