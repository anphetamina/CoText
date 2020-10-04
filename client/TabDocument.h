#ifndef TABDOCUMENT_H
#define TABDOCUMENT_H

#include <QWidget>

namespace Ui {
class TabDocument;
}

class TabDocument : public QWidget
{
    Q_OBJECT

public:
    explicit TabDocument(QWidget *parent = nullptr);
    ~TabDocument();
    Ui::TabDocument * getUi() const;

private:
    Ui::TabDocument *ui;
};

#endif // TABDOCUMENT_H
