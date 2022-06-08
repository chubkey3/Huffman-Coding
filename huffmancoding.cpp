#include <iostream>
#include <map>
#include <string>
#include <bitset>
#include <queue>

using namespace std;

struct node {
    node(string, int, node*, node*, node*);
    string s;
    int c;
    node* next;
    node* left;
    node* right;

};

node::node(string k, int m, node* n = 0, node* l = 0, node* r = 0){
    s = k;
    c = m;
    next = n;
    left = l;
    right = r;
}

int len(node* nd){
    if (nd){
        return 1 + len(nd->next);
    }
    return 0;
}

void llinsert(node* &nd, string k, int l){ 
    if (nd == 0){
        nd = new node(k, l);
    } else {
        
        if (nd->c < l){
            nd = new node(k, l, nd);
        }
        else if (nd->c == l){
            if (nd->s > k){
                nd = new node(k, l, nd);
            } else {
                llinsert(nd->next, k, l);
            }
        }
        else {
            llinsert(nd->next, k, l);
        }
    
    }
    
}

void llinsert2(node* &nd, node* &k){
    if (nd == 0){
        nd = k;
    } else {
        
        if (nd->c < k->c){
            k->next = nd;
            nd = k;
        }
        else if (nd->c == k->c){
            if (nd->s > k->s){
                k->next = nd;
                nd = k;
            } else {
                llinsert2(nd->next, k);
            }
        }
        else {
            llinsert2(nd->next, k);
        }
    
    }
    
}


void combine(node* &nd, node* &root){
    node* n;
    if ((nd->next)->next == 0){
        
        n = new node((nd->next)->s + nd->s, (nd->c)+((nd->next)->c), 0, nd->next, nd); 
        nd = 0;
        llinsert2(root, n);
        
        
    } else {
        combine(nd->next, root);
    }
}

	
void btfree(node* root){ //which order should deleting be: (pre, in, post ) only one of them will work
    
    if (root){
        btfree(root->left);
        btfree(root->right);
        delete root;
    }
}

string search(node* &nd, char k){
    if ((nd->s).size() != 1){
        if (((nd->left)->s).find(k) != string::npos){
            return "0" + search(nd->left, k);
        } else {
            return "1" + search(nd->right, k);
        }   
    }
    
    return "";
}

void decode(node* &nd, uint index, string in, node* r, uint max){
    if (index == max+1){
        cout << endl;
        return;
    }
    
    if (in[index] == '0'){
        
        if (nd->left == 0){
            cout << nd->s;
            decode(r, index, in, r, max);
        } else {
            decode(nd->left, index + 1, in, r, max);
        }
        
    } else if(in[index] == '1') {
        
        if (nd->right == 0){
            cout << nd->s;
            decode(r, index, in, r, max);
        } else {
            decode(nd->right, index + 1, in, r, max);
        }
    }

}


int main(int argc, char* argv[]){
    string s;
    string e;
    string o = "";
    string temp; //bad
    map<char, int> occ;
    map<char, string> table;
    uint i;
    uint padding = 0;
    
    node* root = 0;
    
    if (argv[1]){
        s = argv[1];
    } else {
        s = "mississippi river";
    }
    
    for (i = 0; i<s.size(); i++){
        if (occ[s[i]]){
            occ[s[i]]++;
        } else {
            occ[s[i]] = 1;
        }
    }

    for (auto it = occ.begin(); it != occ.end(); it++){
        temp = it->first;
        llinsert(root, temp, it->second);
    }
    
    
    int length = len(root);
    
    for (int i = 1; i<length; i++){
        combine(root, root);
        
    }
    
    for (auto it = occ.begin(); it != occ.end(); it++){
        table[it->first] = search(root, it->first);
    }
    
    for (i = 0; i<s.size(); i++){
        e = e + table[s[i]];
    }
    
    
    while (e.size()%8 != 0){
        e += "0";
        padding++;
    }
    
    e += bitset<8>(padding + '0').to_string();
    
    cout << e << endl;;
    
    decode(root, 0, e, root, e.size()-8-padding);
    
    btfree(root);
    
    return 0;
}
