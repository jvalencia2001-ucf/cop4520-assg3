#include <algorithm>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <mutex>

// Class for a concurrent linked list.
template <typename T>
class ConcurrentLinkedList
{
public:
  // Constructor or destructor
  ConcurrentLinkedList() : head_(nullptr) {}

  ~ConcurrentLinkedList()
  {
    Node *current = head_;
    while (current)
    {
      Node *next = current->next_;
      delete current;
      current = next;
    }
  }

  // Insert in ascending order. Lock mutex first to guarantee safety.
  void Insert(const T &value)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    Node *newNode = new Node(value);
    if (!head_ || head_->value_ >= value)
    {
      newNode->next_ = head_;
      head_ = newNode;
    }
    else
    {
      Node *current = head_;
      while (current->next_ && current->next_->value_ < value)
      {
        current = current->next_;
      }
      newNode->next_ = current->next_;
      current->next_ = newNode;
    }
  }

  // Delete specified node. Lock mutex first to guarantee safety.
  void Delete(const T &value)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    Node *current = head_;
    Node *prev = nullptr;

    while (current)
    {
      if (current->value_ == value)
      {
        if (prev)
        {
          prev->next_ = current->next_;
        }
        else
        {
          head_ = current->next_;
        }
        delete current;
        return;
      }
      prev = current;
      current = current->next_;
    }
  }

  // Check if an item is contained on the list. Lock mutex first to guarantee safety.
  bool Contains(const T &value)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    Node *current = head_;
    while (current)
    {
      if (current->value_ == value)
      {
        return true;
      }
      current = current->next_;
    }
    return false;
  }

  // Display the list. Lock mutex first to guarantee safety.
  void Display()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    Node *current = head_;
    std::cout << "Linked List: ";
    while (current)
    {
      std::cout << current->value_ << " ";
      current = current->next_;
    }
    std::cout << std::endl;
  }

private:
  // Struct for a node.
  struct Node
  {
    T value_;
    Node *next_;

    Node(const T &value) : value_(value), next_(nullptr) {}
  };

  // Head and the mutex of the linked list.
  Node *head_;
  std::mutex mutex_;
};

int main()
{

  // Create a vector with ints representing all the presents.
  std::vector<int> shuffledBag(500000);
  for (int i = 0; i < 500000; ++i)
  {
    shuffledBag[i] = i + 1;
  }

  // Shuffle the vector using std::shuffle.
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(shuffledBag.begin(), shuffledBag.end(), g);

  // Create a concurrent linked list of ints to represent the gift chain.
  ConcurrentLinkedList<int> giftChain;

  // Create four threads to represent the minotaurs servants.
  std::thread t1([&giftChain, shuffledBag]
                 {
   
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(1, 500000);
    
    // Loop through a fourth of the bad of
    for (int i = 0; i <= 124999; ++i) {
      // Add the gift to the chain.
      giftChain.Insert(shuffledBag[i]);
      // Delete the gift from the chain, effectivaly writing the thank you note
      // for that gift.
      giftChain.Delete(shuffledBag[i]);
      //Check if a random item is in the chain.
      giftChain.Contains(distr(generator));
    } });

  std::thread t2([&giftChain, shuffledBag]
                 {

    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(1, 500000);
    
    for (int i = 125000; i <= 249999; ++i) {
      // Add the gift to the chain.
      giftChain.Insert(shuffledBag[i]);
      // Delete the gift from the chain, effectivaly writing the thank you note
      // for that gift.
      giftChain.Delete(shuffledBag[i]);
      //Check if a random item is in the chain.
      giftChain.Contains(distr(generator));
    } });

  std::thread t3([&giftChain, shuffledBag]
                 {

    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(1, 500000);

    for (int i = 250000; i <= 374999; ++i) {
      // Add the gift to the chain.
      giftChain.Insert(shuffledBag[i]);
      // Delete the gift from the chain, effectivaly writing the thank you note
      // for that gift.
      giftChain.Delete(shuffledBag[i]);
      //Check if a random item is in the chain.
      giftChain.Contains(distr(generator));
    } });

  std::thread t4([&giftChain, shuffledBag]
                 {

    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(1, 500000);

    for (int i = 375000; i <= 499999; ++i) {
      // Add the gift to the chain.
      giftChain.Insert(shuffledBag[i]);
      // Delete the gift from the chain, effectivaly writing the thank you note
      // for that gift.
      giftChain.Delete(shuffledBag[i]);
      //Check if a random item is in the chain.
      giftChain.Contains(distr(generator));
    } });

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  giftChain.Display();

  // A smaller and much more testable case, will be printed as well.
  ConcurrentLinkedList<int> list;

  std::thread t5([&list]
                 {
    for (int i = 0; i < 100; ++i) {
      list.Insert(i);
      if (i < 50) {
        list.Delete(i);
      }
    } });

  std::thread t6([&list]
                 {
    for (int i = 100; i < 200; ++i) {
      list.Insert(i);
      if (i < 150) {
        list.Delete(i);
      }
    } });

  t5.join();
  t6.join();

  list.Display();
}