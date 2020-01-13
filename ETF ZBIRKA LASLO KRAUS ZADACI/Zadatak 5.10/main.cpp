#include <iostream>
using namespace std;

#define MAX_ISPITA 20

class Osoba
{
private:
    string ime;
    unsigned godine;
public:
    Osoba()
    {
        ime = "";
        godine = 0;
    }
    Osoba(string i, int j)
    {
        ime = i;
        godine  = j;
    }
    string getIme()
    {
        return ime;
    }
    unsigned getGodine()
    {
        return godine;
    }
    void setIme(string i)
    {
        ime = i;
    }
    void setGodine(int g)
    {
        godine  = g;
    }

    void operator >(const Osoba& o)
    {
        if(godine > o.godine)
        {
            cout << "Osoba " << ime << "(" << godine << ") je starija od "
                 << "Osobe " << o.ime << "(" << o.godine << ")" << endl;
        }
        else
        {
            cout << "Osoba " << o.ime << "(" << o.godine << ") je starija od "
                 << "Osobe " << ime << "(" << godine << ")" << endl;
        }
    }

    friend ostream &operator<<( ostream &out, const Osoba &o)
    {
        /// ISPIS
        cout << o.ime << " (" << o.godine << ")" << endl;

        return out;
    }
};

class Student : public Osoba
{
private:
    float ocene;
    int polozeno;
public:
    Student() : Osoba()
    {
        polozeno = 1;
        ocene = 0.0;
    }
    Student(string i, int g) : Osoba(i, g)
    {
        polozeno = 1;
        ocene = 0.0;
    }
    Student& operator+=(const float oc)
    {
        if(polozeno <= MAX_ISPITA) {
            this -> ocene = ( this -> ocene + oc);
            polozeno++;
        }
        else {
            cout << endl << "POPUNJENA MESTA ZA SVE OCENE! (" << MAX_ISPITA << ")" << endl;
        }

        return *this;
    }
    int slobodnoOcena()
    {
        return (MAX_ISPITA - polozeno);
    }
    float avgOcene() { return ocene / (polozeno - 1); }

    friend ostream &operator<<(ostream &out, Student &s)
    {
        /// ISPIS
        cout << s.getIme() << " (" << s.getGodine() << ") / " << s.avgOcene() << endl;

        return out;
    }
};

class Imenik : Osoba
{
    private: unsigned kapacitet;
    public:
        Imenik() { kapacitet = 0; }
    Imenik& operator+=(const Osoba &o)
    {
        if(kapacitet < 10) {
      /*      this -> ocene = ( this -> ocene + oc);
            kapacitet++;
        }
        else {
            cout << endl << "POPUNJENA MESTA ZA SVE OCENE! (" << MAX_ISPITA << ")" << endl;
        }

        return *this;
        */
    }
};

int main(void)
{
    Osoba o1("Danijel", 22), o2("Mau", 25);
    o1 > o2;
    cout << o1 << o2;

    Student s1("Daaa", 63), s2("lldslda", 20);
    cout << s1 << s2 << endl;

    s1 += 5.6;
    s1 += 4.4;
    s2 += 10;
    s2 += 9;

    cout << s1 << s2 << endl;

    return 0;
}