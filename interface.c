#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void add()
{
    int fd;
    if ((fd = open("/dev/char_dev", O_WRONLY)) == -1)
    {
        printf("Error in open\n");
    }

    char message[200] = {0};
    char command[210] = {0};

    printf("Enter the data to enter : \n");
    fflush(stdout);
    fgets(message, sizeof(message), stdin);

    strcpy(command, "!add ");
    strcat(command, message);

    write(fd, command, strlen(command));

    printf("Successfully added\n");

    close(fd);
}

void change()
{
    int fd = open("/dev/char_dev", O_WRONLY);

    int num;
    char command[15] = {0};
    char num_to_char[10] = {0};

    printf("Enter the location at which you want the head to point : ");
    scanf("%d", &num);

    sprintf(num_to_char, "%d", num);
    strcpy(command, "!change ");
    strcat(command, num_to_char);

    int err = write(fd, command, strlen(command));

    if (err == -1)
        printf("There was an error\n");
    else
        printf("Successfully changed\n");

    close(fd);
}

void print()
{
    int fd = open("/dev/char_dev", O_WRONLY);

    char command[] = "!print";

    write(fd, command, strlen(command));

    printf("Printed successfully\nWrite sudo dmesg in the terminal to see the stored strings\n");

    close(fd);
}

void pop_back()
{
    int fd = open("/dev/char_dev", O_WRONLY);

    char command[] = "!popback";

    write(fd, command, strlen(command));

    printf("String popped successfully\n");

    close(fd);
}

void get_curr_string()
{
    int fd = open("/dev/char_dev", O_RDONLY);

    char message[200] = {0};

    read(fd, message, sizeof(message));

    printf("String read : %s\n", message);

    close(fd);
}

int main()
{
    int choice;
    bool quit = false;

    while (!quit)
    {
        printf("\n\nMAIN MENU\n\n\nEnter 1 to add a new string\nEnter 2 to change the head\n3Enter 3 to print\nEnter 4 to pop\nEnter 5 to read the current string\nEnter 6 to quit\n\n");

        scanf("%d", &choice);

        char dummy[3];
        fgets(dummy, 2, stdin);

        switch (choice)
        {
        case 1:
            add();
            break;
        case 2:
            change();
            break;
        case 3:
            print();
            break;
        case 4:
            pop_back();
            break;
        case 5:
            get_curr_string();
            break;
        case 6:
            quit = true;
            break;
        default:
            printf("Functionality not defined\n");
            break;
        }
    }

    return 0;
}