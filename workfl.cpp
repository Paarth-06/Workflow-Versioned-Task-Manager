#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <string>
#include <format>
using namespace std;

typedef struct{
    int task_id;
    string task_name;
    string task_status;
    string task_note;
}snap;

namespace wl{


    //serves for all the tasks such as : add user, remove user, show all users, show specific user
    class User{
        protected:
            static unordered_map<int,unordered_map<string,string>> user_data; // {user_id : {user data...}}
            static unordered_map<int,unordered_map<int,snap>> user_task; //stores data for the tasks per user
            static unordered_map<int,unordered_map<int,vector<snap>>> history; // user_id : {task_id : {vector......}}

        public :
            snap tk; //for tasks
            snap his; //for history
            static vector<string> task_status;
            User(){}
            User(string user_name,string user_gender,string user_age,string user_email,int user_id){
                if(check_user_usr_data(user_id) == 0){
                    user_data[user_id]["Name"] = user_name;
                    user_data[user_id]["Gender"] = user_gender;
                    user_data[user_id]["Age"] = user_age;
                    user_data[user_id]["Email"] = user_email;
                    cout<<"User created successfully..."<<endl;
                }
                else{
                    cout<<"Sorry the user already exist, please try a different user id..."<<endl;
                    return;
                }
            }
            void remove_user(int user_id){
                if(check_user_usr_data(user_id) == 1){
                    string choice;
                    cout<<"Are your sure to remove the user ? | y/n : ";
                    getline(cin,choice);
                    if (choice == "y" || choice == "Y"){
                        user_data.erase(user_id);
                        cout<<"User removed successfully..."<<endl;
                    }
                    else if (choice == "n" || choice == "N"){
                        cout<<"Aborting..."<<endl;
                        return;
                    }
                    else{
                        cout<<"Please enter a valid option...";
                        return;
                    }
                }
            }
        
            //shows all users present into the user_data
            void view_all_users(){
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

            //shows specific user only
            void view_user(int user_id){
                if(check_user_usr_data(user_id) == 1){
                    cout<<"User Id : "<<user_id<<endl;
                    for(const auto& pair : user_data.at(user_id)){
                        cout<<"  "<<pair.first<<" : "<<pair.second<<endl;
                    }
                }
            }

            //checking if the user_id exist in user_data
            bool check_user_usr_data(int user_id){
                if(user_data.find(user_id) != user_data.end()){
                    return 1; //found
                }
                else{
                    return 0; //not found
                }
            }

            //checking if the user_id -> task exist in user_task
            bool check_task_existance(int user_id,int task_id){
                auto userIt = user_task.find(user_id);
                if (userIt == user_task.end()){
                    
                    return 0;   // user has no tasks
                }

                if (userIt->second.find(task_id) != userIt->second.end()){

                    return 1;   // task exists
                }
                else{

                    return 0;   // task does not exist
                }

            }

            //checking if user_id exist in history or not
            bool check_history(int user_id){
                if(history.find(user_id) != history.end()){
                    return 1;
                }
                else{
                    return 0;
                }
            }

            //checking if the task of the user exist in history or not
            bool check_history_task(int user_id,int task_id){
                auto userIt = history.find(user_id);
                if (userIt == history.end()){
                    
                    return 0;   // user has no tasks
                }

                if (userIt->second.find(task_id) != userIt->second.end()){

                    return 1;   // task exists
                }
                else{

                    return 0;   // task does not exist
                }
            }



    };

    class Tasks : public User{

        public :
            Tasks(){}
            Tasks(string user_name,string user_gender,string user_age,string user_email,int user_id) : User(user_name,user_gender,user_age,user_email,user_id){}

            //storing user data into history....
            void take_snap(int user_id,int task_id){
                if(check_user_usr_data(user_id) == 1 && check_task_existance(user_id,task_id) == 1){
                
                    auto userIt = user_task.find(user_id);
                    if (userIt == user_task.end()) return;

                    auto taskIt = userIt->second.find(task_id);
                    if (taskIt == userIt->second.end()) return;

                    history[user_id][task_id].push_back(taskIt->second);
            

                    cout << "Saved in memory...\n"; 

                }
                else{
                    cout<<"Enter correct combination of user and task id..."<<endl;
                    return;
                }
            }
            void roll_back(int user_id,int task_id){
                if(check_history_task(user_id,task_id) == 1){
                    
                    cout<<"All the tasks are : "<<endl;
                    int choice;
                    view_history(user_id);
                    cout<<"Enter the version you want to roll back to : ";
                    cin>>choice;
                    if(choice >=0 && choice < history.at(user_id).at(task_id).size()){
                        user_task[user_id][task_id] = history[user_id][task_id][choice];
                        cout<<"Version restored..."<<endl;
                    }
                    else{
                        cout<<"Sorry that index does not exist, please try a valid index..."<<endl;
                        return;
                    }
                }
                else{
                    cout<<"Sorry the combination of user id and task id were invalid.."<<endl;
                    return;
                }
            }

            void view_history(int user_id){
                if(check_history(user_id) == 1){
                    const auto& user = history.find(user_id);
                    const auto& task = user->second;
                    cout<<"User Id : "<<user_id<<endl;
                    for(const auto& task_pair : task){
                        int task_id = task_pair.first;
                        const vector<snap>& versions = task_pair.second;
                        cout<<"Task id : "<<task_id<<endl;
                        for(size_t version_index = 0;version_index < versions.size();++version_index){
                            const snap& s = versions[version_index];
                            cout << "    Version Index: " << version_index << "\n";
                            cout << "      Name   : " << s.task_name << "\n";
                            cout << "      Status : " << s.task_status << "\n";
                            cout << "      Note   : " << s.task_note << "\n";
                        }

                    }

                    
                }
            }

            void create_task(string task_name,string task_status,string task_note,int user_id,int task_id){
                if(check_user_usr_data(user_id) == 1 && check_task_existance(user_id,task_id) == 0){
                    tk.task_id = task_id;
                    tk.task_name = task_name;
                    tk.task_status = task_status;
                    tk.task_note = task_note;
                    user_task[user_id][task_id] = tk;
                    take_snap(user_id,task_id);
                    cout<<"Task added successfully..."<<endl;
                }

            }

            void remove_task(int user_id,int task_id){
                if(check_user_usr_data(user_id) == 1 && check_task_existance(user_id,task_id) == 1){
                    string choice;
                    cout<<"Are you sure to delete the task ? | y/n : ";
                    getline(cin,choice);
                    if(choice == "y" || choice == "Y"){
                        take_snap(user_id,task_id);
                        user_task.at(user_id).erase(task_id);
                        cout<<"Task deleted successfully....."<<endl;
                    }
                    else if(choice == "n" || choice == "N"){
                        cout<<"Aborting..."<<endl;
                        return;
                    }
                    else{
                        cout<<"Enter a valid option...."<<endl;
                        return;
                    }
                }
            }

            void change_task_status(int user_id,int task_id,int choice){
                if(check_task_existance(user_id,task_id) == 1){ // 1 = task_status[0] -> TO-DO, 2 = task_status[1] -> IN_PROGRESS, 3 = task_status[3] -> COMPLETED
                    if(choice == 1){
                        user_task.at(user_id).at(task_id).task_status = task_status[0];
                        take_snap(user_id,task_id);
                        cout<<"Task status changed successfully..."<<endl;
                    }
                    else if(choice == 2){
                        user_task.at(user_id).at(task_id).task_status = task_status[1];
                        take_snap(user_id,task_id);
                        cout<<"Task status changed successfully..."<<endl;
                    }
                    else if(choice == 3){
                        user_task.at(user_id).at(task_id).task_status = task_status[2];
                        take_snap(user_id,task_id);
                        cout<<"Task status changed successfully..."<<endl;
                    }
                    else{
                        cout<<"Please enter a valid choice..."<<endl;
                        return;
                    }
                }
            }
            void change_task_note(int user_id,int task_id,string note){
                if(check_task_existance(user_id,task_id) == 1){
                    take_snap(user_id,task_id);
                    user_task.at(user_id).at(task_id).task_note = note;
                    cout<<"Note changed successfully..."<<endl;
                }
                else{
                    cout<<"The combination of user id and task id is invalid..."<<endl;
                    return;
                }
            }
            void change_task_title(int user_id,int task_id,string title){
                if(check_task_existance(user_id,task_id)==1){
                    take_snap(user_id,task_id);
                    user_task.at(user_id).at(task_id).task_name = title;
                    cout<<"Task title change successfully..."<<endl;
                }
                else{
                    cout<<"The combination of user id and task id is invalid..."<<endl;
                    return;
                }
            }
            void view_task(int user_id){
                if (check_user_usr_data(user_id) == 0) {
                    cout << "Sorry the user does not exist..." << endl;
                    return;
                }

                auto it = user_task.find(user_id);
                if (it == user_task.end() || it->second.empty()) {
                    cout << "No tasks available for this user.\n";
                    return;
                }

                for (const auto& [task_id, curr] : user_task[user_id]) {
                    cout << "Task id     : " << curr.task_id << endl;
                    cout << "Task title  : " << curr.task_name << endl;
                    cout << "Task status : " << curr.task_status << endl;
                    cout << "Task note   : " << curr.task_note << endl;
                    cout << "---------------------------" << endl;
                }
            }



    };










}
vector<string> wl::User::task_status = {
    "TO-DO",
    "IN_PROGRESS",
    "COMPLETED"
};
unordered_map<int,unordered_map<string,string>> wl::User::user_data;
unordered_map<int,unordered_map<int,snap>> wl::User::user_task;
unordered_map<int,unordered_map<int,vector<snap>>> wl::User::history;










int main(){
    int choice;
    string name,email,age,gender,title,note,status;
    int user_id,task_id,stat_choice;
    wl::Tasks t;
    while (true){
        cout<<"\nWelcome to work co...."<<endl;
        cout<<"\n1.Add User \n2.Remove User \n3.Show all users \n4.Show a specific user \n5.Add task \n6.Remove task \n7.Change task title \n8.Change task status \n9.Change task note \n10.Restore Version \n11.View tasks \n12.Exit "<<endl;
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
            cout<<"Enter your desired User Id : ";
            cin>>user_id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Enter your email : ";
            getline(cin,email);
            t = wl::Tasks(name,gender,age,email,user_id);

        }
        else if(choice == 2){
            cout<<"Enter the user id to remove : ";
            cin>>user_id;
            t.remove_user(user_id);
        }
        else if(choice == 3){
           t.view_all_users();
        }
        else if(choice == 4){
            cout<<"Enter your user id : ";
            cin>>user_id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            t.view_user(user_id);

        }
        else if(choice == 5){
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
            t.create_task(title,status,note,user_id,task_id);

        }
        else if(choice == 6){
           cout<<"Enter user id : ";
           cin>>user_id;
           cout<<"Enter the task id to delete : ";
           cin>>task_id;
           t.remove_task(user_id,task_id);
        }
        else if(choice == 7){
            cout<<"Enter user id : ";
            cin>>user_id;
            cout<<"Enter task id to change the status of : ";
            cin>>task_id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Enter the new task title : ";
            getline(cin,title);
            t.change_task_title(user_id,task_id,title);
        }
        else if(choice == 8){
            cout<<"Enter user id : ";
            cin>>user_id;
            cout<<"Enter the task id : ";
            cin>>task_id;
            cout<<"Enter which task status do you want : \n1.TO-DO \n2.IN_PROGRESS \n3.COMPLETED \nYour response : ";
            cin>>stat_choice;
            t.change_task_status(user_id,task_id,stat_choice);
        }
        else if(choice == 9){
            cout<<"Enter your user id : ";
            cin>>user_id;
            cout<<"Enter task id : ";
            cin>>task_id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Enter new note for the task : ";
            getline(cin,note);
            t.change_task_note(user_id,task_id,note);
        }
        else if(choice == 10){
            cout<<"Enter user id : ";
            cin>>user_id;
            cout<<"Enter task id : ";
            cin>>task_id;
            t.roll_back(user_id,task_id);
        }
        else if(choice == 11){
            cout<<"Enter user id : ";
            cin>>user_id;
            t.view_task(user_id);
        }
        else if(choice == 12){
            break;
        }
        else{
            cout<<"Please enter a valid option...";
        }
    }
    return 0;
}