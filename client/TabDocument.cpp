#include <QtWidgets/QBoxLayout>
#include "TabDocument.h"
#include "ui_TabDocument.h"
#include "TextEditor.h"

TabDocument::TabDocument(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabDocument)
{
    ui->setupUi(this);

    /*TextEditor* editor = new TextEditor(0, *ui, this);
    editor->show();
    this->adjustSize();*/
}

TabDocument::~TabDocument()
{
    delete ui;
}

Ui::TabDocument * TabDocument::getUi() const{
    return ui;
}
