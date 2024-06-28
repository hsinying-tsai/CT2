#include"defineCoordinate.h"
#include <QDebug>

void defineCoordinate::defNode(QPoint flaws,int index,bool isHead)
{
    if(isHead == true){
        vector_PG_flaw.clear();
    }
    vector_PG_flaw.append(flaws);
    current = (node*)malloc(sizeof(node));
    current->x = flaws.x();
    current->y = flaws.y();

    // definite node pattern number
    current->index = index;
    if(isHead == true){
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
}

