#include "lib/mainwindow.h"
#include <QApplication>
#include <QString>
#include <time.h>
#include <stdio.h>
#include <string>
#include <math.h>

#include "lib/tp5.h"

MainWindow* w = nullptr;
using std::size_t;
using std::string;

int hash(string key)
{
    int i =0;
    int n = key.size();
    int hash_value = 0;

    while(key[i] != '\0')
    {
        hash_value += (int)key[i]*pow(128, n-i);
        i++;
    }

    return hash_value;
}

struct BinarySearchTree : public BinaryTree
{

    string key;
    int key_hash;

    int value;

    BinarySearchTree* left;
    BinarySearchTree* right;

    virtual ~BinarySearchTree() {}
    virtual void setValue(QVariant value) override {this->value = value.toInt();};
    virtual QString toString() const override
    {return QString("%1:\n%2").arg(QString::fromStdString(key)).arg(value);}
    const Node* leftChild() const override {return left;};
    const Node* rightChild() const override {return right;};

    BinarySearchTree(string key, int value)
    {
        this->key = key;
        this->value = value;
        this->key_hash = hash(key);

        this->left = this->right = nullptr;
    }

    void insertNode(BinarySearchTree* node)
    {
        if(node->key_hash < this->key_hash)
        {
            if(this->left == NULL)
                this->left = node;
            else
                this->left->insertNode(node);
        }
        else
        {
            if(this->right == NULL)
                this->right = node;
            else
                this->right->insertNode(node);
        }
    }

    void insertNode(string key, int value)
    {
        this->insertNode(new BinarySearchTree(key, value));
    }
};

struct Map
{
    Map() {
        this->root = nullptr;
    }

    void insert(string key, int value)
    {
        if (!this->root)
            this->root = new BinarySearchTree(key, value);
        else
            this->root->insertNode(key, value);
    }

    int get(string key)
    {
        BinarySearchTree* base = this->root;
        while(base != NULL)
        {
            if(base->key_hash == hash(key))
                return base->value;
            else
            {
                if(base->key_hash < hash(key))
                    base = base->right;
                else
                    base = base->left;
            }
        }
        return 0;
    }

    BinarySearchTree* root;
};


int main(int argc, char *argv[])
{
    Map map;

    map.insert("Laurinne", 9);
    map.insert("Luc", 5);
    map.insert("Alois", 14);
    map.insert("Paul", 10);
    map.insert("Bojack", 15);
    map.insert("Mr.PeanutB", 20);

    QApplication a(argc, argv);
    MainWindow::instruction_duration = 200;
    w = new BinarySearchTreeWindow(map.root);
    w->show();

    printf("map[\"Laurinne\"]=%d\n", map.get("Laurinne"));
    printf("map[\"Paul\"]=%d\n", map.get("Paul"));

    w->resize(750,750);
    return a.exec();
}
