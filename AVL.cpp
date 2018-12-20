#include <iostream>
#include <cstdlib>
#include <ctime>

template <typename T, typename Less = std::less<T>>
class AVL{
  public:
    AVL() { root_ = nullptr; }

    void insert(const T& val) { insert(val, root_); }
    
    //void remove(const T& val) { remove(val, root_); }

    void postorder() { postorder(root_); }

    bool operator[](const T& val) {
      node* curr = root_;
      while (curr != nullptr) {
        if (curr->data == val) return true;
        bool it = less_(val, curr->data);
        curr = curr->next[it ? 0 : 1];
      }
      return false;
    }

    const T& operator()(size_t k) {
      if (k <= 0 || k > root_->count) {
        throw "Out of Range error: \n";  // fix me
      }
      node* nnode = root_;
      do {
        size_t size = 0;
        if (root_->next[0] != nullptr) {
          size = root_->next[0]->count;
        }
        nnode = root_;
        bool pos = less_(k,size + 1);
        root_ = root_->next[pos ? 0 : 1];
        if (!pos) k -= size + 1;
      } while (k >= 1);
      return nnode->data;
    }

    ~AVL() { destroy(root_); }

  private:

    struct node {
      const T data;
      node* next[2];
      size_t count;
      int balance : 2;
      node(const T& val) : data(val), count(1), balance(0){
        next[0] = nullptr;
        next[1] = nullptr;
      }
    };

    node* root_;
    Less less_;

    void insert (const T& val, node*& root) {
      if (root == nullptr){
        root = new node(val);
        return;
      }
      bool pos = less_(val, root->data);
      int bf = pos ? -1 : 1;
      insert(val,root->next[pos ? 0 : 1]);
      root->count++;
      root->balance+=bf;
      rotate(val,root);
      root->balance = 0;
      if(root->next[0] != nullptr) {
        root->next[0]->balance = 0;
      }
      if(root->next[1] != nullptr) {
        root->next[1]->balance = 0;
      }
    }

    node* single_rotation(node* root, bool pos) {
      node* curr = root->next[pos];
      curr->count = root->count;
      root->next[pos] = curr->next[!pos];
      curr->next[!pos] = root;
      curr->next[!pos]->count = 1;
      return curr;
    }

    void double_rotation(node*& root, bool pos) {
      root->next[pos] = single_rotation(root->next[pos], pos);
    }

    void rotate(const T& val, node*& root) {
      if (root->balance > 1) {  // fix me
        if (root->next[0]->balance > 0) {
          root = single_rotation(root,0);
        } else {
          double_rotation(root,0);
        }
      } else if (root->balance < -1) {  // fix me
        if (root->next[1]->balance > 0) {
          root = single_rotation(root,1);
        } else {
          double_rotation(root,1);
        }
      }
    }
    
    // void remove(const T& val, node*& root) {
    //   if (root == nullptr) return;
    //   if (root->data == val) { del(root); return; }
    //   bool it = less_(val,root->data);
    //   remove(val,root->next[it ? 0 : 1]);
    //   if (root == nullptr) return;
    //   //rotate(val,root);
    // }
    //
    // void del(node*& root) {
    //   if (root->next[0] && (root)->next[1]){
    //       node* temp = find_min((root)->next[1]);
    //
    //       //(*root)->data = temp->data;
    //       node* curr = temp;
    //       temp = root;
    //       root = curr;
    //       //delete(temp);
    //       remove(temp->data,temp->next[1]);
    //  } else if (!(root)->next[0]){
    //     node* temp = root;
    //     root = (root)->next[1];
    //     delete temp;
    //
    //   } else if (!(root)->next[1]){
    //     node* temp = root;
    //     root = (root)->next[0];
    //     delete temp;
    //   }
    // }
    //
    // node* find_min(node* root) {
    //   if (root == nullptr) return root;
    //   if (root->next[0] == nullptr) return root;
    //   return (root->next[0]);
    // }

    void destroy(node* root) {
      if (root == nullptr) return;
      destroy(root->next[0]);
      destroy(root->next[1]);
      delete(root);
    }
};

int main(void) {
  AVL<int> avl;
  int n = 1000000;

  srand(time(NULL));
  
  clock_t c = clock();
  for (int i = 0; i < n; ++i) {
    int num = rand();
    if (!avl[num]) {
      avl.insert(num);
    } else {
      --i;
    }
  }
  c = clock() - c;
  double s = c * 1.0 / CLOCKS_PER_SEC;
  std::cout << n << " random inserts in " << s << " sec" << std::endl;

  c = clock();
  for (int i = 0; i < n; ++i) {
    avl.insert(i);
  }
  c = clock() - c;
  s = c * 1.0 / CLOCKS_PER_SEC;
  std::cout << n << " sequential inserts in " << s << " sec" << std::endl;
}
