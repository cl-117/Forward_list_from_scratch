#ifndef MY_FORWARD_LIST_HPP
#define MY_FORWARD_LIST_HPP

#include <algorithm>
#include <cstdlib> 
#include <ctime> 

template < typename T >
  class Forward_list {
    public:
      class Node {
        public:
          T data {};
          Node * next = nullptr;
          Node() {}
          Node(T input_data, Node * next_node = nullptr) {
            data = input_data;
            next = next_node;
        }~Node() {}
      };

    private:
      unsigned size_ = 0;
      Node * head_ = nullptr;

    public:
      Forward_list();
      ~Forward_list();
      Forward_list(const Forward_list < T > & other);
      Forward_list(std::initializer_list < T > input);
      Forward_list(int x); // Random constructor (custom)
      void push_front(const T & data);
      void pop_front();
      void alter_data(int n, T d); //Alter data function function (custom)
      T front() const;
      T back() const; //Back function (custom)
      void display() const;
      bool empty() const;
      unsigned size() const;
      void merge(Forward_list & other);
      Forward_list split();
      void sort();

    private:
      void merge_sort(Forward_list & );
  };

//Default constructor
template < typename T >
  Forward_list < T > ::Forward_list() {}

//Deconstructor 
template < typename T >
  Forward_list < T > ::~Forward_list() {
    while (head_ != nullptr) {
      Node * tmp = head_;
      head_ = head_ -> next;
      delete tmp;
      --size_;
    }
  }

//Copy constructor -> copy another list
template < typename T >
  Forward_list < T > ::Forward_list(const Forward_list & other) {
    if (other.head_ == nullptr) return; //If there is an empty list, return nullptr
    Node * node = new Node(other.head_ -> data, nullptr);
    this -> head_ = node;
    Node * temp = other.head_;
    this -> size_++;
    while (temp -> next != nullptr) { //While the next node is not null, link the nodes together.
      node -> next = new Node(temp -> next -> data, nullptr);
      node = node -> next;
      temp = temp -> next;
      this -> size_++;
    }
  }

//Random constructor 
template < typename T>
  Forward_list < T > :: Forward_list(int x) { //Takes input of int x, will make random forward_list of that size
    Node * node = new Node ( rand()%10, nullptr);
    this -> head_ = node;
    Node * temp = node;
    this -> size_ = x;
    for(int i = 0; i < this->size()-1; i++){
      Node * node = new Node ( rand()%10, nullptr);
      temp -> next = node;
      temp = node;
    } 
  }

//Constructor from initializer list -> create a forward list via input
template < typename T >
  Forward_list < T > ::Forward_list(std::initializer_list < T > input) {
    auto it = input.begin(); //                 Initialise iterator to the first node of the input list
    Node * node = new Node( * it, nullptr); //  Create a new node for the new list
    Node * temp = node;  //                     Temporary node points to new node and acts as the linker between nodes
    this -> head_ = node; //                    Head node of new list will become this node
    this -> size_++; //                         Increase the size for this new node
    std::advance(it, 1); //                     Advance the iterator 1 node so we can link up the rest of the list
    for (it; it != input.end(); it++) { //      Iterate through the input, link them together via/as nodes
      Node * node = new Node( * it, nullptr);
      temp -> next = node;
      temp = temp -> next;
      size_++;
    }
  }

//Front function -> Return the data in the front node of the list
template < typename T >
  T Forward_list < T > ::front() const {
    if (this -> head_ == nullptr) return 0; //If list is null, return 0
    return this -> head_ -> data; //Get head data
  }

//Back function -> Return the data at the back node of the list
template < typename T >
  T Forward_list < T > :: back() const {
    Node * node = head_;
    while(node->next!=nullptr) {
      node = node -> next;
    }
    return node -> data;
  }
  
//Push function -> Add element to front of list -> add a new head
template < typename T >
  void Forward_list < T > ::push_front(const T & data) {
    Node * node = new Node(data, this -> head_); //Add a new node (new head) to the list, adjust the size
    this -> size_++;
    this -> head_ = node;
  }

// Pop function -> Remove the front element of the list -> remove the head
template < typename T >
  void Forward_list < T > ::pop_front() {
    if (head_ != nullptr) { //As long as there is a head, change the head, reduce the size
      Node * temp = head_ -> next;
      head_ = temp;
      this -> size_--;
    }
  }

//Alter data function
template < typename T >
  void Forward_list < T > ::alter_data(int n, T d) {
    Node * node = head_; //Initalises the searching node to the head
    for(int i = 0; i<n-1; i++){
        node = node->next; //Loops until it finds the desired node
    }
    node -> data = d; //Changes the data
  }

// Display function -> Print out the list
template < typename T >
  void Forward_list < T > ::display() const {
    Node * temp = head_;
    while (temp != nullptr) { //While the node isnt null, print, move on
      std::cout << temp -> data << " ";
      temp = temp -> next;
    }
    std::cout<<std::endl;
  }

//Empty function -> return bool to determine if list is empty
template < typename T >
  bool Forward_list < T > ::empty() const {
    return (head_ == nullptr);
  }

//Size function -> return size of list
template < typename T >
  unsigned Forward_list < T > ::size() const {
    return size_;
  }

//Split function -> split function in 2 (if odd # nodes, first half should have 1+ more)
template < typename T >
  Forward_list < T > Forward_list < T > ::split() {
    Forward_list < T > returned_list;
    Node * temp = head_;
    int mid;
    (size() % 2 == 0)? mid = this->size() / 2 : mid = this->size()/ 2 + 1; //Even or odd
    for (int i = 1; i < mid; i++) temp = temp -> next; //Split first half
    returned_list.head_ = temp -> next; //Head for second half is the next node
    temp -> next = nullptr; //Split the list
    returned_list.size_ = this->size() - mid;
    this->size_ = mid;
    return returned_list; //Return second half
  }


//Merging function -> merge 2, sorted, lists together
template < typename T >
  void Forward_list < T > ::merge(Forward_list & other) {
    Node * node_1 = this -> head_;
    Node * node_2 = other.head_; //Initalise heads
    Node * temp_1 = new Node(0, node_1);
    Node * temp_2 = temp_1; //Initalise intermediete nodes
    while (node_1 != nullptr && node_2 != nullptr) { //While both nodes are not null and can be checked simultaneously 
      if (node_1 -> data <= node_2 -> data) { //Check if node1 data > node2 data link the larger one
        temp_2 -> next = node_1;
        node_1 = node_1 -> next;
      } else {
        temp_2 -> next = node_2;
        node_2 = node_2 -> next;
      }
      temp_2 = temp_2 -> next;
    }
    if (node_1 != nullptr) temp_2 -> next = node_1; //this list has finished, merge the lists
    if (node_2 != nullptr) temp_2 -> next = node_2; //other list has finished, merge the lists
    this -> head_ = temp_1 -> next;
    temp_1 = nullptr;
    other.head_ = nullptr;
  }

//Merge sort -> recursive function to sort
template < typename T >
  void Forward_list < T > ::merge_sort(Forward_list & my_list) {
    if (my_list.size() == 0 || my_list.size() == 1) return;
    Forward_list < T > second = my_list.split();
    merge_sort(my_list);
    merge_sort(second);
    my_list.merge(second);
  }

//Sort -> calls merge sort function
template < typename T >
  void Forward_list < T > ::sort() {
    merge_sort( * this);
  }

#endif //217 lines of code, 17 lines of comments
