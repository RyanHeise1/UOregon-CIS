#include "bst.h"

// ---------------------------------------
// Node class
// Default constructor
Node::Node() {
    key = 0;
    right = NULL;
    left = NULL;
    parent = NULL;
}
// Constructor
Node::Node(int in) {
    key = in;
    right = NULL;
    left = NULL;
    parent = NULL;
}
// Destructor
Node::~Node() {

}

// Add parent 
void Node::add_parent(Node* in) { parent = in; }

// Add to left of current node
void Node::add_left(Node* in) { left = in; }

// Add to right of current node
void Node::add_right(Node* in) { right = in; }

// Get key
int Node::get_key() { return key; }

// Get parent node
Node* Node::get_parent() { return parent; }

// Get left node
Node* Node::get_left() { return left; }

// Get right node
Node* Node::get_right() { return right; }

// Print the key to ostream to
// Do not change this
void Node::print_info(ostream& to)
{
    to << key << endl;
}
// ---------------------------------------


// ---------------------------------------
// BST class
// Walk the subtree from the given node
void BST::inorder_walk(Node* in, ostream& to)
{
    if (in != NULL){
        inorder_walk(in->get_left(), to);
        in->print_info(to);
        inorder_walk(in->get_right(), to);
    }
    //inorder_walk(root, to);
}
// Constructor
BST::BST()
{
    root = NULL;
}
// Destructor
BST::~BST() {
    while(root != NULL){
        delete_node(tree_min());
    }
}
// Insert a node to the subtree
void BST::insert_node(Node* in) {
    // cout << "Inserting: " << in->get_key() << " into the tree" << endl;
    Node* y = NULL;
    Node* x = root;
    // Traverse BST until you find the right place to insert
    while (x != NULL){
        y = x;
        if (in->get_key() < x->get_key()){
            x = x->get_left();
        }else{
            x = x->get_right();
        }
    }
    // Insert the node
    in->add_parent(y);
    if (y == NULL){
        //cout << "Root node is: " << in->get_key() << endl;
        root = in;
    }else if(in->get_key() < y->get_key()){
        //cout << "Insered: " << in->get_key() << " on the LEFT" << endl;
        y->add_left(in);
    }else{
        //cout << "Insered: " << in->get_key() << " on the RIGHT" << endl;
        y->add_right(in);
    }
}

void BST::transplant(Node* u, Node* v){
    if (u->get_parent() == NULL){
        root = v;
    } 
    else if(u == u->get_parent()->get_left()){
        u->get_parent()->add_left(v);
    }
    else{
        u->get_parent()->add_right(v);
    }
    if (v != NULL){
        v->add_parent(u->get_parent());
    }
}

// Delete a node to the subtree
void BST::delete_node(Node* out)
{
   if (out->get_left() == NULL){
       transplant(out, out->get_right());
   }else if(out->get_right() == NULL){
       transplant(out, out->get_left()); 
   }
   else{
       Node* y = get_min(out->get_right());
       if (y->get_parent() != out){
           transplant(y, y->get_right());
           y->add_right(out->get_right());
           y->get_right()->add_parent(y);
       }
       transplant(out, y);
       y->add_left(out->get_left());
       y->get_left()->add_parent(y);
   }
   delete out;
}

// minimum key in the BST
Node* BST::tree_min()
{
    Node* min_val = root;
    while(min_val->get_left() != NULL){
        min_val = min_val->get_left();
    }
    return min_val;
}
// maximum key in the BST
Node* BST::tree_max()
{
    Node* max_val = root;
    while(max_val->get_right() != NULL){
        max_val = max_val->get_right();
    }
    return max_val;
}
// Get the minimum node from the subtree of given node
Node* BST::get_min(Node* in)
{
    Node* min_node = in;
    //cout << min_node->get_left()->get_key() << endl;
    while(min_node->get_left() != NULL){
        min_node = min_node->get_left();
    }
    return min_node;
}
// Get the maximum node from the subtree of given node
Node* BST::get_max(Node* in)
{
    Node* max_node = in;
    while(max_node->get_right() != NULL){
        max_node = max_node->get_right();
    }
    return max_node;
}
// Get successor of the given node
Node* BST::get_succ(Node* in)
{
    Node* succ;
    if (in->get_right() != NULL){ // in has a right subtree
        return get_min(in->get_right());
    }
    succ = in->get_parent();
    // cout << "Succ: " << succ << "  Succ right: " << succ->get_right() << "  In: " << in << endl;
    while(succ != NULL and in == succ->get_right()) {
        in = succ;
        succ = succ->get_parent();
    }
    return succ;
}
// Get predecessor of the given node
Node* BST::get_pred(Node* in)
{
    Node* pred;
    if (in->get_left() != NULL){ // in has a right subtree
        return get_max(in->get_left());
    }
    pred = in->get_parent();
    // cout << "Succ: " << succ << "  Succ right: " << succ->get_right() << "  In: " << in << endl;
    while(pred != NULL and in == pred->get_left()){
        in = pred;
        pred = pred->get_parent();
    }
    return pred;
}
// Walk the BST from min to max
void BST::walk(ostream& to)
{
    if (root != NULL){
        inorder_walk(root, to);
    }
}
// Search the tree for a given key
Node* BST::tree_search(int search_key)
{
    Node* curr_node = root;
    while(curr_node != NULL and curr_node->get_key() != search_key){
        if (search_key > curr_node->get_key()){
            curr_node = curr_node->get_right();
        }else{
            curr_node = curr_node->get_left();
        }
    }
    return curr_node;
}
// ---------------------------------------
