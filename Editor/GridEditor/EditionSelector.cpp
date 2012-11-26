#include "EditionSelector.h"

EditionSelector::EditionSelector(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::Dialog);
    QLabel  *presentation = new QLabel(tr("Two edit modes are available, you just have to choose one to begin the adventure!"));
    QTabWidget *tabs = new QTabWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QWidget *pageManualEditor = new QWidget(); //Pages pour les onglets
    QWidget *pageAssistedEditor = new QWidget();

    //Page de l'édition manuelle
    QLabel *pictureManualEditor = new QLabel();
    pictureManualEditor->setPixmap(QPixmap("icons/manualeditor.jpg"));
    QPushButton *buttonManualEditor = new QPushButton(tr("Begin"));

    QVBoxLayout *vboxManualEditor = new QVBoxLayout;
    vboxManualEditor->addWidget(pictureManualEditor);
    vboxManualEditor->addWidget(buttonManualEditor);
    pageManualEditor->setLayout(vboxManualEditor);


    //Page de l'édition assistée
    QLabel *pictureAssistedEditor = new QLabel();
    pictureAssistedEditor->setPixmap(QPixmap("icons/assistededitor.jpg"));
    QPushButton *buttonAssistedEditor = new QPushButton(tr("Begin"));

    QVBoxLayout *vboxAssistedEditor = new QVBoxLayout;
    vboxAssistedEditor->addWidget(pictureAssistedEditor);
    vboxAssistedEditor->addWidget(buttonAssistedEditor);
    pageAssistedEditor->setLayout(vboxAssistedEditor);

    tabs->addTab(pageManualEditor, tr("Manual Editor"));
    tabs->addTab(pageAssistedEditor, tr("Assisted Editor"));

    mainLayout->addWidget(presentation);
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);

    connect(buttonAssistedEditor, SIGNAL(clicked()), this, SLOT(newAssistedEditor()));
    connect(buttonManualEditor, SIGNAL(clicked()), this, SLOT(newManualEditor()));
}

/**
 * @brief EditionSelector::newAssistedEditor
 *
 * Demande la création d'un widget d'édition assistée dans la fenêtre parente puis quitte le sélecteur d'éditeur.
 */
void EditionSelector::newAssistedEditor()
{
    close();
    emit newEditor(ASSISTED_EDITOR);
}

/**
 * @brief EditionSelector::newManualEditor
 *
 * Demande la création d'un widget d'édition manuelle dans la fenêtre parente puis quitte le sélecteur d'éditeur.
 */
void EditionSelector::newManualEditor()
{
    close();
    emit newEditor(MANUAL_EDITOR);
}
