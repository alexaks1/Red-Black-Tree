#include <iostream>
using namespace std;

#define BLACK 0
#define RED 1

template <typename T>
class Node//узел дерева
{
public:
    T data;
    Node* parent;
    Node* left;
    Node* right;
    bool Color;//0 - черный, 1 - красный

    Node(T data = T(), bool Color = RED, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr)//новый вставленный элемент всегда окрашивается в красный, если не корень
    {
        this->data = data;
        this->Color = Color;
        this->parent = parent;
        this->left = left;
        this->right = right;
       // left->Color = right->Color = BLACK;//листья всегда окрашиваются в чёрный
    }
};

template <typename T>
class RedBlackTree//красно-чёрное дерево
{
private:
    Node<T>* Root;//корень дерева
   //int size;//размер дерева

    bool getColor(Node<T>*& node)//0 - черный, 1 - красный
    {
        if (node == nullptr)
            return BLACK;
        return node->Color;
    }
    void setColor(Node<T>*& node, bool Color)
    {
        if (node != nullptr)
            node->Color = Color;
    }
    //Дальше повороты https://ru.wikipedia.org/wiki/%D0%94%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%BE%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE_%D0%BF%D0%BE%D0%B8%D1%81%D0%BA%D0%B0#%D0%91%D0%B0%D0%BB%D0%B0%D0%BD%D1%81%D0%B8%D1%80%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%B0
    void rotateLeft(Node<T>* a)//поворот дерева влево
    {
        Node<T>* b;
        /*Делаем из левого ребёнка b правого ребёнка a*/
        b = a->right;
        a->right = b->left;
        if (b->left != nullptr)
            b->left->parent = a;
        /*Делаем из родителя a родителя b и делаем b ребёнком родителя a*/
        b->parent = a->parent;
        if (b->parent == nullptr)
            Root = b;
        else if (a == a->parent->left)
            a->parent->left = b;
        else
            a->parent->right = b;
        /*Делаем a левым ребёнком b и b родителем a*/
        b->left = a;
        a->parent = b;
    }
    void rotateRight(Node<T>* a)//поворот дерева вправо
    {
        Node<T>* b;
        /*Делаем из правого ребёнка b левого ребёнка a*/
        b = a->left;
        a->left = b->right;
        if (b->right != nullptr) {
            b->right->parent = a;
        }
        /*Делаем из родителя a родителя b и делаем b ребёнком родителя a*/
        b->parent = a->parent;
        if (b->parent == nullptr) {
            Root = b;
        }
        else if (a == a->parent->left) {
            a->parent->left = b;
        }
        else {
            a->parent->right = b;
        }
        /*Делаем b родителем a и a правым ребёнком b*/
        b->right = a;
        a->parent = b;
    }
    //поиск минимального узла поддерева с корнем node
    Node<T>* minNode(Node<T>* node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }
    //Дальше фиксы для вставки и удаления, чтобы не нарушать свойства красно-чёрного дерева
    void insertFix(Node<T>* z) {
        while ((z->parent != nullptr) && (z->parent->Color == RED)) {

            /* Родитель z - левый ребёнок дедушки z*/
            if (z->parent == z->parent->parent->left) {

                /*правый ребёнок дедушки z красный */
                if (z->parent->parent->right != nullptr && z->parent->parent->right->Color == RED)
                {
                    z->parent->Color = BLACK;
                    z->parent->parent->right->Color = BLACK;
                    z->parent->parent->Color = RED;
                    z = z->parent->parent;
                }
                /*правый ребёнок дедушки z не красный*/
                else {

                    /* z - правый ребёнок родителя z*/
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }

                    z->parent->Color = BLACK;
                    z->parent->parent->Color = RED;
                    rotateRight(z->parent->parent);
                }
            }
            /*Родитель z - правый ребёнок дедушки z*/
            else {

                /*Левый дядя z или левый ребёнок дедушки z также красный*/
                if (z->parent->parent->left != nullptr && z->parent->parent->left->Color == RED)
                {
                    z->parent->Color = BLACK;
                    z->parent->parent->left->Color = BLACK;
                    z->parent->parent->Color = RED;
                    z = z->parent->parent;
                }

                /*Левый дядя z не красный*/
                else {
                    /* z - левый ребёнок родителя z*/
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }

                    z->parent->Color = BLACK;
                    z->parent->parent->Color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        Root->Color = BLACK;
    }
    //Тут примерно то же самое, но для случая, когда удаляем чёрный узел
    void removeFix(Node<T>* x) {
        Node<T>* w;
        while (x != Root && x->Color == BLACK) {

            if (x == x->parent->left) {
                w = x->parent->right;

                if (w->Color == RED) {
                    w->Color = BLACK;
                    x->parent->Color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }

                if (w->left->Color == BLACK && w->right->Color == BLACK) {
                    w->Color = RED;
                    x->parent->Color = BLACK;
                    x = x->parent;
                }
                else {

                    if (w->right->Color == BLACK) {
                        w->Color = RED;
                        w->left->Color = BLACK;
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    w->Color = x->parent->Color;
                    x->parent->Color = BLACK;
                    x->right->Color = BLACK;
                    rotateLeft(x->parent);
                    x = Root;
                }
            }
            else {
                w = x->parent->left;

                if (w->Color == RED) {
                    w->Color = BLACK;
                    x->parent->Color = BLACK;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }

                if (w->left->Color == BLACK && w->right->Color == BLACK) {
                    w->Color = RED;
                    x->parent->Color = BLACK;
                    x = x->parent;
                }
                else {

                    if (w->left->Color == BLACK) {
                        w->Color = RED;
                        w->right->Color = BLACK;
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    w->Color = x->parent->Color;
                    x->parent->Color = BLACK;
                    w->left->Color = BLACK;
                    rotateRight(x->parent);
                    x = Root;

                }
            }
        }
        x->Color = BLACK;
    }
public:
    RedBlackTree()
    {
        Root = nullptr;
        //size = 0;
    }
    //Вставка элемента
    bool insert(T element)//true - успешно вставлен, false - уже есть - ok
    {
        if (Root == nullptr)
        {
            Root = new Node<T>(element, BLACK);
        }
        else
        {   
            Node<T>* current = Root;
            while ((element < current->data) && (current->left != nullptr) || (element > current->data) && (current->right != nullptr))
            {               
                if (element < current->data)
                    current = current->left;
                if (element > current->data)
                    current = current->right;
            }    
            if (element == current->data)
                return false;//такой элемент уже есть
            Node<T>* newEl = new Node<T>(element, RED, current);//каждый новый элемент всегда красный
            //Присваиваем родителю ссылку на ребёнка 
            if (element < current->data)
                current->left = newEl;
            if (element > current->data)
                current->right = newEl;
            insertFix(newEl);
        }
        return true;//элемент вставлен
    }
    //Поиск элемента
    bool find(T element)//true - эл. найден, false - нет - ok
    {
        Node<T>* current = Root;
        while (current != nullptr)
        {
            if (element == current->data)
                return true;//-найден
            else if (element < current->data)
                current = current->left;
            else if (element > current->data)
                current = current->right;
        }
        return false;//-не найден
    }
    //Удаление элемента
    bool remove(T element)// - not ok
    {
        Node<T>* current = Root;
        while (current != nullptr)
        {
            if (element == current->data)
            {
                if ((current->left == nullptr) && (current->right == nullptr))//если у элемента нет детей, то просто удаляем
                {
                    if (current != Root)
                    {
                        if (current == current->parent->left)//удаляем ссылку на него у родителя
                            current->parent->left = nullptr;
                        else
                            current->parent->right = nullptr;
                    }
                    delete current;
                    if (Root == current)
                    {
                        Root = nullptr;
                    }
                }
                else if ((current->left != nullptr) && (current->right == nullptr))//если есть только левый ребёнок
                {
                    if (current != Root)
                    {
                        if (current == current->parent->left)//даём родителю элемента ссылку на левого ребёнка элемента
                            current->parent->left = current->left;
                        else
                            current->parent->right = current->left;
                    }
                    else
                    {
                        Root = current->left;
                        Root->parent = nullptr;
                    }
                    delete current;
                }
                else if ((current->left == nullptr) && (current->right != nullptr))//если есть только правый ребёнок
                {
                    if (current != Root)
                    {
                        if (current == current->parent->left)////даём родителю элемента ссылку на правого ребёнка элемента
                            current->parent->left = current->right;
                        else
                            current->parent->right = current->right;
                    }
                    else
                    {
                        Root = current->right;
                        Root->parent = nullptr;
                    }
                    delete current;
                }
                else if ((current->left != nullptr) && (current->right != nullptr))//если есть оба ребёнка
                {
                    Node<T>* successor = minNode(current);//ищем следующий элемент от текущего
                    if (current != Root)
                    {
                        if (current == current->parent->left)//даём родителю элемента ссылку на следующий после элемента
                            current->parent->left = successor;
                        else
                            current->parent->right = successor;
                        //Даём "следующему элементу" ссылку на родителя изначального элемента
                        successor->parent = current->parent;
                    }
                    else
                    {
                        successor->parent = nullptr;
                        Root = successor;
                    }
                    //Даём "следующему элементу" ссылки на детей изначального элемента
                    successor->left = current->left;
                    successor->right = current->right;
                    delete current;
                }

                return true;//найден и удалён
            }
            
            else if (element < current->data)
                current = current->left;
            else if (element > current->data)
                current = current->right;
            
        }
        //Если удаляем черный узел, то начинаются проблемы
        bool OriginalColor = current->Color;
        if (OriginalColor == BLACK)
            removeFix(current);
        return false;//элемент не найден и не удалён
    }  
};

int main()
{
    RedBlackTree<int> tree;
    //Закомментировано то, что делает result равным 0
    bool result = 1;
    result &= tree.insert(5);
    result &= tree.find(5);
    result &= tree.insert(10);
    //result &= tree.insert(5);
    //result &= tree.find(12);
    result &= tree.insert(33);
    result &= tree.insert(15);
    result &= tree.insert(11);
    result &= tree.insert(3);
    result &= tree.find(11);
    result &= tree.remove(5);
    //result &= tree.remove(5);

    if (result)
        cout << "Alright";
    else
        cout << "Shit";
    return 0;
}


