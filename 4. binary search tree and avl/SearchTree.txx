  template <typename E>         // constructor
  SearchTree<E>::SearchTree() : T(), n(0)
    { T.addRoot(); T.expandExternal(T.root()); }  // create the super root

  template <typename E>         // deconstructor
  SearchTree<E>::~SearchTree()
    { }  // create the super root
  
  template <typename E>         // get virtual root
  typename SearchTree<E>::TPos SearchTree<E>::root() const
    { return T.root().left(); }       // left child of super root
  
   template <typename E>          // iterator to first entry
  typename SearchTree<E>::Iterator SearchTree<E>::begin() {
    TPos v = root();          // start at virtual root
    while (v.isInternal()) v = v.left();    // find leftmost node
    return Iterator(v.parent());
  }
  
  template <typename E>         // iterator to end entry
  typename SearchTree<E>::Iterator SearchTree<E>::end()
    { return Iterator(T.root()); }      // return the super root
    
  template <typename E>         // remove key k entry
  void SearchTree<E>::erase(const K& k) { //throw(NonexistentElement) {
    TPos v = finder(k, root());       // search from virtual root
    if (v.isExternal())         // not found?
      return;//NonexistentElement("Erase of nonexistent");
    eraser(v);            // remove it
  }
  
  template <typename E>         // erase entry at p
  void SearchTree<E>::erase(const Iterator& p)
    { eraser(p.v); }
    
  template <typename E>         // find entry with key k
  typename SearchTree<E>::Iterator SearchTree<E>::find(const K& k) {
    TPos v = finder(k, root());       // search from virtual root
    if (v.isInternal()) return Iterator(v);   // found it
    else return end();          // didn't find it
  }  
  
  template <typename E>         // insert (k,x)
  typename SearchTree<E>::Iterator SearchTree<E>::insert(const K& k, const V& x)
    { TPos v = inserter(k, x); return Iterator(v); }  
 
  template <typename E>
  int SearchTree<E>::size() const { return n; }  
    
    
//
// ToDo
//    
 
  template <typename E>         // inorder successor
  typename SearchTree<E>::Iterator& SearchTree<E>::Iterator::operator++() {
   
    TPos w = v.right();
    if (w.isInternal()) { // have right subtree?
        do { v = w; w = w.left(); } // move down left chain
        while (w.isInternal());
    }
    else {
        w = v.parent(); // get parent
        while (v == w.right()) // move up right chain
            { v = w; w = w.parent(); }
        v = w; // and first link to left
    }
    return *this;
   
  }
     
    
    
  template <typename E>         // remove utility
  typename SearchTree<E>::TPos SearchTree<E>::eraser(TPos& v) {
       
    TPos w;
    if (v.left().isExternal()) w = v.left(); // remove from left
    else if (v.right().isExternal()) w = v.right(); // remove from right
    else { // both internal?
        w = v.right(); // go to right subtree
        do { w = w.left(); } while (w.isInternal()); // get leftmost node
        TPos u = w.parent();
        (*v).setKey((*u).key()); (*v).setValue((*u).value()); // copy w’s parent to v
    }
    n--; // one less entry
    return T.removeAboveExternal(w); 
    
  }
      
  template <typename E>         // find utility
  typename SearchTree<E>::TPos SearchTree<E>::finder(const K& k, const TPos& v) {
   
    if (v.isExternal()) return v; // key not found
    if (k < (*v).key()) return finder(k, v.left()); // search left subtree
    else if ((*v).key() < k) return finder(k, v.right()); // search right subtree
    else return v;
    
  }
  
  template <typename E>         // insert utility
  typename SearchTree<E>::TPos SearchTree<E>::inserter(const K& k, const V& x) {
  
    TPos v = finder(k, root()); // search from virtual root
    while (v.isInternal()) // key already exists?
        v = finder(k, v.right()); // look further
    T.expandExternal(v); // add new internal node
    (*v).setKey(k); (*v).setValue(x); // set entry
    n++; // one more entry

    return v;
    
  }
  