/*STUDENT REPORT MANAGEMENT SYSTEM 
MADE BY- ANISH BENJWAL CO20307
         LALIT KUMAR CO20328
         VYOM VERMA CO20368
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <direct.h>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <tuple>
#include <algorithm>
#include <string>

using namespace std;

// ALL CLASSES USED AND THEIR FUNCTIONS

class Person
{
protected:
    vector<string> subjects; //Subjects stored
    string rollNumber = "", userName = "";
    string path;

public:
    string firstName, lastName;
    virtual void loadDat(){};
    virtual void viewMarks(string subject, string roll){};
    friend void retrive(string line);
    virtual bool checkExistence();
};

//Administrator
class admin
{
    string username;            //username for admin
    string password;            //password for admin
    int session = -1;           //holds the session status(if admin is logged in)
public:
    admin();
    void login();                   //login menu for admin
    void changePassword();          //funtion to change login password for admin
    int check() { return session; } //to check if the admin is logged in or not
    void admin_menu();              //used to get to login menu for admin and other functions
    void addStandard();             //To create new standard simultaneously the class teacher assigned. It also declares subjects and teachers assigned to them.
    void addNewTeacher();           //to create new teacher
    void listStand();               //lists all the standards in the system
    void listTeachers();            //lists all the teachers in the system
};

//Standards

class Standard : public Person
{
    string standard, classIncharge;                         //standard Name
    vector<string> subTeachers;
    vector<vector<tuple<int, int>>> marks;   //marks ordered subject wise  [subject1->{(rollno1,marks1), (rollno2,marks2),....},subject2->{(roll1,marks1),(roll2,marks2),...},...}
    vector<int> rollNo;                      //roll numbers stored in increasing order
    vector<vector<tuple<int, int>>> ordered; //marks ordered roll number wise
                                             //  [{(1,56),(2,70)},{(1,45),(2,87)}]
                                             //  [student1->{subject1->(1,56),subject2->(1,45),...},student2->{subject1->(2,70),subject2->(2,87),...},...]
public:
     Standard(string stand)
    { //initializes standard and loads data
        standard = stand;
        loadDat();
    }
    void loadDat();                           //Loads Data into variables defined above
    void viewMarks(string subject, string c); //to view marks of a specific subject in the standard
    bool checkExistence();                    //to check if standard exists or not
    void showAllMarks(int c);                 //show all marks of students in the standard in any 2 ordered ways
    void view();
};

//Teacher
class teacher : public Person
{
    //string username, firstName, lastName;                     //holds username, first name and last name of the teacher logged in
    vector<string> classes;        //holds their specific classes taught by the teacher logged in
    string classIncharge = "None"; //defines if the teacher logged in is incharge of any standar, if yes then holds the standard name
    int session = -1;              //check if session is active(if any teacher is logged in)

public:
    teacher(string user = "")
    {
        userName = user;
        path = "teachers\\detailsTeach.txt";
    }
    void login();                                //teacher login
    void teacherMenu();                          //Menu after teacher logs in(only class incharge can view marks of the whole class)
    void loadDat();                              //loads data into the dataStructure defined above
    void addStudent();                           //adds Student into the class the teacher is Incharge of(not available if teacher logged in is not incharge of any class)
    void inputMarks();                           //input marks into specific subject of a class as defined by the admin(teacher can only input marks in the subject teached by them)
    void viewMarks(string subject, string roll); //view marks of the subject taught
    void reset();                                //reset the password of the teacher logged in
    void view();                                 //views data of the teacher show in list of teacher inside admin
    void operator=(string usr) { userName = usr; }
    //~teacher();
};


//Student
class Student : public Person
{
    //string rollNumber, firstName,lastName; //declaration of student variables
    string standard;

public:
    Student(string roll = "", string stand = "")
    {
        standard = stand;
        rollNumber = roll;
        path = "Standards\\" + standard + "\\students.txt";
    }
    vector<string> loadDatS(string roll, string classStand); //loads the data of student from file
    void add(string classStand);                             //function to add student
    void operator=(tuple<string, string> info);              //to change standard and roll number for object
    };

//OTHER VARIOUS FUNCTIONS USED
void MAIN_MENU();
string encode(string pass){
    string enPass="";
    for (int i=0; i<pass.length(); i++){
        enPass += pass[i]+5;
    }
    return enPass;
}

string decode(string enPass){
    string pass="";
    for (int i=0; i<enPass.length(); i++){
        pass += enPass[i]-5;
    }
    return pass;
}

vector<string> retrieveBracketed(string subjects)
{
    vector<string> sub_vec;
    string temp = "";
    int open = 0;
    for (int i = 0; i < subjects.length(); i++)
    {
        if (subjects[i] == '(')
        {
            open++;
            continue;
        }
        else
        {
            if (subjects[i] == ')')
            {
                open--;
                sub_vec.push_back(temp);
                temp = "";
            }
            else if (open > 0)
                temp += subjects[i];
        }
    }
    return sub_vec;
}

vector<string> retrieve(string line) //splits line from blank spaces and returns a vector of strings  ex- English Maths-> data={English,Maths,..}
{
    vector<string> data;
    string temp = "";
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ' ')
        {
            data.push_back(temp);
            temp = "";
        }
        else
            temp += line[i];
    }
    data.push_back(temp);
    return data;
}

tuple<int, int> retrieveMarks(string line)
{
    tuple<int, int> result;
    string f = "", s = "";
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ' ')
        {
            for (int k = i; k < line.length(); k++)
                s += line[k];
            break;
        }
        else
            f += line[i];
    }
    get<0>(result) = stoi(f);
    get<1>(result) = stoi(s);

    return result;
}

void modifyMarks(string rol, string marks, string standard, string subject)
{

    ofstream tempFile("Standards\\" + standard + "\\Subjects\\temp.txt");
    ifstream org("Standards\\" + standard + "\\Subjects\\" + subject + ".txt");
    string line;
    vector<string> data;
    int check = 0;
    string rollN = rol + " ";
    char *search = &rollN[0];
    while (getline(org, line))
    {
        if (line.find(search, 0) != string::npos)
        {
            tempFile << rollN + marks + "\n";
            check = 1;
        }
        else
            tempFile << line + "\n";
    }
    if (check != 1)
        tempFile << rollN + marks + "\n";

    string a = "Standards\\" + standard + "\\Subjects\\" + subject + ".txt";
    string b = "Standards\\" + standard + "\\Subjects\\temp.txt";

    const char *orgFile = &a[0];
    const char *newFile = &b[0];

    tempFile.close();
    org.close();

    remove(orgFile);
    rename(newFile, orgFile);
}

int modifyClassIncharge(string teacher, string standard)
{
    ofstream tempFile("teachers\\temp.txt");
    ifstream org("teachers\\detailsTeach.txt");
    string line;
    vector<string> dat;

    //teacher += " ";
    char *search = &teacher[0];
    while (getline(org, line))
    {
        if (line.find(search, 0) != string::npos)
        {
            dat = retrieve(line);
            if (dat.size() == 5){
                return stoi(dat[4]);
            }
            string newLine = line + " " + standard + "\n";
            tempFile << newLine;
        }
        else
            tempFile << line + "\n";
    }

    const char *orgFile = "teachers\\detailsTeach.txt";
    const char *newFile = "teachers\\temp.txt";

    tempFile.close();
    org.close();

    remove(orgFile);
    rename(newFile, orgFile);
    return -1;
}

void modifySubjectAssign(vector<string> subList, vector<string> teachList, string standard)
{
    for (int i = 0; i < subList.size(); i++)
    {
        string teacher = teachList[i];
        string sub = subList[i];
        ofstream tempFile("teachers\\temp.txt");
        ifstream org("teachers\\subAssign.txt");
        string line;

        //teacher += " ";
        char *search = &teacher[0];
        while (getline(org, line))
        {
            if (line.find(search, 0) != string::npos)
            {
                string newLine = line + " (" + standard + "," + sub + ")\n";
                tempFile << newLine;
            }
            else
                tempFile << line + "\n";
        }

        const char *orgFile = "teachers\\subAssign.txt";
        const char *newFile = "teachers\\temp.txt";

        tempFile.close();
        org.close();

        remove(orgFile);
        rename(newFile, orgFile);
    }
}

void display_vector(vector<string> sub) //for test pourposes
{
    cout << "[";
    for (int i = 0; i < sub.size(); i++)
    {
        cout << sub[i] << ", ";
    }
    cout << "]" << endl;
}

inline int create_directory(string path)
{
    char *folder = &path[0];
    return mkdir(folder);
}

//DEFINITION OF THE FUNCTIONS DECLARED IN VARIOUS CLASSES ABOVE

void admin::changePassword(){
    cout<<"Enter old password: ";
    string usrInput; cin>>usrInput;
    if (usrInput == password){
        cout<<"New Password: ";
        string np, cnp; cin>>np;
        cout<<"Confirm New Password: ";
        cin>>cnp;
        if (np == cnp){
            ofstream dB("admin.txt", ios::trunc);
            dB<<"admin "+encode(np)+"\n";
            dB.close();
        }
        cout<<"Password changed Successfully!"<<endl;
        getch();
        admin_menu();
    }
    else{
        cout<<"Wrong Password!"<<endl;
        getch();
        admin_menu();
    }
}


admin::admin(){
    ifstream dB("admin.txt");
    if (dB.is_open()==false){
        dB.close();
        ofstream first("admin.txt");
        first<<"admin "+encode("admin")+"\n";
        first.close();
        username = "admin";
        password = "admin";

    }
    else{
        vector<string> dat;
        string line;
        while (getline(dB, line)){
            dat = retrieve(line);
            username = dat[0];
            password = decode(dat[1]);
        }
         dB.close();
    }
}

void admin::login() //used to get login menu for admin
{   system("cls");
    string user, pass;
    cout << "\n\n\t\t\t\t\t\tADMIN" << endl;
    cout << "\n\t\t\t\tEnter username: ";
    cin >> user;
    cout << "\t\t\t\tEnter password: ";
    char ch;
    ch = getch();
    while (ch != '\r')
    {
        pass += ch;
        cout << "*";
        ch = getch();
    }
    cout << endl;
    if (user == username && pass == password)
        session = 1;
    else if(user!=username||pass!=password)
    {
        cout<<"\n\nLOGIN UNSUCCESSFUL "<<endl;
        getch();
        login();
    }
}

void admin::admin_menu() //functions to be performed by admin
{
    system("cls");
     if(session== -1)
    {
        cout << "\n\t\t\t\tLOGIN UNSUCCESSFUL!";
        MAIN_MENU();
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
    cout << setw(65) << right << setfill('-') << "A D M I N    M E N U"<< setw(55) <<""<<setfill('-') << endl << endl;
    cout << setw(64) << setfill(' ');
    cout << "(1) Add new teacher" << endl;
    cout << setw(68) << setfill(' ');
    cout << "(2) Create new standard" << endl;
    cout << setw(66) << setfill(' ');
    cout << "(3) List of Standards" << endl;
    cout << setw(62) << setfill(' ');
    cout << "(4) List Teachers" << endl;
    cout << setw(64) << setfill(' ');
    cout << "(5) Update Password" << endl;
    cout << setw(55) << setfill(' ');
    cout<<"(6) Logout"<< endl<<endl;
    int num;
    cout << "Enter the corresponding number to select role.\n";
    cin >> num;
    switch (num)
    {
    case 1:
        system("cls");
        addNewTeacher();
        break;
    case 2:
        system("cls");
        addStandard();
        break;
    case 3:
        system("cls");
        listStand();
        break;
    case 4:
        system("cls");
        listTeachers();
        break;
    case 5:
        system("cls");
        changePassword();
    case 6:
        system("cls");
        session = -1;
        break;
    default:

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x75);
        cout << "TRY AGAIN";
        getch();
        system("cls");
        system("Color 70");
    }
    admin_menu();
}
void admin::addStandard()
/*This function creates a "Standards" folder if not present and then creates another folder by the name of the specified standard inside "Standards"
which further creates "Subjects" folder(containing the files of particular subjects),details,txt and students.txt */
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
    if (session == -1)
        return;
    create_directory("Standards");
    string n;
    cout << "Enter number of standard: ";
    cin >> n;
    string className = "Standards\\" + n;
    char *ab = &className[0];
    int check = mkdir(ab);
    if (check == -1)
    {   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x74);
        cout << "Standard with same name already exists!" << endl;
        getch();
        return;
    }
    string directory = "Standards\\" + n + "\\Subjects";
    char *cd = &directory[0];
    check = mkdir(cd);
    string classTeacher;
    ofstream allDatInp("Standards\\dat.txt", ios::app);
    allDatInp << n + "\n";
    allDatInp.close();
re_enter_classincharge:
    cout << "Enter username of class incharge: ";
    cin >> classTeacher;
    teacher tt(classTeacher);
    if (tt.checkExistence())
    {
        int k = modifyClassIncharge(classTeacher, n);
            if(k!=-1)
            {cout<<"This teacher is already a class incharge of standard "<<k<<" !"<<endl;
            goto re_enter_classincharge;;}
        if (check == 0)
        {
            int subjects;
            cout << "Enter total number of subjects: ";
            cin >> subjects;

            string subjectList;
            string teacherList;
            vector<string> teachList, subList;
            for (int i = 0; i < subjects; i++)
            {
                string temp, teach;
                cout << i + 1 << ". Subject Name: ";
                cin.ignore();
                getline(cin, temp);
                ofstream of("Standards\\" + n + "\\Subjects\\" + temp + ".txt");
                of.close();
            re_enter_teacher:
                cout << "Teacher Assigned(Enter only username of the teacher): ";
                cin >> teach;
                tt = teach;
                if (tt.checkExistence())
                {
                    teachList.push_back(teach);
                    subList.push_back(temp);
                    string sub = "(" + temp + ")";
                    subjectList += sub + " ";
                    teacherList += teach + " ";
                }
                else
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x74);
                    cout << "This teacher with such username doesn't exist! " << endl;
                    cout << "TRY AGAIN" << endl;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
                    goto re_enter_teacher;
                }
            }

            modifySubjectAssign(subList, teachList, n);

            subjectList += "\n";
            teacherList += "\n";
            ofstream db("Standards\\" + n + "\\details.txt", ios::app);
            db << subjects << "\n";
            db << subjectList;
            db << teacherList;
            db << classTeacher;

            db.close();
        }
        else if (check == -1)
        {   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x74);
            cout << "Class already exists!" << endl;
        }
    }
    else
    {
        cout << "This username doesn't exist!" << endl;
        goto re_enter_classincharge;
    }
}

void admin::listTeachers(){
    ifstream db("teachers\\detailsTeach.txt");
    string line;
    vector<vector<string>> all;
    vector<string> dat;
    while (getline(db, line)){
        dat = retrieve(line);
        all.push_back(dat);
    }

    cout <<"S.No.\t|"<<setw(5)<<right<<setw(5)<<"Username\t|"<< "Full Name"<<endl;

    for (int i=0; i<all.size(); i++){
        string user = all[i][0];
        string full = all[i][1]+" "+all[i][2];
        cout<<i+1<<"\t|"<<setw(5)<<left<<setw(10)<<user<<"\t|"<<setw(20)<<left<<full<<endl;
    }
    db.close();
    cout<<endl<<"Enter -1 to go back to the menu.";
    cout<<endl<<"Enter any S.No. of teacher to see furter details: ";
    int choice;
    cin>>choice;
    if (choice == -1) return;
    teacher t(all[choice-1][0]);
    system("cls");
    t.loadDat();
    t.view();
    listTeachers();
}


void admin::addNewTeacher()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
    if (session == -1)
        return;
    string t = "teachers";
    char *ch = &t[0];
    mkdir(ch);
    ofstream db("teachers\\detailsTeach.txt", ios::app);
    ifstream dbLog("teachers\\detailsTeach.txt");
    string firstN, lastN, username, pass, confirmPass;
    cout << "First Name: ";
    cin >> firstN;
    cout << "Last Name: ";
    cin >> lastN;
re_enter_username:
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> pass;
    cout << "Confirm Password: ";
    cin >> confirmPass;
check_pass:
    if (pass == confirmPass)
    {

        string dat = username + " " + firstN + " " + lastN + " " + encode(pass) + "\n";

        vector<string> passOrg;
        string line;
        string u = username + " ";
        char *search = &u[0];
        while (getline(dbLog, line))
        {
            if (line.find(search, 0) != string::npos)
            {
                passOrg = retrieve(line);
                break;
            }
        }
        if (passOrg.size() != 0)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x74);
            cout << "Username already exists!\nPlease TRY AGAIN!" << endl;
            getch();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
            goto re_enter_username;
        }
        else
        {
            db << dat;
            ofstream dbUser("teachers\\subAssign.txt", ios::app);
            dbUser << username + "\n";
            dbUser.close();
            cout << "Teacher added Successfully!" << endl;
            getch();
        }
    }
    else
    {
        cout << "TRY AGAIN!" << endl;
        cout << "Confirm password: ";
        cin >> confirmPass;
        goto check_pass;
    }
    db.close();
    dbLog.close();
}
void admin::listStand() //lists the name of standards created in "Standard folder"
{
    ifstream listSt("Standards\\dat.txt");
    string line;
    vector<string> standards;
    while (getline(listSt, line))
    {
        standards.push_back(line);
    }
    listSt.close();
    if (standards.size() == 0)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        cout << "No Standards added yet!" << endl;
        getch();
    }
    else
    {   cout<<"  STANDARDS"<<endl;
        for (int i = 0; i < standards.size(); i++)
        {
            cout << i + 1 << ") " << standards[i] << endl;
        }
    }
    cout<<endl<<"Enter -1 to go back to the menu";
    cout << "\nEnter S.No of a standard to view its details: ";// << endl;
    int choice;
    cin>>choice;
    if (choice==-1) return;

    Standard s1(standards[choice-1]);
    s1.view();
    getch();
    system("cls");

    listStand();
}

bool Person::checkExistence()
{
    //standard = classStand;
    ifstream db2(path);
    string line;
    //string r = roll + " ";
    vector<string> dat;
    //cout<<"("<<rollNumber<<")"<<endl;
    while (getline(db2, line))
    {
        dat = retrieve(line);
        if (dat[0] == rollNumber || dat[0] == userName)
            return true;
    }
    db2.close();
    return false;
}
vector<string> Student::loadDatS(string roll, string classStand)
{
    Standard check(classStand);

    if (check.checkExistence() == false)
    {
        return vector<string>{};
    }
    rollNumber = roll;
    roll += " ";
    standard = classStand;
    string line;
    vector<string> dat;
    char *search = &roll[0];
    ifstream db("Standards\\" + standard + "\\students.txt", ios::app);
    while (getline(db, line))
    {
        if (line.find(search, 0) != string::npos)
        {
            dat = retrieve(line);
            break;
        }
    }
    //display_vector(dat);
    return dat;
}
void Student::operator=(tuple<string, string> info){
    rollNumber = get<0>(info);
    standard = get<1>(info);
    path = "Standards\\" + standard + "\\students.txt";
}

void Student ::add(string classStand)
{
    Standard check(classStand);
    if (check.checkExistence() == false)
        return;
    standard = classStand;
    cout << "\nRoll Number: ";
    cin >> rollNumber;
    cout << "First Name: ";
    cin >> firstName;
    cout << "Last Name: ";
    cin >> lastName;
    string rol = rollNumber + " ";
    string line;
    char *search = &rol[0];
    ifstream db2("Standards\\" + standard + "\\students.txt", ios::app);
    while (getline(db2, line))
    {
        if (line.find(search, 0) != string::npos)
        {
            cout << "Roll Number Already Exists!";
            getch();
            return;
        }
    }
    db2.close();

    ofstream db("Standards\\" + standard + "\\students.txt", ios::app);
    string dat = rollNumber + " " + firstName + " " + lastName + "\n";
    db << dat;
    db.close();
    cout << "Student added Successfully!";
}

void display_tuple(vector<tuple<int, int>> sub, string standard)
{
    cout << "Roll Number|" << setw(15) << right << "Name"
         << "\t\t  |" << setw(10) << right << "\t   Marks" << right << endl;
    Student s;
    string first, last;
    int rol, marks;
    for (int i = 0; i < sub.size(); i++)
    {
        rol = get<0>(sub[i]);
        marks = get<1>(sub[i]);
        vector<string> dat;
        string roll = to_string(rol);
        dat = s.loadDatS(roll, standard);
        first = dat[1];
        last = dat[2];
        string fullName = first + " " + last;
        cout << setw(6) << right << roll << setw(6) << "|" << left << setw(30) << fullName << "|" << setw(5) << left << "\t" << marks << endl;
    }
}

void teacher::login()
{
    ifstream dbLog("teachers\\detailsTeach.txt");
    string u, p;

    cout << "\n\n\t\t\t\t\t\tTEACHER" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
    cout << "\n\t\t\tEnter username: ";
    cin >> u;
    cout << "\t\t\tEnter password: ";
    cin >> p;
    vector<string> data = {"","","",""};
    string line;
    u += " ";
    char *search = &u[0];
    while (getline(dbLog, line))
    {
        if (line.find(search, 0) != string::npos)
        {
            data = retrieve(line);
            break;
        }
    }
    if (data[3] == ""){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x74);
        cout << endl<<"\t\t\tUser not Found!" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        getch();
    }
    else if (decode(data[3]) == p)
    {
        session = 1;
        firstName = data[1];
        lastName = data[2];
        userName = data[0];
        if (data.size() == 5)
            classIncharge = data[4];
        loadDat();
    }
    else{
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x74);
        cout<<endl<<"\t\tPassword did not match! Try Again";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        getch();
    }
    dbLog.close();

}

void teacher::teacherMenu()
{
    system("cls");

    if (session == -1)
        return;
    if (classIncharge == "None")
    {
        system("cls");
        system("Color 70");
        cout <<setw(50)<<right<<setfill('-') <<"";
        cout << "Welcome " << firstName << " " << lastName << "! ";
        cout << setw(47) << setfill('-') << "-" << endl<< endl;
        cout << setw(61) << setfill(' ');
        cout << "(1) Input Marks" << endl;
        cout << setw(60) << setfill(' ');
        cout << "(2) View Marks" << endl;
        cout << setw(64) << setfill(' ');
        cout << "(3) Reset Password" << endl;
        cout << setw(75) << setfill(' ');
        cout << "(4) View your account details" << endl;
        cout << setw(56) << setfill(' ');
        cout << "(5) Logout" << endl<< endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
        int num;
        cout << "Enter the corresponding number to select role.\n";
        cin >> num;
        switch (num)
        {
        case 1:
            inputMarks();
            break;
        case 2:
            viewMarks(" ", " ");
            break;
        case 3:
            reset();
            break;
        case 4:
            system("cls");
            view();
            teacherMenu();
            
        case 5:
            session = -1;
            break;
        default:
            teacherMenu();
        }
    }
    else
    {
    label:
        Standard s1(classIncharge);
        system("cls");
        system("Color 70");
        cout <<setw(50)<<right<<setfill('-') <<"";
        cout << "Welcome " << firstName << " " << lastName << "! ";
        cout << setw(47) << setfill('-') << "-" << endl<< endl;
        cout << setw(74) << setfill(' ');
        cout << "(1) Add Student in standard " << classIncharge << endl;
        cout << setw(61) << setfill(' ');
        cout << "(2) Input Marks" << endl;
        cout << setw(82) << setfill(' ');
        cout << "(3) View Marks of a specific subject" << endl;
        cout << setw(89) << setfill(' ');
        cout << "(4) View Marks of all subjects of standard " << classIncharge << endl;
        cout << setw(64) << setfill(' ');
        cout << "(5) View standard "<<classIncharge<<" details" << endl;
        cout << setw(75) << setfill(' ');
        cout << "(6) View your account details" << endl;
        cout << setw(64) << setfill(' ');
        cout << "(7) Reset Password" << endl;
        cout << setw(56) << setfill(' ');
        cout << "(8) Logout" << endl<< endl;
        int num;
        cout << "Enter the corresponding number to select role.\n";
        cin >> num;
        switch (num)
        {
        case 1:
            addStudent();
            break;
        case 2:
            inputMarks();
            break;
        case 3:
            viewMarks(" ", " ");
            break;
        case 4:
            system("cls");
            cout << "1.Sort by Roll Number" << endl;
            cout << "2.Sort by Percentage" << endl;
            int sortManner;
            cout << ">>";
            cin >> sortManner;
            if(sortManner==1||sortManner==2)
            s1.showAllMarks(sortManner);  //function in standard class
            goto label;
            break;
        case 5:
            s1.view();
            break;
        case 6:
            system("cls");
            view();
            break;
        case 7:
            reset();
            break;
        case 8:
               system("cls");
                session = -1;
                break;
        default:
            teacherMenu();
        }

    }
    teacherMenu();
}

void teacher::loadDat()
{
    ifstream dbLog("teachers\\subAssign.txt");
    string line, u = userName;
    char *search = &u[0];
    while (getline(dbLog, line))
    {
        if (line.find(search, 0) != string::npos)
        {
            string sub = "", clas = "";
            int subC = 0, clasC = 0;
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == '(')
                {
                    clasC = 1;
                    clas = "";
                }
                else if (line[i] == ')')
                {
                    subC = 0;
                    subjects.push_back(sub);
                    sub = "";
                }
                else if (line[i] == ',')
                {
                    classes.push_back(clas);
                    clas = "";
                    clasC = 0;
                    subC = 1;
                }
                else
                {
                    if (clasC == 1)
                    {
                        clas += line[i];
                    }
                    else if (subC == 1)
                    {
                        sub += line[i];
                    }
                }
            }
            break;
        }
    }
    dbLog.close();
}

void teacher::view(){
    ifstream dbLog("teachers\\detailsTeach.txt");
    string search = userName + " ";
    string line;
    vector<string> dat;
    while (getline(dbLog, line)){
        if (line.find(search, 0) != string::npos){
            dat = retrieve(line);
            firstName = dat[1];
            lastName = dat[2];
            if (dat.size() == 5) classIncharge = dat[4];
            break;
        }
    }
    dbLog.close();
    //loadDat();
    cout<<"Name: "<<firstName<<" "<<lastName<<"\t\tUsername: "<<userName<<"\t\tClass Incharge: "<<classIncharge<<endl<<endl<<endl;
    cout <<"S.No.\t|"<<setw(5)<<left<<setw(30)<<"Subject"<< " |Standard"<<endl;
    for (int i=0; i<classes.size(); i++){
        cout<<i+1<<"\t|"<<setw(5)<<left<<setw(30)<<subjects[i]<<"\t|"<<setw(20)<<left<<classes[i]<<endl;
    }
    cout<<endl<<endl<<"Press any key to continue..";
    getch();
    system("cls");

}

void teacher::addStudent()
{
    if (session == -1)
        return;
    if (classIncharge == "None")
        return;
    cout << "Report of how many students do you want to add?" << endl;
    int n_stud;
    cin >> n_stud;
    Student s;
    for (int i = 0; i < n_stud; i++)
    {
        s.add(classIncharge);
        cout << setw(100) << setfill('~') << endl<< endl;
    }
    getch();
    system("cls");
    teacherMenu();
}

void teacher::inputMarks()
{
    beginFunc:
    system("cls");
    for (int i = 0; i < subjects.size(); i++)
    {
        cout << i + 1 << ". "
             << "Standard: " << classes[i] << " "
             << "    Subject: " << subjects[i] << endl;
    }
    cout<<"Enter -1 to go back..."<<endl;
    cout << ">>";
    int choice;
    cin >> choice;
    if (choice == -1)
        return;

    Student s1;
    if (choice > subjects.size() || choice <= 0)
        goto beginFunc;
    else
    {
        tuple<string, string> info;
        cout << "Enter the marks again if you want to modify an entry!" << endl;
        string rol, marks;
        vector<string> data;
        again:
        cout << "\nRoll Number: ";
        cin >> rol;
        get<0>(info) = rol;
        get<1>(info) = classes[choice - 1];
        //cout<<rol<<" "<<classes[choice-1]<<endl;
        s1 = info;
        //cout<<s1.checkExistence()<<endl;
        if (!s1.checkExistence()){
            cout<<"Student does not exist! Please try again.";
            goto again;
        }
        cout << "Marks: ";
        cin >> marks;
        modifyMarks(rol, marks, classes[choice - 1], subjects[choice - 1]);
    }
    goto beginFunc;
}

void teacher::viewMarks(string subject, string roll)
{
    system("cls");
    if (session == -1)
        return;
    for (int i = 0; i < subjects.size(); i++)
    {
        cout << i + 1 << ". " <<"Standard: "<<classes[i] << " Subject: " << subjects[i] << endl;
    }
    cout << ">>";
    int choice;
    cin >> choice;
     if (choice == -1)
        teacherMenu();
    if (choice > subjects.size() || choice <= 0)
        viewMarks(" ", " ");
    system("cls");
    string sub = subjects[choice - 1], stand = classes[choice - 1];
    cout << "Standard: " << stand << "\tSubject: " << sub << endl;
    cout << "1.Sort by Roll Number" << endl;
    cout << "2.Sort by Marks" << endl;
    cout<<"Press 3 to go back!"<<endl<<endl;
    int sortManner;
    cout<<"Select your choice"<<endl;
    cout << ">>";
    cin >> sortManner;
    if(sortManner==1||sortManner==2)
    {
        Standard v(stand);
        v.viewMarks(sub, to_string(sortManner - 1));
    }
    else if(sortManner==3)
     viewMarks("","");
    else  viewMarks(" ", " ");
}

void teacher::reset(){
    //string path = "teachers\\detailsTeach.txt", line;
    //string userName = "vyom.v";
    string cnp, np;
    ifstream read(path);
    ofstream write("teachers\\detailsTeachTemp.txt");
    cout<<"Enter Old Password: ";
    string uInp;
    cin>>uInp;
    string line;
    vector<string> dat;
    bool found = false;
    while (getline(read, line)){
         dat = retrieve(line);
    if (dat[0] == userName) {
            found = true;
            if (uInp == decode(dat[3])){
                cout<<"Enter New Password: ";
                cin>>np;
                re_enter_password:
                cout<<"Confirm New Password: ";
                cin>>cnp;
                if (cnp!=np){
                    cout<<"Password and confirm password don't match!"<<endl;
                    cout<<"TRY AGAIN"<<endl;
                    goto re_enter_password;
                }
                string lineMod = "";
                if (dat.size() == 4){
                    for (int i=0; i<dat.size()-1; i++){
                        lineMod+=dat[i]+" ";
                    }
                    lineMod += encode(np);
                }
                else{
                    for (int i=0; i<dat.size()-1; i++){
                        if (i!=3){lineMod+=dat[i]+" ";}
                        if (i==3){lineMod+=encode(np)+" ";}
                    }
                    lineMod+=dat[dat.size()-1];
                }

                write<<lineMod+"\n";

                cout<<"Password Updated Successfully!"<<endl;
                getch();

            }
            else{
                cout<<"Passsword does not match!"<<endl;
                getch();
            }

         }
     else{
             write<<line+"\n";
         }
    }
    write.close();
    read.close();

    string of = "teachers\\detailsTeach.txt", nf = "teachers\\detailsTeachTemp.txt";

    const char* orgFile= &of[0];
    const char* newFile= &nf[0];

    remove(orgFile);
    rename(newFile, orgFile);

}


void Standard::loadDat()
{
    ifstream details("Standards\\" + standard + "\\details.txt");
    string line;
    unsigned int curLine = 1;
    while (getline(details, line))
    {
        if (curLine == 2)
            subjects = retrieveBracketed(line);
        if (curLine == 3)
            subTeachers = retrieve(line);
        if (curLine == 4)
            classIncharge = line;
        curLine++;
    }
    //display_vector(subjects);
    details.close();
    vector<tuple<int, int>> temp;
    for (int i = 0; i < subjects.size(); i++)
    {
        ifstream marksdetails("Standards\\" + standard + "\\Subjects\\" + subjects[i] + ".txt");
        temp.clear();
        string line;
        while (getline(marksdetails, line))
        {
            //cout<<line<<endl;
            temp.push_back(retrieveMarks(line));
        }
        marks.push_back(temp);
        marksdetails.close();
    }
    ifstream studentDetails("Standards\\" + standard + "\\students.txt");
    string line3;
    vector<string> datTemp;
    while (getline(studentDetails, line3))
    {
        datTemp = retrieve(line3);
        rollNo.push_back(stoi(datTemp[0]));
    }
    sort(rollNo.begin(), rollNo.end());
    studentDetails.close();
    //count++;
}

bool secElem(const tuple<int, int> &a1, const tuple<int, int> &a2) //used in Standard::viewMarks to sort by marks in descending order
{
    return (get<1>(a1) > get<1>(a2));
}

void Standard::viewMarks(string subject, string c)
{
    //cout<<"viewMarks Called!"<<endl;
    system("cls");
    int index = -1;
    for (int i = 0; i < subjects.size(); i++)
    {
        if (subject == subjects[i])
        {
            index = i;
        }
    }
    if (index == -1)
        return;
    vector<tuple<int, int>> subMarks = marks[index];
    if (c == "0")
        sort(subMarks.begin(), subMarks.end());
    else if (c == "1")
        sort(subMarks.begin(), subMarks.end(), secElem);
    display_tuple(subMarks, standard);
    cout << "\n\nPress any key to continue!" << endl;
    getch();
}

bool Standard::checkExistence()
{
    int check = create_directory("Standards\\" + standard);
    if (check == -1)
        return true;

    else if (check == 0)
    {
        string cls = "Standards\\" + standard;
        char *ab = &cls[0];
        rmdir(ab);
        return false;
    }
    return false;
}

void Standard::view(){
    system("cls");
    string first;
    string last;
    ifstream dbLog("teachers\\detailsTeach.txt");
    string search = classIncharge + " ";
    string line;
    vector<string> dat;
    while (getline(dbLog, line)){
        if (line.find(search, 0) != string::npos){
            dat = retrieve(line);
            first = dat[1];
            last = dat[2];
            if (dat.size() == 5) classIncharge = dat[4];
            break;
        }
    }
    dbLog.close();
    cout<<"Standard: "<<standard<<"\tClass Incharge: "<<first<<" "<<last<<endl<<endl;
    ifstream student("Standards\\"+standard+"\\students.txt");
    //string line;
    //vector<string> dat;
    vector<tuple<int, string>> students;
    while (getline(student, line)){
        tuple<int, string> temp;
        dat = retrieve(line);
        get<0>(temp) = stoi(dat[0]);
        get<1>(temp) = dat[1]+" "+dat[2];
        students.push_back(temp);
    }
    cout<<"STUDENT DETAILS"<<endl<<endl;
    sort(students.begin(), students.end());
    cout << "Roll Number|" << setw(15) << right << "Name"
         << "\t\t  |"<<endl;
    for (int i=0; i<students.size(); i++){
        cout << setw(6) << right << get<0>(students[i]) << setw(6) << "|" << left << setw(30) << get<1>(students[i])<< "|"<<endl;
    }

    cout<<endl<<"------------------------------------------------------------------------"<<endl<<endl;
    cout<<"SUBJECTS AND TEACHERS"<<endl<<endl;;
    cout << "S.No.\t|" << setw(30) << left <<"Subjects"<<"|"<<setw(20)<<left<<"Full Name"<<"|"<<endl;

    for (int i=0; i<subjects.size(); i++){
        ifstream dbTemp("teachers\\detailsTeach.txt");
        string full;
        search = subTeachers[i] + " ";
        while (getline(dbTemp, line)){
            if (line.find(search, 0) != string::npos){
                dat = retrieve(line);
                full = dat[1]+" "+dat[2];
            }
        }
        dbTemp.close();
        cout<<i+1<<"\t|"<<setw(30)<<left<<subjects[i]<<"|"<<setw(20)<<left<<full<<"|"<<endl;
    }
    cout<<endl<<endl<<"Press any key to continue.";
    getch();
}

void Standard::showAllMarks(int c)
{
    //vector<vector<tuple<int ,int >>> ordered;

    for (int i = 0; i < rollNo.size(); i++)
    {
        vector<tuple<int, int>> tempTemp;
        for (int j = 0; j < subjects.size(); j++)
        {
            tuple<int, int> temp;
            get<0>(temp) = rollNo[i];
            get<1>(temp) = -1;
            for (int k = 0; k < marks[j].size(); k++)
            {
                if (get<0>(marks[j][k]) == rollNo[i])
                {
                    get<1>(temp) = get<1>(marks[j][k]);
                    break;
                }
                //else get<1>(temp) = -1;
            }
            tempTemp.push_back(temp);
        }
        ordered.push_back(tempTemp);
    }
    vector<double> percent;
    for (int i = 0; i < ordered.size(); i++)
    {
        int marksTemp = 0;
        for (int j = 0; j < ordered[i].size(); j++)
        {
            if (get<1>(ordered[i][j]) != -1)
                marksTemp += get<1>(ordered[i][j]);
        }
        percent.push_back(marksTemp / (double)subjects.size());
    }
    cout << "Roll Number|" << setw(15) << right << "Name"
         << "\t\t  |" << setw(10) << right;
    for (int i = 0; i < subjects.size(); i++)
    {
        cout << setw(30)<<right << subjects[i] << "|" ;
    }
    cout << "\tTOTAL\t"
         << setw(8)<<"|" ;
    cout << endl;
    if (c == 1)
    {
        Student s;
        string first, last;
        for (int i = 0; i < ordered.size(); i++)
        {
            vector<string> dat = s.loadDatS(to_string(rollNo[i]), standard);
            string fullName = dat[1] + " " + dat[2];
            cout << setw(6) << right << rollNo[i] << setw(6) << "|" << left << setw(30) << fullName << "|" << setw(10) << right;
            for (int j = 0; j < ordered[i].size(); j++)
            {
                if (get<1>(ordered[i][j]) == -1)
                {
                    cout << setw(24) << right << "  "
                         << "N/A(0)"
                         << "|" << right;
                    ;
                }
                else
                    cout << setw(30) << right << get<1>(ordered[i][j]) << "|" << right; //<<setw(3);
            }
            cout <<  setw(15) << right << percent[i] << "%|" << right;
            cout << endl;
        }
    }
    else if (c == 2)
    {
        vector<tuple<double, int>> percentNew;
        for (int i = 0; i < percent.size(); i++)
        {
            tuple<double, int> temp;
            get<0>(temp) = percent[i];
            get<1>(temp) = i;
            percentNew.push_back(temp);
        }
        //cout<<"sorted"<<endl;
        sort(percentNew.begin(), percentNew.end());

        Student s;
        string first, last;
        for (int m = percentNew.size() - 1; m >= 0; m--)
        {
            int i = get<1>(percentNew[m]);
            vector<string> dat = s.loadDatS(to_string(rollNo[i]), standard);
            string fullName = dat[1] + " " + dat[2];
            cout << setw(6) << right << rollNo[i] << setw(6) << "|" << left << setw(30) << fullName << "|" << setw(10) << right;
            for (int j = 0; j < ordered[i].size(); j++)
            {
                if (get<1>(ordered[i][j]) == -1)
                {
                    cout << setw(24) << right << "  "
                         << "N/A(0)"
                         << "|" << right;
                    ;
                }
                else
                    cout << setw(30) << right << get<1>(ordered[i][j]) << "|" << right; //<<setw(3);
            }
            cout <<  setw(15) << right << percent[i] << "%|" << right;
            cout << endl;
        }
    }
    getch();
}


void MAIN_MENU()
{
    system("cls");
    cout << setw(80) << setfill('-') << "STUDENT REPORT MANAGEMENT SYSTEM";
    cout << setw(40) << setfill('-') << "-" << endl<< endl;
    cout << setw(66) << setfill(' ');
    cout << "(1) Admin" << endl;
    cout << setw(68) << setfill(' ');
    cout << "(2) Teacher" << endl;
    cout << setw(65) << setfill(' ');
    cout << "(3) Exit" << endl << endl;
    int num;
    cout << "Enter the corresponding number to select role.\n";
    cin >> num;
    admin a;//a = new admin;
    teacher t;
    switch (num)
    {
    case 1:
        system("cls");
        a.login();
        a.admin_menu();
        a.~admin();
        break;
    case 2:
        system("cls");
        t.login();
        t.teacherMenu();
        t.~teacher();
        break;
    case 3:
        system("cls");
        cout << "\t\t\t\tTHANK YOU!";
        getch();
        exit(1);
        break;
    default:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x75);
        cout << "TRY AGAIN";
        getch();
        system("cls");
        system("Color 70");
        a.~admin();
        t.~teacher();
        MAIN_MENU();
    }
    MAIN_MENU();
}

int main()
{
    system("cls");
    system("Color 70");
    MAIN_MENU();
    return 0;
}
