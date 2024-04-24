#include"defineCoordinate.h"
#include <QDebug>

void defineCoordinate::defNode(){
    for(int i = 0; i<total_flaw_num ; i++){
        current = (node*)malloc(sizeof(node));
    //        qDebug()<<"Data for nodeF"<<i+1;
        current->x = vector_PG_flaw[i*2];
        current->y = vector_PG_flaw[(i*2)+1];
        // definite node pattern number
        current->index = 1;
//        if(i<3){
//            current->index = 3;
//        }else if(i==3){
//            current->index = 5;
//        }else{
//            current->index = 7;
//        }

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
    //calculate the amount of flaw(not used)

    temp= first;
    for(int i = 1;i<total_flaw_num;i++){
        if(temp->index != temp->next->index){
            number_flaw_pattern++;
        }
        temp = temp->next;
    }
//    setupflawpos({10,10,20,20,50,50,30,30,70,70,90,90});
}

void defineCoordinate::setupflawpos(QVector<int> vectorflaw)
{
    qDebug()<<vectorflaw;
}


