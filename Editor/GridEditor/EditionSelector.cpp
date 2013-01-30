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
  QLabel *presentation = new QLabel(tr("What do you want to do today?"));
  QTabWidget *tabs = new QTabWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout();

  QWidget *pageNew = new QWidget(); //Pages pour les onglets
  QWidget *pageOpen = new QWidget();

  //Page nouveau fichier
  QLabel *pictureNew = new QLabel(tr("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat."));
  pictureNew->setWordWrap(true);
  //pictureNew->setPixmap(QPixmap("icons/manualeditor.jpg"));
  QPushButton *buttonNew= new QPushButton(tr("Begin"));

  QVBoxLayout *vboxNew = new QVBoxLayout;
  vboxNew->addWidget(pictureNew);
  vboxNew->addWidget(buttonNew);
  pageNew->setLayout(vboxNew);


  //Page ouverture fichier
  QLabel *pictureOpen = new QLabel(tr("Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi."));
  pictureOpen->setWordWrap(true);
  //pictureOpen->setPixmap(QPixmap("icons/assistededitor.jpg"));
  QPushButton *buttonOpen = new QPushButton(tr("Begin"));

  QVBoxLayout *vboxOpen = new QVBoxLayout;
  vboxOpen->addWidget(pictureOpen);
  vboxOpen->addWidget(buttonOpen);
  pageOpen->setLayout(vboxOpen);

  tabs->addTab(pageNew, tr("New grid"));
  tabs->addTab(pageOpen, tr("Open a grid"));

  mainLayout->addWidget(presentation);
  mainLayout->addWidget(tabs);
  setLayout(mainLayout);

  connect(buttonNew, SIGNAL(clicked()), parent, SLOT(newGrid()));
  connect(buttonOpen, SIGNAL(clicked()), parent, SLOT(fromXML()));
}
