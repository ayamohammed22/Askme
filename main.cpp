#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
using namespace std;
struct Question
{
    int id;
    string sender_username, reciver_username;
    string Question_text, answer_text;
    bool is_anonymous;
};

struct person
{
    string username, name, email, password;
    bool anonymous;
    vector<Question> send, receive;
};
vector<Question> Questions;
vector<person> persons;
set<string> user_name;
struct
{
    void members_datadownload() {
        ifstream data1, data2;
        data1.open("PERSONS.txt");
        persons.clear();
        user_name.clear();
        if (data1.is_open()) {
            while (data1.good()) {
                persons.emplace_back();
                data1 >> persons.back().username >> persons.back().name >> persons.back().email
                      >> persons.back().password >> persons.back().anonymous;
            }
        }
        else{
            cout<<"Failed downloads data of members\n";
        }
        data1.close();
        data2.open("USERNAME.txt", ios::in);
        user_name.clear();
        if (data2.is_open()) {
            while (data2.good()) {
                string s;
                data2 >> s;
                user_name.emplace(s);
            }
        }
        else{
            cout<<"Failed downloads username of members\n";
        }
        data2.close();
    }
    void members_data_upload(){
        ofstream data1,data2;
        data1.open("PERSONS.txt",ios :: trunc);
        if(data1.is_open()){
            for(auto p:persons)
                data1<<p.username<<" "<<p.name<<" "<<p.email<<" "<<p.password<<" "<<p.anonymous<<"\n";
        }
        else{
            cout<<"Failed downloads of members data \n";
        }
        data1.close();

        data2.open("USERNAME.txt",ios :: trunc);
        if(data2.is_open()){
            for(auto p:persons)
                data2<<p.username<<"\n";
        }
        else{
            cout<<"Failed downloads of members data \n";
        }
        data2.close();
        return ;
    }
    void members_data_update(){
        ofstream data1,data2;
        data1.open("PERSONS.txt",ios :: app);
        if(data1.is_open()){
            auto p=persons.back();
            data1<<p.username<<" "<<p.name<<" "<<p.email<<" "<<p.password<<" "<<p.anonymous<<"\n";
        }
        else{
            cout<<"Failed update of members data \n";
        }
        data1.close();

        data2.open("PERSONS.txt",ios :: app);
        if(data2.is_open()){
            auto p=persons.back();
            data2<<p.username<<" "<<p.name<<" "<<p.email<<" "<<p.password<<" "<<p.anonymous<<"\n";
        }
        else{
            cout<<"Failed update username of members\n";
        }
        data2.close();
    }
    void Questions_data_download(){
        for(auto &p:persons){
            p.send.clear();
            p.receive.clear();
        }
        Questions.clear();
        ifstream data;
        data.open("QUESTIONS.txt");
        if (data.is_open())
        {
            while (data.good())
            {
                Questions.emplace_back();
                auto &it = Questions.back();
                data >> it.id >> it.sender_username >> it.reciver_username >> it.is_anonymous;
                getline(data, it.Question_text, '\n');
                getline(data, it.answer_text, '\n');
                for(auto &p:persons){
                    if(p.username == it.sender_username){
                        p.send.emplace_back(it);
                    }
                    if(p.username == it.reciver_username){
                        p.receive.emplace_back(it);
                    }
                }
            }
        }
        else{
            cout<<"Failed download data questions\n";
        }
        data.close();
        return ;
    }
    void Questions_uploaddata(){
        ofstream data("QUESTIONS.txt", ios::trunc);
        if(data.is_open()){
            for(auto &p:persons)
            {
                p.receive.clear();
                p.send.clear();
            }
            for(auto it:Questions) {
                data << it.id <<" "<< it.sender_username <<" "<< it.reciver_username <<" "<< it.is_anonymous<<" "<<it.Question_text<<"\n";
                data<<it.answer_text<<"\n";
                for(auto &p:persons){
                    if(p.username==it.sender_username)
                        p.send.push_back(it);
                    if(p.username==it.reciver_username)
                        p.receive.push_back(it);
                }
            }
        }
        else{
            cout<<"Failed upload data questions\n";
        }
        data.close();
        return ;
    }
    void Questions_update(){
        ofstream data("QUESTIONS.txt",  ios::app);
        if(data.is_open()){
            auto it=Questions.back();
            data << it.id <<" "<< it.sender_username <<" "<< it.reciver_username <<" "<< it.is_anonymous<<" "<<it.Question_text<<"\n";
            data<<it.answer_text<<"\n";
            for(auto &p:persons){
                if(p.username==it.sender_username)
                    p.send.push_back(it);
                if(p.username==it.reciver_username)
                    p.receive.push_back(it);
            }
        }
        else{
            cout<<"Failed upload data questions\n";
        }
        return ;
    }
}database;
struct
{

    person member;
    void print_question_to_me()
    {
        if (member.receive.size() == 0)
        {
            cout << "No Questions"<<endl;
            return;
        }
        for (auto it : member.receive)
        {
            cout << "Question id " << it.id << " from ";
            cout << ((it.is_anonymous) ? it.sender_username : "anonymous") << endl;
        }
        return;
    }


    void print_question_from_me()
    {
        if (member.send.size() == 0)
        {
            cout << "No Questions \n";
        }
        else {
            for (auto it: member.send)
                cout << "Question id " << it.id  <<" to " << it.reciver_username << endl;
        }
        return;
    }

    void answer_question()
    {
        cout << "Enter Id of question" << endl;
        for(int tryy=0;tryy<=10;tryy++)
        {
            int id;
            cin >> id;
            bool findd=false;
            for (auto &it : member.receive)
            {
                if (it.id == id) {
                    findd=true;
                    if (not it.answer_text.empty()) {
                        cout << "Question Already answered" << endl;
                        return;
                    }
                }
            }
            if(not findd) {
                cout << "Invalid input , Try again !!" << endl;
                continue;
            }
            cout<<"Enter Your answer :";
            string ans;
            cin.ignore();
            getline(cin,ans);
            for(auto &it:Questions){
                if(it.id==id){
                    it.answer_text=ans;
                }
            }
            database.Questions_uploaddata();
           return ;
        }
    }


    void delete_question()
    {
        cout << "Enter Id of question" << endl;
        for(int tryy=0;tryy<=10;tryy++)
        {
            int id; cin>>id;
            bool findd=false;
            for(auto it:member.send){
                if(it.id==id){
                    findd=true;
                    break;
                }
            }
            if(not findd){
                cout<<"Invalid Input , please Try again !! \n";
                continue;
            }
            int indx=0;
            for(int i=0;i<Questions.size();i++){
                if(Questions[i].id==id){
                    indx=i;
                }
            }
            Questions.erase(next(Questions.begin(),indx));
            database.Questions_uploaddata();
            return ;
        }
    }

    void ask_question()
    {
        cout << "Enter username of receiver " << endl;
        string username;
        cin >> username;
        person member2;
        for (auto it : persons)
        {
            if (it.username == username)
            {
                member2 = it;
                break;
            }
        }
        int idd=0;
        for(auto it:Questions)
            idd=max(it.id,idd);
        Questions.emplace_back();
        if (member2.anonymous)
        {
            cout << "Is anonymous Question (1/0) :";
            cin >> Questions.back().is_anonymous;
        }
        else{
            cout<<"Question anonymous sender is not allow \n";
        }
        cout << "Enter Your Question :";
        cin.ignore();
        getline(cin,Questions.back().Question_text);
        Questions.back().id = ++idd;
        Questions.back().sender_username=member.username;
        Questions.back().reciver_username=member2.username;
        database.Questions_uploaddata();
        return;
    }
    void list_of_users()
    {
        int cnt = 1;
        for (auto it : persons)
            cout << cnt++ << ") is username " << it.username << " ,name " << it.name << " and email" << it.email << endl;
    }

} manager_of_menu;
void main_page(person member)
{
    while(true){
        manager_of_menu.member = member;
        cout << "Menu : " << endl;
        cout << "1) Print questions to me " << endl;
        cout << "2) Print questions from me " << endl;
        cout << "3) Answer question" << endl;
        cout << "4) Delete question" << endl;
        cout << "5) Ask question" << endl;
        cout << "6) List System user" << endl;
        cout << "7) Log out" << endl;
        while (true)
        {
            cout << "Enter number in range (1-7)" << endl;
            int x;
            cin >> x;
            if (x == 1)
            {
                manager_of_menu.print_question_to_me();
                break;
            }
            if (x == 2)
            {
                manager_of_menu.print_question_from_me();
                break;
            }
            if (x == 3)
            {
                manager_of_menu.answer_question();
                break;
            }
            if (x == 4)
            {
                manager_of_menu.delete_question();
                break;
            }
            if (x == 5)
            {
                manager_of_menu.ask_question();
                break;
            }
            if (x == 6)
            {
                manager_of_menu.list_of_users();
                break;
            }
            if (x == 7)
            {
                return;
            }
            cout << "Invalid number try again\n";
        }
    }
}
void signin()
{
    string username, password;
    cout << "Enter your username and password :";
    cin >> username >> password;
    person member ;
    bool ok=false;
    for (auto it : persons)
    {
        if (it.username == username and it.password == password)
        {
            member = it;
            ok=true;
            break;
        }
    }
    if (! ok)
    {
        cout << "username or password are wrong\n";
        return;
    }
    main_page(member);
}

void signup()
{
    person p;
    while (true)
    {
        string user;
        cout << "Enter username(no spaces): ";
        cin >> user;
        cout << "\n";
        if (user_name.find(user) == user_name.end())
        {
            p.username = user;
            user_name.insert(user);
            cin.ignore();
            cout << "Enter Your name: ";
            getline(cin, p.name);
            cout << "\n";
            cout << "Enter Your password: ";
            cin >> p.password;
            cout << "\n";
            cout << "Enter Your email: ";
            cin >> p.email;
            cout << "\n";
            cout << "Allow anonymous questions?(1 or 0): ";
            cin >> p.anonymous;
            persons.push_back(p);
            database.members_data_upload();
            signin();
            break;
        }
        cout << "Already used ,Please Try again \n";
    }
}

void main_menu()
{
    cout << "MENU:" << endl;
    cout << "1) Sign Up" << endl;
    cout << "2) Sign In" << endl;
    cout << "Enter number in range (1-2)" << endl;
    int x;
    cin >> x;
    while (true)
    {
        if (x == 1)
        {
            signup();
            break;
        }
        if (x == 2)
        {
            signin();
            break;
        }
        cout << "Invalid number try again\n";
    }
    return;
}
int main()
{
    database.members_datadownload();
    database.Questions_data_download();

    main_menu();
    return 0;
}
