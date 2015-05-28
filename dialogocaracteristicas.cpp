#include "dialogocaracteristicas.h"
#include "ui_dialogocaracteristicas.h"

DialogoCaracteristicas::DialogoCaracteristicas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogoCaracteristicas)
{
    ui->setupUi(this);
}

DialogoCaracteristicas::~DialogoCaracteristicas()
{
    delete ui;
}
