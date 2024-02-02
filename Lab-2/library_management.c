#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Book
{
    char acq_date[20];
    char id[10];
    char author[25];
    char pub_date[20];
    char title[20];
    char genre[10];
    char ISBN[20];
    int quantity;
};


void addBook(struct Book* book)
{
    FILE* file = fopen("library.dat","ab+");
    if(file != NULL)
    {
        fwrite(book, sizeof(struct Book),1,file);
        fclose(file);
    }
    else
    {
        printf("\nError opening file\n");
    }

    FILE* transac = fopen("transactions.dat","ab");
    if(transac != NULL)
    {
        fprintf(transac,"New book added. Id = %s\n",book->id);
    }else
    {
        printf("\nError opening transactions file\n");
    }
    fclose(transac);
    fclose(file);

}

void check_details()
{
    int choice;
    printf("By id: press 1 \nBy Title: press 2");
    scanf("%d",&choice);
    FILE* file = fopen("library.dat","rb");
    if(choice == 1)
    {
        char target_id[10];
        printf("Enter target id: ");
        scanf("%s",target_id);
        if(file != NULL)
        {
            struct Book* book = (struct Book*)malloc(sizeof(struct Book));
            while(fread(book, sizeof(struct Book),1,file) == 1)
            {
                if(strcmp(book->id,target_id) == 0)
                {
                    printf("\nDetails found :-\n");
                    printf("Acquisition date: %s\n",book->acq_date);
                    printf("Book id: %s\n",book->id);
                    printf("Author name %s\n",book->author);
                    printf("Publication date: %s\n",book->pub_date);
                    printf("Book title: %s\n",book->title);
                    printf("Genre: %s\n",book->genre);
                    printf("ISBN number: %s\n",book->ISBN);
                    printf("Quantity: %d\n\n",book->quantity);

                    FILE* transac = fopen("transactions.dat","ab");
                    if(transac != NULL)
                    {
                        fprintf(transac,"Details of a book with Id = %s was checked\n",target_id);
                    }else
                    {
                        printf("Error opening transactions file");
                    }
                    fclose(transac);
                    return;
                }
            }
            printf("\nThe book with the given id does not exist\n");
        }else
        {
            printf("An error occurred");
        }


    }
    else if(choice == 2)
    {
        char target_title[20];
        printf("Enter title: ");
        scanf("%s",target_title);
        if(file != NULL)
        {
            struct Book* book = (struct Book*)malloc(sizeof(struct Book));
            while(fread(book, sizeof(struct Book),1,file) == 1)
            {

                if(strcmp(book->title,target_title) == 0)
                {
                    printf("\nDetails found :-\n");
                    printf("Acquisition date: %s\n",book->acq_date);
                    printf("Book id: %s\n",book->id);
                    printf("Author name %s\n",book->author);
                    printf("Publication date: %s\n",book->pub_date);
                    printf("Book title: %s\n",book->title);
                    printf("Genre: %s\n",book->genre);
                    printf("ISBN number: %s\n",book->ISBN);
                    printf("Quantity: %d\n\n",book->quantity);

                    FILE* transac = fopen("transactions.dat","ab");
                    if(transac != NULL)
                    {
                        fprintf(transac,"Details of a book with title = %s was checked\n",target_title);
                    }else
                    {
                        printf("\nError opening transactions file\n");
                    }
                    fclose(transac);
                    return;
                }
            }
            printf("The book with the given title does not exist");
        }


    }
    else
    {
        printf("\nInvalid choice\n");
    }
    fclose(file);

}

void update_record(char id[10])
{
    FILE* file = fopen("library.dat","r+b");
    if(file != NULL)
    {
        struct Book* book = (struct Book*)malloc(sizeof(struct Book));
        while(fread(book, sizeof(struct Book),1,file) == 1)
        {
            if(strcmp(book->id,id) == 0)
            {
                int qty,c;
                char pub_date[20];
                printf("Do you want to update the number of books available? Press 1 for yes and 0 for no: ");
                scanf("%d",&c);
                if(c == 1)
                {
                    printf("Enter the quantity: ");
                    scanf("%d",&qty);
                    book->quantity = qty;
                }
                printf("Do you want to change the publication date? Press 1 for yes and 0 for no: ");
                scanf("%d",&c);
                if(c == 1)
                {
                    printf("Enter the new publication date: ");
                    scanf("%s",pub_date);
                    strcpy(book->pub_date,pub_date);
                }
                fseek(file, -sizeof(struct Book), SEEK_CUR);
                fwrite(book,sizeof(struct Book),1,file);
                fclose(file);

                FILE* transac = fopen("transactions.dat","ab");
                if(transac != NULL)
                {
                    fprintf(transac,"Details of a book with Id = %s was updated\n",book->id);
                }else
                {
                    printf("Error opening transactions file");
                }
                fclose(transac);
                return;
            }
        }
        printf("Book with given id is not found :-(");
    }
    fclose(file);
}

void remove_record()
{
    char id[10];
    printf("Enter id of the record to be removed: ");
    scanf("%s",id);
    int check = 0;

    FILE* file = fopen("library.dat","r+b");
    if(file != NULL)
    {
        FILE* temp = fopen("temp.dat","ab");
        struct Book* book = (struct Book*)malloc(sizeof(struct Book));
        while(fread(book,sizeof(struct Book),1,file) == 1)
        {
            if(strcmp(book->id,id) != 0)
            {
                fwrite(book,sizeof(struct Book),1,temp);
            }else{
                check++;
            }
        }
        fclose(temp);
        fclose(file);
        remove("library.dat");
        rename("temp.dat","library.dat");
        if(check == 0)
        {
            printf("\nThe book with the given id does not exist\n");
            return;
        }
        printf("Record deleted successfully!");

        FILE* transac = fopen("transactions.dat","ab");
        if(transac != NULL)
        {
            fprintf(transac,"A book with Id = %s was removed\n",book->id);
        }else
        {
            printf("Error opening transactions file");
        }
        fclose(transac);
    }else
    {
        printf("Error opening the file!");
    }
    fclose(file);

}

void view_catalog()
{
    FILE* file = fopen("library.dat","rb");
    int i = 1;
    if(file != NULL)
    {
        printf("Acquisition date   Book id   Author name  Publication date  Book title  Genre  ISBN number  Quantity");
        struct Book* book = (struct Book*)malloc(sizeof(struct Book));
        while(fread(book, sizeof(struct Book),1,file) == 1)
        {
            //printf("\nBook %d\n",i);
            printf("%s",book->acq_date);
            printf("%s",book->id);
            printf("%s",book->author);
            printf("Publication date: %s",book->pub_date);
            printf("%s",book->title);
            printf("%s",book->genre);
            printf("%s",book->ISBN);
            printf("%d",book->quantity);
            printf("\n\n");
            i++;
        }

        FILE* transac = fopen("transactions.dat","ab+");
        if(transac != NULL)
        {
            fprintf(transac,"Catalog was viewed\n");
        }else
        {
            printf("Error opening transactions file");
        }
         fclose(transac);
    }else{
        printf("File not found");
    }
    fclose(file);
}

void printTransactions() {
    FILE* transac = fopen("transactions.dat", "rb");

    if (transac != NULL) {
        char buffer[100];

        while (fgets(buffer, sizeof(buffer), transac) != NULL) {
            printf("%s", buffer);
        }

        fclose(transac);
    } else {
        printf("Error opening transactions file\n");
    }
}



int main()
{
   char acq_date[20];
   char id[10];
   char author[25];
   char pub_date[20];
   char title[20];
   char genre[10];
   char ISBN[20];
   int quantity;
   int isExit = 0;
   int choice;
   while(!isExit)
   {
   printf("\nEnter 1 to add a new book.\nEnter 2 to update the details of a specific book.\nEnter 3 to print the transaction history.\nEnter 4 to check details of a specific book.\nEnter 5 to remove a specific book.\nEnter 6 to view the catalog.\nEnter 7 to exit.\nEnter your choice:");
   scanf("%d",&choice);
   switch(choice)
   {
    case 1:
        printf("Enter acquisition date: ");
        scanf("%s",acq_date);
        printf("Enter book id: ");
        scanf("%s",id);
        printf("Enter author name: ");
        scanf("%s",author);
        printf("Enter publication date: ");
        scanf("%s",pub_date);
        printf("Enter book title: ");
        scanf("%s",title);
        printf("Enter genre: ");
        scanf("%s",genre);
        printf("Enter ISBN number: ");
        scanf("%s",ISBN);
        printf("Enter quantity: ");
        scanf("%d",&quantity);

        struct Book* temp = (struct Book*)malloc(sizeof(struct Book));
        strcpy(temp->acq_date,acq_date);
        strcpy(temp->id,id);
        strcpy(temp->author,author);
        strcpy(temp->pub_date,pub_date);
        strcpy(temp->title,title);
        strcpy(temp->genre,genre);
        strcpy(temp->ISBN,ISBN);
        temp->quantity = quantity;
        addBook(temp);
        break;
    case 2:
        printf("Enter the id of the book you want to update: ");
        scanf("%s",id);
        update_record(id);
        break;
    case 3:
        printTransactions();
        break;
    case 4:
        check_details();
        break;
    case 5:
        remove_record();
        break;
    case 6:
        view_catalog();
        break;
    case 7:
        isExit = 1;
        break;
   }

   }
}
