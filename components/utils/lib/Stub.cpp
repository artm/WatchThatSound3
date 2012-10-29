#include "Stub.hpp"

Stub::Stub(QObject *parent) :
    QObject(parent)
{
}

void Stub::unimplemented()
{
    QMessageBox msgBox;
    msgBox.setText("The selected function haven't been integrated into the tool yet.");
    msgBox.exec();
}
