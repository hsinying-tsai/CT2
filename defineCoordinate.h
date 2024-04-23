#ifndef DEFINECOORDINATE_H
#define DEFINECOORDINATE_H
#include<QVector>
#include<QString>
class defineCoordinate
{
public:

    int center_posX =0, center_posY = 0, number_flaw_pattern = 0;
    //assume flaw pattern are pattern3(10,20,50), pattern5(30), pattern7(70,90)
    QVector<int> vector_PG_flaw = {-5,3,-106,-33,-124,-14};
    struct Node{
        int x,y,index;
        struct Node* next;
        struct Node* prev;
    };
    int total_flaw_num = 3;
    typedef struct Node node;
    node *first, *current, *previous = NULL,*temp;
    void defNode();
    void setupflawpos(QVector<int>);
};
#endif // DEFINECOORDINATE_H
