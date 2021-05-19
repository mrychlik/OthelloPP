#include <iterator>

/**
 * template for Node for use in the Stack/Queue/PriQueue implimentations below
 * supports empty initiation and initiation with a value 
**/ 
template <typename type> class Node {
  public:
    Node* next;
    type* load;
    signed char value;

    Node() {
      this->next = nullptr;
      this->load = nullptr;
      this->value = 0;
    }

    Node(type* l) {
      this->next = nullptr;
      this->load = l;
      this->value = 0;
    }
};

/**
 * Custom implimentation of a FILO Stack of arbitrary type
 * content is in pointer form
 * supports deletion including or excluding the pointed to content. (~Stack() leaves content, run delStackAndContent() manually)
 * push/pop/peek/forward iteration works as expected
 * I will also be using this as a addition/iteration Bag implementation, as it features constant addition time
**/
template <typename type> class Stack {
  private:
    Node<type>* first;

  public:

    Stack() {
      this->first = nullptr;
    }

    ~Stack() {
      delStackLeaveContent();
    }

    void delStackLeaveContent() {
      Node<type>* current = first;
      Node<type>* previous;

      while(current != nullptr) {
        previous = current;
        current = current->next;
        delete previous;
      } 
    }

    void delStackAndContent() {
      Node<type>* current = first;
      Node<type>* previous;

      while(current != nullptr) {
        previous = current;
        current = current->next;
        delete previous->load;
        delete previous;
      }
    }

    void push(type* l) {
      Node<type>* add = new Node<type>(l);
      add->next = first;
      first = add;
    }

    type* pop() {
      type* contents = first->load;
      Node<type>* hold = first;
      first = first->next;
      delete hold;
      return contents;
    }

    type* peek() {
      return first->load;
    }
  
    class iterator {
      public: 
        Node<type>* n;

        iterator(Node<type>* a) {
          this->n = a;
        }

        iterator& operator++ () {
          this->n = this->n->next;
          return *this;
        }

        bool operator== (const iterator a) {
          return a.n == this->n;
        }

        bool operator!= (const iterator a) {
          return a.n != this->n;
        }

        type* operator* () {
          return this->n->load;
        }
    };

    iterator begin() {
      return iterator(first);
    }

    iterator end() {
      return iterator(nullptr);
    }
};

