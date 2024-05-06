#include"defineCoordinate.h"
#include <QDebug>

void defineCoordinate::defNode(QVector<QPoint> flaws)
{
    vector_PG_flaw = flaws;
    for(int i=0;i<flaws.size();i++){
        current = (node*)malloc(sizeof(node));
        current->x = flaws[i].x();
        current->y = flaws[i].y();

        // definite node pattern number
        current->index = 1;

        if(i==0){
            first = current;
            previous = current;
            current->prev = NULL;
        }else{
            previous->next = current;
            current->prev = previous;
            current->next = NULL;
            previous = current;
        }
    }
    current = first;
}

