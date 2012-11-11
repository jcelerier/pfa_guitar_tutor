 #include <QApplication>
 #include <QtGui>

 class MusicSelect : public QWidget
 {
 public:
     MusicSelect(QWidget *parent = 0);
 };

 MusicSelect::MusicSelect(QWidget *parent)
     : QWidget(parent)
 {
     setFixedSize(800, 550);
     QPushButton *suivant = new QPushButton(tr("Suivant"));
     suivant->setFont(QFont("Times", 18, QFont::Bold));

     QSlider *slider = new QSlider(Qt::Horizontal);
     slider->setRange(0, 99);
     slider->setValue(0);

     QProgressBar *progress = new QProgressBar;
     progress->setValue(0);

     QObject::connect(slider, SIGNAL(sliderMoved(int)), progress, SLOT(setValue(int)));
     QObject::connect(suivant, SIGNAL(clicked()), qApp, SLOT(quit()));

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(slider);
     layout->addWidget(progress);
     layout->addWidget(suivant);
     setLayout(layout);
 }

/*
 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     MusicSelect widget;
     widget.show();
     return app.exec();
 }
*/
