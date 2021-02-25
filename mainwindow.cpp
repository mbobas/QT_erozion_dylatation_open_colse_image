 #include "mainwindow.h"
#include "ui_mainwindow.h"

// Konstruktor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Pobieramy współrzędne (względem głównego okna) lewego, górnego naroznika ramki
    startX = ui->frame->x();
    startY = ui->frame->y();

    // Pobieramy wymiary ramki
    width = ui->frame->width();
    height = ui->frame->height();

    QTextStream(stdout) << "W: " << width << "H: " << height;

    // Tworzymy obiekt klasy QImage o wymiarach równych wymiarom ramki
    // Format_RGB32 to 32 bitowe RGB (z kanałem alfa ustawionym na wartość 255)
    img = new QImage(width, height, QImage::Format_RGB32);
    img_copy = new QImage(width, height, QImage::Format_RGB32);
    imgbw = new QImage(width, height, QImage::Format_RGB32);
    imgbw_copy = new QImage(width, height, QImage::Format_RGB32);
    imgdyl = new QImage(width, height, QImage::Format_RGB32);
    imgero = new QImage(width, height, QImage::Format_RGB32);
    // Wczytujemy plik graficzny do drugiego obiektu klasy QImage
    // jeżeli ścieżka zaczyna się od dwukropka tzn. że plik
    // znajduje się w zasobach projektu
    img_org = new QImage(":/flower.jpg");

    // Kopiujemy fragment wczytanego obrazu do obiektu klasy QImage
    // który będzie wyświetlany na ekranie
    // wymiary fragmentu odpowiadają wymiarom pola rysunkowego
    *img = img_org->copy(0,0,width,height);
    //tworzymy kopie zapasowa
    *img_copy = img->copy(0,0,width,height);

    // Zamiast kopiowania fragmentu, można przeskalować obraz
    // *img = img_org->scaled(width,height);
    on_komunikatChange(komunikat);
}
//*****************************************************************************************************


// Destruktor
MainWindow::~MainWindow()
{
    delete ui;
}
//*****************************************************************************************************


// Funkcja ,,odrysowująca'' okno aplikacji. Wywoływana automatycznie podczas uruchamiania aplikacji
// i w innych sytuacjach, w których zachodzi konieczność ponownego narysowania okna.
// Można też wymusić jej wywołanie za pomocą funkcji update()
void MainWindow::paintEvent(QPaintEvent*)
{
    // Obiekt przy pomocy, którego możemy rysować po elementach interfejsu graficznego aplikacji
    QPainter p(this);

    // Metoda drawImage wyświetla zawartość obiektu *img klasy QImage w głównym oknie aplikacji
    // startX i startY to współrzędne (względem głównego okna) lewego górnego narożnika
    // wyświetlanego obiektu
    p.drawImage(startX, startY, *img);
}
//*****************************************************************************************************


// Funkcja zamalowująca na czarno wszystkie piksele obrazu *img
void MainWindow::clean()
{
    // deklaracja wskaźnika do poruszania się po pikselach obrazu
    unsigned char *wsk;

    // przechodzimy po wszystkich wierszach obrazu
    for(int i=0; i<height; i++)
    {
        // ustawiamy wskaźnik na początku i-tego wiersza
        wsk = img->scanLine(i);

        // przechodzimy po wszystkich piselach i-tego wiersza
        // i nadajemy im wartość (0,0,0) odpowiadającą kolorowi czarnemu
        for(int j=0; j<width; j++)
        {
            wsk[4*j] = 0;   // ustawiamy składową ,,niebieską'' na 0
            wsk[4*j+1] = 0; // ustawiamy składową ,,zielonąą'' na 0
            wsk[4*j+2] = 0; // ustawiamy składową ,,czerwoną'' na 0
        }
    }
}

// Funkcja zamalowująca na czarno wszystkie piksele obrazu *img
void MainWindow::reset()
{
    //img_org = new QImage(":/flower.jpg");

    // Kopiujemy fragment wczytanego obrazu do obiektu klasy QImage
    // który będzie wyświetlany na ekranie
    // wymiary fragmentu odpowiadają wymiarom pola rysunkowego
    *img = img_copy->copy(0,0,width,height);

    // Zamiast kopiowania fragmentu, można przeskalować obraz
    // *img = img_org->scaled(width,height);
    update();
    czarnobialy = false;
    on_komunikatChange("Reset wykonany");

}


//*****************************************************************************************************



// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaźnik na obiekt aplikacji
    // metoda quit() powoduje zakończenie działania aplikacji z kodem wyjścia 0 (sukces)
    qApp->quit();
}
//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_cleanButton_clicked()
{
    clean();

    // Po zmodyfikowaniu obiektu QImage odświeżamy okno aplikacji, aby zobaczyc zmiany
    update();
}

void MainWindow::on_pushButton_clicked()
{
    // Wyświetlamy okno wyboru pliku.
    // Jeżeli plik zostanie wybrany funkcja zwróci jego nazwę
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "../obrazki", "Image Files (*.png *.jpg *.bmp)");

    // Jeżeli nazwa nie jest pusta (plik został wybrany)
    if(!fileName.isNull())
    {
        // Ładujemy wybrany plik
        img_org->load(fileName);
        // A nastepnie kopiujemy jego fragment o wymiarach
        // odpowiadających wymiarom naszego pola rysowania
        // do wyświetlanego w nim obiektu QImage
        *img = img_org->copy(0,0,width,height);
        *img_copy = img->copy(0,0,width,height);

        update();
    }
}
void MainWindow::colorPicker(int x,int y){
    QRgb color = img->pixel(x,y);
    QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
}

void MainWindow::paint_on_black_and_white(){
    // przechodzimy po wszystkich wierszach obrazu
    *imgbw = img->copy(0,0,width,height);
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            QRgb color = imgbw->pixel(i,j);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            int skalaSzarosci =(qRed(color) + qGreen(color) + qBlue(color))/3;
            if (skalaSzarosci<128){
                 imgbw->setPixel(i, j, qRgb(0,0,0));
            }else {
                imgbw->setPixel(i,j, qRgb(255,255,255));
            }
        }
    }
    *img = imgbw->copy(0,0,width,height);
    update();
    on_komunikatChange("Obraz zmieniony na Czarno-Bioały");
}

void MainWindow::check_surrounding_8pixels(int x, int y){


    //sprawdzanie 8 pixeli otaczajacych true lub false - poźniej
}


void MainWindow::dylatacja(){

    //TŁO - biały
    //obraz - czarny
    //napotykamy na czarnego sąsiada, to zamalowywyjemy pobrany punkt - pixel ma 8'u sąsiadów.

    // przechodzimy po wszystkich wierszach obrazu
    //dane pobieramy z imgdyl
    //zmiana danych nastepuje na pliku img...

    *imgdyl = img->copy(0,0,width,height);


    //wypelniamy obrzeza rami kolorem TŁA - białym - wypelniamy niewidocznych sasiadow
//    for (int i=0; height-startX+1; i++){
//        img->setPixel(startX-1, startY+i, qRgb(255,255,255));
//        //QTextStream(stdout) << "ustawiono tło białe: " << startX-1 << " " << startY+i;
//    }


    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int sumaCzarnychSasiadow=0;
            //pobieramy kolor pixela i 8 sąsiadów

            QRgb color = imgdyl->pixel(i,j);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            int kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgdyl->pixel(i-1,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgdyl->pixel(i,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgdyl->pixel(i+1,j+1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgdyl->pixel(i-1,j);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgdyl->pixel(i+1,j);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgdyl->pixel(i-1,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgdyl->pixel(i,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }
            color = imgdyl->pixel(i+1,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaCzarnychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }


            //jesli sumaCzarnychSasiadow jes wieksza od zera to malujemy na czarno wybrany pixel
            if (sumaCzarnychSasiadow > 0){
                img->setPixel(i, j, qRgb(0,0,0));
            }else {
                //malujemy na bialo jeś☺li brak czarnych sasiadow
                 img->setPixel(i,j, qRgb(255,255,255));
            }
        }
    }

    //*img = imgdyl->copy(0,0,width,height);
    update();

    //dylatacja
    on_komunikatChange("Dylatacja zrobiona");
}

void MainWindow::erozja(){

    //TŁO - biały
    //obraz - czarny
    //napotykamy na czarnego sąsiada, to zamalowywyjemy pobrany punkt - pixel ma 8'u sąsiadów.

    // przechodzimy po wszystkich wierszach obrazu
    //dane pobieramy z imgdyl
    //zmiana danych nastepuje na pliku img...

    *imgero = img->copy(0,0,width,height);


    //wypelniamy obrzeza rami kolorem TŁA - białym - wypelniamy niewidocznych sasiadow
//    for (int i=0; height-startX+1; i++){
//        img->setPixel(startX-1, startY+i, qRgb(255,255,255));
//        //QTextStream(stdout) << "ustawiono tło białe: " << startX-1 << " " << startY+i;
//    }


    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int sumaBialychSasiadow=0;
            //pobieramy kolor pixela i 8 sąsiadów

            QRgb color = imgero->pixel(i,j);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            int kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada == 0 ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgero->pixel(i-1,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0 ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgero->pixel(i,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0  ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgero->pixel(i+1,j+1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0  ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgero->pixel(i-1,j);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0 ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgero->pixel(i+1,j);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0 ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgero->pixel(i-1,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0 ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }

            color = imgero->pixel(i,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0 ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }
            color = imgero->pixel(i+1,j-1);
            //QTextStream(stdout) << qRed(color) << qGreen(color) << qBlue(color);
            kolorSasiada =(qRed(color) + qGreen(color) + qBlue(color));
            //jesli sasiad jest czarny to sumaCzarnychSasiadow ++
            if (kolorSasiada > 0 ) {
                sumaBialychSasiadow++;
                //QTextStream(stdout) << "sumaCzarnychSasiadow" << sumaCzarnychSasiadow;
            }


            //jesli sumaCzarnychSasiadow jes wieksza od zera to malujemy na czarno wybrany pixel
            if (sumaBialychSasiadow > 0){
                img->setPixel(i, j, qRgb(255,255,255));
            }else {
                //malujemy na bialo jeś☺li brak czarnych sasiadow
                 img->setPixel(i,j, qRgb(0,0,0));
            }
        }
    }

    //*img = imgdyl->copy(0,0,width,height);
    update();

    //dylatacja
    on_komunikatChange("Dylatacja zrobiona");
}

void MainWindow::on_czarnobialy_clicked()
{
    czarnobialy = true;
    paint_on_black_and_white();
}

void MainWindow::on_reset_clicked()
{
    reset();

}

void MainWindow::on_dylatacja_clicked()
{
    if (czarnobialy == true) {
        dylatacja();
        on_komunikatChange("nastapiła DYLATACJA");

    } else {
        on_komunikatChange("Zmień obraz na czarno biały");
    }

}

void MainWindow::on_komunikatChange(string komunikat)
{
    ui->label1->setText(QString::fromStdString(komunikat));

}

void MainWindow::on_erozja_clicked()
{
    if (czarnobialy == true) {
        erozja();
        on_komunikatChange("nastapiła EROZJA");

    } else {
        on_komunikatChange("Zmień obraz na czarno biały");
    }
}
