#include <iostream>
#include <cstdlib>
#include <ctime>
#include <assert.h>

template <typename T, typename Less = std::less<T>>
class BST{

  public:

  BST() : root_(nullptr) {}

  void inorder() { inorder(root_); }

  bool operator[](const T& val) {
    node* curr = root_;
    while (curr != nullptr){
      if (less_(val, curr->data)) {
        curr = curr->next[0];
      } else if (less_(curr->data,val)) {
        curr = curr->next[1];
      } else {
        return true;
      }
    }
    return false;
  }

  const T& operator()(size_t k) {
    if (k <= 0 || k > root_->count){
      std::cout << "Out of Range error" << std::endl;
      return NULL;
    }
    node* new_node = root_;
    do {
      size_t size = 0;
      if (root_->next[0] != nullptr) {
        size = root_->next[0]->count;
      }
      new_node = root_;
      bool it = less_(k, size + 1);
      root_ = root_->next[it ? 0 : 1];
      if (it == 0) k -= size + 1;
    } while (k >= 1);
    return new_node->data;
  }

  bool insert(const T& val) {
    node** curr = &root_;
    node*** ts = stack;
    while (*curr != nullptr) {
      *(++ts) = curr;
      if (less_(val, (*curr)->data)) {
        curr = &((*curr)->next[0]);
      } else if (less_((*curr)->data, val)) {
        curr = &((*curr)->next[1]);
      } else {
        return false;
      }
    }
    *curr = new node(val);
    return true;
  }

  void destroy(void) {
    node** curr;
    node*** ts = stack;
    if (root_ == nullptr) return;
    *(++ts) = &root_;
    while (ts != stack) {
      curr = *ts;
      if ((*curr)->next[0]) {
        *(++ts) = &((*curr)->next[0]);
      } else if ((*curr)->next[1]) {
        *(++ts) = &((*curr)->next[1]);
      }
      else {
        delete *curr;
        *curr = nullptr;
        ts--;
      }
    }
    root_ = nullptr;
  }

  ~BST() { destroy(); }

  private:

  struct node {
    const T data;
    node* next[2];
    size_t count;
    node(const T& val)
        : data(val),
        count(1) {
      next[0] = nullptr;
      next[1] = nullptr;
    }
  };

  node* root_;
  Less less_;
  node** stack[64];

  void inorder(node* root) {
   if (root != nullptr) {
      inorder(root->next[0]);
      std::cout << root->data << std::endl;
      inorder(root->next[1]);
   }
  }
};

int main(void) {
  BST<int> bst;
  int n = 10000;
  int loops = 1000000/n;

  srand(time(NULL));

  int* nums = new int[n];

  clock_t isum = 0;
  clock_t ssum = 0;
  for (int i = 0; i < loops; ++i) {
    for (int j = 0; j < n; ++j) {
      nums[j] = rand();
    }
    clock_t c = clock();
    for (int j = 0; j < n; ++j) {
      bst.insert(nums[j]);
    }
    isum += clock() - c;
    c = clock();
    for (int j = 0; j < n; ++j) {
      bst[nums[j]];
    }
    ssum += clock() - c;
    //bst.inorder();
    bst.destroy();
  }

  double s = isum * 1.0 / CLOCKS_PER_SEC;
  int rate = n*loops / s;
  std::cout << n << "x" << loops << " random inserts -> rate: " << rate << std::endl;

  s = ssum * 1.0 / CLOCKS_PER_SEC;
  rate = n*loops / s;
  std::cout << n << "x" << loops << " random searches -> rate: " << rate << std::endl;

  delete[] nums;
}
