#ifndef HUFFMANNNODE_H
#define HUFFMANNNODE_H

#include <QString>

#include "lib/binarytree.h"
#include "lib/array.h"

struct HuffmanNode : TemplateNode<HuffmanNode>
{
    HuffmanNode(unsigned char c='\0', int frequences=0)
    {
        this->frequences = frequences;
        this->character = c;
        this->code = std::string();
    }

    HuffmanNode(const HuffmanNode& node)
    {
        this->frequences = node.frequences;
        this->character = node.character;
        this->code = std::string(node.code);
    }

    virtual ~HuffmanNode() {}

    HuffmanNode* find(int) {return nullptr;}

    virtual bool isLeaf() const {return !this->left && !this->right;}

    QString toString() const
    {
        return QString("[char: %1, frequences: %2]").arg(character).arg(frequences);
    }

    void insertNode(HuffmanNode*);
    void processCodes(std::string baseCode);
    void fillCharactersArray(HuffmanNode** nodes_for_chars);

    int frequences;
    unsigned char character;
    std::string code;
};


class HuffmanHeap : public TemplateArray<HuffmanNode>
{
public:
    virtual ~HuffmanHeap() {}

    HuffmanHeap(uint size=100) : TemplateArray<HuffmanNode>()
    {
        if (size>0)
        {
            _data.resize(size);
            for (uint i=0; i<size; ++i)
                _data[i] = -1;
        }
    }

    size_t effectiveSize() const;

    QString toString() const;

    HuffmanNode& get(uint index) {
        return _data[index];
    }

    int leftChild(int nodeIndex);

    int rightChild(int nodeIndex);

    void insertHeapNode(int heapSize, unsigned char c, int frequences);

    void heapify(int heapSize, int nodeIndex);

    void buildHeap(TemplateArray<HuffmanNode>& numbers);

    void heapSort();
};

#endif // HUFFMANNNODE_H
