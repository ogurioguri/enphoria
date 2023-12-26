#include <iostream>
#include"MemoryRiver.hpp"
#include"store.hpp"
#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<set>
#include<cstdio>
#include <algorithm>
#include <cctype>
bool cut(std::string &kind, std::string &value, const std::string &input) {
  bool flag = false;
	
  std::string k = input.substr(1, 4);
  if (k == "ISBN") {
	flag = true;
	kind = k;
	value = input.substr(6);
	return flag;
  }
  if (k == "name") {
	flag = true;
	kind = k;
	value = input.substr(6);
	if (value.size() < 3) {
	  flag = false;
	  return flag;
	} else {
	  value = value.substr(1, value.size() - 2);
	}
  }
  k = input.substr(1, 5);
  if (k == "price") {
	flag = true;
	kind = k;
	value = input.substr(7);
	if (value.size() == 0) {
	  flag = false;
	  return flag;
	}
  }
  k = input.substr(1, 6);
  if (k == "author") {
	flag = true;
	kind = k;
	value = input.substr(8);
	if (value.size() < 3) {
	  flag = false;
	  return flag;
	} else {
	  value = value.substr(1, value.size() - 2);
	}
  }
  k = input.substr(1, 7);
  if (k == "keyword") {
	flag = true;
	kind = k;
	value = input.substr(8);
	if (value.size() < 3) {
	  flag = false;
	  return flag;
	} else {
	  value = value.substr(2, value.size() - 3);
	}
  }
  return flag;

}
void transfer(char *object, const std::string &a) {
  int size_a = a.size();
  for (int i = 0; i < size_a; ++i) {
	object[i] = a[i];
  }
  object[size_a] = '\0';

}

struct customer {
  char UsrID[31] = {'\0'};
  char Password[31] = {'\0'};
  int Privilege = 0;
};

//书籍的信息
struct book {
  char ISBN[21] = {'\0'};
  char BookName[61] = {'\0'};
  char Author[61] = {'\0'};
  char Keyword[61] = {'\0'};
  long long Quantity = 0;
  double Price = 0;
  bool operator<(const book &other) const {
	return std::strcmp(ISBN, other.ISBN) < 0;
  }
};

struct employee_conduct {
  char UsrID[31] = {'\0'};
  char conduct[10] = {'\0'};
  char ISBN[21] = {'\0'};
  char BookName[61] = {'\0'};
  char Author[61] = {'\0'};
  long long Quantity = 0;
  double Price = 0;
};
//存储顾客信息(按Id升序的索引）
store identity;
//1存放position(注意最后要保存的）
MemoryRiver<customer, 1> data_base_identity;

//存储书本信息
MemoryRiver<book, 1> book_store;
//按各种顺序升序的索引
store book_ISBN;
store book_name;
store book_author;
store book_keyword;

struct finance_report {
  char ISBN[21] = {'\0'};
  char BookName[61] = {'\0'};
  char Author[61] = {'\0'};
  long long Quantity = 0;
  double Price = 0;
  bool flag = false;
};

struct log_conduct {
  char conduct[21] = {'\0'};
  char ID[31] = {'\0'};
  char book_name[61] = {'\0'};
  long long Quantity = 0;
};

//财务支出情况
store finance;
MemoryRiver<double, 1> data_base_finance;

int finance_size = 0;
MemoryRiver<double, 1> total_finance;

MemoryRiver<log_conduct, 1> data_log;
//员工操作信息
store employee;
MemoryRiver<employee_conduct, 1> data_base_employee;

std::vector<std::string> stack;
int size = 0;
//查权限
std::unordered_map<std::string, int> identity_data;
//第二个int存放index
std::unordered_map<int, int> select_book;
//
bool initialise() {
  bool flag = false;
  identity.initialise("identity_link", "identity_lump");
  book_ISBN.initialise("book_ISBN_link", "book_ISBN_lump");
  book_author.initialise("author_link", "author_lump");
  book_name.initialise("name_link", "name_lump");
  book_keyword.initialise("keyword_link", "keyword_lump");
  if (!data_base_identity.exist("data_base_identity")) {
	int number;
	data_base_identity.get_info(number, 1);
	data_base_identity.position = number;
  } else {
	data_base_identity.initialise("data_base_identity");
	flag = true;
  }
  if (!book_store.exist("book_store")) {
	int number;
	book_store.get_info(number, 1);
	book_store.position = number;
  } else {
	book_store.initialise("book_store");
  }
  if (!data_base_finance.exist("data_base_finance")) {
	int number;
	data_base_finance.get_info(number, 1);
	data_base_finance.position = number;
  } else {
	data_base_finance.initialise("data_base_finance");
  }
  if (!data_base_employee.exist("data_base_employee")) {
	int number;
	data_base_employee.get_info(number, 1);
	data_base_employee.position = number;
  } else {
	data_base_employee.initialise("data_base_employee");
  }
  if (!total_finance.exist("total_finance")) {
	int number;
	total_finance.get_info(number, 1);
	total_finance.position = number;
	finance_size = (number - 4) / sizeof(double);
  } else {
	total_finance.initialise("total_finance");
  }
  if (!data_base_finance.exist("data_base_finance")) {
	int number;
	data_base_finance.get_info(number, 1);
	data_base_finance.position = number;
  } else {
	data_base_finance.initialise("data_base_finance");
  }
  if (!data_log.exist("data_log")) {
	int number;
	data_log.get_info(number, 1);
	data_log.position = number;
  } else {
	data_log.initialise("data_log");
  }
  return flag;
}

void project_end() {
  identity.end("identity_link", "identity_lump");
  book_ISBN.end("book_ISBN_link", "book_ISBN_lump");
  book_author.end("author_link", "author_lump");
  book_name.end("name_link", "name_lump");
  book_keyword.end("keyword_link", "keyword_lump");
  if (!data_base_identity.exist("data_base_identity")) {
	data_base_identity.write_info(data_base_identity.position, 1);
  }
  if (!book_store.exist("book_store")) {
	book_store.write_info(book_store.position, 1);
  }
  if (!data_base_finance.exist("data_base_finance")) {
	data_base_finance.write_info(data_base_finance.position, 1);
  }
  if (!data_base_employee.exist("data_base_employee")) {
	data_base_employee.write_info(data_base_employee.position, 1);
  }
  if (!total_finance.exist("total_finance")) {
	total_finance.write_info(total_finance.position, 1);
  }
  if (!data_log.exist("data_log")) {
	data_log.write_info(data_log.position, 1);
  }
  if (!data_base_finance.exist("data_base_finance")) {
	data_base_finance.write_info(data_base_finance.position, 1);
  }
}
//登录栈


void su(const std::string &input_id, const std::string &input_pass) {
  bool flag = true;
  std::vector<int> result;
  identity.find(input_id, result);
  if (result.empty()) {
	flag = false;
  } else {
	customer read_value;
	data_base_identity.read(read_value, result[0]);
	if ((size != 0 && identity_data[stack[size - 1]] > read_value.Privilege)
		|| std::strcmp(input_pass.c_str(), read_value.Password) == 0) {
	  identity_data[input_id] = read_value.Privilege;
	  stack.push_back(input_id);
	  size++;
	  log_conduct conduct;
	  copy(conduct.ID, input_id.c_str());
	  transfer(conduct.conduct, "su");
	  data_log.write(conduct);
	} else {
	  flag = false;
	}
  }
  if (!flag) {
	std::cout << "Invalid\n";
  }
}

void logout() {
  if (stack.empty()) {
	std::cout << "Invalid\n";
  } else {
	auto iter = select_book.find(size);
	log_conduct conduct;
	copy(conduct.ID, stack[size - 1].c_str());
	transfer(conduct.conduct, "logout");
	data_log.write(conduct);
	stack.pop_back();
	//对选择进行操作
	if (iter != select_book.end()) {
	  select_book.erase(iter);
	}
	size--;
  }
}

void regist(const std::string &input_id, const std::string &input_password, const std::string &input_username) {
  if (input_username.empty()) {
	std::cout << "Invalid\n";
	return;
  }
  std::vector<int> result;
  identity.find(input_id, result);
  if (!result.empty()) {
	std::cout << "Invalid\n";
	return;
  } else {
	customer new_id;
	copy(new_id.UsrID, input_id.c_str());
	copy(new_id.Password, input_password.c_str());
	new_id.Privilege = 1;
	int index = data_base_identity.write(new_id);
	key_value data;
	copy(data.name, input_id.c_str());
	data.value = index;
	identity.insert(data);
	log_conduct conduct;
	copy(conduct.ID, input_id.c_str());
	transfer(conduct.conduct, "register");
	data_log.write(conduct);
  }
}

void passwd(const std::string &id, const std::string &current_id, const std::string &now_id) {
  if (size == 0) {
	std::cout << "Invalid\n";
	return;
  } else {
	std::vector<int> result;
	identity.find(id, result);
	if (result.empty()) {
	  std::cout << "Invalid\n";
	  return;
	} else {
	  if (current_id.empty()) {
		if (stack[size - 1] == "root") {
		  customer read_value;
		  int index = result[0];
		  data_base_identity.read(read_value, index);
		  copy(read_value.Password, now_id.c_str());
		  data_base_identity.update(read_value, index);
		  log_conduct conduct;
		  copy(conduct.ID, id.c_str());
		  transfer(conduct.conduct, "passwd");
		  data_log.write(conduct);
		} else {
		  std::cout << "Invalid\n";
		  return;
		}
	  } else {
		customer read_value;
		int index = result[0];
		data_base_identity.read(read_value, index);
		if (stack[size - 1] == "root" || std::strcmp(read_value.Password, current_id.c_str()) == 0) {
		  copy(read_value.Password, now_id.c_str());
		  data_base_identity.update(read_value, index);
		  log_conduct conduct;
		  copy(conduct.ID, id.c_str());
		  transfer(conduct.conduct, "passwd");
		  data_log.write(conduct);
		} else {
		  std::cout << "Invalid\n";
		  return;
		}
	  }
	}
  }
}

void useradd(const std::string &id, const std::string &password, int privilege, const std::string &name) {
  if (stack.empty() || identity_data[stack[size - 1]] <= privilege || name == "") {
	std::cout << "Invalid\n";
	return;
  } else {
	std::vector<int> result;
	identity.find(id, result);
	if (!result.empty()) {
	  std::cout << "Invalid\n";
	  return;
	} else {
	  customer new_id;
	  copy(new_id.UsrID, id.c_str());
	  copy(new_id.Password, password.c_str());
	  new_id.Privilege = privilege;
	  int index = data_base_identity.write(new_id);
	  key_value data;
	  copy(data.name, id.c_str());
	  data.value = index;
	  identity.insert(data);
	  log_conduct conduct;
	  copy(conduct.ID, id.c_str());
	  transfer(conduct.conduct, "useradd");
	  data_log.write(conduct);
	}
  }
}

void delete_id(const std::string &id) {
  if (size == 0 || stack[size - 1] != "root") {
	std::cout << "Invalid\n";
	return;
  } else {
	bool flag = true;
	//确认是否登录
	for (auto k : stack) {
	  if (k == id) {
		flag = false;
		break;
	  }
	}
	if (!flag) {
	  std::cout << "Invalid\n";
	  return;
	}
	std::vector<int> result;
	identity.find(id, result);
	if (result.empty()) {
	  std::cout << "Invalid\n";
	  return;
	} else {
	  key_value to_delete;
	  transfer(to_delete.name, id);
	  to_delete.value = result[0];
	  identity.erase(to_delete);
	  log_conduct conduct;
	  copy(conduct.ID, id.c_str());
	  transfer(conduct.conduct, "delete");
	  data_log.write(conduct);
	}
  }
}

void Print_book(book read_value) {
  std::cout << read_value.ISBN << '\t' << read_value.BookName << '\t' << read_value.Author << '\t'
			<< read_value.Keyword << '\t';
  printf("%.2f\t", read_value.Price);
  std::cout << read_value.Quantity << '\n';
}

void search_ISBN(const std::string &isbn) {
  std::vector<int> data;
  book_ISBN.find(isbn, data);
  if (!data.empty()) {
	book read;
	book_store.read(read, data[0]);
	Print_book(read);
  } else {
	std::cout << '\n';
  }
}

void search_name(const std::string &name) {
  std::set<book> result;
  std::vector<int> data;
  book_name.find(name, data);
  for (auto &i : data) {
	book read;
	book_store.read(read, i);
	result.insert(read);
  }
  if (!result.empty()) {
	for (const auto &iter : result) {
	  Print_book(iter);
	}
  } else {
	std::cout << '\n';
  }

}

void search_key(const std::string &key) {
  std::set<book> result;
  std::vector<int> data;
  book_keyword.find(key, data);
  for (auto &i : data) {
	book read;
	book_store.read(read, i);
	result.insert(read);
  }
  if (!result.empty()) {
	for (const auto &iter : result) {
	  Print_book(iter);
	}
  } else {
	std::cout << '\n';
  }
}

void search_author(const std::string &author) {
  std::set<book> result;
  std::vector<int> data;
  book_author.find(author, data);
  for (auto &i : data) {
	book read;
	book_store.read(read, i);
	result.insert(read);
  }
  if (!result.empty()) {
	for (const auto &iter : result) {
	  Print_book(iter);
	}
  } else {
	std::cout << '\n';
  }
}

void buy(const std::string &ISBN, int quantity) {
  std::vector<int> data;
  book_ISBN.find(ISBN, data);
  if (size == 0 || quantity <= 0 || data.empty()) {
	std::cout << "Invalid\n";
	return;
  } else {
	int index = data[0];
	book by;
	book_store.read(by, index);
	if (quantity > by.Quantity) {
	  std::cout << "Invalid\n";
	  return;
	} else {
	  double total = by.Price * quantity;
	  printf("%.2f\n", total);
	  by.Quantity -= quantity;
	  book_store.update(by, index);
	  total_finance.write(total);
	  finance_size++;
	  log_conduct conduct;
	  copy(conduct.ID, stack[size - 1].c_str());
	  transfer(conduct.conduct, "buy");
	  copy(conduct.book_name, by.BookName);
	  conduct.Quantity = quantity;
	  data_log.write(conduct);
	}
  }
}

void select(std::string ISBN) {
  if (size == 0 || identity_data[stack[size - 1]] < 3) {
	std::cout << "Invalid\n";
	return;
  } else {
	book read_value;
	std::vector<int> data;
	book_ISBN.find(ISBN, data);
	if (data.empty()) {
	  transfer(read_value.ISBN, ISBN);
	  int index = book_store.write(read_value);
	  key_value add;
	  transfer(add.name, ISBN);
	  add.value = index;
	  book_ISBN.insert(add);
	  select_book[size] = index;
	} else {
	  select_book[size] = data[0];
	}
  }
}

bool change_ISBN(const std::string &ISBN) {
  bool flag = true;
  if (select_book.find(size) == select_book.end()) {
	std::cout << "Invalid\n";
	flag = false;
	return flag;
  } else {
	int index = select_book[size];
	book change;
	book_store.read(change, index);
	if (std::strcmp(change.ISBN, ISBN.c_str()) == 0) {
	  std::cout << "Invalid\n";
	  flag = false;
	  return flag;
	}
	key_value init;
	copy(init.name, change.ISBN);
	init.value = index;
	std::vector<int> result;
	book_ISBN.find(ISBN, result);
	if (!result.empty()) {
	  std::cout << "Invalid\n";
	  flag = false;
	  return flag;
	}
	book_ISBN.erase(init);
	transfer(change.ISBN, ISBN);
	book_store.update(change, index);
	transfer(init.name, ISBN);
	book_ISBN.insert(init);
	log_conduct conduct;
	copy(conduct.ID, stack[size - 1].c_str());
	transfer(conduct.conduct, "modify_ISBN");
	copy(conduct.book_name, change.BookName);
	data_log.write(conduct);
  }
  return flag;
}

bool change_name(const std::string &name) {
  bool flag = true;
  if (select_book.find(size) == select_book.end()) {
	std::cout << "Invalid\n";
	flag = false;
	return flag;
  } else {
	int index = select_book[size];
	book change;
	book_store.read(change, index);
	key_value init;
	copy(init.name, change.BookName);
	init.value = index;
	book_name.erase(init);
	transfer(change.BookName, name);
	book_store.update(change, index);
	transfer(init.name, name);
	book_name.insert(init);
	log_conduct conduct;
	copy(conduct.ID, stack[size - 1].c_str());
	transfer(conduct.conduct, "modify_name");
	copy(conduct.book_name, change.BookName);
	data_log.write(conduct);
  }
  return flag;
}

bool change_author(const std::string &Author) {
  bool flag = true;
  if (select_book.find(size) == select_book.end()) {
	std::cout << "Invalid\n";
	flag = false;
	return flag;
  } else {
	int index = select_book[size];
	book change;
	book_store.read(change, index);
	key_value init;
	copy(init.name, change.Author);
	init.value = index;
	book_author.erase(init);
	transfer(change.Author, Author);
	book_store.update(change, index);
	transfer(init.name, Author);
	book_author.insert(init);
	log_conduct conduct;
	copy(conduct.ID, stack[size - 1].c_str());
	transfer(conduct.conduct, "modify_Author");
	copy(conduct.book_name, change.BookName);
	data_log.write(conduct);
  }
  return flag;
}

bool change_keyword(const std::string &keyword) {
  bool flag = true;
  if (select_book.find(size) == select_book.end()) {
	std::cout << "Invalid\n";
	flag = false;
	return flag;
  } else {
	int index = select_book[size];
	book change;
	book_store.read(change, index);
	std::string key;
	key = change.Keyword;
	std::vector<std::string> tokens;
	std::stringstream ss__(keyword);
	std::string token;
	while (std::getline(ss__, token, '|')) {
	  tokens.push_back(token);
	}
	for (int i = 0; i < tokens.size(); ++i) {
	  for (int j = i + 1; j < tokens.size(); ++j) {
		if (tokens[i] == tokens[j]) {
		  std::cout << "Invalid\n";
		  flag = false;
		  return flag;
		}
	  }
	}
	tokens.clear();
	// 使用stringstream按'/'进行划分
	std::stringstream ss(key);

	while (std::getline(ss, token, '|')) {
	  tokens.push_back(token);
	}
	key_value init;
	init.value = index;
	for (const std::string &t : tokens) {
	  copy(init.name, t.c_str());
	  book_keyword.erase(init);
	}
	transfer(change.Keyword, keyword);
	book_store.update(change, index);
	tokens.clear();
	std::stringstream ss_(keyword);
	while (std::getline(ss_, token, '|')) {
	  tokens.push_back(token);
	}
	for (const std::string &t : tokens) {
	  copy(init.name, t.c_str());
	  book_keyword.insert(init);
	}
	log_conduct conduct;
	copy(conduct.ID, stack[size - 1].c_str());
	transfer(conduct.conduct, "modify_keyword");
	copy(conduct.book_name, change.BookName);
	data_log.write(conduct);

  }
  return flag;
}

bool change_prince(const double &price) {
  bool flag = true;
  if (select_book.find(size) == select_book.end()) {
	std::cout << "Invalid\n";
	flag = false;
	return flag;
  } else {
	int index = select_book[size];
	book change;
	book_store.read(change, index);
	change.Price = price;
	book_store.update(change, index);
	log_conduct conduct;
	copy(conduct.ID, stack[size - 1].c_str());
	transfer(conduct.conduct, "modify_Price");
	copy(conduct.book_name, change.BookName);
	data_log.write(conduct);
  }
  return flag;
}

void import_book(int quantity, double cost) {
  if (cost <= 0 || quantity <= 0) {
	std::cout << "Invalid\n";
	return;
  } else {
	if (select_book.find(size) == select_book.end()) {
	  std::cout << "Invalid\n";
	  return;
	} else {
	  int index = select_book[size];
	  book import;
	  book_store.read(import, index);
	  import.Quantity += quantity;
	  book_store.update(import, index);
	  finance_size++;
	  cost *= -1;
	  total_finance.write(cost);
	  log_conduct conduct;
	  copy(conduct.ID, stack[size - 1].c_str());
	  transfer(conduct.conduct, "import");
	  copy(conduct.book_name, import.BookName);
	  data_log.write(conduct);
	}
  }
}
void log() {
  for (int iter = 4, i = 0; iter <= data_log.position; ++i, iter += sizeof(log_conduct)) {
	log_conduct read;
	data_log.read(read, iter);
	std::cout << read.ID << '\t' << read.conduct << '\t' << read.book_name;
	if(read.Quantity!=0){
	  std::cout<<'\t'<<read.Quantity << '\n';
	}
	else{
	  std::cout<<'\n';
	}
  }
}

void count_finance(double &income, double &cost, int count) {
  if (count > finance_size) {
	std::cout << "Invalid\n";
	return;
  }
  if (count == 0) {
	std::cout << '\n';
  } else {
	for (int iter = total_finance.position - sizeof(double), i = 0; i < count; ++i, iter -= sizeof(double)) {
	  double read;
	  total_finance.read(read, iter);
	  if (read < 0) {
		cost += -read;
	  } else {
		income += read;
	  }
	}
  }
  std::cout << "+ ";
  printf("%.2f", income);
  std::cout << " - ";
  printf("%.2f", cost);
  std::cout << '\n';

}
int main() {
  //初始化
  freopen("out", "w", stdout);
  //int c = 1;
  bool flag = initialise();
  //创建管理员账户
  if (flag) {
	customer super;
	copy(super.UsrID, "root");
	copy(super.Password, "sjtu");
	super.Privilege = 7;
	int index = data_base_identity.write(super);
	key_value data;
	copy(data.name, "root");
	data.value = index;
	identity.insert(data);
  }
  std::string your_conduct;
  while (getline(std::cin, your_conduct)) {
	//std::cout << c;
	//c++;
	bool valid = false;
	std::istringstream iss(your_conduct);
	std::string token;
	iss >> token;
	//退出指令
	if (token == "quit" || token == "exit") {
	  break;
	}
	if (std::all_of(token.begin(), token.end(), [](unsigned char c) { return std::isspace(c); })) {
	  valid = true;
	  continue;
	}
	//用户管理
	if (token == "su") {
	  valid = true;
	  std::string id, password;
	  iss >> id;
	  iss >> password;
	  su(id, password);
	}
	if (token == "logout") {
	  valid = true;
	  logout();
	}
	if (token == "register") {
	  valid = true;
	  std::string id, password, username, k;
	  iss >> id;
	  iss >> password;
	  iss >> username;
	  if (iss >> k) {
		std::cout << "Invalid\n";
		continue;
	  }
	  regist(id, password, username);
	}
	if (token == "passwd") {
	  valid = true;
	  std::string id, current_pass, now_pass;
	  iss >> id;
	  iss >> current_pass;
	  iss >> now_pass;
	  if (now_pass.empty()) {
		now_pass = current_pass;
		current_pass = "";
	  }
	  passwd(id, current_pass, now_pass);
	}
	if (token == "useradd") {
	  valid = true;
	  std::string id, pass, name, k;
	  int privilege;
	  iss >> id;
	  iss >> pass;
	  iss >> privilege;
	  iss >> name;
	  if (iss >> k) {
		std::cout << "Invalid\n";
		continue;
	  }
	  useradd(id, pass, privilege, name);
	}
	if (token == "delete") {
	  valid = true;
	  std::string id;
	  iss >> id;
	  delete_id(id);
	}

	if (token == "show") {
	  valid = true;
	  std::string input;
	  iss >> input;
	  if (input == "finance") {
		if (size == 0 || identity_data[stack[size - 1]] != 7) {
		  std::cout << "Invalid\n";
		  continue;
		} else {
		  std::string count;
		  iss >> count;
		  int number;
		  double income = 0, cost = 0;
		  if (count.empty()) {
			number = finance_size;
		  } else {
			number = stoi(count);
		  }
		  count_finance(income, cost, number);
		}

	  } else {
		if (input.empty()) {
		  if (size == 0) {
			std::cout << "Invalid\n";
			continue;
		  }
		  std::vector<int> result;
		  book_ISBN.Print(result);
		  book read_value;
		  int size = result.size();
		  for (int i = 0; i < size; ++i) {
			book_store.read(read_value, result[i]);
			std::cout << read_value.ISBN << '\t' << read_value.BookName << '\t' << read_value.Author << '\t'
					  << read_value.Keyword << '\t';
			printf("%.2f\t", read_value.Price);
			std::cout << read_value.Quantity << '\n';
		  }
		} else {
		  if (size == 0) {
			std::cout << "Invalid\n";
			continue;
		  }
		  std::string conduct, content;
		  bool good = cut(conduct, content, input);
		  if (!good) {
			std::cout << "Invalid\n";
			continue;
		  } else {
			if (conduct == "ISBN") {
			  search_ISBN(content);
			  continue;
			}
			if (conduct == "name") {
			  search_name(content);
			  continue;
			}
			if (conduct == "author") {
			  search_author(content);
			  continue;
			}
			if (conduct == "keyword") {
			  search_key(content);
			  continue;
			}
		  }
		}
	  }

	}
	if (token == "buy") {
	  valid = true;
	  std::string ISBN;
	  int quantity;
	  iss >> ISBN;
	  iss >> quantity;
	  buy(ISBN, quantity);
	}

	if (token == "select") {
	  valid = true;
	  std::string ISBN;
	  iss >> ISBN;
	  select(ISBN);
	}

	if (token == "modify") {
	  valid = true;
	  std::string word;
	  //bool whole_conduct[5]={0};
	  while (iss >> word) {
		std::string conduct, content;
		bool good = cut(conduct, content, word);
		if (!good) {
		  std::cout << "Invalid\n";
		  break;
		} else {
		  if (conduct == "ISBN") {
			bool valid_ = change_ISBN(content);
			if (valid_) {
			  continue;
			} else {
			  break;
			}
		  }
		  if (conduct == "name") {
			bool valid_ = change_name(content);
			if (valid_) {
			  continue;
			} else {
			  break;
			}
		  }
		  if (conduct == "author") {
			bool valid_ = change_author(content);
			if (valid_) {
			  continue;
			} else {
			  break;
			}
		  }
		  if (conduct == "keyword") {
			bool valid_ = change_keyword(content);
			if (valid_) {
			  continue;
			} else {
			  break;
			}
		  }
		  if (conduct == "price") {
			double price = std::stod(content);
			bool valid_ = change_prince(price);
			if (valid_) {
			  continue;
			} else {
			  break;
			}

		  }
		}
	  }
	}
	if (token == "import") {
	  valid = true;
	  int quantity;
	  std::string total_cost;
	  iss >> quantity;
	  iss >> total_cost;
	  if (total_cost.empty()) {
		std::cout << "Invalid\n";
		continue;
	  } else {
		double cost = std::stod(total_cost);
		import_book(quantity, cost);
	  }
	}
	if (token == "log") {
	  log();
	}
	if (!valid) {
	  std::cout << "Invalid\n";
	}
  }
  project_end();
  return 0;
}
