#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include<math.h>
#include <sstream>
#include <fstream>
using namespace std;
const string  clients = "clients.txt";
const string users = "users.txt";
const string delim = "/##/";

short read_choice() {
	short choice;
	do {
		cout << "Enter your choice: ";
		cin >> choice;
		if (cin.fail() || choice < 1 || choice > 8) {
			cin.clear(); // clear the error flag
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore the invalid input
			cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
		}
	} while (cin.fail() || choice < 1 || choice > 8);
	return choice;


}
short read_choice_2() {
	short choice;
	do {
		cout << "Enter your choice: ";
		cin >> choice;
		if (cin.fail() || choice < 1 || choice > 4) {
			cin.clear(); // clear the error flag
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore the invalid input
			cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
		}
	} while (cin.fail() || choice < 1 || choice > 4);
	return choice;
}
short read_choice_3() {
	short choice;
	do {
		cout << "Enter your choice: ";
		cin >> choice;
		if (cin.fail() || choice < 1 || choice > 6) {
			cin.clear(); // clear the error flag
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore the invalid input
			cout << "Invalid choice. Please enter a number between 1 and 6." << endl;
		}
	} while (cin.fail() || choice < 1 || choice > 6);
	return choice;
}
enum main_options {
	
	SHOW_ALL_CLIENTS = 1,
	ADD_CLIENT ,
	FIND_CLIENT,
	DELETE_CLIENT,
	UPDATE_CLIENT,
	TRANSECTION,
	USER_MANAGMENT,
	EXIT
};
enum trans_options {
	DEPOSIT = 1,
	WITHDRAW,
	TOTAL,
	GO_BACK
};
enum user_options {
	show_user = 1,
	ADD_USER ,
	FIND_USER,
	DELETE_USER,
	UPDATE_USER
};
enum user_permission {
	All = -1, 
	Show_clients = 1,
	Add_clients = 2,
	Find_clients = 4,
	Delete_clients = 8,
	update_clients = 16,
	Transection = 32,
	Manage_users = 64,
};
int read_permission() {
	int total_permission = 0;
	char ch;
	cout << "Do you want to give a full access to this user? (y/n): ";
	cin >> ch;
	if (tolower(ch) == 'y') {
		return -1; // Full access
	}
	cout << "Do you want to give access to ? (y/n):\n ";
	string arr[8] = { "Show Clients", "Add Clients", "Find Clients", "Delete Clients", "Update Clients", "Transection", "Manage Users" };
	for (short i = 0; i < 7; i++) {
		cout << 1 + i << ": " << arr[i] << endl;
		cin >> ch;
		if (tolower(ch) == 'y') {
			total_permission += pow(2, i);
		}


	}
	if (total_permission == 127) { // 127 is the sum of all permissions
		return -1; // Full access
	}
	return total_permission;
}
bool check_permission(int permission, user_permission check) {
	if (permission == -1) return true; // Full access
	return (permission & check) == check; // Check if the permission includes the required permission
}

void ShowAccessDeniedMessage() { 
	cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n"; 
}

void show_main_screen();

string system_date() {
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);

	string date = to_string(now.tm_year + 1900) + "/" +
		to_string(now.tm_mon + 1) + "/" +
		to_string(now.tm_mday);

	return date;
}

struct user_info {
	string username;
	string password;
	string permission;
	string date = system_date();
	bool mark_for_delete = false;
};
struct client_info {
	string ID;
    string name;
    string PIN_code;
	string Phone_number;
	string balance;
	string date = system_date();
	bool mark_for_delete = false;
};
user_info user_loged_in;
vector<string> split_str(string str, string delimiter = delim) {
	vector<string> tokens;
	short pos = 0;
	while ((pos = str.find(delimiter)) != string::npos) {
		string word = str.substr(0, pos);
		if (word != "") {
			tokens.push_back(word);

		}
		str.erase(0, pos + delimiter.length());

	}
	if (str != "") {
		tokens.push_back(str);
	}
	return tokens;
}
string convert_to_line(client_info client, string delimiter = delim) {
	return client.ID + delimiter + client.name + delimiter + client.PIN_code + delimiter + client.Phone_number + delimiter +
		client.balance + delimiter + client.date;
}
string convert_to_line_user(user_info user, string delimiter = delim) {
	return user.username + delimiter + user.password + delimiter + user.permission + delimiter + user.date;
}

// this for converting to record from line
client_info convert_to_record(string line, string delimiter = delim) {
	client_info client;
	vector<string> tokens = split_str(line, delimiter);
	client.ID = tokens[0];
	client.name = tokens[1];
	client.PIN_code = tokens[2];
	client.Phone_number = tokens[3];
	client.balance = tokens[4];
	client.date = tokens[5];
	return client;

}
user_info convert_to_record_user(string line, string delimiter = delim) {
	user_info user;
	vector<string> tokens = split_str(line, delimiter);
	user.username = tokens[0];
	user.password = tokens[1];
	user.permission = tokens[2];
	user.date = tokens[3];
	return user;
}



// load data from file
vector <client_info> load_clients(string file_name = clients) {
	vector<client_info> clients_list;
	fstream file;
	file.open(file_name, ios::in);

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (line != "") {
				client_info client = convert_to_record(line);
				clients_list.push_back(client);
			}
		}
		file.close();
	}
	else {
		cout << "File not found!" << endl;
	}
	return clients_list;

}


// load users from file
vector <user_info> load_users(string file_name = users) {
	vector<user_info> users_list;
	fstream file;
	file.open(file_name, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (line != "") {
				user_info user = convert_to_record_user(line);
				users_list.push_back(user);
			}
		}
		file.close();
	}
	else {
		cout << "File not found!" << endl;
	}
	return users_list;
}


void print_single_client(client_info client) {
	cout << "========================================================================================\n";
	cout << "Client ID: " << client.ID << endl;
	cout << "Client Name: " << client.name << endl;
	cout << "Client PIN Code: " << client.PIN_code << endl;
	cout << "Client Phone Number: " << client.Phone_number << endl;
	cout << "Client Balance: " << client.balance << endl;
	cout << "Client Date: " << client.date << endl;
	cout << "========================================================================================\n";
}
void print_single_user(user_info user) {
	cout << "========================================================================================\n";
	cout << "Username:    " << user.username << endl;
	cout << "Password:    " << user.password << endl;
	cout << "Permission:  " << user.permission << endl;
	cout << "=========================================================================================\n";
}

// find user by username i put it here to check if the user is exist or not when i add a new user 
bool find_user_by_username(user_info& user, string username, string file_name = users) {
	vector<user_info> users_list = load_users(file_name);
	for (auto u : users_list) {
		if (u.username == username) {
			user = u;
			return true;
		}
	}
	return false;
}
bool find_user_by_username_and_password(string user_name, string password, user_info& user, string file_name = users) {
	vector<user_info> users_list = load_users(file_name);
	for (user_info u : users_list) {
		if (u.username == user_name && u.password == password) {
			user = u;
			return true;
		}
	}
	return false;
}

void find_user_func() {
	user_info user;
	char choice;
	do {
		system("cls");
		string username;
		cout << "Enter Username: ";
		getline(cin >> ws, username);
		if (find_user_by_username(user, username)) {
			cout << "\n========================================================================================\n";
			cout << "\tFound User!" << endl;
			cout << "\tUser Information:\n";
			print_single_user(user);
		}
		else {
			cout << "User not found!" << endl;
		}
		cout << "Do you want to search for another user? (y/n): ";
		cin >> choice;
	} while (tolower(choice) == 'y');

}
client_info read_client_info() {
	client_info client;
	cout << "Enter Client ID: ";
	getline(cin >> ws, client.ID);
	cout << "Enter Client Name: ";
	getline(cin, client.name);
	cout << "Enter Client PIN Code: ";
	getline(cin, client.PIN_code);
	cout << "Enter Client Phone Number: ";
	getline(cin, client.Phone_number);
	cout << "Enter Client Balance: ";
	getline(cin, client.balance);
	return client;
}
user_info read_user_info() {
	user_info user;
	cout << "Enter Username: ";
	getline(cin >> ws, user.username);
	while (find_user_by_username(user, user.username)) {
		cout << "Username already exists! Please enter a different username: ";
		getline(cin >> ws, user.username);
	}
	cout << "Enter Password: ";
	getline(cin >> ws, user.password);
	user.permission = to_string(read_permission());
	return user;
}


void print_client_info(client_info client) {
	cout << "|"<< left << setw(10) << client.ID << "|" <<left << setw(30)<<client.name <<
		"|" << left << setw(10) << client.PIN_code << "|" << left << setw(15) << client.Phone_number << "|"
		<< left << setw(10) << client.balance << "|" << left << setw(10) << client.date << "|" << endl;
}
void print_user_info(user_info user) {
	cout << "|" << left << setw(15) << user.username << "|" << left << setw(15) << user.password
		<< "|" << left << setw(10) << user.permission << "|" << left << setw(10) << user.date << "|" << endl;
}

// add client
void add_client(client_info client, string file_name = clients) {
	fstream file;
	file.open(file_name, ios::out| ios::app);
	if (file.is_open()) {
		string line = convert_to_line(client);
		file << line << endl;
		file.close();
	}


	
}
void add_client_function() {

	client_info client;
	char choice;

	do {
		client = read_client_info();
		add_client(client);
		cout << "Client added successfully!" << endl;
		cout << "Do you want to add another client? (y/n): ";
		cin >> choice;
	} while (tolower(choice) == 'y');
}
// add user
void add_user(user_info user, string file_name = users) {
	fstream file;
	file.open(file_name, ios::out | ios::app);
	if (file.is_open()) {
		string line = convert_to_line_user(user);
		file << line << endl;
		file.close();
	}
	else {
		cout << "File not found!" << endl;
}
}
void add_user_function() {
	user_info user;
	char choice;
	do {
		user = read_user_info();
		add_user(user);
		cout << "User added successfully!" << endl;
		cout << "Do you want to add another user? (y/n): ";
		cin >> choice;
	} while (tolower(choice) == 'y');
}


// show all clients
void show_all_clients(string file_name = clients) {
	fstream file;
	file.open(file_name, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (line != "") {
				client_info client = convert_to_record(line);
				print_client_info(client);
			}
		}
		file.close();
	}
	else {
		cout << "File not found!" << endl;



	}
}
void row_client_info() {
	cout << "============================================================================================\n";
	cout << "                                  Client Information                                     \n";
	cout << "============================================================================================\n";
	   cout << "|" << left << setw(10) << "ID" << "|"
		<< left << setw(30) << "Name" << "|"
		<< left << setw(10) << "PIN Code" << "|"
		<< left << setw(15) << "Phone Number" << "|"
		<< left << setw(10) << "Balance" << "|"
		<< left << setw(10) << "Date" << "|" << endl;
	   cout << "============================================================================================\n";

}
void show_client_func() {
	if (check_permission(stoi(user_loged_in.permission), user_permission::Show_clients) == false) {
		ShowAccessDeniedMessage();
		return;
	}
	row_client_info();
	show_all_clients();
	cout << "============================================================================================\n";
}
// FOR USER	
void row_user_info() {
	cout << "=========================================================================================\n";
	cout << "                                  User Information                                     \n";
	cout << "=========================================================================================\n\n";
	cout << "|" << left << setw(15) << "Username" << "|"
		<< left << setw(15) << "Password" << "|"
		<< left << setw(10) << "Permission" << "|"
		<< left << setw(10) << "Date" << "|" << endl;
	cout << "=========================================================================================\n";
}
void show_all_users(string file_name = users) {
	fstream file;
	file.open(file_name, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (line != "") {
				user_info user = convert_to_record_user(line);
				print_user_info(user);
			}
		}
		file.close();
	}
	else {
		cout << "File not found!" << endl;
	}
}
void show_user_func() {
	row_user_info();
	show_all_users();
	cout << "=========================================================================================\n";
}



// find client by ID
bool find_client_by_ID(client_info &client,string ID, string file_name = clients) {
	vector<client_info> clients_list = load_clients(clients);
	for (auto clients : clients_list) {
		if (clients.ID == ID) {
			client = clients;
			return true;
		}
	}
	return false;
}
void find_client_func() {
	client_info client;
	char choice;
	do {
		string ID;
		cout << "Enter Client ID: ";
		getline(cin >> ws, ID);
		if (find_client_by_ID(client, ID)) {
			cout << "\n========================================================================================\n";
			cout << "\tFound Client!" << endl;
			cout << "\tClient Information:\n";
			print_single_client(client);
		}
		else {
			cout << "Client not found!" << endl;
		}
		cout << "Do you want to search for another client? (y/n): ";
		cin >> choice;

	} while (tolower(choice) == 'y');
	
}
// delete client
bool mark_for_deleting(vector<client_info>& clients_list, const string& ID, client_info& clients) {
	for (auto& client : clients_list) {
		if (client.ID == ID) {
			clients = client;
			client.mark_for_delete = true;
			return true;
		}
	}
	return false;
}
// for user
bool mark_for_deleting_user(vector<user_info>& users_list, const string& username, user_info& user) {
	for (auto& u : users_list) {
		if (u.username == username) {
			user = u;
			u.mark_for_delete = true;
			return true;
		}
	}
	return false;
}


vector<client_info> save_clients(const vector<client_info>& client_list, string file_name = clients) {
	std::ofstream file(file_name, std::ios::out );
	if (file.is_open()) {
		for (const auto& client : client_list) {
			if (!client.mark_for_delete) {
				string line = convert_to_line(client);
				file << line << '\n';
			}
		}
	}
	return client_list;
}

// for user 
vector<user_info> save_users(const vector<user_info>& user_list, string file_name = users) {
	std::ofstream file(file_name, std::ios::out);
	if (file.is_open()) {
		for (const auto& user : user_list) {
			if (!user.mark_for_delete) {
				string line = convert_to_line_user(user);
				file << line << '\n';
			}
		}
	}
	return user_list;
}

void delete_client(const string& ID, const string& file_name = clients) {
	vector<client_info> clients_list = load_clients(file_name);
	client_info client;
	if (mark_for_deleting(clients_list, ID,client)) {
		cout << "\n========================================================================================\n";
		print_single_client(client);
		cout << "Are you sure you want to delete this client? (y/n): ";
		char choice;
		cin >> choice;
		if (tolower(choice) != 'y') {
			cout << "Client deletion cancelled!" << endl;
			return;
		}
		save_clients(clients_list, file_name);
		cout << "Client deleted successfully!" << endl;
	}
	else {
		cout << "Client not found!" << endl;
	}
}

void delete_client_func() {
	char choice;
	do {
		string ID;
		cout << "Enter Client ID to delete: ";
		getline(cin >> ws, ID);
		delete_client(ID);
		cout << "Do you want to delete another client? (y/n): ";
		cin >> choice;

	} while (tolower(choice) == 'y');
}

// delete user
void delete_user(string username, string file_name = users) {
	vector<user_info> user = load_users(file_name);
	user_info user_info;
	if (mark_for_deleting_user(user, username, user_info)) {
		cout << "\n========================================================================================\n";
		print_single_user(user_info);
		cout << "Are you sure you want to delete this user? (y/n): ";
		char choice;
		cin >> choice;
		if (tolower(choice) != 'y') {
			cout << "User deletion cancelled!" << endl;
			return;
		}
		save_users(user, file_name);
		cout << "User deleted successfully!" << endl;
	}
	else {
		cout << "User not found!" << endl;
	}

}
void delete_user_func() {
	char choice  = 'y';
	user_info user;
	do {
		string username;
		cout << "Enter Username to delete: ";
		getline(cin >> ws, username);
		if (username == "Admin") {
			cout << "You cannot delete the admin user!" << endl;
			cout << "Do you want to delete another user? (y/n): ";
			cin >> choice;
			continue;
		}
		
		else if (!find_user_by_username(user, username)) {
			cout << "User not found!" << endl;
			cout << "Do you want to delete another user? (y/n): ";
			cin >> choice;
			continue;
		}
		delete_user(username);
		cout << "Do you want to delete another user? (y/n): ";
		cin >> choice;

	} while (tolower(choice) == 'y');
}


// for updating client
client_info read_client_info_for_update(client_info & client) {
	client.ID = client.ID; // Keep the same ID for update
	cout << "Enter Client Name: ";
	getline(cin >> ws, client.name);
	cout << "Enter Client PIN Code: ";
	getline(cin, client.PIN_code);
	cout << "Enter Client Phone Number: ";
	getline(cin, client.Phone_number);
	cout << "Enter Client Balance: ";
	getline(cin, client.balance);
	client.date = system_date(); // Update the date to current date
	return client;
	}
void update_client(string ID, string file_name = clients) {
	vector<client_info> clients_list = load_clients(file_name);
	client_info client;
	if (find_client_by_ID(client, ID, file_name)) {
		print_single_client(client);
		cout << "Do you want to update this client? (y/n): ";
		char choice;
		cin >> choice;
		if (tolower(choice) != 'y') {
			cout << "Client update cancelled!" << endl;				
			return;
		}
		else
		{
			client = read_client_info_for_update(client);
			for (auto& c : clients_list) {
				if (c.ID == ID) {
					c = client; // Update the client information
					break;
				}
			}
			save_clients(clients_list, file_name);
			cout << "Client updated successfully!" << endl;
		}
		}

		else {
			cout << "Client not found!" << endl;
		}
	}
void update_client_func() {
		char choice;
		do {
			string ID;
			cout << "Enter Client ID to update: ";
			getline(cin >> ws, ID);
			update_client(ID);
			cout << "Do you want to update another client? (y/n): ";
			cin >> choice;

		} while (tolower(choice) == 'y');
	}

// user update 
user_info read_user_info_for_update(string username ,user_info& user) {
	user.username = user.username;
	cout << "Enter New Password: ";
	getline(cin >> ws, user.password);
	user.permission = to_string(read_permission());
	return user;

}
void update_user( string file_name = users) {
	vector<user_info> users_list = load_users(file_name);
	user_info user;
	string name;
	cout << "Enter Username to update: ";
	getline(cin >> ws, name);
	if (name == "Admin") {
		cout << "You cannot update the admin user!" << endl;
		return;
	}
	else if (!find_user_by_username(user, name, file_name)) {
		cout << "User not found!" << endl;
		return;
	}
	else {
		print_single_user(user);
		cout << "Do you want to update this user? (y/n): ";
		char choice;
		cin >> choice;
		if (tolower(choice) != 'y') {
			cout << "User update cancelled!" << endl;
			return;
		}
		else
		{
			user = read_user_info_for_update(name, user);
			for (auto& u : users_list) {
				if (u.username == name) {
					u = user; // Update the user information
					break;
				}
			}
			save_users(users_list, file_name);
			cout << "User updated successfully!" << endl;
		}
	}
	
}
void update_user_func() {
	char choice;
	do {
		update_user();
		cout << "Do you want to update another user? (y/n): ";
		cin >> choice;
	} while (tolower(choice) == 'y');
}



// transection section
	void trans_row_info() {
		cout << "========================================================================================\n";
		cout << "                                  Transaction Information                                \n";
		cout << "========================================================================================\n";
		cout << "|" << left << setw(10) << "ID" << "|"
			<< left << setw(30) << "Name" << "|" << left << setw(20) << "Balance" << "|" << endl;
		cout << "========================================================================================\n";
	}


void deposit(string ID, string amount) {
		vector<client_info> clients_list = load_clients(clients);
		for (client_info cl : clients_list) {
			if (cl.ID == ID) {
				print_single_client(cl);
				cout << "\nDo you want to deposit " << amount << " to this account? (y/n): ";
				char choice;
				cin >> choice;
				if (tolower(choice) == 'y') {
					double current_balance = stod(cl.balance);
					double deposit_amount = stod(amount);
					current_balance += deposit_amount;
					cl.balance = to_string(current_balance);
					cl.date = system_date(); // Update the date to current date
					save_clients(clients_list, clients);
					cout << "Deposit successful! New balance: " << cl.balance << endl;
					return;
				}
				else {
					cout << "Deposit cancelled!" << endl;
					return;
				}
			}

		}


	}

void deposit_func() {
		char choice;
		client_info client;
		do {

			string ID, amount;
			cout << "Enter Client ID to deposit: ";
			getline(cin >> ws, ID);
			if (find_client_by_ID(client, ID)) {
				cout << "Enter Amount to Deposit: ";
				getline(cin >> ws, amount);
				deposit(ID, amount);
			}
			else {
				cout << "Client not found!" << endl;
			}
			cout << "Do you want to deposit money to another client? (y/n): ";
			cin >> choice;
		} while (tolower(choice) == 'y');
}
void withdraw(string ID, string amount) {
	vector<client_info> clients_list = load_clients(clients);
	for (client_info cl : clients_list) {
		if (cl.ID == ID) {
			double current_balance = stod(cl.balance);
			double withdraw_amount = stod(amount);
			if (current_balance >= withdraw_amount) {
				print_single_client(cl);
				cout << "do You want to withdraw " << withdraw_amount << " from this account ? (y/n): ";
				char choice;
				cin >> choice;
				if (tolower(choice) != 'y') {
					cout << "Withdrawal cancelled!" << endl;
					return;
				}
				else
				{
					current_balance -= withdraw_amount;
					cl.balance = to_string(current_balance);
					cl.date = system_date(); // Update the date to current date
					save_clients(clients_list, clients);
					cout << "Withdrawal successful! New balance: " << cl.balance << endl;
				}
			}
			else {
				cout << "Insufficient balance!" << endl;
			}
			return;
		}
	}
}
void withdraw_func() {
	string ID, amount;
	char choice;
	client_info client;
	do {
		cout << "Enter Client ID to withdraw: ";
		getline(cin >> ws, ID);
		if (find_client_by_ID(client, ID)) {

			cout << "Enter Amount to Withdraw: ";
			getline(cin >> ws, amount);
			withdraw(ID, amount);
		}
		else {
			cout << "Client not found!" << endl;
		}
		cout << "Do you want to withdraw money from another client? (y/n): ";
		cin >> choice;
	} while (tolower(choice) == 'y');
}
double Total_balances() {
	vector<client_info> clients_list = load_clients(clients);
	double total_balance = 0.0;
	for (const auto& client : clients_list) {
		total_balance += stod(client.balance);
	}
	return total_balance;

}
void print_total_balance(client_info client) {
	cout << "|" << left << setw(10) << client.ID << "|"
		<< left << setw(30) << client.name << "|"
		<< left << setw(20) << client.balance << "|" << endl;
}
void show_total_balances() {
	trans_row_info();
	vector<client_info> clients_list = load_clients(clients);
	for (const auto& client : clients_list) {
		print_total_balance(client);
	}


}
void Total_balances_func_screen() {
	system("cls");
	show_total_balances();
	double total_balance = Total_balances();
	cout << "\n========================================================================================\n";
	cout << "Total Balances in the Bank: " << total_balance << endl;
	cout << "========================================================================================\n";
}
void deposit_funct_screen() {
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Money Screen";
	cout << "\n-----------------------------------\n";
	deposit_func();
}
void withdraw_funct_screen() {
	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Money Screen";
	cout << "\n-----------------------------------\n";
	withdraw_func();
}


// for transaction management
void trans_main_screen();
void go_to_trans_screen() {
	cout << "Press any key to go back to transaction menu....\n";
	system("pause>0");
	system("cls");
	trans_main_screen();
}
void perform_trans_funct(trans_options option) {

	switch (option)
	{
	case trans_options::DEPOSIT:
		system("cls");
		deposit_funct_screen();
		go_to_trans_screen();
		break;
	case trans_options::WITHDRAW:
		system("cls");
		withdraw_funct_screen();
		go_to_trans_screen();
		break;
	case trans_options::TOTAL:
		system("cls");
		Total_balances_func_screen();
		go_to_trans_screen();
		break;
	case trans_options::GO_BACK:
		system("cls");
		show_main_screen();
		break;
	default:
		break;
	}
}

void trans_main_screen() {
	system("cls");	
	if (check_permission(stoi(user_loged_in.permission), user_permission::Transection) == false) {
		ShowAccessDeniedMessage();
		return;
	}
	cout << "========================================================================================\n";
	cout << "Welcome to the Transaction Management System\n";
	cout << "1. Deposit Money\n";
	cout << "2. Withdraw Money\n";
	cout << "3. Total Balances\n";
	cout << "4. Go to the main screen\n";
	cout << "========================================================================================\n";
	perform_trans_funct((trans_options)read_choice_2());
}


// for function show screen
void show_add_function_screen() {
	system("cls");
	if (check_permission(stoi(user_loged_in.permission), user_permission::Add_clients) == false) {
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tAdd a new client Screen";
	cout << "\n-----------------------------------\n";
	add_client_function();
}
void show_find_function_screen() {
	system("cls");
	if (check_permission(stoi(user_loged_in.permission), user_permission::Find_clients) == false) {
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tFind a client Screen";
	cout << "\n-----------------------------------\n";
	find_client_func();
}
void show_delete_function_screen() {
	system("cls");
	if (check_permission(stoi(user_loged_in.permission), user_permission::Delete_clients) == false) {
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tDelete a client Screen";
	cout << "\n-----------------------------------\n";
	delete_client_func();
}
void show_update_function_screen() {
	system("cls");
	if (check_permission(stoi(user_loged_in.permission), user_permission::update_clients) == false) {
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate a client Screen";
	cout << "\n-----------------------------------\n";
	update_client_func();
}
void Show_end() {
	cout << "***********************************\n\n";
	cout << "Program End :-)\n\n";
	cout << "***********************************\n\n";
}

// for function user to show screen
void show_add_user_function_screen() {
	cout << "\n-----------------------------------\n";
	cout << "\tAdd a new user Screen";
	cout << "\n-----------------------------------\n";
	add_user_function();
}
void show_find_user_function_screen() {
	cout << "\n-----------------------------------\n";
	cout << "\tFind a user Screen";
	cout << "\n-----------------------------------\n";
	find_user_func();
}
void show_delete_user_function_screen() {
	cout << "\n-----------------------------------\n";
	cout << "\tDelete a user Screen";
	cout << "\n-----------------------------------\n";
	delete_user_func();
}
void show_update_user_function_screen() {
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate a user Screen";
	cout << "\n-----------------------------------\n";
	update_user_func();
}
void perform_user_funct(user_options option);
void show_user_main_screen() {
	system("cls");
	if (check_permission(stoi(user_loged_in.permission), user_permission::Manage_users) == false) {
		ShowAccessDeniedMessage();
		return;
	}
	cout << "=========================================================================================\n";
	cout << "\t\tWelcome to the User Management System\n";
	cout << "===========================================================================================\n";
	cout << "1. Show All Users\n";
	cout << "2. Add User\n";
	cout << "3. Find User\n";
	cout << "4. Delete User\n";
	cout << "5. Update User\n";
	cout << "6. Go to the main screen\n";
	cout << "===========================================================================================\n";
	perform_user_funct((user_options)read_choice_3());

}
void go_to_user_screen() {
	cout << "Press any key to go back to user menu....\n";
	system("pause>0");
	system("cls");
	show_user_main_screen();
}
void perform_user_funct(user_options option) {
	switch (option)
	{
	case user_options::show_user:
		system("cls");
		show_user_func();
		go_to_user_screen();
		break;
	case user_options::ADD_USER:
		system("cls");
		show_add_user_function_screen();
		go_to_user_screen();
		break;
	case user_options::FIND_USER:
		system("cls");
		show_find_user_function_screen();
		go_to_user_screen();
		break;
	case user_options::DELETE_USER:
		system("cls");
		show_delete_user_function_screen();
		go_to_user_screen();
		break;
	case user_options::UPDATE_USER:
		system("cls");
		show_update_user_function_screen();
		go_to_user_screen();
		break;
	default:
		system("cls");
		show_main_screen();
	}
}
void log_in();

void go_main_screen() {
	cout << "Press any key to go back to main menu....\n";
	system("pause>0");
	system("cls");
	show_main_screen();
	
}
void perform_mian_funct(main_options Option) {
	switch (Option) {
	case main_options::SHOW_ALL_CLIENTS:
		system("cls");
		show_client_func();
		go_main_screen();
		break;
	case main_options::ADD_CLIENT :
		system("cls");
		show_add_function_screen();
		go_main_screen();
		break;
	case main_options::FIND_CLIENT:
		system("cls");
		show_find_function_screen();
		go_main_screen();
		break;
	case main_options::DELETE_CLIENT:
		system("cls");
		show_delete_function_screen();
		go_main_screen();
		break;
	case main_options::UPDATE_CLIENT:
		system("cls");
		show_update_function_screen();
		go_main_screen();
		break;
		case main_options::TRANSECTION:
		system("cls");
		trans_main_screen();
		go_main_screen();
		break;
		case main_options::USER_MANAGMENT:
		system("cls");
		show_user_main_screen();
		go_main_screen();
		break;
	case 8:
		system("cls");
		log_in();
	default:
		cout << "Invalid choice! Please try again." << endl;
	}
}

void show_main_screen() {
	system("cls");
	cout << "========================================================================================\n";
	cout << "Welcome to the Bank Management System\n";
	cout << "1.Show All Clients\n";
	cout << "2. Add Client\n";
	cout << "3. Find Client\n";
	cout << "4. Delete Client\n";
	cout << "5. Update Client\n";
	cout << "6. Transection\n";
	cout << "7. User management\n";
	cout << "8. Log out\n";
	cout << "========================================================================================\n";
	perform_mian_funct((main_options)read_choice());
}

bool load_user_info(string user_name, string password) {
	if (find_user_by_username_and_password(user_name, password, user_loged_in)) {
		return true;
	}
	return false;
}

void log_in() {
	string user_name, password;
	bool failed = false;
	do {
		system("cls");
		cout << "========================================================================================\n";
		cout << "                         log in screen\n";
		cout << "========================================================================================\n";
		if (failed) {
			cout << "Invalid username or password. Please try again.\n";
		}
		cout << "Enter Username: ";
		getline(cin >> ws, user_name);
		cout << "Enter Password: ";
		getline(cin >> ws, password);
		failed = !load_user_info(user_name, password);
	} while (failed);
	show_main_screen();
}

int main()
{
	log_in();

	


}