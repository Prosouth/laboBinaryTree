//
//  Binary Search Tree
//
//  Copyright (c) 2017 Olivier Cuisenaire. All rights reserved.
//
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <queue>
#include <cassert>
#include <stdexcept>

using namespace std;

template <typename T>
class BinarySearchTree 
{
public:
  
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

private:
  /**
   *  @brief Noeud de l'arbre.
   *
   * contient une cle et les liens vers les sous-arbres droit et gauche.
   */
  struct Node 
  {
    const value_type key; // clé non modifiable
    Node* right;          // sous arbre avec des cles plus grandes
    Node* left;           // sous arbre avec des cles plus petites
    size_t nbElements;    // nombre de noeuds dans le sous arbre dont
                          // ce noeud est la racine
    
    Node(const_reference key)  // seul constructeur disponible. key est obligatoire
    : key(key), right(nullptr), left(nullptr), nbElements(1)
    {
      cout << "(C" << key << ") ";
    }
    ~Node()               // destructeur
    {
      cout << "(D" << key << ") ";
    }
    Node() = delete;             // pas de construction par défaut
    Node(const Node&) = delete;  // pas de construction par copie
    Node(Node&&) = delete;       // pas de construction par déplacement
  };
  
  /**
   *  @brief  Racine de l'arbre. nullptr si l'arbre est vide
   */
  Node* _root;
  
public:
  /**
   *  @brief Constructeur par défaut. Construit un arbre vide
   *  @remark Complexité O(1)
   */
  BinarySearchTree() : _root(nullptr)
  { }
  
  /**
   *  @brief Constucteur de copie.
   *
   *  @param other le BinarySearchTree à copier
   *  @remark Complexité O(1)
   *
   */
  BinarySearchTree(BinarySearchTree& other) 
  {
      try
      {
        _root = nullptr;
        copyNodes(_root, other._root);  
      }
      catch(...)
      {
        other.deleteSubTree(_root);
        throw;
      }
  }
  
  /**
   * 
   * @brief Fonction qui permet de copier tous les éléments d'un ABR
   * 
   * 
   * @param r
   * @param nodeToCopy
   * 
   * @remark Complexité 0(1)
   */
  void copyNodes(Node *& r, Node *nodeToCopy)
  {
        if (nodeToCopy) 
        {
            r = new Node(nodeToCopy->key);
            r->nbElements = nodeToCopy->nbElements;

            copyNodes(r->left, nodeToCopy->left);
            copyNodes(r->right, nodeToCopy->right);
        }
    }

  
  /**
   *  @brief Opérateur d'affectation par copie.
   *
   *  @param other le BinarySearchTree à copier
   *  @remark Compexité en moyenne en O(1)
   */
  BinarySearchTree& operator=(const BinarySearchTree& other) 
  {
        Node* tmp;
        try 
        {

            copyNodes(tmp, other._root);
            deleteSubTree(_root);
            _root = tmp;
        } 
        catch (...) 
        {
            deleteSubTree(tmp);
            throw;
        }
        return *this;
  }
  
  /**
   *  @brief Echange le contenu avec un autre BST
   *
   *  @param other le BST avec lequel on echange le contenu
   *  @remark Compexité en moyenne en O(1)
   */
  void swap(BinarySearchTree& other) noexcept 
  {
      Node* tmp = other._root;
      other._root = _root;
      _root = tmp;
  }
  
  /**
   *  @brief constructeur de copie par déplacement
   *
   *  @param other le BST dont on vole le contenu
    @remark Compexité en O(1)
   *
   */
  BinarySearchTree(BinarySearchTree&& other) noexcept 
  {
      _root = other._root;
      other._root = nullptr;
  }
  
  /**
   *  @brief Opérateur d'affectation par déplacement.
   *
   *  @param other le BST dont on vole le contenu
   * @remark Compexité en moyenne en O(n)
   */
  BinarySearchTree& operator= ( BinarySearchTree&& other ) noexcept 
{
        Node* tmp = _root;
        _root = other._root;
        deleteSubTree(tmp);
        other._root = nullptr;
        return *this;
  }
  
  //
  // @brief Destructeur
  // @remark Compexité en moyenne en O(n)
  // Ne pas modifier mais écrire la fonction
  // récursive privée deleteSubTree(Node*)
  //
  ~BinarySearchTree() 
  {
    deleteSubTree( _root );
  }
  
private:
  //
  // @brief Fonction détruisant (delete) un sous arbre
  //
  // @param r la racine du sous arbre à détruire.
  //          peut éventuellement valoir nullptr
  // @remark Compexité en moyenne en O(n)
  static void deleteSubTree(Node* r) noexcept 
  {
      if(r)
      {
          deleteSubTree(r->left);
          deleteSubTree(r->right);
          delete r;
          r->nbElements--;          
      }
  }

public:
  //
  // @brief Insertion d'une cle dans l'arbre
  //
  // @param key la clé à insérer.
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée insert(Node*&,const_reference)
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  void insert( const_reference key) {
    insert(_root,key);
  }
  
private:
  //
  // @brief Insertion d'une cle dans un sous-arbre
  //
  // @param r la racine du sous-arbre dans lequel
  //          insérer la cle.
  // @param key la clé à insérer.
  //
  // @return vrai si la cle est inseree. faux si elle etait deja presente.
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  // Si la cle est deja presente, cette fonction ne fait rien.
  // x peut éventuellement valoir nullptr en entrée.
  // la fonction peut modifier x, reçu par référence, si nécessaire
  //
  static bool insert(Node*& r, const_reference key) 
  {
    if (!r)
    {
        r = new Node(key); 
        return true;
    }
    else if (key < r->key)
    {    
        if(!insert(r->left, key))
        {
            return false;
        }
    }
    else if (key > r->key)
    {
        if(!insert(r->right, key))
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    r->nbElements++;
    return true;
  }
  
public:
  //
  // @brief Recherche d'une cle.
  //
  // @param key la cle a rechercher
  //
  // @return vrai si la cle trouvee, faux sinon.
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée contains(Node*,const_reference)
  //
  bool contains( const_reference key ) const noexcept 
  {
    return contains(_root,key);
  }
  
private:
  //
  // @brief Recherche d'une cle dans un sous-arbre
  //
  // @param key la cle a rechercher
  // @param r   la racine du sous-arbre
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  // @return vrai si la cle trouvee, faux sinon.
  //
  static bool contains(Node* r, const_reference key) noexcept 
  {
      if (!r)
      {
          return false;
      }
      else if (key < r->key)
      {
          contains(r->left, key);
      }
      else if (key > r->key)
      {
          contains(r->right, key);
      }
      else
      {
          return true;
      }
  }
  
public:
  //
  // @brief Recherche de la cle minimale.
  //
  // @return une const reference a la cle minimale
  //
  // @exception std::logic_error si necessaire
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
  //
  const_reference min() const 
  {
    if (_root == nullptr)
    {
        throw std::logic_error("logic_error_min");
    }

    Node* tmpNode = _root;

    while (tmpNode->left != nullptr)
    {
        tmpNode = tmpNode->left;
    }

    return tmpNode->key;
  }

  //
  // @brief Supprime le plus petit element de l'arbre.
  //
  // @exception std::logic_error si necessaire
  //
  // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
  // @remark Compexité en moyenne en O(log(n))
  void deleteMin() 
  {
      delete removeMinAndReturnIt(_root);
  }
  
  static Node* removeMinAndReturnIt(Node* leaf)
  {
      if (leaf == nullptr) 
      {
         throw logic_error("std::Logic_error");
      }

      Node *cur = leaf;
      cur->nbElements--;
      if (!cur->left) 
      {
          cur = cur->right;
          return cur;
      }
      while (cur->left->left) 
      {
          cur = cur->left;
          cur->nbElements--;
      }
      Node* cur_left = cur->left;
      
      if (cur_left->right) 
      {
          cur->left = cur_left->right;
      } 
      else 
      {
          cur->left = nullptr;         
      }
      
      return cur_left;
   }
  
  
  //
  // @brief Supprime l'element de cle key de l'arbre.
  //
  // @param key l'element a supprimer
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  // si l'element n'est pas present, la fonction ne modifie pas
  // l'arbre mais retourne false. Si l'element est present, elle
  // retourne vrai
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée deleteElement(Node*&,const_reference)
  //
  bool deleteElement( const_reference key) noexcept 
  {
    return deleteElement( _root, key );
  }
  
private:
  //
  // @brief Supprime l'element de cle key du sous arbre.
  //
  // @param r la racine du sous arbre
  // @param key l'element a supprimer
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  // si l'element n'est pas present, la fonction ne modifie pas
  // l'arbre mais retourne false. Si l'element est present, elle
  // retourne vrai
    static bool deleteElement(Node*& r, const_reference key) noexcept
    {
        if (r) 
        {
            if (key < r->key) // key recherchée inférieure au noeud actuel, on va
            { // chercher à gauche
                if (deleteElement(r->left, key)) // si on trouve la clé
                {
                    r->nbElements--;
                    return true;
                } 
                else 
                {
                    return false;
                }
            } 
            else if (key > r->key) // key recherchée supérieure au noeud actuel, on
            { // va vers la droite
                if (deleteElement(r->right, key)) // si on a trouvé la clé
                {
                    r->nbElements--;
                    return true;
                } 
                else 
                {
                    return false;
                }
            } 
            else 
            { 
                if (!r->right) // on a la key
                {
                    delete r;
                    r = r->left;
                } 
                else if (!r->left) 
                {
                    delete r;
                    r = r->right;
                } 
                else // algo de suppression de Hibbard
                {
                    Node *tmp = r;
                    r = removeMinAndReturnIt(r->right);
                    r->nbElements = tmp->nbElements - 1;
                    r->left = tmp->left;
                    r->right = tmp->right;
                    delete tmp;
                }
                return true;
            }
        } 
        else 
        { 
            return false;
        }
    }
    
public:
  //
  // @brief taille de l'arbre
  //
  // @return le nombre d'elements de l'arbre
  // @remark Compexité en moyenne en O(1)
  size_t size() const noexcept 
  {
      return _root->nbElements;
  }
  
  static size_t size(Node* r) noexcept 
  { 
      return r ? r->nbElements : 0;
  }
  //
  // @brief cle en position n
  //
  // @return une reference a la cle en position n par ordre croissant des
  // elements
  //
  // @exception std::logic_error si nécessaire
  //
  // ajoutez le code de gestion des exceptions, puis mettez en oeuvre
  // la fonction recursive nth_element(Node*, n)
  // @remark Compexité en O(n)
  const_reference nth_element(size_t n) const 
  {
      if(n > size(_root)) 
      {
          throw std::logic_error("logic_error_nth_element");
      }
      return nth_element(_root,n);
  }
  
private:
  //
  // @brief cle en position n dans un sous arbre
  //
  // @param r la racine du sous arbre. ne peut pas etre nullptr
  // @param n la position n
  //
  // @return une reference a la cle en position n par ordre croissant des
  // elements
  // @remark Compexité en O(n)
  static const_reference nth_element(Node* r, size_t n) noexcept 
  {
      if (r) 
      {
          size_t s = 0;
          if (r->left) 
          {
              s = r->left->nbElements;
          }
          if (n < s) 
          {
              return nth_element(r->left, n);
          } 
          else if (n > s) 
          {
              return nth_element(r->right, n - s - 1);
          } 
          else 
          {
              return r->key;
          }
      }
      return -1;
  }
  
public:
  //
  // @brief position d'une cle dans l'ordre croissant des elements de l'arbre
  //
  // @param key la cle dont on cherche le rang
  //
  // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée rank(Node*,const_reference)
  // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
  size_t rank(const_reference key) const noexcept 
  {
        return rank(_root, key);
  }
  
private:
  //
    // @brief position d'une cle dans l'ordre croissant des elements du sous-arbre
    //
    // @param key la cle dont on cherche le rang
    // @param r la racine du sous arbre
    //
    // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
    // @remark Complexité en O(h) où h est la hauteur de l’arbre, en moyenne O(log(n)), au pire O(n) si l’arbre est dégénéré
    static size_t rank(Node* r, const_reference key) noexcept 
    {
        size_t nbElementCmp;
        if (r) 
        {
            size_t s = 0;
            if (key < r->key) 
            {
                s = rank(r->left, key);
                if (s != -1) 
                {
                    return s;
                }
            }
            else if (key > r->key) 
            {
                s = rank(r->right, key);
                if (s != -1) 
                {
                    if (!r->left) 
                    {
                        nbElementCmp = 0;
                    }
                    else 
                    {
                        nbElementCmp = r->left->nbElements;
                    }
                    return s + nbElementCmp + 1;
                }
            }
            else 
            {
                if (!r->left) 
                {
                    nbElementCmp = 0;
                }
                else 
                {
                    nbElementCmp = r->left->nbElements;
                }
                return nbElementCmp;
            }
        } 
        else 
        {
            return -1;
        }
        return -1;
    }
    
public:
  //
  // @brief linearise l'arbre
  //
  // transforme l'arbre en une liste simplement chainee composee des memes
  // noeuds que precedemment, mais dont les pointeurs left sont tous egaux
  // a nullptr. Cette liste doit toujours respecter les conditions d'un
  // arbre binaire de recherche
  //
  // Ne pas modifier cette fonction qui sert essentiellement a tester la
  // fonction recursive linearize(Node*, Node*&, size_t&) utilisée par
  // la methode publique arborize
  // @remark Complexité en O(n)
  void linearize() noexcept 
  {
    size_t cnt = 0;
    Node* list = nullptr;
    linearize(_root,list,cnt);
    _root = list;
  }
  
private:
  //
  // @brief linearise une sous arbre tree en une liste
  //
  // @param tree pointe vers la racine de l'arbre a lineariser
  // @param list reference a la tete de la liste a creer. sera modifiee
  //             par la fonction pour que list pointe vers le plus petit
  //             element de tree
  // @param cnt  calcule au fure et a mesure le nombre d'elements de la liste
  //             cree. l'effet de la fonction doit etre d'ajouter le nombre
  //             d'elements du sous-arbre de racine tree. Cependant, vous
  //             avez uniquement le droit d'utiliser l'opérateur ++.
  // @remark Compexité en O(n)
  static void linearize(Node* tree, Node*& list, size_t& cnt) noexcept 
  {
      // Parcours symétrique inverse pour les mettre dans une liste
      // sans oublier de mettre l'élément de gauche à nul sous peine
      // de créer une boucle infinie
      if(tree)
      {
          linearize(tree->right, list, cnt); // on va à l'élément plus à droite
          tree->right = list; // sauve la liste dans l'élément suivant
          list = tree; // affecte l'arbre courant à la liste
          cnt++; 
          list->nbElements = cnt; // nbElement s'incrémente vu qu'on vient de relier un nouveau noeud
          linearize(tree->left, list, cnt);
          tree->left = nullptr; // on détache à gauche
      }
  }
  
  
public:
  //
  // @brief equilibre l'arbre
  //
  // applique l'algorithme d'equilibrage de l'arbre par linearisation et
  // arborisation
  //
  // Ne pas modifier cette fonction.
  // @remark Complexité O(n)
  void balance() noexcept 
  {
    size_t cnt = 0;
    Node* list = nullptr;
    linearize(_root,list,cnt);
    arborize(_root,list,cnt);
  }
  
private:
  //
  // @brief arborise les cnt premiers elements d'une liste en un arbre
  //
  // @param tree reference dans laquelle il faut ecrire la racine de l'arbre
  //             arborise par la fonction
  // @param list IN - reference a la tete de la liste a parcourir. La liste
  //                  est composee de Node dont le pointer left est nullptr
  //             OUT - debut de la suite de la liste dont on a utilise cnt
  //                   elements
  // @param cnt  nombre d'elements de la liste que l'on doit utiliser pour
  //             arboriser le sous arbre
  // @remark Complexité O(n)
    static void arborize(Node *& tree, Node *& list, size_t cnt) noexcept 
    {
        Node *subTreeR;
        Node *subTreeL; 
        if (cnt > 0)
        {
            size_t cntL = (cnt - 1) / 2; // pour compteur pour le s-a gauche
            size_t cntR = cnt - cntL - 1; // pour compteur pour le s-a droite
            arborize(subTreeL, list, cntL); 
            tree = list; 
            list = list->right;
            arborize(subTreeR, list, cntR); 
            tree->nbElements = cntL + cntR + 1; // + 2 à cause du --cnt
            tree->right = subTreeR;
            tree->left = subTreeL; 
        } 
        else 
        {
            tree = nullptr;
        }
    }
  
public:
  //
  // @brief Parcours pre-ordonne de l'arbre
  //
  // @param f une fonction capable d'être appelée en recevant une cle
  //          en parametre. Pour le noeud n courrant, l'appel sera
  //          f(n->key);
  // @remark Compexité en moyenne en O(n)
  template <typename Fn>
  void visitPre (Fn f) 
  {
      if(_root != nullptr)
      {
          parcoursPreOrdonne(_root, f);
      }
  }
  
  template<typename Fn>
  void parcoursPreOrdonne(Node *leaf, Fn f)
  {
      f(leaf->key);

      if(leaf->left != nullptr)
      {
          parcoursPreOrdonne(leaf->left, f);
      }
      
      if(leaf->right != nullptr)
      {
          parcoursPreOrdonne(leaf->right, f);
      }
  }
  
  //
  // @brief Parcours symétrique de l'arbre
  //
  // @param f une fonction capable d'être appelée en recevant une cle
  //          en parametre. Pour le noeud n courrant, l'appel sera
  //          f(n->key);
  // @remark Compexité en moyenne en O(n)
  template <typename Fn>
  void visitSym (Fn f) 
  {
      if(_root != nullptr)
      {
          parcoursSymetrique(_root, f);
      }
  }
  
  template <typename Fn>
  void parcoursSymetrique(Node *leaf, Fn f)
  {
      if(leaf->left != nullptr)
      {
          parcoursSymetrique(leaf->left, f);
      }
     
      f(leaf->key);
      
      if(leaf->right != nullptr)
      {
          parcoursSymetrique(leaf->right, f);
      }
  }
  
  //
  // @brief Parcours post-ordonne de l'arbre
  //
  // @param f une fonction capable d'être appelée en recevant une cle
  //          en parametre. Pour le noeud n courrant, l'appel sera
  //          f(n->key);
  // @remark Compexité en moyenne en O(n)
  template < typename Fn >
  void visitPost (Fn f) 
  {
      if(_root != nullptr)
      {
          parcoursPostOrdonne(_root, f);
      }
  }
  
  template<typename Fn>
  void parcoursPostOrdonne(Node *leaf, Fn f)
  {
      if(leaf->left != nullptr)
      {
          parcoursPostOrdonne(leaf->left, f);
      }
      
      if(leaf->right != nullptr)
      {
          parcoursPostOrdonne(leaf->right, f);
      }
      
      f(leaf->key);
  }
  
  
  //
  // Les fonctions suivantes sont fournies pour permettre de tester votre classe
  // Merci de ne rien modifier au dela de cette ligne
  //
  void display() const 
  {
    stringstream ss1, ss2, ss3;
    displayKeys(ss1);
    displayKeys(ss3);
    displayCounts(ss2);
    string l1, l2;
    
    size_t W = 11;
    while( getline(ss3,l1))
      if (l1.length() > W) W = l1.length();

    displayKeys(ss1);
    
    cout << "\n";
    cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
    cout << "| "<< left << setw(W) << "key" << "| " << setw(W) << "nbElements" << "|" << endl;
    cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
    while( getline(ss1,l1) and getline(ss2, l2) ) {
      cout << "| "<< left << setw(W) << l1 << "| " << setw(W) << l2 << "|" << endl;
    }
    cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
  }
  
  void displayKeys(ostream& os = cout) const 
  {
    display([](Node* n) -> const_reference { return n->key; }, os);
  }
  
  void displayCounts(ostream& os = cout) const 
  {
    display([](Node* n) -> size_t { return n->nbElements; }, os);
  }
  
  //
  // utilise un parcours de l'arbre en largeur avec
  // un noeud sentinelle newLevel qui traque les
  // changements de niveaux
  //
  template <typename Fn>
  void display (Fn func, ostream& os = cout ) const 
  {
    Node* newLevel = (Node*) -1;
    // addresse non nulle dont on est sur qu'elle ne contient pas
    // vraiment un Node. Utilisée comme sentinelle.
    
    queue<Node*> Q;
    Q.push(_root);
    Q.push(newLevel);
    
    while(!Q.empty()) {
      Node* cur = Q.front();
      Q.pop();
      
      if(cur == newLevel) {
        os << endl;
        if(!Q.empty())
          Q.push(newLevel);
      } else if(cur == nullptr) {
        os << "- ";
      } else {
        os << func(cur) << " ";
        Q.push(cur->left);
        Q.push(cur->right);
      }
    }
  }
};

int main()
{
    return EXIT_SUCCESS;
}