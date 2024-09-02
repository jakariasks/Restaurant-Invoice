#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<windows.h>

void menu();
void creat_new_invoice();
void show_all_invoice();
void show_invoice();
void generate_bill_header(char name[50], char date[30]);
void generate_bill_body(char item[30], int qty, float price);
void generate_bill_footer(float total);
void print_line();
void gotoxy(int x, int y);

typedef struct
{
    char item[20];
    float price;
    int qty;
} Items;

typedef struct
{
    char customer[50];
    char date[50];
    int number_of_items;
    Items itm[50];
} Orders;

int main()
{
    int opt, n;
    char cont_flag = 'y';
    // Dashboard
    while (cont_flag == 'y')
    {
        system("cls");


        printf("\t============ SKS RESTAURANT ============\n");
        printf("\n\nPlease Select Your Preferred Operation");
        printf("\n\n1. Generate Invoice");
        printf("\n\n2. Show All Invoices");
        printf("\n\n3. Search Invoice");
        printf("\n\n4. Exit");

        printf("\n\nYour Choice:\t");
        scanf("%d", &opt);
        fgetc(stdin); // To consume the newline character left by scanf

        switch (opt)
        {
        case 1:
            creat_new_invoice();
            break;

        case 2:
            show_all_invoice();
            break;

        case 3:
            show_invoice();
            break;

        case 4:
            printf("\n\t\tBye Bye :)\n\n");
            exit(0);

        default:
            printf("Sorry, invalid option.");
            break;
        }

        printf("\nDo you want to perform another operation? [y/n]:\t");
        scanf(" %c", &cont_flag); // Notice the space before %c to skip any leftover newline
    }

    printf("\n\t\tBye Bye :)\n\n");

    return 0;
}



void creat_new_invoice()
{
    int opt, n;
    float total = 0;
    Orders ord;
    char save_bill = 'y';
    system("cls");
    printf("\nPlease Enter The Name of the Customer:\t");
    fgets(ord.customer, 50, stdin);
    ord.customer[strlen(ord.customer) - 1] = 0; // Remove newline character
    strcpy(ord.date, __DATE__);
    printf("\nPlease Enter the Number of Items:\t");
    scanf("%d", &n);
    ord.number_of_items = n;
    for (int i = 0; i < n; i++)
    {
        fgetc(stdin); // Consume the newline character left by scanf
        printf("\n\n");
        printf("Please Enter Item %d:\t", i + 1);
        fgets(ord.itm[i].item, 20, stdin);
        ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0; // Remove newline character
        printf("Please Enter The Quantity:\t");
        scanf("%d", &ord.itm[i].qty);
        printf("Please Enter The Unit Price:\t");
        scanf("%f", &ord.itm[i].price);
        total += ord.itm[i].qty * ord.itm[i].price;
    }
    generate_bill_header(ord.customer, ord.date);
    for (int i = 0; i < ord.number_of_items; i++)
    {
        generate_bill_body(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
    }
    generate_bill_footer(total);

    printf("\n\nDo You Want to Save the Invoice? [y/n]:\t");
    scanf(" %c", &save_bill); // Notice the space before %c to skip any leftover newline

    if (save_bill == 'y')
    {
        FILE *fp;
        fp = fopen("RestaurantBill.dat", "a+");
        if (fp == NULL)
        {
            printf("\n\nError opening file!");
            exit(1);
        }
        fwrite(&ord, sizeof(Orders), 1, fp);
        if (fwrite != 0)
            printf("\n\nSuccessfully Saved");
        else
            printf("\n\nError Saving");
        fclose(fp);
    }
}


void show_all_invoice()
{
    FILE *fp;
    Orders ord;
    system("cls");
    fp = fopen("RestaurantBill.dat", "r");
    if (fp == NULL)
    {
        printf("\n\nError opening file!");
        exit(1);
    }
    printf("\n  ***** Your Previous Invoices *****\n");
    while (fread(&ord, sizeof(Orders), 1, fp))
    {
        float tot = 0;
        generate_bill_header(ord.customer, ord.date);

        for (int i = 0; i < ord.number_of_items; i++)
        {
            generate_bill_body(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
            tot += ord.itm[i].qty * ord.itm[i].price;
        }
        generate_bill_footer(tot);
    }
    fclose(fp);
}
void show_invoice()
{
    FILE *fp;
    Orders ord;
    int invoice_found = 0;
    char name[50];
    printf("Enter The Name of the Customer:\t");
    fgets(name, 50, stdin);
    name[strlen(name) - 1] = 0; // Remove newline character
    system("cls");

    fp = fopen("RestaurantBill.dat", "r");
    if (fp == NULL)
    {
        printf("\n\nError opening file!");
        exit(1);
    }

    printf("\t***** Invoice for %s *****\n", name);
    while (fread(&ord, sizeof(Orders), 1, fp))
    {
        float tot = 0;

        if (strcmp(ord.customer, name) == 0)
        {
            generate_bill_header(ord.customer, ord.date);
            for (int i = 0; i < ord.number_of_items; i++)
            {
                generate_bill_body(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                tot += ord.itm[i].qty * ord.itm[i].price;
            }
            generate_bill_footer(tot);
            invoice_found = 1;
        }
    }
    if (!invoice_found)
    {
        printf("Sorry, the invoice for %s was not found.", name);
    }
    fclose(fp);
}





// Function to generate bill header
void generate_bill_header(char name[50], char date[30])
{
    printf("\n\n");
    printf("\t     SKS. Restaurant");
    print_line();
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    print_line();
    printf("\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n");
    print_line();
    printf("\n\n");
}

// Function to generate bill body
void generate_bill_body(char item[30], int qty, float price)
{
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

// Function to generate bill footer
void generate_bill_footer(float total)
{
    printf("\n");
    float discount = 0.1 * total;
    float netTotal = total - discount;
    float VAT = 0.09 * netTotal;
    float grand_total = netTotal + 2 * VAT;
    print_line();
    printf("\nSub Total \t\t\t%0.2f", total);
    printf("\nDiscount @10%s\t\t\t %0.2f", "%", discount);
    printf("\n\t\t\t    -----------");
    print_line();
    printf("\nNet Total \t\t\t%0.2f", netTotal);
    printf("\nVat @9%s  \t\t\t%0.2f", "", VAT);
    print_line();
    printf("\nGrand Total \t\t\t%0.2f", grand_total);
    print_line();
}

// Function to print a line
void print_line()
{
    printf("\n");
    for (int i = 0; i < 40; i++)
    {
        printf("-");
    }
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
