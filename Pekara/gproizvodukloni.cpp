#include "gproizvodukloni.h"
#include "ui_gproizvodukloni.h"
#include <QMessageBox>
#include "konekcija.h"
#include <QDateTime>

gProizvodUkloni::gProizvodUkloni(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gProizvodUkloni)
{
    ui->setupUi(this);

    on_dugmeUcitajAz_clicked();

    // Podešavanje pozadinske slike
    QPixmap pixmap;
    pixmap.load(":/Resursi/Dialozi/ukloni.png");

    QPalette palette;
    palette.setBrush(QPalette::Window, pixmap);
    this->setPalette(palette);
}

gProizvodUkloni::~gProizvodUkloni()
{
    delete ui;
}

void gProizvodUkloni::on_dugmeIzlazAz_clicked()
{
    this -> close();
}

void gProizvodUkloni::on_dugmeUcitajAz_clicked()
{
    Konekcija baza;
    QSqlQueryModel *modal = new QSqlQueryModel();

    baza.dbOpen();

    QSqlQuery upit;

    upit.exec("SELECT Sifra FROM gotoviProizvodi");
    modal->setQuery(upit);

    ui -> comboBoxID -> setModel(modal);

    baza.dbClose();
}

void gProizvodUkloni::on_comboBoxID_currentIndexChanged()
{
    Konekcija baza;
    QString trenutniIndeks = ui -> comboBoxID -> currentText();
    QString jedMere;
    QSqlQuery upit;

    baza.dbOpen();

    upit.exec("SELECT Naziv, Jed_Mere, Kolicina, Cena FROM gotoviProizvodi WHERE Sifra = '" + trenutniIndeks + "'");

    if( upit.exec() )
    {
        while( upit.next() )
        {
            ui -> lineEdit_naziv_az -> setText(upit.value(0).toString());
            ui -> lineEdit_kolicina_az -> setText(upit.value(2).toString());
            ui -> lineEdit_nbCena -> setText(upit.value(3).toString());

            jedMere = upit.value(1).toString();
            // setovanje indeksa comboBox-a za jedinicu mere
            if(jedMere == "Gram")
                ui -> magacinIzbor_az -> setCurrentIndex(0);
            else if(jedMere == "Kilogram")
                ui -> magacinIzbor_az -> setCurrentIndex(1);
            else if(jedMere == "Komad")
                ui -> magacinIzbor_az -> setCurrentIndex(2);
            else
                ui -> magacinIzbor_az -> setCurrentIndex(3);
         }
    }

    baza.dbClose();
}

void gProizvodUkloni::on_ukloniDugme_clicked()
{
    Konekcija baza;
    baza.dbOpen();
    QString sif = ui -> comboBoxID -> currentText();
    int sifra = sif.toInt();

    QString Naziv = ui -> lineEdit_naziv_az -> text();
    QString kol = ui -> lineEdit_kolicina_az -> text();
    QString NC = ui -> lineEdit_nbCena -> text();
    QString JedinicaMere = ui -> magacinIzbor_az -> currentText();

    QSqlQuery upit;
    QMessageBox msgBox;

    QDate cd = QDate::currentDate();
    QTime ct = QTime::currentTime();

    QString datum, vreme, Datum, Opis;

    datum = cd.toString();
    vreme = ct.toString();

    Datum = datum + " " + vreme;
    Opis = "Uklonjen finalni proizvod " + Naziv + ", količina " + kol + " " + JedinicaMere.toLower() + ", po ceni od " + NC + " dinara. ";

    QSqlQuery izmena;

    izmena.prepare("INSERT INTO istorijaIzmena(Datum, Opis) VALUES(:Datum, :Opis)");

    izmena.bindValue(":Datum", Datum);
    izmena.bindValue(":Opis", Opis);

    izmena.exec();

    upit.prepare("DELETE FROM gotoviProizvodi WHERE Sifra = :sifra");

    upit.bindValue(":sifra", sifra);

    // PORUKA O USPESNOM DODAVANJU
    msgBox.setWindowTitle("Obaveštenje");
    msgBox.setText("Proizvod je uspešno uklonjen!");
    msgBox.setIcon(QMessageBox::Information);

    upit.exec();
    msgBox.exec();

    ui -> lineEdit_naziv_az -> clear();
    ui -> lineEdit_kolicina_az -> clear();
    ui -> lineEdit_nbCena -> clear();



    on_dugmeUcitajAz_clicked(); // ponovo ucitaj podatke

    baza.dbClose();
}