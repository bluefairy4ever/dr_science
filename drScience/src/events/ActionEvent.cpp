//
//  ActionEvent.cpp
//  drScience
//
//  Created by Matthew Parish on 10/18/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "events/ActionEvent.h"

#include <iostream>

using std::cout;

ActionEvent::ActionEvent(EventType type) : Event(type)
{
    cout << "Action event created\n";
}

ActionEvent::~ActionEvent()
{
    ;
}