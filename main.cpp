/**
 * Simulador de banco mediante una consola
 */
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstddef>
#include <regex>
using namespace std;

/** Constantes */
/** Expresiones regulares para validar nombres/apellidos y contraseñas. */
const regex NOMBRES_PATRON("[A-Za-z]");
const regex PASSWORD_PATRON("(\\w+)(\\w*)");
const bool ACTIVAR_PATRON_PASSWORD = false;
const bool CUENTA_ADMIN = true;
/* Enumeración que define los tipos de cuenta */
enum TipoCuenta {
    CAJA_AHORRO = 0,
    CUENTA_CORRIENTE = 1
};

struct Usuario {
    string usuario;
    string nombre;
    string apellido;
    string password;
    float dinero;
};

/** Patrón de diseño que nos permite crear una instanciación única. */
class RedLink {
    private:
        map<string, Usuario> users;
    public:
        void registrarUsuario(string nombre, Usuario userData) {
            users[nombre] = userData;
        }
        Usuario obtenerUsuarioPorNombre(string nombre) {
            return users[nombre];
        }

        vector<Usuario> filtrarConDinero() {
            vector<Usuario> lista;
            for(auto itr = users.begin(); itr != users.end(); itr++)
            {
                if(itr->second.dinero > 0)
                {
                    lista.push_back(itr->second); //agregamos la coincidencia, es decir, agregamos el usuario que tiene dinero disponible > 0
                }
            }
            return lista;
        }
};


int main() {
    RedLink banco;
    if(CUENTA_ADMIN)
    {
        banco.registrarUsuario("admin", {"admin", "Admin", "none", "admin", 0.0});
    }
    int request;
    bool dentroSistema = false;
    Usuario usuarioDentro;
    string response = "";
    do
    {
        if(!dentroSistema)
        {
            cout << "¡Hola usuario bienvenido a RedLink Banking! Elija las siguientes opciones:" << endl;
            cout << "1: para registrarse" << endl; 
            cout << "2: para ingresar al sistema" << endl;
            cin >> request;
            switch(request)
            {
                case 1:
                {
                    cout << "Ingrese su nombre de usuario:" << endl;
                    string us;
                    cin >> us;
                    
                    Usuario user = banco.obtenerUsuarioPorNombre(us);
                    if(user.nombre == "" && user.password == "")
                    {
                        string  nombre, apellido,password;
                        float dinero = 0.0;
                        cout << "Ahora ingrese su nombre:" << endl;
                        cin >> nombre;
                        if(regex_search(nombre, NOMBRES_PATRON))
                        {
                             cout << "Ahora ingrese su apellido:" << endl;
                             cin >> apellido;
                             if(regex_search(apellido, NOMBRES_PATRON))
                             {
                                 cout << "Ahora ingrese su contraseña:" << endl;
                                 cin >> password;
                                 cout << "Es un patron correcto?: " << (regex_search(password, PASSWORD_PATRON) ? "si" : "no") << endl;
                                 cout << "Ingrese la cantidad de dinero que depositará:" << endl;
                                 cin >> dinero;
                                 banco.registrarUsuario(us, {us, nombre, apellido, password, dinero});//ingresamos el usuario al map
                                 cout << "Usuario " << us << " registrado con éxito!" << endl;
                             }
                             else
                             {
                                  cout << "No puede crear un apellido con números o símbolos solo se permiten [A-Z] y [a-z]." << endl;
                             }
                        }
                        else
                        {
                            cout << "No puede crear un nombre con números o símbolos solo se permiten [A-Z] y [a-z]." << endl;
                        }
                        
                    }
                    else
                    {
                        cout << "El usuario " << us << " ya existe, intente crear uno diferente." << endl;
                    }
                    break;
                }
                case 2:
                {
                    string us, pass;
                    cout << "Ingrese su nombre de usuario:" << endl;
                    cin >> us;
                    Usuario user = banco.obtenerUsuarioPorNombre(us);
                    cout << user.nombre << endl;
                    if(user.usuario != "")
                    {
                        if(user.password == pass)
                        {
                            dentroSistema = true;
                            usuarioDentro = user;
                        }
                        else
                        {
                            cout << "¡La contraseña no es la correcta!" << endl;
                        }
                    }
                    else
                    {
                        cout << "El usuario \"" << us << "\" es inexistente, intentelo de nuevo." << endl;
                    }
                    break;
                }
            }
        }
        else
        {
            cout << "¡Hola " << usuarioDentro.nombre << "! Usted ingresó al sistema, elija las siguientes opciones:" << endl;
            if(usuarioDentro.nombre == "admin")
            {
                
            }
            else
            {
                cout << "1: Para ingresar dinero." << endl;
                cout << "2: Para imprimir ticket." << endl;
                cout << "3: Salir del sistema." << endl;
            }
            cin >> request;
            switch(request)
            {
                case 1:
                {
                    float dinero = 0.0;
                    cout << "Ingrese la cantidad de dinero que desee" << endl;
                    cin >> dinero;
                    float dineroAnterior = usuarioDentro.dinero;
                    usuarioDentro.dinero += dinero;
                    banco.registrarUsuario(usuarioDentro.usuario, usuarioDentro);//re-ingresamos los valores cambiados
                    cout << "Se agregaron " << dinero << "$ a " << dineroAnterior << "$" << (dineroAnterior > 0 ? "que habían anteriormente" : "") << endl;
                }
                break;
            }
        }
        
    } while ((response != "no"));
    
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
}