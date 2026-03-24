#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define students "file.txt"
#define requests "file1.txt"
#define teachers "file2.txt"
#define logins "file3.txt"
#define marks "file4.txt"


// struct of personal information (first name, last name, id)
typedef struct {
    int id;
    char fn[20],ln[20];
} info;

// struct of date (day, month, year)
typedef struct {
    int d,m,y;
} date;

// struct of student
typedef struct {
    info p_info;
    date birthday;
    char subject[15];
    bool active;
} student;

// struct of job application (request for working)
typedef struct {
    info p_info;
    char subject[15];
    char date_r[12];
    char reply[6];
} job_app;

// struct of teacher (id_pre = the id of teacher in file of job app so the teacher have two ids (the first when he made a job app and second after registering him in file of teacher))
typedef struct {
    info p_info;
    char subject[15];
    bool active;
    int id_pre;
} teacher;

// struct of login's information ( id & password)
typedef struct {
    int id;
    char password[9];
} login;

// struct of marks (student's id, teacher's id, student's mark, the subject)
typedef struct {
    int id_s,id_t;
    float mark;
    char subject[15];
} mark;

// generate password for new registered teachers
char * generate_password(){
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz#%!@&_-+$";
    char * word = malloc(9);
    int i;
    for ( i = 0; i <= 8; i++){
        if (i != 8)
            *(word + i) = characters[rand() % strlen(characters)];
        else
            *(word + i) = '\0';
    }
    return word;
}

// check if password is unique (two teacher or more don't have the same password)
bool is_password_unique(char password[]){
    FILE *f = fopen(logins,"rb");
    login object;
    while (fread(&object,sizeof(object),1,f) == 1) {
        if(strcmp(object.password,password) == 0){
            fclose(f);
            return false;
        }
    }
    fclose(f);
    f = NULL;
    return true;
}

// generate id of student (start from 1000)
int generate_id_st(){
    FILE *f = NULL;
    f = fopen(students,"rb");
    if (!f) exit(1);
    fseek(f,0,SEEK_END);
    int n = ftell(f)/sizeof(student);
    fclose(f);
    f = NULL;
    return n+1000;
}

// add student in student's file
void add_student (FILE **k){
    int c;
    student object;
    printf("\n---------- ADD STUDENT ----------\n");
    object.p_info.id = generate_id_st();
    printf("First name: ");
    scanf("%19s",object.p_info.fn);
    printf("Last name: ");
    scanf("%19s",object.p_info.ln);
    printf("Date of birth .. day: ");
    scanf("%d",&object.birthday.d);
    printf("Month: ");
    scanf("%d",&object.birthday.m);
    printf("Year: ");
    scanf("%d",&object.birthday.y);
    printf("Subject: ");
    scanf("%14s",object.subject);
    object.active = true;
    printf("---------------------------------\n");
    *k = fopen(students,"ab");
    if (!(*k)) exit(1);
    fseek(*k,0,SEEK_END);
    fwrite(&object,sizeof(object),1,*k);
    fclose(*k);
    *k = NULL;
    printf("===>|ADDED SUCCESSFULLY|<===\n");
}

// delete student (deleted_flag == 0 or false)
void delete_student (FILE **k){
    int id;
    printf("\n----------- DELETE STUDENT -----------\n");
    printf("Enter the id of student: ");
    scanf("%d",&id);
    if (id < generate_id_st()){
        student object;
        *k = fopen(students,"rb+");
        fseek(*k,(id - 1000)*sizeof(object),SEEK_SET);
        fread(&object,sizeof(object),1,*k);
        object.active = false;
        fseek(*k,-sizeof(object),SEEK_CUR);
        fwrite(&object,sizeof(object),1,*k);
        fclose(*k);
        *k = NULL;
        printf("===>|DELETED SUCCESSFULLY|<===\n");
    }
    else
        printf("There is not student with this id: %d\n",id);
    printf("-------------------------------------\n");
}

// list dtudents (all student with their first and last name, birthday, id, subject and deleted flag)
void display_student(){
    FILE *f = NULL;
    f = fopen(students,"rb");
    if (!f) exit(1);
    student object;
    printf("\n---------- LIST OF STUDENT ----------\n");
    while (fread(&object,sizeof(object),1,f) != 0){
        sleep(1);
        printf("|%05d|%-20s|%-20s|%02d-%02d-%04d|%-15s|%d|\n",object.p_info.id,object.p_info.fn,object.p_info.ln,object.birthday.d,object.birthday.m,object.birthday.y,object.subject,object.active);
    }
    printf("-------------------------------------\n");
    fclose(f);
    f = NULL;
}

// student's mark (helpful function to (id_card_s))
void mark_of_student(int id){
    FILE * f = fopen(marks,"rb");
    mark object;
    while (fread(&object,sizeof(object),1,f) == 1) {
        if (object.id_s == id){
            printf("Mark : %04.2f\n",object.mark);
            break;
        }
    }
}


// id card (display the information of student after he enter his id)
void id_card_s(){
    FILE *f;
    int id;
    printf("Enter your id: ");
    scanf("%d",&id);
    printf("\n---------- STUDENT'S ID CARD ----------\n");
    if (id < generate_id_st()){
        f = fopen(students,"rb");
        student object;
        fseek(f,(id - 1000)*sizeof(object),SEEK_SET);
        fread(&object,sizeof(object),1,f);
        if (!object.active)
            printf("You are deleted\n");
        else{
            printf("ID: %d \nFirst_name: %s \nLast_name: %s \nSubject: %s\n",object.p_info.id,object.p_info.fn,object.p_info.ln,object.subject);
            mark_of_student(object.p_info.id);
        }
        fclose(f);
    }
    else
        printf("There is not student with this id: %d\n",id);
    printf("---------------------------------------\n");
}

// generate id of job application (started from 0)
int generate_id_ja(){
    FILE *f = NULL;
    f = fopen(requests,"rb");
    if (!f) exit(1);
    fseek(f,0,SEEK_END);
    int n = ftell(f)/sizeof(job_app);
    fclose(f);
    f = NULL;
    return n;
}

// generate the key (helpful function)
int generate_key(){
    return (rand()%100)+5433 ;
}

// if someone know that the ids of job app are ordered, he will spy on other's job application
// so the real id in file is not just from generate_id_ja() but it is the combination between the id (orderd) and the key(random)

// add job_app
void add_request (FILE **k){
    job_app object;
    int key = generate_key();
    int id = generate_id_ja();
    printf("\n---------- ADD REQUEST ----------\n");
    object.p_info.id =  id + key;
    printf("First name: ");
    scanf("%19s",object.p_info.fn);
    printf("Last name: ");
    scanf("%19s",object.p_info.ln);
    printf("Subject: ");
    scanf("%14s",object.subject);
    strcpy(object.date_r,__DATE__);
    strcpy(object.reply,"");
    printf("===> The id to check your job app: %d\n",id);
    printf("===> REMARK: the real ID for checking your JOB APP is (KEY + ID) such that your key: %d\n",key);
    printf("---------------------------------\n");
    *k = fopen(requests,"ab");
    if (!(*k)) exit(1);
    fseek(*k,0,SEEK_END);
    fwrite(&object,sizeof(object),1,*k);
    fclose(*k);
    *k = NULL;
    printf("===>|ADDED SUCCESSFULLY|<===\n");
}

// display list of request
void display_request(){
    FILE *f = NULL;
    f = fopen(requests,"rb");
    if (!f) exit(1);
    job_app object;
    printf("\n---------- LIST OF REQUEST ----------\n");
    while (fread(&object,sizeof(object),1,f) != 0)
        printf("|%05d|%-20s|%-20s|%-15s|%s|%-5s|\n",object.p_info.id,object.p_info.fn,object.p_info.ln,object.subject,object.date_r,object.reply);
    printf("-------------------------------------\n");
    fclose(f);
    f = NULL;
}


// search (helpful function to check_my_ja())
void search (int id){
    FILE * f = fopen(teachers,"rb");
    if (!f) exit(1);
    teacher obj1;
    login obj2;
    bool found = false;
    while (fread(&obj1,sizeof(obj1),1,f) == 1) {
        if (obj1.id_pre == id){
            found = true;
            id = obj1.p_info.id;
            break;
        }
        fclose(f);
        f = NULL;
    }
    if (found){
        f = fopen(logins,"rb");
        if (!f) exit(1);
        while (fread(&obj2,sizeof(obj2),1,f) == 1) {
            if (obj2.id == id){
                printf("Your id as a teacher: %d\n",obj2.id);
                printf("Your password: %s\n",obj2.password);
                break;
            }
            fclose(f);
            f = NULL;
        }
    }
}

// check my job_app
void check_my_ja(){
    bool check = false;
    FILE *f;
    int id;
    teacher obj1;
    printf("Enter your id: ");
    scanf("%d",&id);
    printf("\n---------- MY JOB APP ----------\n");
    f = fopen(requests,"rb");
    job_app object;
    while (fread(&object,sizeof(object),1,f) == 1){
        if (object.p_info.id == id){
            check = true;
            break;
        }
    };
    if (check){
        if (strcmp(object.reply,"done") == 0){
            FILE *p = fopen(teachers,"rb");
            while (fread(&obj1,sizeof(obj1),1,p) == 1){
                if (obj1.id_pre == id){
                    printf("ID: %d \nFirst_name: %s \nLast_name: %s \nSubject: %s\nAm I acceptable: %s\n",obj1.id_pre,obj1.p_info.fn,obj1.p_info.ln,obj1.subject,"true");
                    search(id);
                    break;
                }
            }
            fclose(p);
            p = NULL;
        }
        else if (strcmp(object.reply,"") == 0)
            printf("No reply\n");
        else{
            printf("ID: %d \nFirst_name: %s \nLast_name: %s \nSubject: %s\nAm I acceptable: %s\n",object.p_info.id,object.p_info.fn,object.p_info.ln,object.subject,object.reply);
            search(id);
        }
        fclose(f);
    }
    else
        printf("There is not job app with this id: %d\n",id);
    printf("--------------------------------\n");
}

// process job_apps (the pricipal can reply with true, false or no replying)
void process(FILE **k){
    *k = fopen(requests,"rb+");
    if (! (*k)) exit(1);
    fseek(*k,0,SEEK_SET);
    char reply[6];
    job_app object;
    printf("\n----------- PROCESS JOB APPS -----------\n");
    while (fread(&object,sizeof(object),1,*k) == 1) {
        if (strcmp(object.reply,"") == 0){
            printf("Job app: %s - %s - %s\n",object.p_info.fn,object.p_info.ln,object.subject);
            printf("Is it acceptable(true/false): ");
            scanf("%5s",reply);
            //printf("\n");
            strcpy(object.reply,reply);
            fseek(*k,-sizeof(object),SEEK_CUR);
            fwrite(&object,sizeof(object),1,*k);
        }
    }
    printf("----------------------------------------\n");
    fclose(*k);
    *k = NULL;
}

// generate id of teacher (start from 100)
int generate_id_te(){
    FILE *f = NULL;
    f = fopen(teachers,"rb");
    if (!f) {
        exit(1);
    };
    fseek(f,0,SEEK_END);
    int n = ftell(f)/sizeof(teacher);
    fclose(f);
    f = NULL;
    return n + 100;
}

// refresh list of request (when a job app is processed, the pricipal do a refresh of file so if the replay is true or false, it will be "done")
void refresh(FILE **k){
    *k = fopen(requests,"rb+");
    if (! (*k)) exit(1);
    job_app object;
    while (fread(&object,sizeof(object),1,*k) == 1) {
        if ((strcmp(object.reply,"true") == 0) || (strcmp(object.reply,"false") == 0)){
            strcpy(object.reply,"done");
            fseek(*k,-sizeof(object),SEEK_CUR);
            fwrite(&object,sizeof(object),1,*k);
        }
    }
    fclose(*k);
    *k = NULL;
}


// add teacher (the adding of teacher is from job app when the replay is true)
void add_teacher(FILE **k,FILE **y){
    FILE *f = fopen(requests,"rb");
    fseek(f,0,SEEK_SET);
    if (!f) exit(1);
    *k = fopen(teachers,"ab");
    if (! (*k)) exit(1);
    *y = fopen(logins,"ab");
    if (! (*y)) exit(1);
    fseek(*y,0,SEEK_END);
    fseek(*k,0,SEEK_END);
    job_app obj1;
    teacher obj2;
    login obj3;
    char pw[9];
    while (fread(&obj1,sizeof(obj1),1,f) == 1) {
        if(strcmp(obj1.reply,"true") == 0){
            obj2.p_info.id = generate_id_te();
            strcpy(obj2.p_info.fn,obj1.p_info.fn);
            strcpy(obj2.p_info.ln,obj1.p_info.ln);
            strcpy(obj2.subject,obj1.subject);
            obj2.active = true;
            obj2.id_pre = obj1.p_info.id;
            fwrite(&obj2,sizeof(obj2),1,*k);
            obj3.id = obj2.p_info.id;
            strcpy(pw,generate_password());
            while (!(is_password_unique(pw)))
                strcpy(pw,generate_password());
            strcpy(obj3.password,pw);
            fwrite(&obj3,sizeof(obj3),1,*y);
        }
    }
    fclose(*k); *k = NULL;
    fclose(*y); *y = NULL;
    fclose(f); f = NULL;
    refresh(&(*k));
    printf("===>|ADDED SUCCESSFULLY|<===\n");
}

// list teacherss
void display_teacher(){
    FILE *f = NULL;
    f = fopen(teachers,"rb");
    if (!f) exit(1);
    teacher object;
    printf("\n---------- LIST OF TEACHER ----------\n");
    while (fread(&object,sizeof(object),1,f) != 0)
        printf("|%05d|%-20s|%-20s|%-15s|%d|\n",object.p_info.id,object.p_info.fn,object.p_info.ln,object.subject,object.active);
    printf("-------------------------------------\n");
    fclose(f);
    f = NULL;
}

// delete teacher (deleted_flag == 0 of false)
void delete_teacher (FILE **k){
    int id;
    printf("\n----------- DELETE TEACHER -----------\n");
    printf("Enter the id of teacher: ");
    scanf("%d",&id);
    if (id < generate_id_te()){
        teacher object;
        *k = fopen(teachers,"rb+");
        fseek(*k,(id - 100)*sizeof(object),SEEK_SET);
        fread(&object,sizeof(object),1,*k);
        object.active = false;
        fseek(*k,-sizeof(object),SEEK_CUR);
        fwrite(&object,sizeof(object),1,*k);
        fclose(*k);
        *k = NULL;
        printf("===>|DELETED SUCCESSFULLY|<===\n");
    }
    else
        printf("There is not teacher with this id: %d\n",id);
    printf("-------------------------------------\n");
}

// id card (when teacher can find their infromation like password, id, his subject and his name)
void id_card_t(int id){
    FILE *f;
    FILE *y;
    printf("\n---------- TEACHER'S ID CARD ----------\n");
    if (id < generate_id_te()){
        f = fopen(teachers,"rb");
        if (!f) exit(1);
        teacher object;
        fseek(f,(id - 100)*sizeof(object),SEEK_SET);
        fread(&object,sizeof(object),1,f);
        if (!object.active)
            printf("You are deleted\n");
        else{
            printf("ID: %d \nFirst_name: %s \nLast_name: %s \nSubject: %s\n",object.p_info.id,object.p_info.fn,object.p_info.ln,object.subject);
            y = fopen(logins,"rb");
            if (!y) exit(1);
            login obj;
            while (fread(&obj,sizeof(obj),1,y) == 1) {
                if (obj.id == id){
                    printf("Password: %s\n",obj.password);
                    break;
                }
            }
            fclose(y); y = NULL;
        }
        fclose(f); f = NULL;
    }
    else
        printf("There is not teacher with this id: %d\n",id);
    printf("---------------------------------------\n");
}

// check account of teacher (helpful function for checking that teacher is active and their information of log in is correct)
bool check_account(login obj){
    FILE * f = fopen(logins,"rb"); if (!f) exit(1);
    FILE * p = fopen(teachers,"rb"); if (!p) exit(1);
    teacher obj2;
    fseek(p,(obj.id - 100)*sizeof(teacher),SEEK_SET);
    fread(&obj2,sizeof(obj2),1,p);
    fclose(p); p = NULL;
    login obj1;
    while (fread(&obj1,sizeof(obj1),1,f) == 1) {
        if ((obj1.id == obj.id) && (strcmp(obj1.password,obj.password) == 0) && (obj2.active != 0)){
            fclose(f);
            f = NULL;
            return true;
        }
    }
    fclose(f);
    f = NULL;
    return false;
}

// check student is found in mark's (helpful function to check that student we fill its mark)
bool is_student_in_marks(int id){
    FILE *f = fopen(marks,"rb");
    if (!f) exit(1);
    mark object;
    while (fread(&object,sizeof(object),1,f) == 1) {
        if (object.id_s == id){
            fclose(f);
            f = NULL;
            return true;
        }
    }
    fclose(f);
    f = NULL;
    return false;
}

// fill marks (where teacher can fill marks of his students)
void fill_marks(login obj, FILE **l){
    if (!check_account(obj))
        printf("ERROR (You aren't registred in this school)\n");
    else {
        FILE *f = fopen(students,"rb"); if (!f) exit(1); student obj1;
        FILE *p = fopen(teachers,"rb"); if (!p) exit(1); teacher obj2;
        fseek(p,(obj.id - 100)*sizeof(teacher),SEEK_SET);
        fread(&obj2,sizeof(obj2),1,p);
        fclose(p); p = NULL;
        *l = fopen(marks,"ab"); if (! (*l)) exit(1); mark obj3;
        fseek(*l,0,SEEK_END);
        while (fread(&obj1,sizeof(obj1),1,f) == 1) {
            if ((!is_student_in_marks(obj1.p_info.id)) && (strcmp(obj1.subject,obj2.subject) == 0) && (obj1.active != 0)){
                printf("\n%05d-%-20s-%-20s---\n",obj1.p_info.id,obj1.p_info.fn,obj1.p_info.ln);
                printf("The mark: ");
                scanf("%f",&obj3.mark);
                obj3.id_s = obj1.p_info.id;
                obj3.id_t = obj2.p_info.id;
                strcpy(obj3.subject,obj1.subject);
                fwrite(&obj3,sizeof(obj3),1,*l);
                printf("--------------------------------------------------\n");
            }
        }
        fclose(*l); *l = NULL;
        fclose(f); f = NULL;
    }
}

// listing students's list (teacher can see just their student not all student of school)
void listing (login obj){
    if (!check_account(obj))
        printf("ERROR (You aren't registred in this school)\n");
    else{
        FILE *f = fopen(marks,"rb"); if (!f) exit(1);
        FILE *p = fopen(students,"rb"); if (!p) exit(1);
        mark obj1;
        student obj2;
        printf("\n---------- YOUR STUDENT'S LIST ----------\n");
        while (fread(&obj1,sizeof(obj1),1,f) == 1) {
            if (obj1.id_t == obj.id){
                fseek(p,(obj1.id_s - 1000)*sizeof(student),SEEK_SET);
                fread(&obj2,sizeof(obj2),1,p);
                printf("|%05d|%-20s|%-20s|%-15s|Is Active: %d|Mark: %04.2f|\n",obj2.p_info.id,obj2.p_info.fn,obj2.p_info.ln,obj1.subject,obj2.active,obj1.mark);
            }
        }
        printf("-----------------------------------------\n");
        fclose(f); f = NULL;
        fclose(p); p = NULL;
    }
}


// preparation of file (do reset for any file)
void file_preparation(){
    int num; char c;
    printf("\n---------- FILE PREPARATION ----------\n");
    printf("1- student's file \n2- Teacher's file \n3- Job's APP file\n ===> ");
    scanf("%d",&num);
    printf("--------------------------------------\n");
    switch (num) {
        case 1:{
            FILE * f = fopen(students,"w"); if (!f) exit(1); fclose(f); f = NULL;
            f = fopen(marks,"w"); if (!f) exit(1); fclose(f); f = NULL;
        }break;
        case 2:{
            FILE * f = fopen(teachers,"w"); if (!f) exit(1); fclose(f); f = NULL;
            f = fopen(logins,"w"); if (!f) exit(1); fclose(f); f = NULL;
        }break;
        case 3:{
            FILE * f = fopen(requests,"w"); if (!f) exit(1); fclose(f); f = NULL;
        }break;
        default: printf("ERROR\n");
    }
    printf("\n--- -- Sign out -- ---\n");
    printf("(y/n): ");
    scanf(" %c",&c);
    printf("----------------------\n");
    if (c != 'y')
        file_preparation();

}



// the system (after this comment, all the functions are for interface)
// begin
void F_Director (){
    FILE *f, *p;
    int number; char c;
    printf("\n---------- HI Mr.PRINCIPAL ----------\n");
    printf("1- Add a student\n2- Delete a student\n3- Display students's list\n4- Display teachers's list\n5- Display list of JOB APP\n6- Processing Job Apps\n7- Add a teacher\n8- Delete a teacher\n9- refresh\n=> Choose a number from list:  ");
    scanf("%d",&number);
    printf("-------------------------------------\n");
    switch (number) {
        case 1:add_student(&f);
        break;
        case 2:delete_student(&p);
        break;
        case 3:display_student();
        break;
        case 4:display_teacher();
        break;
        case 5:display_request();
        break;
        case 6:process(&f);
        break;
        case 7:add_teacher(&p,&f);
        break;
        case 8:delete_student(&p);
        break;
        case 9:refresh(&f);
        break;
        default:printf("==> Your choose isn't available\n");
    }
    printf("\n--- -- Sign out -- ---\n");
    printf("(y/n): ");
    scanf(" %c",&c);
    printf("----------------------\n");
    if (c != 'y')
        F_Director();
}

void F_teacher (login obj){
    FILE *f, *p;
    int number; char c;
    printf("\n---------- HI Mr.TEACHER ----------\n");
    printf("1- My ID card\n2- Fill marks\n3- Display my students's list\n=> Choose a number from list:  ");
    scanf("%d",&number);
    printf("-------------------------------------\n");
    switch (number) {
        case 1:id_card_t(obj.id);
        break;
        case 2:fill_marks(obj,&f);
        break;
        case 3:listing(obj);
        break;
        default:printf("==> Your choose isn't available\n");
    }
    printf("\n--- -- Sign out -- ---\n");
    printf("(y/n): ");
    scanf(" %c",&c);
    printf("----------------------\n");
    if (c != 'y')
        F_teacher(obj);
}

void F_JobApp (){
    FILE *f;
    int number; char c;
    printf("\n---------- HI Mr.PRINCIPAL ----------\n");
    printf("1- Add a Job Application\n2- check My Job App\n=> Choose a number from list:  ");
    scanf("%d",&number);
    printf("-------------------------------------\n");
    switch (number) {
        case 1:add_request(&f);
        break;
        case 2:check_my_ja();
        break;
        default:printf("==> Your choose isn't available\n");
    }
    printf("\n--- -- Sign out -- ---\n");
    printf("(y/n): ");
    scanf(" %c",&c);
    printf("----------------------\n");
    if (c != 'y')
        F_JobApp();
}

void THE_SYSTEM (){
    int number; char c;
    printf("\n---------- WELCOME TO A MINIATURE MODEL OF SCHOOL ADMINISTRATION ----------\n");
    printf("1- The principal\n2- The teachers\n3- JOB APP\n4- The student\n5- ADMIN\n=> Choose your role by writing its number from the list:  ");
    scanf("%d",&number);
    printf("---------------------------------------------------------------------------\n");
    switch (number) {
        case 1:{
            login object;
            printf("YOUR ID: ");
            scanf("%d",&object.id);
            printf("YOUR PASSWORD: ");
            scanf("%s",object.password);
            object.password[8] = '\0';
            if ((object.id == 0) && (strcmp(object.password,"d") == 0))
                F_Director();
            else
                printf("Sorry, your data is wrong\n");
        }break;
        case 2:{
            login object;
            printf("YOUR ID: ");
            scanf("%d",&object.id);
            printf("YOUR PASSWORD: ");
            scanf("%s",object.password);
            object.password[8] = '\0';
            if (check_account(object))
                F_teacher(object);
            else
                printf("Sorry, your data is wrong\n");
        }break;
        case 3:F_JobApp();
        break;
        case 4:{
            printf("Dear student .. we will displaying your information\n");
            id_card_s();
        }break;
        case 5:{
            printf("\n=>The only role for Admin is RESET the system\n");
            login object;
            printf("YOUR ID: ");
            scanf("%d",&object.id);
            printf("YOUR PASSWORD: ");
            scanf("%s",object.password);
            object.password[8] = '\0';
            if ((object.id == 1) && (strcmp(object.password,"a") == 0))
                file_preparation();
            else
                printf("Sorry, you aren't the Admin .. just he can do RESET for system\n");

        }break;
        default:printf("==> Your choose isn't available\n");
    }
    printf("\n--- -- Sign out from system -- ---\n");
    printf("(y/n): ");
    scanf(" %c",&c);
    printf("----------------------------------\n");
    if (c != 'y')
        THE_SYSTEM();
}
// end



int main(){
    srand(time(NULL));
    THE_SYSTEM();
    return 0;
}
