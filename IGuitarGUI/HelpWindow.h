#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWebKitWidgets/QWebView>

/**
 * @brief Affichage de l'aide
 */
class HelpWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit HelpWindow(QWidget *parent = 0);
        ~HelpWindow();

    private:
        /**
         * @brief m_page Sert à afficher les pages web
         */
        QWebView *m_page;

        /**
         * @brief m_close Bouton "fermer"
         */
        QPushButton *m_close;

        /**
         * @brief m_layout ayout ou l'on dispose la fenêtre d'aide
         */
        QVBoxLayout *m_layout;
};

#endif // HELPWINDOW_H
