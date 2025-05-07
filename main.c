#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>


#define MAX_ACCOUNTS 100

struct Account{
    char name[100];
    int accNum;
    char pin[10];
    float balance;
} ;
struct Account accounts[MAX_ACCOUNTS];
int accountCount=0;

void savetoFile();
void loadfromFile();
void logTransaction(int userIndex, const char *type, float amount);



void createAccount();
int login();
void atmMenu(int userIndex);
void checkBalance(int userIndex);
void depositMoney(int userIndex);
void withdrawMoney(int userIndex);
void viewTransactionHistory(int userIndex);

void createAccount()
{
    struct Account newAccount;
    if(accountCount>=MAX_ACCOUNTS){
        printf("Maximum number of accounts created\n");
        return;
    }
    printf("Enter name of account holder:\n");
    getchar();
    fgets(newAccount.name,sizeof(newAccount.name),stdin);
    newAccount.name[strcspn(newAccount.name,"\n")]='\0';

    printf("Enter account number");
    scanf("%d",&newAccount.accNum);

    printf("Set a 4 digit pin");
    scanf("%s",newAccount.pin);

    newAccount.balance=0.0;
    accounts[accountCount++]=newAccount; //stores the new account in the array with the account count index
    printf("Account created successfully!");
    savetoFile();
 
}
int login(){
    int accNo;
    char enteredPin[10];

    printf("Enter account number");
    scanf("%d",&accNo);

    printf("Enter pin");
    scanf("%s",enteredPin);

    for(int i=0;i<accountCount;i++)
    {
        if(accounts[i].accNum==accNo && strcmp(accounts[i].pin,enteredPin)==0){
            printf("Login successful! Welcome, %s",accounts[i].name);
            return i;
        }
    }
    return -1;
}

void atmMenu(int userIndex){
    int choice;
    while(1){
        printf("\n ATM Menu\n");
        printf("1. Check Balance\n");
        printf("2.Deposit Money\n");
        printf("3.Withdraw Money\n");
        printf("4.View transaction history\n");
        printf("5.Logout\n");
        scanf("%d",&choice);

        switch(choice)
{
    case 1:
    checkBalance(userIndex);
    break;

    case 2:
    depositMoney(userIndex);
    break;

    case 3:
    withdrawMoney(userIndex);
    break;

    case 4:
    viewTransactionHistory(userIndex);
    break;

    case 5:
    printf("Logging out...\n");
    return;

    default:
    printf("Invalid choice\n");
}    }
}

void checkBalance(int userIndex){
    printf("Current balance: %.2f\n",accounts[userIndex].balance);
}

void withdrawMoney(int userIndex){
    float bal;
    bal=accounts[userIndex].balance;
    printf("Enter amount you want to withdraw");
    float sum;
    
    if(scanf("%f",&sum)!=1||sum>bal){
    printf("Invalid input");
    return;}
    else{
        bal=bal-sum;
        accounts[userIndex].balance=bal;
        printf("Amount has been withdrawn");
        savetoFile();
        logTransaction(userIndex,"Withdrawal",sum);
    }
}
void depositMoney(int userIndex){
    float sum;
    printf("Enter amount you want to deposit");
    if(scanf("%f",&sum)!=1||sum<=0){
    printf("Invalid input");
    while (getchar() != '\n');
        return;}
    accounts[userIndex].balance+=sum;
    printf("Amount has been deposited\n");
    printf("New balance: %.2f",accounts[userIndex].balance);
    savetoFile();
    logTransaction(userIndex,"Deposit",sum);

}

void savetoFile()
{
FILE *f=fopen("account.txt","w");
if(f==NULL){
    printf("Error opening file\n");
    return;
}
for(int i=0;i<accountCount;i++)
{
    fprintf(f,"%s\n %d\n %s\n %.2f\n",accounts[i].name,accounts[i].accNum,accounts[i].pin, accounts[i].balance);

}
fclose(f);

}

void loadfromFile(){
    FILE *f = fopen("account.txt", "r");
    if(f==NULL)
    return;
    accountCount=0;
    while (fgets(accounts[accountCount].name, sizeof(accounts[accountCount].name), f)) {
        accounts[accountCount].name[strcspn(accounts[accountCount].name, "\n")] = '\0'; // remove newline

        fscanf(f, "%d\n", &accounts[accountCount].accNum);
        fgets(accounts[accountCount].pin, sizeof(accounts[accountCount].pin), f);
        accounts[accountCount].pin[strcspn(accounts[accountCount].pin, "\n")] = '\0';

        fscanf(f, "%f\n", &accounts[accountCount].balance);

        accountCount++;
    }

fclose(f);
}


void logTransaction(int userIndex, const char *type, float amount) {
    char filename[50];
    sprintf(filename, "transactions_%d.txt", accounts[userIndex].accNum);

    FILE *f = fopen(filename, "a"); 
    if (f == NULL) {
        printf("Error logging transaction.\n");
        return;
    }

time_t now = time(NULL);
fprintf(f, "[%s] %s: %.2f | Balance: %.2f\n", ctime(&now), type, amount, accounts[userIndex].balance);



    fclose(f);
}

void viewTransactionHistory(int userIndex) {
    char filename[50];
    sprintf(filename, "transactions_%d.txt", accounts[userIndex].accNum);

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("No transaction history found for this account.\n");
        return;
    }

    char line[200];
    printf("\n--- Transaction History ---\n");
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    fclose(f);
    printf("---------------------------\n");
}



int main()
   {
    loadfromFile();
    int choice;
    while(1){
        printf("\n=======ATM System=======\n");
        printf("1. Create Account\n");
        printf("2.Login\n");
        printf("3.Exit\n");
        printf("Choose an option");
        scanf("%d",&choice);

        switch (choice){
            case 1:
            createAccount();
            break;

            case 2:{
            int userIndex;
            userIndex=login();
            if (userIndex != -1) {
                atmMenu(userIndex);
            } else {
                printf("Login failed.\n");
            }
            break;}

            case 3:
            printf("Thank you for using our ATM system!");
            return 0;

            default:
            printf("Invalid choice");
        }}
    }


