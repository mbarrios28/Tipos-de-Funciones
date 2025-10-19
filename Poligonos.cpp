#include <iostream>
#include <cmath>
#include <functional>
#include <vector>
using namespace std;

class Poligono;

class Punto {
    friend class Poligono;
    private:
        int x;
        int y;
    
    public:
        Punto (){}
        Punto (int x, int y){
            this->x = x;
            this->y = y;
        }
        void setX (int x){
            this->x = x;
        }
        void setY (int y){
            this->y = y;
        }
        int getX () const{
            return x;
        }
        int getY() const{
            return y;
        }

        friend double distancia( Punto& p1, Punto& p2);

        bool operator==(Punto& p){
            return p.getX() == x && p.getY() == y;
        }
};

double distancia (Punto& p1, Punto& p2){
    return sqrt(pow(p1.getX()-p2.getX(),2) + pow(p1.getY()-p2.getY(),2));
}
class Poligono {
    private:
        int n;
        Punto* puntos;
        int indice = 0;
        inline bool lleno (){
            return indice == n;
        }
    public:
        function<double()> perimetro;
        Poligono (int n){
            this->n = n;
            puntos = new Punto[n];

            this->perimetro = [this]() -> double {
                double peri = 0;
                for (int i = 0; i < this->n; i++) {
                    peri += distancia(puntos[i], puntos[(i + 1) % this->n]);
                }
                return peri;
            };
        }
        
        ~Poligono() {
            delete[] puntos;
        }

        void addPunto(Punto x){
            if (!lleno()){
                puntos[indice] = x;
                indice++;
            }
        }

        double area (){
            // metodo para calcular área con la fórmula del zapato
            double area = 0;
            for (int i = 0; i < n; ++i) {
                int j = (i + 1) % n;
                double xi = puntos[i].getX();
                double yi = puntos[i].getY();
                double xj = puntos[j].getX();
                double yj = puntos[j].getY();
                area += xi * yj - xj * yi; 
            }
            return abs(area)/2;
        }
        
        void mostrarPoligono(){
            cout << "\nEste polígono tiene " << n << " lados." << endl;
            cout << "Su perímetro es: " << perimetro() << endl;
            cout << "Su area es: " << area() << endl;
        }
};

class Triangulo : public Poligono {
    public:
        Triangulo() : Poligono(3) {}
};

class Cuadrilatero : public Poligono {
    public:
        Cuadrilatero() : Poligono(4) {}
};

int main() {
    vector<Poligono*> figuras; //Se usa covarianza para que un vector de polígonos acepte Triangulos o Cuadrados.
    int opcion;

    do {
        cout << "\nMENÚ POLÍGONOS" << endl;
        cout << "1. Crear triángulo" << endl;
        cout << "2. Crear cuadrilátero" << endl;
        cout << "3. Mostrar todos los polígonos" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                Triangulo* t = new Triangulo(); //Al usar punteros, se usa covarianza
                cout << "Ingrese los 3 puntos (x y):" << endl;
                for (int i = 0; i < 3; i++) {
                    int x, y;
                    cout << "Punto " << i + 1 << ": ";
                    cin >> x >> y;
                    t->addPunto(Punto(x, y));
                }
                figuras.push_back(t);//Por ello está linea no da error
                break;
            }
            case 2: {
                Cuadrilatero* c = new Cuadrilatero();
                cout << "Ingrese los 4 puntos (x y):" << endl;
                for (int i = 0; i < 4; i++) {
                    int x, y;
                    cout << "Punto " << i + 1 << ": ";
                    cin >> x >> y;
                    c->addPunto(Punto(x, y));
                }
                figuras.push_back(c);
                break;
            }
            case 3: {
                cout << "\nPOLÍGONOS ALMACENADOS" << endl;
                if (figuras.empty()) {
                    cout << "No hay polígonos registrados." << endl;
                } else {
                    for (Poligono* p : figuras) {
                        p->mostrarPoligono();
                    }
                }
                break;
            }
            case 4:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción inválida." << endl;
                break;
        }

    } while (opcion != 4);

    for (Poligono* p : figuras) delete p; //Liberar memoria

    return 0;
}