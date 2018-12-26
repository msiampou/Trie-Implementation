#include <iostream>
#include <cstdlib>
#include <ctime>
#include <assert.h>

template <typename T, typename Less = std::less<T>>
class AVL{

  public:

    AVL() : root_(0) {}

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
        throw "Out of Range error: \n";   //fix me
      }
      node* nnode = root_;
      do {
        size_t size = 0;
        if (root_->next[0] != nullptr) {
          size = root_->next[0]->count;
        }
        nnode = root_;
        bool it = less_(k, size + 1);
        root_ = root_->next[it ? 0 : 1];
        if (it == 0) k -= size + 1;
      } while (k >= 1);
      return nnode->data;
    }

    bool insert(const T& val){
      node** curr = &root_;
      size_t i = 0;
      while (*curr != nullptr) {
        stack[i++] = curr;
        if (less_(val, (*curr)->data)) {
          curr = &((*curr)->next[0]);
        } else if (less_((*curr)->data ,val)) {
          curr = &((*curr)->next[1]);
        } else {
          return false;
        }
      }
      *curr = new node(val);
      for (size_t j = 0; j < i; ++j) {
        size_t it = i -j -1;
        bool pos = less_(val,(*stack[it])->data);
        rotate(*stack[it],pos);
      }
      return true;
    }

    ~AVL() { destroy(root_); }

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

    int getHeight(node *n){
      if (!n) return 0;
      bool pos = less_(getHeight(n->next[0]),getHeight(n->next[1]));
      return getHeight(n->next[pos])+1;
    }

    int BalanceFactor(node* n) { return (getHeight(n->next[0]) - getHeight(n->next[1])); }

    node* double_rotation(node* root, bool pos){
      node *curr = root->next[pos];
      root->next[pos] = single_rotation(curr,!pos);
      node* temp = single_rotation(root,pos);
      if (temp->balance != 1) {
        temp->next[pos]->balance = 0;
        temp->next[!pos]->balance = -(temp->balance);
      } else {
        temp->next[pos]->balance = pos ? -1 : 1;
        temp->next[!pos]->balance = 0;
      }
      temp->balance = 0;
      return temp;
    }

    node* single_rotation(node* root, bool pos){
      //std::cout << "single rot" << std::endl;
      node *curr = root->next[pos];
      curr->count = root->count;
      root->next[pos] = curr->next[!pos];
      curr->next[!pos] = root;
      curr->next[!pos]->count = 1;
      //std::cout << curr->data << " " << curr->balance << std::endl;
      curr->balance += pos ? -1 : 1;
      //std::cout << curr->data << " " << curr->balance << std::endl;
      //std::cout << root->data << " " << root->balance << std::endl;
      root->balance = -(curr->balance);
      //std::cout << root->data << " " << root->balance << std::endl;
      return curr;
    }

    void rotate(node*& n) {
      int bal_factor = BalanceFactor(n);
      if (bal_factor > 1) {
        std::cout << n->data << " to rotate1" << std::endl;
        if (BalanceFactor(n->next[0]) > 0) {
            n = single_rotation(n,0);
        } else {
            n = double_rotation(n,0);
        }
      }
      else if (bal_factor < -1) {
       std::cout << n->data << " to rotate2" << std::endl;
        if (BalanceFactor(n->next[1]) > 0) {
          n = double_rotation(n,1);
        } else {
          n = single_rotation(n,1);
        }
      }
    }

    void rotate(node*& n, bool pos) {
      if (pos) {
        if (n->balance < 0) {
          //std::cout << n->data << " to rotate1" << std::endl;
          if ((n->next[0])->balance != 1) {
              n = single_rotation(n,0);
          } else {
              n = double_rotation(n,0);
          }
        } else {
          n->balance -=1;
        }
      } else {
        if (n->balance > 0) {
          //std::cout << n->data << " to rotate2" << std::endl;
          if ((n->next[1])->balance != -1) {
            n = single_rotation(n,1);
          } else {
            n = double_rotation(n,1);
          }
        }
        else {
          n->balance +=1;
        }
      }
    }

    void postorder(node* root) {
     if (root != nullptr) {
        postorder(root->next[0]);
        std::cout << root->data << "->" << root->balance << std::endl;
        postorder(root->next[1]);
     }
    }

    void destroy(node* root) {
      if (root != nullptr) {
        destroy(root->next[0]);
        destroy(root->next[1]);
        delete(root);
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
  int n = 1000000;

  srand(time(NULL));

  int* nums = new int[n];

  clock_t isum = 0;
  clock_t ssum = 0;
  for (int j = 0; j < 1000; j++) {
    for (int i = 0; i < 1000; ++i) {
      nums[i] = rand();
    }
    clock_t c = clock();
    for (int i = 0; i < 500; ++i) {
      avl.insert(nums[i]);
    }
    isum += clock() - c;
    c = clock();
    for (int i = 0; i < 1000; ++i) {
      avl[nums[i]];
    }
    ssum += clock() - c;
  }

  double s = isum * 1.0 / CLOCKS_PER_SEC;
  std::cout << n << " random inserts in " << s << " sec" << std::endl;

  clock_t c = clock();
  for (int i = 1; i <= n; ++i) {
    avl.insert(i);
  }
  c = clock() - c;
  s = c * 1.0 / CLOCKS_PER_SEC;
  std::cout << n << " sequential inserts in " << s << " sec" << std::endl;

  s = ssum * 1.0 / CLOCKS_PER_SEC;
  std::cout << n << " random serches in " << s << " sec" << std::endl;

}
