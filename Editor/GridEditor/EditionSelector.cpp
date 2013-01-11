#include "EditionSelector.h"
#include "GridEditor.h"

/**
 * @brief EditionSelector::EditionSelector
 * @param parent Fenêtre parente
 *
 * Crée la fenêtre d'invite de sélection d'éditeur.
 */
EditionSelector::EditionSelector(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::Dialog);
    QLabel  *presentation = new QLabel(tr("What do you want to do today?"));
    QCheckBox *checkbox = new QCheckBox(tr("Show this screen at startup"), this);
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
    checkbox->setCheckState(Qt::Checked);

    mainLayout->addWidget(presentation);
    mainLayout->addWidget(tabs);
    mainLayout->addWidget(checkbox);
    setLayout(mainLayout);

    connect(buttonAssistedEditor, SIGNAL(clicked()), this, SLOT(newAssistedEditor()));
    connect(buttonManualEditor, SIGNAL(clicked()), this, SLOT(newManualEditor()));
    connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(setShowOption(int)));
}

/**
 * @brief EditionSelector::newAssistedEditor
 *
 * Demande la création d'un widget d'édition assistée dans la fenêtre parente puis quitte le sélecteur d'éditeur.
 */
void EditionSelector::newAssistedEditor()
{
    close();
    ((GridEditor*) parent())->newEditor(ASSISTED_EDITOR);
}

/**
 * @brief EditionSelector::newManualEditor
 *
 * Demande la création d'un widget d'édition manuelle dans la fenêtre parente puis quitte le sélecteur d'éditeur.
 */
void EditionSelector::newManualEditor()
{
    close();
    ((GridEditor*) parent())->newEditor(MANUAL_EDITOR);
}

/**
 * @brief EditionSelector::setShowOption
 * @param state 0 si et seulement si le sélectionneur d'éditeur ne doit pas s'afficher au démarrage.
 *
 * Sauvegarde s'il faut afficher ou non le sélectionneur d'éditeur au démarrage.
 */
void EditionSelector::setShowOption(int state)
{
    QSettings* settings = new QSettings("GuitarTutor", "GridEditor");
    settings->setValue("ShowEditionSelector", state);
    delete settings;
}
