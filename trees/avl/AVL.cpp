#include <iostream>
#include <cstdlib>
#include <ctime>
#include <assert.h>

template <typename T, typename Less = std::less<T>>
class AVL{

  public:

  AVL() : root_(nullptr) {}

  void check() { check(root_); }

  void postorder() { postorder(root_); }

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
      cout << "Out of Range error" << endl;
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

  bool insert(const T& val){
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
    while (ts != stack) {
      curr = *ts;
      bool pos = less_(val, (*curr)->data);
      if (rotate(*curr, pos) == true) return true;
      ts--;
    }
    return true;
  }

  void destroy(void){
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

  ~AVL() { destroy(); }

  private:

  struct node {
    const T data;
    int balance :2;
    node* next[2];
    size_t count;
    node(const T& val)
        : data(val),
        count(1),
        balance(0){
      next[0] = nullptr;
      next[1] = nullptr;
    }
  };

  node* root_;
  Less less_;
  node** stack[64];

  void double_rotation(node*& root, bool pos){
    node* temp = root;
    node* curr = temp->next[pos];
    root = curr->next[!pos];
    curr->next[!pos] = root->next[pos];
    temp->next[pos] = root->next[!pos];
    root->next[!pos] = temp;
    root->next[pos] = curr;
    int bf = pos ? -1 : 1;
    if (root->balance != bf) {
      curr->balance = 0;
      temp->balance = -(root->balance);
    } else {
      curr->balance = pos ? 1 : -1;
      temp->balance = 0;
    }
    root->balance = 0;
  }

  void single_rotation(node*& root, bool pos){
    node *curr = root->next[pos];
    curr->count = root->count;
    root->next[pos] = curr->next[!pos];
    curr->next[!pos] = root;
    curr->next[!pos]->count = 1;
    curr->balance += pos ? -1 : 1;
    root->balance = -(curr->balance);
    root = curr;
  }

  bool rotate(node*& n, bool pos) {
    if (pos) {
      if (n->balance < 0) {
        if ((n->next[0])->balance != 1) {
            single_rotation(n,0);
        } else {
            double_rotation(n,0);
        }
        return true;

      } else {
        n->balance--;
        if(n->balance == 0) return true;
      }
    } else {
      if (n->balance > 0) {
        if ((n->next[1])->balance != -1) {
          single_rotation(n,1);
        } else {
          double_rotation(n,1);
        }
        return true;

      } else {
        n->balance++;
        if(n->balance == 0) return true;
      }
    }
    return false;
  }

  void postorder(node* root) {
   if (root != nullptr) {
      postorder(root->next[0]);
      std::cout << root->data << "->" << root->balance << std::endl;
      postorder(root->next[1]);
   }
  }

  size_t check(node* p) {
    if (p == NULL) return 0;
    size_t l = check(p->next[0]);
    size_t r = check(p->next[1]);
    size_t d = (l > r) ? (l - r) : (r - l);
    std::cout << d << std::endl;
    assert(d <= 1);
    return ((l > r) ? l : r) + 1;
  }
};

int main(void) {
  AVL<int> avl;
  int n = 1000;
  int loops = 1000/n;

  srand(time(NULL));

  int* nums = new int[n];

  clock_t isum = 0;
  clock_t ssum = 0;
  clock_t qsum = 0;
  for (int i = 0; i < loops; ++i) {
    for (int j = 0; j < n; ++j) {
      nums[j] = rand();
    }
    clock_t c = clock();
    for (int j = 0; j < n; ++j) {
      avl.insert(nums[j]);
    }
    isum += clock() - c;
    c = clock();
    for (int j = 0; j < n; ++j) {
      avl.insert(j);
    }
    qsum += clock() - c;
    c = clock();
    for (int j = 0; j < n; ++j) {
      avl[nums[j]];
    }
    ssum += clock() - c;
    avl.destroy();
  }

  double s = isum * 1.0 / CLOCKS_PER_SEC;
  int rate = n*loops / s;
  std::cout << n << "x" << loops << " random inserts -> rate: " << rate << std::endl;

  s = qsum * 1.0 / CLOCKS_PER_SEC;
  rate = n*loops / s;
  std::cout << n << "x" << loops << " sequential inserts -> rate: " << rate << std::endl;

  s = ssum * 1.0 / CLOCKS_PER_SEC;
  rate = n*loops / s;
  std::cout << n << "x" << loops << " random searches -> rate: " << rate << std::endl;
  delete[] nums;
}
