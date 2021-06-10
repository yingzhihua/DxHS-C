#include "uihandler.h"

static UIHandler *uihandler = nullptr;

UIHandler::UIHandler(QObject *parent) : QObject(parent)
{

}

UIHandler *UIHandler::getPtr()
{
    if (uihandler == nullptr)
        uihandler = new UIHandler();
    return uihandler;
}

void UIHandler::GoPage(PageId id)
{
    emit uihandler->Go(id);
}
