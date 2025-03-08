#include "RecordPtr.hpp"
#include "LeafNode.hpp"

LeafNode::LeafNode(const TreePtr &tree_ptr) : TreeNode(LEAF, tree_ptr) {
    this->data_pointers.clear();
    this->next_leaf_ptr = NULL_PTR;
    if(!is_null(tree_ptr))
        this->load();
}

//returns max key within this leaf
Key LeafNode::max() {
    auto it = this->data_pointers.rbegin();
    return it->first;
}



//inserts <key, record_ptr> to leaf. If overflow occurs, leaf is split
//split node is returned

// INSERT <key> inserts key into heap and B+-Tree

TreePtr LeafNode::insert_key(const Key &key, const RecordPtr &record_ptr) {

    TreePtr new_leaf = NULL_PTR; //if leaf is split, new_leaf = ptr to new split node ptr
    LeafNode* node_leaf = dynamic_cast<LeafNode*>(TreeNode::tree_node_factory(LEAF));
    
    if(this->data_pointers.count(key))
        return new_leaf;
    
    this->data_pointers[key]=record_ptr;
    this->size = this->size + 1;
    
    if(!this->overflows()){
        this->dump();
        return new_leaf;
    }
               
    new_leaf = node_leaf->tree_ptr;
    node_leaf->next_leaf_ptr = this->next_leaf_ptr;
    this->size = MIN_OCCUPANCY;
    this->next_leaf_ptr = new_leaf;
    
    auto ins = this->data_pointers.begin();

    for(int k=0;k<this->size;k++){
        ins++;
    }

    while(ins != this->data_pointers.end()){
        node_leaf->data_pointers[(*ins).first] = (*ins).second;
        node_leaf->size++;
        ins = this->data_pointers.erase(ins);
    }

    node_leaf->dump();
    this->dump();
    delete node_leaf;
    return new_leaf;
}


//key is deleted from leaf if exists
/*
Handling Underflow - 
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
void LeafNode::redis_l(TreeNode* node) {
    auto left_redis = this->data_pointers.rbegin();
    node->insert_key(left_redis->first,left_redis->second);
    this->delete_key(left_redis->first);
}

void LeafNode::redis_r(TreeNode* node) {
    auto right_redis = this->data_pointers.begin();
    node->insert_key(right_redis->first,right_redis->second);
    this->delete_key(right_redis->first);
}

void LeafNode::merge_fun(TreeNode* node) {
    LeafNode* merge = dynamic_cast<LeafNode*>(node);
    this->data_pointers.insert(merge->data_pointers.begin(),merge->data_pointers.end());
    this->next_leaf_ptr = merge->next_leaf_ptr;
    this->size += merge->size;
    merge->delete_node();
    this->dump();
}


void LeafNode::delete_key(const Key &key){
    if(this->data_pointers.end() != this->data_pointers.find(key)){
        this->data_pointers.erase(key);
        this->size--;
        this->dump();
    }
}

//runs range query on leaf
void LeafNode::range(ostream &os, const Key &min_key, const Key &max_key) const {
    BLOCK_ACCESSES++;
    for(const auto& data_pointer : this->data_pointers){
        if(data_pointer.first >= min_key && data_pointer.first <= max_key)
            data_pointer.second.write_data(os);
        if(data_pointer.first > max_key)
            return;
    }
    if(!is_null(this->next_leaf_ptr)){
        auto next_leaf_node = new LeafNode(this->next_leaf_ptr);
        next_leaf_node->range(os, min_key, max_key);
        delete next_leaf_node;
    }
}

//exports node - used for grading
void LeafNode::export_node(ostream &os) {
    TreeNode::export_node(os);
    for(const auto& data_pointer : this->data_pointers){
        os << data_pointer.first << " ";
    }
    os << endl;
}

//writes leaf as a mermaid chart
void LeafNode::chart(ostream &os) {
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    for(const auto& data_pointer: this->data_pointers) {
        chart_node += to_string(data_pointer.first) + " ";
    }
    chart_node += "]";
    os << chart_node << endl;
}

ostream& LeafNode::write(ostream &os) const {
    TreeNode::write(os);
    for(const auto & data_pointer : this->data_pointers){
        if(&os == &cout)
            os << "\n" << data_pointer.first << ": ";
        else
            os << "\n" << data_pointer.first << " ";
        os << data_pointer.second;
    }
    os << endl;
    os << this->next_leaf_ptr << endl;
    return os;
}

istream& LeafNode::read(istream& is){
    TreeNode::read(is);
    this->data_pointers.clear();
    for(int i = 0; i < this->size; i++){
        Key key = DELETE_MARKER;
        RecordPtr record_ptr;
        if(&is == &cin)
            cout << "K: ";
        is >> key;
        if(&is == &cin)
            cout << "P: ";
        is >> record_ptr;
        this->data_pointers.insert(pair<Key,RecordPtr>(key, record_ptr));
    }
    is >> this->next_leaf_ptr;
    return is;
}