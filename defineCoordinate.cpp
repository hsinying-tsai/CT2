#include"defineCoordinate.h"
#include <QDebug>

void defineCoordinate::defNode(QVector<QPoint> flaws,int index,bool isHead)
{
    qDebug()<<"123->"<<flaws<<"->"<<index;
    if(isHead == true){
        vector_PG_flaw.clear();
    }
    vector_PG_flaw.append(flaws);
    for(int i=0;i<flaws.size();i++){
        current = (node*)malloc(sizeof(node));
        current->x = flaws[i].x();
        current->y = flaws[i].y();

        // definite node pattern number
        current->index = index;
        qDebug()<<"--"<<i;
        if(isHead == true){
            qDebug()<<"First"<<flaws[i].x()<<flaws[i].y();
            first = current;
            previous = current;
            current->prev = NULL;
            isHead = false;
        }else{
            previous->next = current;
            current->prev = previous;
            current->next = NULL;
            previous = current;
        }
        qDebug()<<"vector_PG_flaw"<<vector_PG_flaw;
    }
//    current = first;
}

