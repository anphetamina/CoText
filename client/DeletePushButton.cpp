//
// Created by dimar on 10/10/2020.
//

#include "DeletePushButton.h"

DeletePushButton::DeletePushButton(int i) : index(i) {
    connect ( this, &DeletePushButton::released , this, &DeletePushButton::releasedReceived  );
}


void DeletePushButton::releasedReceived(){
    emit releasedWithIndex(index);
}