#ifndef DIALOGOCARACTERISTICAS_H
#define DIALOGOCARACTERISTICAS_H

#include <QDialog>

namespace Ui {
class DialogoCaracteristicas;
}

class DialogoCaracteristicas : public QDialog
{
    Q_OBJECT

public:
    explicit DialogoCaracteristicas(QWidget *parent = 0);
    ~DialogoCaracteristicas();

private:
    Ui::DialogoCaracteristicas *ui;
};

#endif // DIALOGOCARACTERISTICAS_H
