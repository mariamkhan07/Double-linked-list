#include <iostream>
#include <string>

using namespace std;

// Data class that stores student age and name
class Data {
public:
    int age;
    string name;
    Data(int age, string name ) {
        this->age = age;
        this->name = name;
    }
    void print() {
        cout << age << " " << name << endl;
    }

    //function to compare data objects for counting multiples
    bool compareData(const Data& otherData) const {
        return (age == otherData.age) && (name == otherData.name);
    }
    //helper function used for removing multiples
    bool operator==(const Data& other) const {
        return age == other.age && name == other.name;
    }
};

// Node class for Doubly Linked List
template <typename T>
class Node {
public:
    T *value;
    Node<T> *next;
    Node<T> *prev;

    Node(T *value) {
        this->value = value;
        next = nullptr;
        prev = nullptr;
    }
    void print() {
        value->print();
    }
};

template <typename T>
class DoubleLinkedList {
private:
    Node<T> *head;
    Node<T> *tail;
    int length;
public:
    // Constructor
    DoubleLinkedList(T *value) {
        Node<T> *newNode;
        newNode = new Node<T>(value);
        head = newNode;
        tail = newNode;
        length = 1;
    }

    // Destructor
    ~DoubleLinkedList() {
        Node<T> *temp = head;
        while (head) {
            head = head->next;
            delete temp;
            temp = head;
        }
    }

    void printList() {
        Node<T> *temp = head;
        while (temp != nullptr) {
            temp->print();
            temp = temp->next;
        }
    }

    //** Part One: delete methods
    void deleteAtHead() {
        if (length == 0) {
            cout << "List is empty." << endl;
            return;
        }
        head->value->print();

        Node<T> *temp = head;
        if (length == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        length--;
    }

    void deleteAtTail() {
        if (length == 0) {
            cout << "List is empty." << endl;
            return;
        }
        tail->value->print();

        Node<T> *temp = tail;
        if (length == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
            tail->prev = nullptr;
        }
        delete temp;
        length--;
    }


    void deleteAtIndex(int index) {
        if (index < 0 || index >= length)
            return;
        if (index == 0)
            return deleteAtHead();
        if (index == length - 1)
            return deleteAtTail();

        Node<T> *temp = head;
        for (int i = 0; i < index; i++) {
            temp = temp->next;
        }
        temp->value->print();

        //updating pointers
        Node<T> *prevNode = temp->prev;
        Node<T> *nextNode = temp->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        delete temp;
        length--;
    }

   //deletes entire list
    void deleteList() {
        Node<T> *temp = head;
        while (head) {
            head = head->next;
            delete temp;
            temp = head;
        }

        tail = nullptr;
        length = 0;
    }


//**Part two: sorting list-bubble sort
    void sortList() {
        if (length <= 1) {
            cout << "List already sorted/empty";
            return;
        }
        bool swapped;
        Node<T> *temp;
        Node<T> *last = nullptr;

        do {
            swapped = false;
            temp = head;

            while (temp->next != last) {
                if (temp->value->age > temp->next->value->age) {
                    swap(temp->value, temp->next->value);
                    swapped = true;
                }
                temp = temp->next;
            }
            last = temp;
        } while (swapped);

        printList();  //printing sorted list
    }

    //**Part three: removing multiples
    void removeMultiples() {
        Node<T> *temp = head;

        while (temp != nullptr) {
            Node<T> *currentNode = temp->next;
            while (currentNode != nullptr) {
                if (*(temp->value) == *(currentNode->value)) {
                    Node<T> *nodeToDelete = currentNode;
                    currentNode = currentNode->next;
                    // Updating pointers
                    if (nodeToDelete->prev) {
                        nodeToDelete->prev->next = nodeToDelete->next;
                    } else {
                        head = nodeToDelete->next;
                    }
                    if (nodeToDelete->next) {
                        nodeToDelete->next->prev = nodeToDelete->prev;
                    } else {
                        tail = nodeToDelete->prev;
                    }
                    delete nodeToDelete;
                    length--;
                } else {
                    currentNode = currentNode->next;
                }
            }
            temp = temp->next;
        }
        printList();
    }

    //**part four: counting multiples
    int countMultiples(T value) {
        int count = 0;
        Node<T> *temp = head;

        while (temp != nullptr) {
            if (temp->value->compareData(value)) {
                count++;
            }
            temp = temp->next;
        }
        return count;
    }

    Node<T> *get(int index) {
        if (index < 0 || index >= length)
            return nullptr;
        Node<T> *temp = head;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        return temp;
    }

    void append(T *value) {
        Node<T> *newNode = new Node<T>(value);
        if (length == 0) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        length++;
    }

    void prepend(T *value) {
        Node<T> *newNode = new Node<T>(value);
        if (length == 0) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        length++;
    }

    bool insert(int index, T *value) {
        if (index < 0 || index > length)
            return false;
        if (index == 0) {
            prepend(value);
            return true;
        }
        if (index == length) {
            append(value);
            return true;
        }
        Node<T> *newNode = new Node<T>(value);
        Node<T> *prev1 = get(index - 1);
        Node<T> *next1 = prev1->next;
        newNode->prev = prev1;
        newNode->next = next1;
        prev1->next = newNode;
        next1->prev = newNode;
        length++;
        return true;
    }

    //**part 5- splitting the list
    void headTailSplit() {
        int mid = length / 2;

        //creating 2 lists
        DoubleLinkedList<T> listA(nullptr);
        DoubleLinkedList<T> listB(nullptr);

        listA.head = head;
        listA.tail = get(mid - 1);
        listA.length = mid;


        listB.head = get(mid);
        listB.tail = tail;
        listB.length = length - mid;


        if (listA.tail) {
            listA.tail->next = nullptr;
        }
        if (listB.head) {
            listB.head->prev = nullptr;
        }

        // reset the original list
        head = nullptr;
        tail = nullptr;
        length = 0;


        cout << "List A:" << endl;
        listA.printList();

        cout << "List B:" << endl;
        listB.printList();
    }

    //**part 6- reversing list
    void reverseList() {
        Node<T> *curr = head;
        Node<T> *temp = nullptr;

        while (curr != nullptr) {
            temp = curr->prev;
            curr->prev = curr->next;
            curr->next = temp;

            curr = curr->prev;
        }

        Node<T> *tempHead = head;
        head = tail;
        tail = tempHead;

        printList(); // Print reversed list
    }

};

DoubleLinkedList<Data>* createList(){
    DoubleLinkedList<Data> *ll1;
    ll1 = new DoubleLinkedList<Data>(new Data(24, "Alex"));
    ll1->append(new Data(20,"John"));
    ll1->append(new Data(20, "John"));
    ll1->append(new Data(32, "Lily"));
    ll1->append(new Data(17, "Frank"));
    ll1->append(new Data(46, "Kanye"));

    return ll1;
}

int main() {
    DoubleLinkedList<Data> *ll1 = createList();

    int choice;
    int age;
    string name;

    while (true) {
        cout << "Menu: " << endl;
        cout << "1. Create a List" << endl;
        cout << "2. Delete a List" << endl;
        cout << "3. Insert at Head" << endl;
        cout << "4. Insert at Tail" << endl;
        cout << "5. Insert at Index" << endl;
        cout << "6. Delete at Head" << endl;
        cout << "7. Delete at Tail" << endl;
        cout << "8. Delete at Index" << endl;
        cout << "9. Reverse List" << endl;
        cout << "10. Sort List" << endl;
        cout << "11. Count Multiples" << endl;
        cout << "12. Delete Multiples" << endl;
        cout << "13. Split List Even Odd" << endl;
        cout << "14. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (ll1) {
                    ll1->printList();
                } else {
                    cout << "List does not exist." << endl;

                }
                break;

            case 2:
                if (ll1) {
                    ll1->deleteList();
                    ll1 = nullptr;
                    cout << "List deleted" << endl;
                }
                break;

            case 3:
                cout << "Enter student age: ";
                cin >> age;
                cout << "Enter student name: ";
                cin >> name;

                if (ll1) {
                    Data *newData = new Data(age, name);
                    ll1->prepend(newData);
                    cout << "Node inserted at the head." << endl;
                }
                break;

            case 4:
                cout << "Enter student age: ";
                cin >> age;
                cout << "Enter student name: ";
                cin >> name;

                if (ll1) {
                    Data *newData = new Data(age, name);
                    ll1->append(newData);
                    cout << "Node inserted at tail" << endl;
                } else {
                    cout << "List does not exist" << endl; //remind user to create list
                }
                break;

            case 5:
                if (ll1) {
                    int index;
                    cout << "Enter the index where you want to insert: ";
                    cin >> index;

                    cout << "Enter student age: ";
                    cin >> age;
                    cout << "Enter student name: ";
                    cin >> name;

                    if (ll1->insert(index, new Data(age, name))) {
                        cout << "Node inserted at index " << index << "." << endl;
                    }
                      break;

                    case 6:
                        if (ll1) {
                            ll1->deleteAtHead();
                            cout << "Node deleted at the head." << endl;
                        }
                    break;

                    case 7:
                        if (ll1) {
                            ll1->deleteAtTail();
                            cout << "Node deleted at the tail." << endl;
                        }
                         break;

                    case 8:
                        if (ll1) {
                            int indexToDelete;
                            cout << "Enter the index to delete: ";
                            cin >> indexToDelete;

                            if (indexToDelete >= 0) {
                                ll1->deleteAtIndex(indexToDelete);
                                cout << "Node deleted at index " << indexToDelete << endl;
                            }
                        }
                         break;

                    case 9:
                        if(ll1)
                            ll1->reverseList();
                        break;

                    case 10:
                        if (ll1)
                            ll1->sortList();
                        break;

                    case 11:
                        if (ll1) {
                            int ageToCount;
                            string nameToCount;
                            //can add multiples to update the list-then check
                            cout << "Enter student age to count: ";
                            cin >> ageToCount;
                            cout << "Enter student name to count: ";
                            cin >> nameToCount;

                            Data dataToCount(ageToCount, nameToCount);

                            int count = ll1->countMultiples(dataToCount);
                            cout << "Count: " << count << endl;
                        }
                          break;

                    case 12:
                        if (ll1)
                            ll1->removeMultiples();
                           break;

                    case 13:
                        if (ll1)
                            ll1->headTailSplit();
                          break;

                    case 14:
                      cout << "Bye!!" << endl;
                       exit(0);

                    default:
                        cout<< "Invalid option. Try again.";
                        break;
                }
        }

    }

}