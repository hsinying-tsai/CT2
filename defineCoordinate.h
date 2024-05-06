#ifndef DEFINECOORDINATE_H
#define DEFINECOORDINATE_H
#include <QVector>
#include <QString>
#include <QPoint>
class defineCoordinate
{
public:

    int center_posX =0, center_posY = 0, number_flaw_pattern = 0;

    //assume flaw pattern are pattern3(10,20,50), pattern5(30), pattern7(70,90)
    QVector<QPoint> vector_PG_flaw;
    struct Node{
        int x,y,index;
        struct Node* next;
        struct Node* prev;
    };
    typedef struct Node node;
    node *first, *current, *previous = NULL,*temp;
    void defNode(QVector<QPoint> flaws);
};
#endif // DEFINECOORDINATE_H
