#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QFileDialog> // otwieranie plikow
#include <QTextStream>

namespace Ui {
class MainWindow;
}

// Deklaracja głównej klasy naszego programu
// Jest ona podklasą klasy QMainWindow
// standardowej klasy Qt reprezentującej główne okno aplikacji
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// Sloty to specjalne funkcje (metody) wywoływane
// po wystąpieniu pewnych zdarzeń np. po kliknięciu przycisku
private slots:
    void on_exitButton_clicked();
    void on_cleanButton_clicked();

    void paintEvent(QPaintEvent*);

    void on_pushButton_clicked();

    void on_czarnobialy_clicked();
    void paint_on_black_and_white();
    void colorPicker(int x,int y);
    void reset();

    void on_reset_clicked();
    void check_surrounding_8pixels();

private:
    // ui to wskaźnik za pomocą którego mamy dostęp
    // do elementów interfejsu użytkownika
    Ui::MainWindow *ui;

    QImage *img, *img_copy, *img_org, *imgbw, *imgbw_copy;  // wysiwetalnie // img_org drugi obraz
    int width, height, startX, startY;

    void clean();

};

#endif // MAINWINDOW_H
