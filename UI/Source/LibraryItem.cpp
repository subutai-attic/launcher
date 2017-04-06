#include "LibraryItem.h"

LibraryItem::LibraryItem(const  std::string& title) :
    _title(title)
{

}

LibraryItem::~LibraryItem()
{

}


void LibraryItem::paint(Graphics& g)
{
    g.setColour(Colours::white);
}

void LibraryItem::resized()
{

}

