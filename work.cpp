#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <limits>
using namespace std;

typedef struct{ //structure to store the history and task(acts like a blueprint on how the things are supposed to be stored)....
        int id;
        string title;
        string status;
        string note;
    }snap;

namespace wk{
    
    class User{
        public:
            snap t; //object for task
            snap s; //object for history
            static unordered_map<int,unordered_map<string,string>> user_data; // user_id {user data....}
            static unordered_map<int,unordered_map<int,vector<snap>>> history; //user_id {task_id{task history....}}
            static unordered_map<int,unordered_map<int,snap>> task; // user_id{task_id{tasks.....}}
            User(){}
            User(string name,string gender,string age,string email,int user_id){         //Adding a user..
                if(user_data.find(user_id)== user_data.end()){ // checking if the user_id already exist or not, if not we'll create one...
                    user_data[user_id]["Name"]= name;
                    user_data[user_id]["Age"] = age;
                    user_data[user_id]["Gender"] = gender;
                    user_data[user_id]["Email"] = email;
                    cout<<"User created successfully..."<<endl;
                }
                else{
                    cout<<"Sorry the user already exists..."<<endl;
                    return;
                }
            
            
            }
            void remove_user(int user_id){
                if(user_data.find(user_id) != user_data.end()){ // checking if the user exists or not, if yes then proceed or else return
                    string choice;
                    cout<<"Do you wish to remove the user ? y/n : ";
                    getline(cin,choice);
                    if(choice == "y" || choice == "Y"){
                        user_data.erase(user_id);
                        cout<<"User removed successfully..."<<endl;
                    }
                    else if(choice == "n" || choice == "N"){
                        cout<<"Exiting..."<<endl;
                        return;
                    }
                }
            }

            void view_user(){
                for (const auto& userPair : user_data) { //printing data from user_data through for each loop, auto& means uuserPair is a referencing to user_data and type is automatically detected
                    int user_id = userPair.first;
                    const auto& fields = userPair.second;
                    cout << "User ID: " << user_id << '\n';

                    for (const auto& fieldPair : fields) {
                        cout << "  " << fieldPair.first << " : " << fieldPair.second << '\n';
                    }

                    cout << "----------------------\n";
                }
            }
    };
 
    class Tasks : public User{
        public:
            Tasks(){}
            Tasks(string name,string gender,string age,string email,int user_id) : User(name,gender,age,email,user_id){}
            static vector<string> status; //creating a vector for status, so that status changing is more reliable and faster.

            // Taking a snapshot of the current data after doing some changes and storing it in history so that roll back could be done.
            void take_snap(int user_id,int task_id){
                if (task.find(user_id) != task.end() && task.at(user_id).find(task_id) != task.at(user_id).end()){
                    s.id = task_id;
                    s.title = t.title;
                    s.status = t.status;
                    s.note = t.note;
                    history[user_id][task_id].push_back(s);
                    cout<<"Snapshot taken successfully...."<<endl;
                    
                }
            }
            void add_task(int user_id,int task_id,string title,string status,string note){
                auto& userTasks = task[user_id];
                if(task.at(user_id).find(task_id) == task.at(user_id).end()){
                    t.id = task_id;
                    t.title = title;
                    t.status = status;
                    t.note = note;
                    task[user_id][task_id] = t;
                    take_snap(user_id,task_id);
                    cout<<"Task Created successfully.."<<endl;
                }
                else{
                    cout<<"Task id already exists, please enter another task id..."<<endl;
                    return;
                }
            }

            void change_status(int user_id,int task_id){
                if (task.find(user_id) != task.end() && task.at(user_id).find(task_id) != task.at(user_id).end()){
                    int choice;
                    cout<<"What statu do you want to change the current task to : \n1.todo \n2.in_progress \n3.Completed : ";
                    cin>>choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if(choice == 1){
                        task.at(user_id).at(task_id).status = status[0];
                        t.status = status[0];
                        take_snap(user_id,task_id);
                    }
                    else if(choice==2){
                        task.at(user_id).at(task_id).status = status[1];
                        t.status = status[1];
                        take_snap(user_id,task_id);
                    }
                    else if(choice == 3){
                        task.at(user_id).at(task_id).status = status[2];
                        t.status = status[2];
                        take_snap(user_id,task_id);
                    }
                    else{
                        cout<<"Choose a valid option..."<<endl;
                        return;
                    }
                }
                else{
                    cout<<"Sorry, the combination of the user id and task id was invalid, please try again..."<<endl;
                    return;
                }
            }
            void remove_task(int user_id,int task_id){
                if (task.find(user_id) != task.end() && task.at(user_id).find(task_id) != task.at(user_id).end()){
                    string choice;
                    cout<<"Are you sure to delete this task ? this can be later recovered by simply restoring the previous version | y/n : ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin,choice);
                    if(choice == "y" || choice == "Y"){
                        take_snap(user_id,task_id);
                        task.at(user_id).erase(task_id);
                        cout<<"Task deleted successfully....";
                    }
                    else if(choice == "n" || choice == "N"){
                        cout<<"Aborting delete...";
                        return;
                    }
                }
                else{
                    
                    cout<<"Sorry, the combination of the user id and task id was invalid, please try again..."<<endl;
                    return;
                }
                
            }
            // rolling back to the last saved version that is second last index of the history.
            void jump_back(int user_id, int task_id) {
                if (task.find(user_id) == task.end() ||
                    task.at(user_id).find(task_id) == task.at(user_id).end()) {
                    cout << "Task not found\n";
                    return;
                }
                if (history.find(user_id) == history.end() ||
                    history.at(user_id).find(task_id) == history.at(user_id).end()) {
                    cout << "No history found\n";
                    return;
                }
                auto &vec = history[user_id][task_id];
                if (vec.size() < 2) { //checking if in history there are more than 2 indexes , else returning to the main program without rolling back.
                 cout<<"Sorry cannot restore version since only one version exist...."<<endl;
                 return;   
                }
                size_t second_last_index = vec.size() - 2; // second last index of history
            
                // 4️⃣ Assign second last snapshot
                task[user_id][task_id] = history[user_id][task_id][second_last_index]; // rolled back to the previous saved version..
                cout<<"Version restored successfully...."<<endl;
            }

            void view_task(int user_id){
                if (task.find(user_id) == task.end()) {
                    cout << "Sorry the user does not exist..." << endl;
                    return;
                }

                if (task[user_id].empty()) {
                    cout << "No tasks available for this user." << endl;
                    return;
                }

                for (const auto& [task_id, curr] : task[user_id]) {
                    cout << "Task id     : " << curr.id << endl;
                    cout << "Task title  : " << curr.title << endl;
                    cout << "Task status : " << curr.status << endl;
                    cout << "Task note   : " << curr.note << endl;
                    cout << "---------------------------" << endl;
                }
            }


    };










}
vector<string> wk::Tasks::status = {
                "todo",
                "in_progress",
                "Completed"
            };
unordered_map<int,unordered_map<string,string>> wk::User::user_data;
unordered_map<int,unordered_map<int,vector<snap>>> wk::User::history;
unordered_map<int,unordered_map<int,snap>> wk::User::task;







int main(){
    int choice;
    string name,email,age,gender,title,note,status;
    int user_id,task_id,stat_choice;
    wk::Tasks t;
    while (true){
        cout<<"\nWelcome to work co...."<<endl;
        cout<<"\n1.Add User : \n2.Remove User : \n3.Show all users : \n4.Add task : \n5.Remove task : \n6.Change status of a task : \n7.Revert to last saved version \n8.View your tasks \n9.Exit"<<endl;
        cout<<"Your action : ";
        cin>>choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(choice == 1){
            cout<<"Enter your name : ";
            getline(cin,name);
            cout<<"Enter your age : ";
            getline(cin,age);
            cout<<"Enter your gender : ";
            getline(cin,gender);
            cout<<"Enter your desired user_id : ";
            cin>>user_id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Enter your email : ";
            getline(cin,email);
            t = wk::Tasks(name,gender,age,email,user_id);

        }
        else if(choice == 2){
            cout<<"Enter the user id to remove : ";
            cin>>user_id;
            t.remove_user(user_id);
        }
        else if(choice == 3){
           t.view_user();
        }
        else if(choice == 4){
            cout<<"Enter the user_id : ";
            cin>>user_id;
            cout<<"Enter the desired task id for the task : ";
            cin>>task_id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Enter the title of the task : ";
            getline(cin,title);
            cout<<"Enter the status of the task | eg : todo, in_process, completed. : ";
            getline(cin,status);
            cout<<"Enter a note for the task : ";
            getline(cin,note);
            t.add_task(user_id,task_id,title,status,note);

        }
        else if(choice == 5){
           cout<<"Enter user id : ";
           cin>>user_id;
           cout<<"Enter the task id to delete : ";
           cin>>task_id;
           t.remove_task(user_id,task_id);
        }
        else if(choice == 6){
            cout<<"Enter user id : ";
            cin>>user_id;
            cout<<"Enter task id to change the status of : ";
            cin>>task_id;
            t.change_status(user_id,task_id);
        }
        else if(choice == 7){
            cout<<"Enter user id : ";
            cin>>user_id;
            cout<<"Enter the task id to revert : ";
            cin>>task_id;
            t.jump_back(user_id,task_id);
        }
        else if(choice == 8){
            cout<<"Enter your user id : ";
            cin>>user_id;
            t.view_task(user_id);
        }
        else if(choice == 9){
            break;
        }
        else{
            cout<<"Please enter a valid option...";
        }
    }
    return 0;
}
