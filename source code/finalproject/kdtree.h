/*
 * Kdtree is used to quickly load the POIIDs within a specific range
 */

#ifndef KDTREE_H
#define KDTREE_H
#include <QVector>

struct KDItem
{
    double key[2]; //key[0] for latitude, key[1] for longitude
    int value;
    KDItem(double _latitude, double _longitude, int _value)
    {
        key[0] = _latitude;
        key[1] = _longitude;
        value = _value;
    }
};

class KDtree
{
private:
    struct Node
    {
        Node *left, *right;
        double key[2];
        int value;
        Node(KDItem _item);
    };
    Node *root;

    QVector<int> rangeSearch(Node *root, int dim, double lower_bound[], double upper_bound[]);
    void insert(Node *&root, KDItem item, int dim);
    void makeEmpty(Node *rt);

public:
    KDtree();
    ~KDtree();
    void insert(KDItem item);
    QVector<int> rangeSearch(double lowX, double highX, double lowY, double highY);
};

#endif // KDTREE_H
