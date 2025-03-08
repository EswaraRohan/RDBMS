#include "InternalNode.hpp"

//creates internal node pointed to by tree_ptr or creates a new one
InternalNode::InternalNode(const TreePtr &tree_ptr) : TreeNode(INTERNAL, tree_ptr) {
    this->keys.clear();
    this->tree_pointers.clear();
    if (!is_null(tree_ptr))
        this->load();
}

//max element from tree rooted at this node
Key InternalNode::max() {
    Key max_key = DELETE_MARKER;
    TreeNode* last_tree_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    max_key = last_tree_node->max();
    delete last_tree_node;
    return max_key;
}

//if internal node contains a single child, it is returned
TreePtr InternalNode::single_child_ptr() {
    if (this->size == 1)
        return this->tree_pointers[0];
    return NULL_PTR;
}



//inserts <key, record_ptr> into subtree rooted at this node.
//returns pointer to split node if exists
TreePtr InternalNode::insert_key(const Key &key, const RecordPtr &record_ptr) {
    InternalNode* node_tree = dynamic_cast<InternalNode*>(TreeNode::tree_node_factory(INTERNAL));
    TreePtr new_tree_ptr = NULL_PTR;
    int dummy = 0;
    while(this->keys[dummy]<key && this->size>dummy+1 && DELETE_MARKER != this->keys[dummy]){
        dummy++;
    }

    auto node = TreeNode::tree_node_factory(this->tree_pointers[dummy]);
    TreePtr treeptr = node->insert_key(key,record_ptr);
    if(is_null(treeptr)){
        delete node;
        return new_tree_ptr;
    }

    this->size = this->size + 1;
    this->tree_pointers.push_back(NULL_PTR);
    int var = this->size - 1;
    for(int k = var ; k >= dummy+2 ; k--){
        this->tree_pointers[k]=this->tree_pointers[k-1];
    }

    this->tree_pointers[dummy+1] = treeptr;
    this->keys.push_back(DELETE_MARKER);

    int var2 = this->size - 2;

    for(int k=var2;k>=dummy+1;k--){
        this->keys[k] = this->keys[k-1];
    }

    this->keys[dummy] = node->max();
    delete node;

    if(!this->overflows()){
        this->dump();
        return new_tree_ptr;
    }

    new_tree_ptr = node_tree->tree_ptr;
    
    int var3 = this->size;
    for(int k=MIN_OCCUPANCY;k<var3;k++){
        node_tree->tree_pointers.push_back(this->tree_pointers[k]);
        node_tree->size++;
    }

    this->tree_pointers.resize(MIN_OCCUPANCY);
    int var4 = this->size-1;
    for(int k=MIN_OCCUPANCY;k<var4;k++){
        node_tree->keys.push_back(this->keys[k]);
    }
    int var5 = MIN_OCCUPANCY-1;
    this->keys.resize(var5);
    this->size = MIN_OCCUPANCY;

    node_tree->dump();
    this->dump();
    delete node_tree;
    return new_tree_ptr;
}


//deletes key from subtree rooted at this if exists
/*
Handling UNderflow - 
1) Redistribution With a Sibling 
2) Merge with a sibling

Minimum condition for one node - 2*ceil(fanout/2)
Maximum condition for a node - fanout

CASE 1 - REDISTRIBUTION WITH A SIBLING - 
if(underflown_node.size + sibling_size  >= 2*min_condn){
    this<----sibling
}

CASE 2 - MERGE WITH A SIBLING
if(underflown_node.size + sibling_size <= fanout){
    new_node<--- this+sibling (merge)
}

 Preference Order For Deletion - 

1) Redistribute with left sibling
2) Merge WIth left sibling
3) Redistribute with right sibling
4) Merge with right sibling

*/

void InternalNode::redis_l(TreeNode* node) {
    InternalNode* rnode_redis = dynamic_cast<InternalNode*>(node);

    rnode_redis->keys.push_back(DELETE_MARKER);

    rnode_redis->tree_pointers.push_back(NULL_PTR);

    rnode_redis->size++;

    int var = rnode_redis->size;
    int var2 = var-1;

    for(int k=var2;k>0;k--){
        rnode_redis->tree_pointers[k] = rnode_redis->tree_pointers[k-1];
    }

    int var3 = rnode_redis->size -2;
    for(int k=var3;k>0;k--){
        rnode_redis->keys[k] = rnode_redis->keys[k-1];
    }
    
    int var4 = this->size -1;
    rnode_redis->tree_pointers[0] = this->tree_pointers[var4];

    auto redis_right = TreeNode::tree_node_factory(rnode_redis->tree_pointers[0]);
    rnode_redis->keys[0] = redis_right->max();
    delete redis_right;
    this->size--;

    this->keys.pop_back();
    this->tree_pointers.pop_back();
    this->dump();
    rnode_redis->dump();
}


void InternalNode::redis_r(TreeNode* node) {
    InternalNode* lnode_redis = dynamic_cast<InternalNode*>(node);

    auto redis_left = TreeNode::tree_node_factory(lnode_redis->tree_pointers.back());

    lnode_redis->keys.push_back(redis_left->max());
    lnode_redis->tree_pointers.push_back(this->tree_pointers[0]);
    delete redis_left;
    lnode_redis->size++;
    this->size--;

    int var10 = this->size;

    for(int k=0;k<var10;k++){
        this->tree_pointers[k]=this->tree_pointers[k+1];
    }

    int var11 = this->size - 1;
     
    for(int k=0;k<var11;k++){
        this->keys[k] = this->keys[k+1];
    }

    this->keys.pop_back();
    this->tree_pointers.pop_back();
    this->dump();
    lnode_redis->dump();
}



void InternalNode::merge_fun(TreeNode* node) {
    InternalNode* merge = dynamic_cast<InternalNode*>(node);
    auto merge_node = TreeNode::tree_node_factory(this->tree_pointers.back());
    this->keys.push_back(merge_node->max());
    delete merge_node;

    int var = merge->size;

    for(int k=0;k<var;k++){
        this->tree_pointers.push_back(merge->tree_pointers[k]);
    }
    
    int var2 = merge->size - 1;

    for(int k=0;k<var2;k++){
        this->keys.push_back(merge->keys[k]);
    }

    this->size += merge->size;
    merge->delete_node();
    this->dump();
}


void InternalNode::delete_key(const Key &key) {
    TreePtr new_tree_ptr = NULL_PTR;
    int dummy = 0;
    while(this->keys[dummy]<key && this->size>dummy+1 && DELETE_MARKER != this->keys[dummy]){
        dummy++;
    }

    auto delnode = TreeNode::tree_node_factory(this->tree_pointers[dummy]);
    delnode->delete_key(key);
    
    int var = this->size -1;

    if(var>dummy){
        this->keys[dummy] = delnode->max();
    }
        
    if(!delnode->underflows()){
        delete delnode;
        this->dump();
        return;
    }

    if(0<dummy){
        auto deletenode_l = TreeNode::tree_node_factory(this->tree_pointers[dummy-1]);

        if(MIN_OCCUPANCY * 2 <= delnode->size + deletenode_l->size) {
            deletenode_l->redis_l(delnode);
            this->keys[dummy-1] = deletenode_l->max();
        }

        else {
            deletenode_l->merge_fun(delnode);
            int var3 = this->size - 1;
            for(int k=dummy;k<var3;k++){
                this->tree_pointers[k] = this->tree_pointers[k+1];
            }
            int var5 = dummy-1;
            int var6 = this->size - 2;

            for(int k=var5;k<var6;k++){
                this->keys[k] = this->keys[k+1];
            }
            this->tree_pointers.pop_back();
            this->keys.pop_back();
            this->size--;
        }
        delete deletenode_l;
    }

    else{
        auto deletenode_r = TreeNode::tree_node_factory(this->tree_pointers[dummy+1]);

        if(MIN_OCCUPANCY*2 <= delnode->size+deletenode_r->size) {
            deletenode_r->redis_r(delnode);
            this->keys[dummy] = delnode->max();
        }

        else {
            delnode->merge_fun(deletenode_r);

            int var12 = dummy+1;
            int var13 = this->size - 1;

            for(int k=var12;k<var13;k++){
                this->tree_pointers[k] = this->tree_pointers[k+1];
            }

            int var14 = this->size - 2;
            
            for(int k=dummy;k<var14;k++){
                this->keys[k] = this->keys[k+1];
            }
        
            this->tree_pointers.pop_back();
            this->keys.pop_back();
            this->size--;
        }
        delete deletenode_r;
    }

    delete delnode;
    this->dump();
}

//runs range query on subtree rooted at this node
void InternalNode::range(ostream &os, const Key &min_key, const Key &max_key) const {
    BLOCK_ACCESSES++;
    for (int i = 0; i < this->size - 1; i++) {
        if (min_key <= this->keys[i]) {
            auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
            child_node->range(os, min_key, max_key);
            delete child_node;
            return;
        }
    }
    auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    child_node->range(os, min_key, max_key);
    delete child_node;
}

//exports node - used for grading
void InternalNode::export_node(ostream &os) {
    TreeNode::export_node(os);
    for (int i = 0; i < this->size - 1; i++)
        os << this->keys[i] << " ";
    os << endl;
    for (int i = 0; i < this->size; i++) {
        auto child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        child_node->export_node(os);
        delete child_node;
    }
}

//writes subtree rooted at this node as a mermaid chart
void InternalNode::chart(ostream &os) {
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    chart_node += "]";
    os << chart_node << endl;

    for (int i = 0; i < this->size; i++) {
        auto tree_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        tree_node->chart(os);
        delete tree_node;
        string link = this->tree_ptr + "-->|";

        if (i == 0)
            link += "x <= " + to_string(this->keys[i]);
        else if (i == this->size - 1) {
            link += to_string(this->keys[i - 1]) + " < x";
        } else {
            link += to_string(this->keys[i - 1]) + " < x <= " + to_string(this->keys[i]);
        }
        link += "|" + this->tree_pointers[i];
        os << link << endl;
    }
}

ostream& InternalNode::write(ostream &os) const {
    TreeNode::write(os);
    for (int i = 0; i < this->size - 1; i++) {
        if (&os == &cout)
            os << "\nP" << i + 1 << ": ";
        os << this->tree_pointers[i] << " ";
        if (&os == &cout)
            os << "\nK" << i + 1 << ": ";
        os << this->keys[i] << " ";
    }
    if (&os == &cout)
        os << "\nP" << this->size << ": ";
    os << this->tree_pointers[this->size - 1];
    return os;
}

istream& InternalNode::read(istream& is) {
    TreeNode::read(is);
    this->keys.assign(this->size - 1, DELETE_MARKER);
    this->tree_pointers.assign(this->size, NULL_PTR);
    for (int i = 0; i < this->size - 1; i++) {
        if (&is == &cin)
            cout << "P" << i + 1 << ": ";
        is >> this->tree_pointers[i];
        if (&is == &cin)
            cout << "K" << i + 1 << ": ";
        is >> this->keys[i];
    }
    if (&is == &cin)
        cout << "P" << this->size;
    is >> this->tree_pointers[this->size - 1];
    return is;
}
