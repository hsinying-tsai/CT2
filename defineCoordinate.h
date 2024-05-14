#ifndef DEFINECOORDINATE_H
#define DEFINECOORDINATE_H
#include <QVector>
#include <QString>
#include <QPoint>
class defineCoordinate
{
public:

    int center_posX =0, center_posY = 0;

    QVector<QPoint> vector_PG_flaw;
    struct Node{
        int x,y,index;
        struct Node* next;
        struct Node* prev;
    };
    typedef struct Node node;
    node *first, *current, *previous = NULL,*temp;
    void defNode(QVector<QPoint> flaws, int index, bool isHead);
};
#endif // DEFINECOORDINATE_H
