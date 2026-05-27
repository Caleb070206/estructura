#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

// -------------------- PROTOTIPOS DE FUNCIONES --------------------
// Para que puedan ser usadas en cualquier orden

string intToString(int num);
string doubleToString(double num);
void limpiarBuffer();
void pausar();

// Lista de productos
struct Producto;
void agregarProducto(int id, string nombre, double precio, int stockInicial);
Producto* buscarProducto(int id);
bool productoExiste(int id);
void actualizarStock(int id, int cantidad, bool esIncremento);
void eliminarProducto(int id);
void actualizarProducto(int id, string nuevoNombre, double nuevoPrecio);
void mostrarInventario();

// Cola de pedidos
struct Pedido;
void registrarPedido(string cliente, int idProducto, int cantidad);
bool atenderPedido();
void mostrarPedidosPendientes();

// Pila de operaciones
struct Operacion;
void registrarOperacion(string descripcion);
void mostrarUltimasOperaciones(int cantidad = 10);

// Liberar memoria
void liberarMemoria();

// Submenús
void submenuGestionProductos();
void submenuGestionStock();
void submenuGestionPedidos();
void submenuConsultas();

// -------------------- ESTRUCTURAS --------------------
struct Producto {
    int id;
    string nombre;
    double precio;
    int stock;
    Producto* siguiente;
};

struct Pedido {
    string cliente;
    int idProducto;
    int cantidad;
    Pedido* siguiente;
};

struct Operacion {
    string descripcion;
    Operacion* siguiente;
};

// Variables globales
Producto* listaProductos = NULL;
Pedido* frenteCola = NULL;
Pedido* finalCola = NULL;
Operacion* pilaOperaciones = NULL;

// -------------------- FUNCIONES AUXILIARES --------------------
string intToString(int num) {
    ostringstream oss;
    oss << num;
    return oss.str();
}

string doubleToString(double num) {
    ostringstream oss;
    oss << num;
    return oss.str();
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

// -------------------- LISTA (PRODUCTOS) --------------------
Producto* buscarProducto(int id) {
    Producto* actual = listaProductos;
    while (actual != NULL) {
        if (actual->id == id)
            return actual;
        actual = actual->siguiente;
    }
    return NULL;
}

bool productoExiste(int id) {
    return buscarProducto(id) != NULL;
}

void agregarProducto(int id, string nombre, double precio, int stockInicial) {
    if (productoExiste(id)) {
        cout << "Error: Ya existe un producto con ID " << id << endl;
        registrarOperacion("Intento fallido: agregar producto duplicado ID " + intToString(id));
        return;
    }
    Producto* nuevo = new Producto;
    nuevo->id = id;
    nuevo->nombre = nombre;
    nuevo->precio = precio;
    nuevo->stock = stockInicial;
    nuevo->siguiente = listaProductos;
    listaProductos = nuevo;
    cout << "Producto agregado exitosamente." << endl;
    registrarOperacion("Producto agregado: ID " + intToString(id) + " - " + nombre + " (Stock: " + intToString(stockInicial) + ")");
}

void actualizarStock(int id, int cantidad, bool esIncremento) {
    Producto* prod = buscarProducto(id);
    if (prod == NULL) {
        cout << "Error: Producto con ID " << id << " no encontrado." << endl;
        registrarOperacion("Intento fallido: actualizar stock de producto inexistente ID " + intToString(id));
        return;
    }
    if (esIncremento) {
        prod->stock += cantidad;
        cout << "Stock aumentado en " << cantidad << ". Nuevo stock: " << prod->stock << endl;
        registrarOperacion("Ingreso: Prod ID " + intToString(id) + " +" + intToString(cantidad) + " | Nuevo stock: " + intToString(prod->stock));
    } else {
        if (prod->stock >= cantidad) {
            prod->stock -= cantidad;
            cout << "Stock reducido en " << cantidad << ". Nuevo stock: " << prod->stock << endl;
            registrarOperacion("Salida: Prod ID " + intToString(id) + " -" + intToString(cantidad) + " | Nuevo stock: " + intToString(prod->stock));
        } else {
            cout << "Error: Stock insuficiente. Disponible: " << prod->stock << ", solicitado: " << cantidad << endl;
            registrarOperacion("Intento fallido: salida sin stock suficiente de ID " + intToString(id));
        }
    }
}

void eliminarProducto(int id) {
    if (listaProductos == NULL) {
        cout << "Inventario vacio." << endl;
        return;
    }
    if (listaProductos->id == id) {
        Producto* temp = listaProductos;
        listaProductos = listaProductos->siguiente;
        delete temp;
        cout << "Producto eliminado." << endl;
        registrarOperacion("Producto eliminado: ID " + intToString(id));
        return;
    }
    Producto* actual = listaProductos;
    while (actual->siguiente != NULL && actual->siguiente->id != id) {
        actual = actual->siguiente;
    }
    if (actual->siguiente == NULL) {
        cout << "Producto no encontrado." << endl;
        registrarOperacion("Intento fallido: eliminar producto inexistente ID " + intToString(id));
        return;
    }
    Producto* temp = actual->siguiente;
    actual->siguiente = actual->siguiente->siguiente;
    delete temp;
    cout << "Producto eliminado." << endl;
    registrarOperacion("Producto eliminado: ID " + intToString(id));
}

void actualizarProducto(int id, string nuevoNombre, double nuevoPrecio) {
    Producto* prod = buscarProducto(id);
    if (prod == NULL) {
        cout << "Producto no encontrado." << endl;
        registrarOperacion("Intento fallido: actualizar producto inexistente ID " + intToString(id));
        return;
    }
    string nombreViejo = prod->nombre;
    double precioViejo = prod->precio;
    prod->nombre = nuevoNombre;
    prod->precio = nuevoPrecio;
    cout << "Producto actualizado correctamente." << endl;
    registrarOperacion("Producto actualizado: ID " + intToString(id) + " | Nombre: " + nombreViejo + " -> " + nuevoNombre + " | Precio: " + doubleToString(precioViejo) + " -> " + doubleToString(nuevoPrecio));
}

void mostrarInventario() {
    if (listaProductos == NULL) {
        cout << "No hay productos en el inventario." << endl;
        return;
    }
    cout << "\n===== INVENTARIO DE PRODUCTOS =====" << endl;
    cout << left << setw(8) << "ID" << setw(20) << "Nombre" << setw(10) << "Precio" << setw(10) << "Stock" << endl;
    cout << "----------------------------------------" << endl;
    Producto* actual = listaProductos;
    while (actual != NULL) {
        cout << left << setw(8) << actual->id << setw(20) << actual->nombre << setw(10) << actual->precio << setw(10) << actual->stock << endl;
        actual = actual->siguiente;
    }
    cout << "========================================" << endl;
}

// -------------------- COLA (PEDIDOS) --------------------
void registrarPedido(string cliente, int idProducto, int cantidad) {
    if (!productoExiste(idProducto)) {
        cout << "Error: El producto ID " << idProducto << " no existe. Pedido no registrado." << endl;
        registrarOperacion("Intento fallido: registrar pedido de producto inexistente ID " + intToString(idProducto));
        return;
    }
    Pedido* nuevo = new Pedido;
    nuevo->cliente = cliente;
    nuevo->idProducto = idProducto;
    nuevo->cantidad = cantidad;
    nuevo->siguiente = NULL;
    if (frenteCola == NULL) {
        frenteCola = nuevo;
        finalCola = nuevo;
    } else {
        finalCola->siguiente = nuevo;
        finalCola = nuevo;
    }
    cout << "Pedido registrado para " << cliente << " (Producto ID " << idProducto << ", Cantidad " << cantidad << ")" << endl;
    registrarOperacion("Pedido encolado: Cliente " + cliente + " - Prod ID " + intToString(idProducto) + " x" + intToString(cantidad));
}

bool atenderPedido() {
    if (frenteCola == NULL) {
        cout << "No hay pedidos pendientes." << endl;
        return false;
    }
    Pedido* pedido = frenteCola;
    frenteCola = frenteCola->siguiente;
    if (frenteCola == NULL) finalCola = NULL;

    Producto* prod = buscarProducto(pedido->idProducto);
    if (prod == NULL) {
        cout << "El producto ID " << pedido->idProducto << " ya no existe. Pedido cancelado." << endl;
        registrarOperacion("Pedido cancelado: Producto ID " + intToString(pedido->idProducto) + " no existe para " + pedido->cliente);
        delete pedido;
        return false;
    }
    if (prod->stock >= pedido->cantidad) {
        prod->stock -= pedido->cantidad;
        cout << "Atendiendo pedido: " << pedido->cliente << " - Producto: " << prod->nombre << " x" << pedido->cantidad << endl;
        cout << "Stock restante: " << prod->stock << endl;
        registrarOperacion("Pedido atendido: " + pedido->cliente + " - Prod ID " + intToString(pedido->idProducto) + " x" + intToString(pedido->cantidad) + " | Nuevo stock: " + intToString(prod->stock));
        delete pedido;
        return true;
    } else {
        cout << "Stock insuficiente para " << pedido->cliente << ". Disponible: " << prod->stock << ", solicitado: " << pedido->cantidad << endl;
        registrarOperacion("Pedido fallido por stock: " + pedido->cliente + " - Prod ID " + intToString(pedido->idProducto) + " (stock " + intToString(prod->stock) + ")");
        delete pedido;
        return false;
    }
}

void mostrarPedidosPendientes() {
    if (frenteCola == NULL) {
        cout << "No hay pedidos en espera." << endl;
        return;
    }
    cout << "\n===== PEDIDOS PENDIENTES (FIFO) =====" << endl;
    Pedido* actual = frenteCola;
    int num = 1;
    while (actual != NULL) {
        cout << num++ << ". Cliente: " << actual->cliente << " | Producto ID: " << actual->idProducto << " | Cantidad: " << actual->cantidad << endl;
        actual = actual->siguiente;
    }
    cout << "=====================================" << endl;
}

// -------------------- PILA (OPERACIONES) --------------------
void registrarOperacion(string descripcion) {
    Operacion* nueva = new Operacion;
    nueva->descripcion = descripcion;
    nueva->siguiente = pilaOperaciones;
    pilaOperaciones = nueva;
}

void mostrarUltimasOperaciones(int cantidad) {
    if (pilaOperaciones == NULL) {
        cout << "No hay operaciones registradas." << endl;
        return;
    }
    cout << "\n===== ULTIMAS OPERACIONES =====" << endl;
    Operacion* actual = pilaOperaciones;
    int cont = 0;
    while (actual != NULL && cont < cantidad) {
        cout << "- " << actual->descripcion << endl;
        actual = actual->siguiente;
        cont++;
    }
    cout << "===============================" << endl;
}

// -------------------- SUBMENÚS --------------------
void submenuGestionProductos() {
    int opcion, id, stock;
    string nombre;
    double precio;
    do {
        system("cls");
        cout << "\n--- GESTION DE PRODUCTOS ---" << endl;
        cout << "1. Registrar nuevo producto" << endl;
        cout << "2. Actualizar producto (nombre/precio)" << endl;
        cout << "3. Eliminar producto" << endl;
        cout << "0. Volver al menú principal" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        switch(opcion) {
            case 1:
                cout << "ID: "; cin >> id;
                if (productoExiste(id)) {
                    cout << "Ya existe un producto con ese ID." << endl;
                    pausar();
                    break;
                }
                cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                cout << "Precio: "; cin >> precio;
                cout << "Stock inicial: "; cin >> stock;
                agregarProducto(id, nombre, precio, stock);
                pausar();
                break;
            case 2:
                cout << "ID del producto a actualizar: "; cin >> id;
                if (!productoExiste(id)) {
                    cout << "Producto no encontrado." << endl;
                    pausar();
                    break;
                }
                cout << "Nuevo nombre: "; cin.ignore(); getline(cin, nombre);
                cout << "Nuevo precio: "; cin >> precio;
                actualizarProducto(id, nombre, precio);
                pausar();
                break;
            case 3:
                cout << "ID del producto a eliminar: "; cin >> id;
                eliminarProducto(id);
                pausar();
                break;
            case 0:
                break;
            default:
                cout << "Opcion invalida." << endl;
                pausar();
        }
    } while(opcion != 0);
}

void submenuGestionStock() {
    int opcion, id, cantidad;
    do {
        system("cls");
        cout << "\n--- GESTIÓN DE STOCK ---" << endl;
        cout << "1. Registrar ingreso de mercaderia" << endl;
        cout << "2. Registrar salida de productos" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        switch(opcion) {
            case 1:
                cout << "ID del producto: "; cin >> id;
                cout << "Cantidad a ingresar: "; cin >> cantidad;
                actualizarStock(id, cantidad, true);
                pausar();
                break;
            case 2:
                cout << "ID del producto: "; cin >> id;
                cout << "Cantidad a retirar: "; cin >> cantidad;
                actualizarStock(id, cantidad, false);
                pausar();
                break;
            case 0:
                break;
            default:
                cout << "Opción invalida." << endl;
                pausar();
        }
    } while(opcion != 0);
}

void submenuGestionPedidos() {
    int opcion, idProducto, cantidad;
    string cliente;
    do {
        system("cls");
        cout << "\n--- GESTION DE PEDIDOS ---" << endl;
        cout << "1. Registrar pedido de cliente" << endl;
        cout << "2. Atender siguiente pedido" << endl;
        cout << "3. Ver pedidos pendientes" << endl;
        cout << "0. Volver al menú principal" << endl;
        cout << "Opción: ";
        cin >> opcion;
        switch(opcion) {
            case 1:
                cout << "Nombre del cliente: "; cin.ignore(); getline(cin, cliente);
                cout << "ID del producto: "; cin >> idProducto;
                cout << "Cantidad: "; cin >> cantidad;
                registrarPedido(cliente, idProducto, cantidad);
                pausar();
                break;
            case 2:
                atenderPedido();
                pausar();
                break;
            case 3:
                mostrarPedidosPendientes();
                pausar();
                break;
            case 0:
                break;
            default:
                cout << "Opción inválida." << endl;
                pausar();
        }
    } while(opcion != 0);
}

void submenuConsultas() {
    int opcion;
    do {
        system("cls");
        cout << "\n--- CONSULTAS ---" << endl;
        cout << "1. Mostrar inventario (stock disponible)" << endl;
        cout << "2. Ver historial de operaciones recientes" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        switch(opcion) {
            case 1:
                mostrarInventario();
                pausar();
                break;
            case 2:
                mostrarUltimasOperaciones(10);
                pausar();
                break;
            case 0:
                break;
            default:
                cout << "Opcion invalida." << endl;
                pausar();
        }
    } while(opcion != 0);
}

// -------------------- MAIN --------------------
int main() {
    int opcionPrincipal;
    do {
        system("cls");
        cout << "\n===========================================" << endl;
        cout << "   SISTEMA DE INVENTARIO - MULTIVENTAS" << endl;
        cout << "===========================================" << endl;
        cout << "1. Gestion de Productos" << endl;
        cout << "2. Gestion de Stock" << endl;
        cout << "3. Gestion de Pedidos" << endl;
        cout << "4. Consultas" << endl;
        cout << "0. Salir" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "Opcion: ";
        cin >> opcionPrincipal;
        switch(opcionPrincipal) {
            case 1: submenuGestionProductos(); break;
            case 2: submenuGestionStock(); break;
            case 3: submenuGestionPedidos(); break;
            case 4: submenuConsultas(); break;
            case 0: cout << "Saliendo del sistema..." << endl; break;
            default: cout << "Opcion invalida." << endl; pausar();
        }
    } while(opcionPrincipal != 0);
    liberarMemoria();
    return 0;
}

void liberarMemoria() {
    Producto* prodActual = listaProductos;
    while (prodActual != NULL) {
        Producto* temp = prodActual;
        prodActual = prodActual->siguiente;
        delete temp;
    }
    Pedido* pedActual = frenteCola;
    while (pedActual != NULL) {
        Pedido* temp = pedActual;
        pedActual = pedActual->siguiente;
        delete temp;
    }
    Operacion* opActual = pilaOperaciones;
    while (opActual != NULL) {
        Operacion* temp = opActual;
        opActual = opActual->siguiente;
        delete temp;
    }
}
