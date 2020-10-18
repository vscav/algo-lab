#include <time.h>
#include <stdio.h>

#include <QApplication>
#include <QDebug>

#include "lib/mainwindow.h"

#include "HuffmanNode.h"

MainWindow* w = nullptr;
using std::size_t;
using std::string;

void HuffmanHeap::insertHeapNode(int heapSize, unsigned char c, int frequences)
{
    int i = heapSize;
    this->set(i, HuffmanNode(c, frequences));
    while (i > 0 && (this->get(i).frequences > this->get(i / 2).frequences))
    {
        swap(i, i / 2);
        i = i / 2;
    }
}

void HuffmanNode::insertNode(HuffmanNode* node)
{
    if (this->frequences <= this->frequences / 3)
    {
        if (this->right == NULL)
            this->right = node;
        else
        {
            if (this->right->isLeaf())
            {
                HuffmanNode* tmp = new HuffmanNode();
                tmp->right = this->right;
                tmp->left = node;
                this->right = tmp;
            }
            else
                this->right->insertNode(node);
        }
    }
    else
    {
        if (this->left == NULL)
            this->left = node;
        else
        {
            if (this->left->isLeaf())
            {
                HuffmanNode* tmp = new HuffmanNode();
                tmp->left = this->left;
                tmp->right = node;
                this->left = tmp;
            }
            else
                this->left->insertNode(node);
        }
    }
}

void HuffmanNode::processCodes(std::string baseCode)
{
    if( this->isLeaf() )
        this->code = baseCode;
    else
    {
        if (this->left != NULL)
            this->left->processCodes(baseCode + '0');
        if (this->right != NULL)
            this->right->processCodes(baseCode + '1');
    }
}

void HuffmanNode::fillCharactersArray(HuffmanNode** nodes_for_chars)
{
    if (!this->left && !this->right)
        nodes_for_chars[this->character] = this;
    else {
        if (this->left)
            this->left->fillCharactersArray(nodes_for_chars);
        if (this->right)
            this->right->fillCharactersArray(nodes_for_chars);
    }
}

void charFrequences(string data, Array& frequences)
{
    for (int i = 0; i < frequences.size(); i++)
        frequences.set(i, 0);

    for (int c = 0; c < (int)data.size(); c++)
        frequences.set(data[c], (frequences.get(data[c]))+1);
}

void huffmanHeap(Array& frequences, HuffmanHeap& heap)
{
    for (int index = 0; index < heap.size(); index++)
        if (frequences[index] > 0)
            heap.insertHeapNode(index, index, frequences.get(index));
}

void huffmanDict(HuffmanHeap& heap, HuffmanNode*& dict)
{
    dict = new HuffmanNode('\0', heap[0].frequences + heap[1].frequences);
    dict->left = &heap[0];
    dict->right = &heap[1];

    for (int i = 2; i < heap.size(); i++)
        dict->insertNode(&heap[i]);
}

string huffmanEncode(HuffmanNode** characters, string toEncode)
{
    string encoded = "";
    for (int i = 0; i < (int)toEncode.size(); i++)
        encoded += (characters[toEncode[i]])->code;
    return encoded;
}

string huffmanDecode(HuffmanNode* dict, string toDecode)
{
    string decoded = "";
    HuffmanNode* d = dict;
    int i = 0;

    if(d != NULL)
    {
        while (i < (int)toDecode.size())
        {
            d = dict;
            while(!d->isLeaf())
            {
                if(toDecode[i] == '0')
                    d = d->left;
                else
                    d = d->right;
                i++;
            }
            decoded += d->character;
        }
    }
    return decoded;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Array::wait_for_operations = false;
    w = new MainWindow();

    string data = "Ouesh, bien ou bien ? Ceci est une chaine de caracteres sans grand"
                  " interet";

    Array& frequences = w->newArray(256);
    HuffmanHeap heap(256);
    HuffmanNode* dict;

    for (int i=0; i<frequences.size(); ++i)
        frequences.__set__(i, 0);

    charFrequences(data, frequences);

    for (int i=0; i<frequences.size(); ++i)
        if (frequences[i]>0)
            qDebug() << (char)i << ": " << frequences[i];

    huffmanHeap(frequences, heap);
    huffmanDict(heap, dict);
    dict->processCodes("");

    HuffmanNode* characters[256];
    memset(characters, 0, 256 * sizeof (HuffmanNode*));
    dict->fillCharactersArray(characters);

    string encoded = huffmanEncode(characters, data);
    string decoded = huffmanDecode(dict, encoded);

    w->updateScene();
    qDebug("Encoded: %s\n", encoded.c_str());
    qDebug("Decoded: %s\n", decoded.c_str());
    w->show();

    return a.exec();
}
