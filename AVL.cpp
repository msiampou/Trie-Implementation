#include <iostream>

template <typename T, typename Less = std::less<T>>
class AVL{
  private:

    struct node {
      const T data;
      node* next[2];
      node(const T& val) : data(val){
        next[0] = NULL; //left
        next[1] = NULL; //right
      }
    };
    node* root;
    Less less;

  public:

    AVL() { root = NULL; }

    void insert(const T& val) { insert(val,&root); }

    void postorder() { postorder(this->root); }

    bool operator[](const T& val){
      return search(val);
    }

    ~AVL() { destroy(this->root); }

  private:

    node* nodeCreate(const T& val){
      node* n = new node(val);
      return n;
    }

    void insert(const T& val, node** root){
      if (!*root){
        *root = nodeCreate(val);
        return;
      }
      bool pos = less(val,(*root)->data);
      insert(val,&(*root)->next[!pos]);
      rotate(root);
    }

    bool search(const T& val){
      node* curr = root;
      while (curr != nullptr){
        if(curr->data == val) return true;
        bool pos = less(val,curr->data);
        curr = curr->next[!pos];
      }
      return false;
    }

    // void remove(const T& val) { remove(val,&root); }
    //
    // void remove(const T& val, node** root){
    //   if (!*root)
    //     return;
    //   else if (L(val,(*root)->data))
    //     remove(val,&(*root)->left);
    //   else if (L((*root)->data,val))
    //     remove(val,&(*root)->right);
    //   else
    //     del(root);
    //   if (!*root) return;
    //   rotate(root);
    // }
    //
    // void del(node** root){
    //   if ((*root)->left && (*root)->right){
    //       node* temp = findMin((*root)->right);
    //       (*root)->data = temp->data;
    //       remove((*root)->data,&(*root)->right);
    //
    //   } else if (!(*root)->left){
    //     node* temp = *root;
    //     *root = (*root)->right;
    //     delete temp;
    //
    //   } else if (!(*root)->right){
    //     node* temp = *root;
    //     *root = (*root)->left;
    //     delete temp;
    //   }
    // }

    node* findMin(node* n){
      if (!n) return NULL;
      if (!n->next[0]) return n;
      return (n->next[0]);
    }

    int getHeight(node *n){
      if (!n) return 0;
      bool pos = less(getHeight(n->next[0]),getHeight(n->next[1]));
      return getHeight(n->next[pos])+1;
    }

    node* rrRotation(node* n){
      node *curr = n->next[1];
      n->next[1] = curr->next[0];
      curr->next[0] = n;
      return curr;
    }

    node* llRotation(node* n){
      node* curr = n->next[0];
      n->next[0] = curr->next[1];
      curr->next[1] = n;
      return curr;
    }

    void rlRotation(node** n) { (*n)->next[1] = llRotation((*n)->next[1]); }

    void lrRotation(node** n) { (*n)->next[0] = rrRotation((*n)->next[0]); }

    int BalanceFactor(node* n) { return (getHeight(n->next[0]) - getHeight(n->next[1])); }

    void rotate(node** n){
      if (less(1,BalanceFactor(*n)) && less(0,BalanceFactor((*n)->next[0])))
          *n = llRotation(*n);
      else if (less(1,BalanceFactor(*n)) && !less(0,BalanceFactor((*n)->next[0])))
          lrRotation(n);
      else if (less(BalanceFactor(*n),-1) && less(0,BalanceFactor((*n)->next[1])))
          rlRotation(n);
      else if (less(BalanceFactor(*n),-1) && !less(0,BalanceFactor((*n)->next[1])))
          *n = rrRotation(*n);
    }

    void postorder(node* root){
      if(!root) return;
      postorder(root->next[0]);
      postorder(root->next[1]);
      std::cout << root->data << " ";
    }

    void destroy(node* root){
      if(!root) return;
      destroy(root->next[0]);
      destroy(root->next[1]);
      delete(root);
    }
};

int main(void){
  AVL<int> avl;
  avl.insert(8);
  avl.insert(5);
  avl.insert(4);
  avl.insert(11);
  avl.insert(15);
  avl.insert(3);
  avl.insert(6);
  avl.insert(2);
  // avl.remove(15);
  // avl.remove(8);
  std::cout << avl[16] << std::endl;
  std::cout << avl[4] << std::endl;
  avl.postorder();
  std::cout << std::endl;
}
