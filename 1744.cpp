#include<bits/stdc++.h>



#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;


template<class T, int info_len = 2>
class MemoryRiver {
 private:
  /* your code here */
  fstream file;
  string file_name;
  int sizeofT = sizeof(T);
 public:

  long position=0;

  MemoryRiver() = default;

  MemoryRiver(const string& file_name) : file_name(file_name) {}

  void initialise(string FN = "") {
	if (FN != "") file_name = FN;
	file.open(file_name, std::ios::out);
	int tmp = 0;
	for (int i = 0; i < info_len; ++i)
	  file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
	file.close();
	position=static_cast<long>(info_len)*static_cast<long>(sizeof (int));
  }

  bool exist(string FN = ""){
	if (FN != "") file_name = FN;
	ifstream file_new(FN.c_str());
	return !file_new.good();

  }

  //读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n) {
	if (n > info_len) return;
	/* your code here */
	file.open(file_name,std::ios::in);
	file.seekg((static_cast< long>(n-1)) *static_cast<long> (sizeof(int)) );
	file.read(reinterpret_cast<char *>(&tmp),sizeof(int));
	file.close();

  }

  //将tmp写入第n个int的位置，1_base
  void write_info(int tmp, int n) {
	if (n > info_len) return;
	/* your code here */
	file.open(file_name,std::ios::in | std::ios::out | std::ios::binary);
	file.seekp((static_cast<long>(n-1)) * static_cast<long>(sizeof(int)) );
	file.write(reinterpret_cast<char *>(&tmp),sizeof(int));
	file.close();
  }

  //在文件合适位置写入类对象t，并返回写入的位置索引index
  //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  //位置索引index可以取为对象写入的起始位置
  int write(T &t) {
	/* your code here */
	file.open(file_name,std::ios::in | std::ios::out | std::ios::binary);
	file.seekp(position);
	file.write(reinterpret_cast<char*>(&t),sizeofT);
	file.close();
	position+=sizeofT;
	return position-sizeofT;

  }

  //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index) {
	/* your code here */
	file.open(file_name,std::ios::in | std::ios::out | std::ios::binary);
	file.seekp(index);
	file.write(reinterpret_cast<char*>(&t),sizeofT);
	file.close();
	if(index==position){
	  position=position+sizeofT;
	}
  }

  //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(T &t, const int index) {
	/* your code here */
	file.open(file_name,std::ios::in);
	file.seekg(index);
	file.read(reinterpret_cast<char*>(&t),sizeofT);
	file.close();
  }

  //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
  void Delete(int index , int size_file) {
	/* your code here */
	//file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
	T number;
	while(index<size_file){
	  read(number, index+sizeofT);
	  update(number,index);
	  //file.seekp()
	  index+=sizeofT;
	}

  }

  void Insult( T &t ,int index , int end){
	T number;
	int init = end;
	while(init>=index){
	  read(number, init);
	  update(number,init+sizeofT);
	  init-=sizeofT;
	}
	update(t,index);
  }
};






//
// Created by Lenovo on 2023/12/10.
//

#ifndef BOOKSTORE__STORE_HPP_
#define BOOKSTORE__STORE_HPP_

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

void copy(char *a, const char *b) {
  int i = 0;
  while (b[i] != '\0') {
	a[i] = b[i];
	i++;
  }
  a[i] = '\0';
}

struct key_value {
  char name[65] = {'\0'};
  int value = 0;
};

struct Node {
  char head_name[65] = {'\0'};
  int next_index = -1;
  int pre_index = -1;
  int lump_head=0;
  int sizeof_lump=0;
};

class store {
 public:
  store() = default;
  //建一个新的块
  void end(){
	if(position_.exist("position")){
	  position_.initialise("position");
	  //long number = 0;
	  position_.write(link.position);
	  //position_.read(number,0);
	  //std::cout<<number<<' ';
	  position_.write(lump.position);
	  //position_.read(number,sizeof (long));
	  // std::cout<<number;
	}
	else{
	  position_.update(link.position,0);
	  position_.update(lump.position,sizeof (long));
	}
  }
  int new_node(int pre, int next) {
	int begin;
	key_value init;
	begin = lump.write(init);
	for (int i = 0; i < lump_size - 1; ++i) {
	  lump.write(init);
	}
	Node node;
	node.lump_head = begin;
	node.next_index = next;
	node.pre_index = pre;
	node.sizeof_lump = 0;
	int k = link.write(node);
	return k;
  }

  //未来可能要分文件
  void initialise() {
	position_.exist("position");
	if (link.exist("LINK")) {
	  link.initialise("LINK");
	  link.write_info(-1, 3);
	  link.write_info(-1, 2);
	}
	else{
	  long position_read;
	  position_.read(position_read,0);
	  link.position = position_read;
	}
	if (lump.exist("LUMP")) {
	  lump.initialise("LUMP");
	}

	else {
	  long position_read;
	  position_.read(position_read, sizeof (long));
	  lump.position = position_read;
	}
  }

  void insert(key_value date) {
	int head, tail;
	link.get_info(head, 2);
	link.get_info(tail, 3);
	//对应链表为空
	if (head == -1) {
	  int flag;
	  link.get_info(flag,1);
	  if(flag==0){
		int rear = new_node(-1, -1);
		link.write_info(rear, 2);
		link.write_info(rear, 3);
		link.write_info(1,1);
		Node node;
		copy(node.head_name, date.name);
		node.sizeof_lump++;
		link.update(node, rear);
		lump.update(date, node.lump_head);
		return;
	  }else{
		int rear = 12;
		link.write_info(rear,2);
		link.write_info(rear,3);
		Node node;
		copy(node.head_name, date.name);
		node.sizeof_lump++;
		link.update(node, rear);
		lump.update(date, node.lump_head);
		return;
	  }
	}
	  //链表非空
	else {
	  int rear = head;
	  Node node_left, node_right;
	  link.read(node_right, rear);
	  //int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;

	  if (std::strcmp(node_right.head_name, date.name) > 0) {
		lump.Insult(date,
					node_right.lump_head,
					node_right.lump_head + sizeof(key_value) * (node_right.sizeof_lump - 1));
		copy(node_right.head_name, date.name);
		node_right.sizeof_lump++;
		link.update(node_right, rear);
		crack(node_right, rear, head, tail);
		return;
	  } else {
		//寻找插入位置
		while (rear != tail) {
		  node_left = node_right;
		  link.read(node_right, node_left.next_index);

		  if (std::strcmp(node_left.head_name, date.name) <= 0 && std::strcmp(node_right.head_name, date.name) >= 0) {
			int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
			while (begin < final) {
			  int mid = (begin + final) / 2;
			  key_value read_value;
			  lump.read(read_value, (mid * sizeof(key_value)));
			  int result = std::strcmp(read_value.name, date.name);
			  if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
				begin = mid + 1;
			  } else {
				final = mid;
			  }
			}
			//如果放在末尾，但是下一个节点开头仍是这个名字
			if (final == node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
			  if (std::strcmp(node_right.head_name, date.name) == 0) {
				key_value right_head;
				lump.read(right_head, node_right.lump_head);
				if (right_head.value > date.value) {
				  //lump.update(date, final * sizeof(lump_size));
				  lump.Insult(date,
							  final * sizeof(key_value),
							  node_left.lump_head + (node_left.sizeof_lump - 1) * sizeof(key_value));
				  node_left.sizeof_lump++;
				  link.update(node_left, rear);
				  crack(node_left, rear, head, tail);
				  return;
				}
			  }
			  //如果该元素的值居然比下一个块的开头的值大,只能继续
			} else {
			  lump.Insult(date,
						  final * sizeof(key_value),
						  node_left.lump_head + (node_left.sizeof_lump - 1) * sizeof(key_value));
			  //按理说不可能插到头部了
			  /* if(final==k*lump_size){
				copy(node_left.head_name,date.name);
			  }*/
			  node_left.sizeof_lump++;
			  link.update(node_left, rear);
			  crack(node_left, rear, head, tail);
			  return;
			}
		  }
		  if (std::strcmp(node_left.head_name, date.name) > 0) {
			break;
		  }
		  rear = node_left.next_index;
		}
		//遍历到了结尾,特判
		if (rear == tail) {
		  node_left = node_right;
		  int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		  while (begin < final) {
			int mid = (begin + final) / 2;
			key_value read_value;
			lump.read(read_value, (mid * sizeof(key_value)));
			int result = std::strcmp(read_value.name, date.name);
			if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
			  begin = mid + 1;
			} else {
			  final = mid;
			}
		  }
		  lump.Insult(date,
					  final * sizeof(key_value),
					  node_left.lump_head + (node_left.sizeof_lump - 1) * sizeof(key_value));
		  //按理说不可能插到头部了  no！
		  /* if(final==k*lump_size){
			copy(node_left.head_name,date.name);
		  }*/
		  if (final * sizeof(key_value) == node_left.lump_head) {
			copy(node_left.head_name, date.name);
		  }
		  node_left.sizeof_lump++;
		  link.update(node_left, rear);
		  crack(node_left, rear, head, tail);
		}
	  }
	}
  }
  void crack(Node node, int rear, int head, int tail) {
	if (node.sizeof_lump >=2* max_number) {
	  //new_code的前后指针
	  int new_tail = new_node(rear, node.next_index);
	  if(rear!=tail){
		int next_index = node.next_index;
		Node next_node;
		link.read(next_node,next_index);
		next_node.pre_index = new_tail;
		link.update(next_node,next_index);
	  }
	  else{
		link.write_info(new_tail,3);
	  }
	  node.next_index = new_tail;
	  Node new_node;
	  link.read(new_node, new_tail);
	  int begin = new_node.lump_head;
	  int transfer_begin = node.lump_head;
	  int target = node.sizeof_lump;
	  int half = target / 2;
	  while (half < target) {
		key_value transfer;
		lump.read(transfer, transfer_begin + half * sizeof(key_value));
		if (half == target / 2) {
		  copy(new_node.head_name, transfer.name);
		}
		lump.update(transfer, begin );
		new_node.sizeof_lump++;
		node.sizeof_lump--;
		begin+=sizeof (key_value);
		half++;
	  }
	  link.update(node, rear);
	  link.update(new_node, new_tail);
	}
  }

  void erase(key_value date) {
	int head, tail;
	link.get_info(head, 2);
	link.get_info(tail, 3);
	if (head == -1) {
	  return;
	}
	int rear = head;
	Node node_left, node_right;
	link.read(node_right, rear);
	if (std::strcmp(node_right.head_name, date.name) > 0) {
	  return;
	} else {
	  while (rear != tail) {
		node_left = node_right;
		link.read(node_right, node_left.next_index);

		if (std::strcmp(node_left.head_name, date.name) <= 0 && std::strcmp(node_right.head_name, date.name) >= 0) {
		  int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		  while (begin < final) {
			int mid = (begin + final) / 2;
			key_value read_value;
			lump.read(read_value, (mid * sizeof(key_value)));
			int result = std::strcmp(read_value.name, date.name);
			if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
			  begin = mid + 1;
			} else {
			  final = mid;
			}
		  }
		  if (final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
			key_value read_date;
			lump.read(read_date, final * sizeof(key_value));
			if (std::strcmp(read_date.name, date.name) == 0 && read_date.value == date.value) {

			  //删除节点
			  lump.Delete(final * sizeof(key_value), node_left.lump_head + (lump_size - 1) * sizeof(key_value));
			  node_left.sizeof_lump--;
			  if (node_left.sizeof_lump == 0) {
				if (rear == head) {
				  head = node_left.next_index;
				  node_right.pre_index = -1;
				  link.write_info(head, 2);
				  link.update(node_left,rear);
				  link.update(node_right,head);
				} else {
				  Node node_pre;
				  link.read(node_pre, node_left.pre_index);
				  node_pre.next_index = node_left.next_index;
				  node_right.pre_index = node_left.pre_index;
				  link.update(node_pre, node_left.pre_index);
				  link.update(node_right, node_left.next_index);
				}
			  } else {
				if (final * sizeof(key_value) == node_left.lump_head) {
				  lump.read(read_date, node_left.lump_head);
				  copy(node_left.head_name, read_date.name);
				  link.update(node_left, rear);
				} else {
				  link.update(node_left, rear);
				}
			  }

			}
		  }
		}
		if (std::strcmp(node_left.head_name, date.name) > 0) {
		  break;
		}
		rear = node_left.next_index;
	  }
	  if (rear == tail) {
		node_left = node_right;
		int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		while (begin < final) {
		  int mid = (begin + final) / 2;
		  key_value read_value;
		  lump.read(read_value, (mid * sizeof(key_value)));
		  int result = std::strcmp(read_value.name, date.name);
		  if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
			begin = mid + 1;
		  } else {
			final = mid;
		  }
		}
		if (final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
		  key_value read_date;
		  lump.read(read_date, final * sizeof(key_value));
		  if (std::strcmp(read_date.name, date.name) == 0 && read_date.value == date.value) {
			lump.Delete(final * sizeof(key_value), node_left.lump_head + (lump_size - 1) * sizeof(key_value));
			node_left.sizeof_lump--;
			if (node_left.sizeof_lump == 0) {
			  if (rear == head) {
				link.write_info(-1, 2);
				link.write_info(-1,3);
				link.update(node_left,rear);
			  } else {
				Node node_pre;
				link.read(node_pre, node_left.pre_index);
				node_pre.next_index = -1;
				link.update(node_pre, node_left.pre_index);
				int new_tail = node_left.pre_index;
				link.write_info(new_tail,3);
			  }
			} else {
			  if (final * sizeof(key_value) == node_left.lump_head) {
				lump.read(read_date, node_left.lump_head);
				copy(node_left.head_name, read_date.name);
				link.update(node_left, rear);
			  } else {
				link.update(node_left, rear);
			  }
			}
		  }
		}
	  }

	}
  }

  void find(const string &date) {
	int head, tail;
	link.get_info(head, 2);
	link.get_info(tail, 3);
	if (head == -1) {
	  std::cout << "null" << '\n';
	  return;
	}
	int rear = head;
	Node node_left, node_right;
	link.read(node_right, rear);
	if (node_right.head_name > date) {
	  std::cout << "null" << '\n';
	  return;
	} else {
	  bool flag = false;
	  while (rear != tail) {
		node_left = node_right;
		link.read(node_right, node_left.next_index);

		if (node_left.head_name <= date && node_right.head_name >= date) {
		  int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		  while (begin < final) {
			int mid = (begin + final) / 2;
			key_value read_value;
			lump.read(read_value, (mid * sizeof(key_value)));
			if (read_value.name < date) {
			  begin = mid + 1;
			} else {
			  final = mid;
			}
		  }
		  key_value read_value;
		  lump.read(read_value, final * sizeof(key_value));
		  while (read_value.name == date && final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
			flag = true;
			std::cout << read_value.value << ' ';
			final++;
			lump.read(read_value, final * sizeof(key_value));
		  }
		}
		if (node_left.head_name > date) {
		  break;
		}
		rear = node_left.next_index;
	  }
	  if (rear == tail) {
		node_left = node_right;
		int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		while (begin < final) {
		  int mid = (begin + final) / 2;
		  key_value read_value;
		  lump.read(read_value, (mid * sizeof(key_value)));
		  if (read_value.name < date) {
			begin = mid + 1;
		  } else {
			final = mid;
		  }
		}
		key_value read_value;
		lump.read(read_value, final * sizeof(key_value));
		while (read_value.name == date && final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
		  flag = true;
		  std::cout << read_value.value << ' ';
		  final++;
		  lump.read(read_value, final * sizeof(key_value));
		}
	  }
	  if (!flag) {
		std::cout << "null" << '\n';
	  } else {
		std::cout << '\n';
	  }
	}
  }

 private:
  MemoryRiver<Node, 3> link;
  //第一个是元素个数?，第二个是头节点,第三个尾节点
  MemoryRiver<key_value, 0> lump;
  const int max_number = 200;
  const int lump_size = 500;
  MemoryRiver<long , 0> position_;
};

#endif //BOOKSTORE__STORE_HPP_





void transfer(char object[] , const std::string & a){
  int size = a.size();
  for(int i =0;i<size;++i){
	object[i]=a[i];
  }
  object[size]='\0';

}

int main() {
  int n;
  std::cin>>n;
  std::getchar();
  store date_base;
  date_base.initialise();
  for(int i =0; i <n;++i){
	std::string words;
	getline(std::cin,words);
	std::istringstream iss(words);
	std::string token;
	iss>>token;
	if(token=="insert"){
	  std::string name;
	  int value;
	  iss>>name;
	  iss>>token;
	  value= std::stoi(token);
	  key_value date ;
	  transfer(date.name,name);
	  date.value = value;
	  date_base.insert(date);
	}
	if(token =="delete"){
	  std::string name;
	  int value;
	  iss>>name;
	  iss>>token;
	  value= std::stoi(token);
	  key_value date ;
	  transfer(date.name,name);
	  date.value = value;
	  date_base.erase(date);
	}
	if(token=="find"){
	  std::string name;
	  iss>>name;
	  date_base.find(name);
	}
	date_base.end();

  }
}
