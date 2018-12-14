#include <iostream>

template <typename T, typename C = std::less<T>>
class AVL{
  private:
    struct node {
      T data, height;
      node* right;
      node* left;
    };
    node* root;
  public:
    AVL() {root = NULL;}
    bool L(T a, T b, C c = C()) {return c(a, b);}
    node* nodeCreate(T val){
      node* n = new node;
      n->data = val;
      n->right = n->left = NULL;
      n->height = 0;
      return n;
    }
    void insert(T val) {insert(val,&root);}
    void insert(T val, node** root){
      if (!*root)
        *root = nodeCreate(val);
      else{
        if (L(val,(*root)->data)){
          insert(val,&(*root)->left);
          rotate(root);
        }
        else{
          insert(val,&(*root)->right);
          rotate(root);
        }
      }
    }
    void remove(T val) {remove(val,&root);}
    void remove(T val, node** root){
      if (!*root)
        return;
      else if (L(val,(*root)->data))
        remove(val,&(*root)->left);
      else if (L((*root)->data,val))
        remove(val,&(*root)->right);
      else
        del(root);
      if (!*root) return;
      rotate(root);
    }
    void del(node** root){
      if ((*root)->left && (*root)->right){
          node* temp = findMin((*root)->right);
          (*root)->data = temp->data;
          remove((*root)->data,&(*root)->right);
      }
      else if (!(*root)->left){
        node* temp = *root;
        *root = (*root)->right;
        delete temp;
      }
      else if (!(*root)->right){
        node* temp = *root;
        *root = (*root)->left;
        delete temp;
      }
    }
    node* findMin(node* n){
      if (!n) return NULL;
      else if (!n->left) return n;
      else return (n->left);
    }
    int getHeight(node *n){
      if (n)
        if (L(getHeight(n->left),getHeight(n->right)))
          return getHeight(n->right)+1;
        else
          return getHeight(n->left)+1;
      else
        return 0;
    }
    node* rrRotation(node* n){
      node *curr = n->right;
      n->right = curr->left;
      curr->left = n;
      return curr;
    }
    node* llRotation(node* n){
      node* curr = n->left;
      n->left = curr->right;
      curr->right = n;
      return curr;
    }
    void rlRotation(node** n){ (*n)->right = llRotation((*n)->right); }
    void lrRotation(node** n){ (*n)->left = rrRotation((*n)->left); }
    int BalanceFactor(node* n) {return (getHeight(n->left) - getHeight(n->right));}
    void rotate(node** n){
      if (L(1,BalanceFactor(*n)) && L(0,BalanceFactor((*n)->left)))
          *n = llRotation(*n);
      else if (L(1,BalanceFactor(*n)) && !L(0,BalanceFactor((*n)->left)))
          lrRotation(n);
      else if (L(BalanceFactor(*n),-1) && L(0,BalanceFactor((*n)->right)))
          rlRotation(n);
      else if (L(BalanceFactor(*n),-1) && !L(0,BalanceFactor((*n)->right)))
          *n = rrRotation(*n);
    }
    void postorder() {postorder(this->root);}
    void postorder(node* root){
      if(!root) return;
      postorder(root->left);
      postorder(root->right);
      std::cout << root->data << " ";
    }
    void destroy(node* root){
      if(!root) return;
      destroy(root->left);
      destroy(root->right);
      delete(root);
    }
    ~AVL() {destroy(this->root);}
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
  avl.remove(15);
  avl.remove(8);
  avl.postorder();
  std::cout << std::endl;
}
