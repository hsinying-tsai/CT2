#ifndef DEFINECOORDINATE_H
#define DEFINECOORDINATE_H
#include<QVector>
#include<QString>
class defineCoordinate
{
public:

    int compe_posX = 0,compe_posY= 0,number_flaw_pattern=0;
    //assume flaw pattern are pattern3(10,20,50), pattern5(30), pattern7(70,90)
    QVector<int> vector_PG_flaw = {10,10,20,20,50,50,30,30,70,70,90,90};
    struct Node{
        int x,y,index;
        struct Node* next;
    };
    int total_flaw_num = 6;
    typedef struct Node node;
    node *first, *current, *previous,*temp;
    void defNode();

};
#endif // DEFINECOORDINATE_H
