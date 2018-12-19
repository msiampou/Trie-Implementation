#include <iostream>
#include <cstdlib>

template <typename T, typename Less = std::less<T>>
class AVL{

  public:

    AVL() { root_ = nullptr; }

    void insert(const T& val) { insert(val,root_); }

    void remove(const T& val) { remove(val,root_); }

    void postorder() { postorder(root_); }

    bool operator[](const T& val){
      node* curr = root_;
      while (curr != nullptr){
        if(curr->data == val) return true;
        bool it = less(val,curr->data);
        curr = curr->next[it ? 0 : 1];
      }
      return false;
    }

    const T& operator()(int pos) {
      if (pos <= 0 || pos > root_->count){
        throw "Out of Range error: \n";
      }
      node* nnode = root_;
      do{
        size_t size = 0;
        if (root_->next[0] != nullptr) {
          size = root_->next[0]->count;
        }
        nnode = root_;
        bool it = less(pos,size+1);
        root_ = root_->next[it ? 0 : 1];
        if (it == 0) pos -= size+1;
      } while (pos>=1);

      return nnode->data;
    }

    ~AVL() { destroy(root_); }

  private:

    struct node {
      const T data;
      node* next[2];
      size_t count;
      int balance : 3;
      node(const T& val) : data(val), count(1), balance(0){
        next[0] = nullptr;
        next[1] = nullptr;
      }
    };

    node* root_;
    Less less;

    node* create (const T& val) {
      node* n = new node(val);
      return n;
    }

    void insert (const T& val, node*& root) {
      if (root == nullptr){
        root = create(val);
        std::cout<< root->data << " me balance: " << root->balance << std::endl;
        return;
      }
      bool it = less(val,root->data);
      int bf = it ? -1 : 1;
      std::cout<<bf<<std::endl;
      std::cout<< root->data << " me balance: " << root->balance << std::endl;
      insert(val,root->next[it ? 0 : 1]);
      root->count++;
      root->balance+=bf;
      rotate(val,root);
    }

//     inline bool search(const T& val) {
      
//     }

//     inline const T& search(node* root, size_t k) {
      
//     }

    void remove(const T& val, node*& root){
      if (root == nullptr) return;
      if (root->data == val) { del(root); return; }
      bool it = less(val,root->data);
      remove(val,root->next[it ? 0 : 1]);
      if (root == nullptr) return;
      //rotate(val,root);
    }

    void del(node*& root){
      if (root->next[0] && (root)->next[1]){
          node* temp = find_min((root)->next[1]);

          //(*root)->data = temp->data;
          node* curr = temp;
          temp = root;
          root = curr;
          //delete(temp);
          remove(temp->data,temp->next[1]);
          std::cout << "edw\n" << std::endl;
     }
      else if (!(root)->next[0]){
        node* temp = root;
        root = (root)->next[1];
        delete temp;

      } else if (!(root)->next[1]){
        node* temp = root;
        root = (root)->next[0];
        delete temp;
      }
    }

    node* find_min(node* root) {
      if (root == nullptr) return root;
      if (root->next[0] == nullptr) return root;
      return (root->next[0]);
    }

    node* rr_rotation(node* root) {
      node* curr = root->next[1];
      curr->count = root->count;
      root->next[1] = curr->next[0];
      curr->next[0] = root;
      curr->next[0]->count = 1;
      return curr;
    }

    node* ll_rotation(node* root) {
      node* curr = root->next[0];
      curr->count = root->count;
      root->next[0] = curr->next[1];
      curr->next[1] = root;
      curr->next[1]->count = 1;
      return curr;
    }

    void rl_rotation(node*& root) { root->next[1] = ll_rotation(root->next[1]); }

    void lr_rotation(node*& root) { root->next[0] = rr_rotation(root->next[0]); }

    void rotate(const T& val, node*& root) {
      if (root->balance == -2){
        if (val < root->next[0]->data){
          root = ll_rotation(root);
        } else{
          lr_rotation(root);
        }
      } else if (root->balance == 2){
        if (val > root->next[1]->data){
          root = rr_rotation(root);
        } else{
          rl_rotation(root);
        }
      }
    }

    void postorder(node* root) {
      if(root == nullptr) return;
      postorder(root->next[0]);
      postorder(root->next[1]);
      std::cout << root->data << " ";
    }

    void destroy(node* root) {
      if(root == nullptr) return;
      destroy(root->next[0]);
      destroy(root->next[1]);
      delete(root);
    }
};

int main(void){
  AVL<int> avl;

  for (int i=1; i<=100; i++){
      avl.insert(i);
  }

  // avl.insert(8);
  // avl.insert(5);
  // avl.insert(4);
  // avl.insert(11);
  // avl.insert(15);
  // avl.insert(3);
  // avl.insert(6);
  // avl.insert(2);
  // avl.postorder();
  // std::cout << std::endl;
  //
  avl.remove(15);
  //avl.remove(8);
  // std::cout << avl[16] << std::endl;
  // std::cout << avl[4] << std::endl;
  avl.postorder();
  std::cout << std::endl;
  // std::cout << avl(1) << std::endl;
  // //std::cout << avl(1) << std::endl;
  // std::cout << std::endl;
}
