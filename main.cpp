#include <iostream>
#include <string>

using namespace std;

// Constante para el tamaño máximo de las estructuras estáticas
const int MAX = 200; 

// -------------------- REGISTROS --------------------
struct Producto {
    int id;
    string nombre;
    double precio;
    int stock;
};

struct Pedido {
    string cliente;
    int idProducto;
    int cantidad;
};

struct Operacion {
    string descripcion;
};

// -------------------- PROTOTIPOS DE FUNCIONES --------------------
void menuPrincipal();
void pausar();

// Funciones de Gestión
int buscarProducto(Producto productos[], int n, int id);
void agregarProducto(Producto productos[], int &n, Operacion operaciones[], int &nOp);
void actualizarStock(Producto productos[], int n, Operacion operaciones[], int &nOp, bool esIncremento);
void eliminarProducto(Producto productos[], int &n, Operacion operaciones[], int &nOp);
void actualizarProducto(Producto productos[], int n, Operacion operaciones[], int &nOp);
void mostrarInventario(Producto productos[], int n);

// Funciones de Pedidos (Simulación estática con Arrays)
void registrarPedido(Pedido pedidos[], int &nPed, Producto productos[], int n, Operacion operaciones[], int &nOp);
void atenderPedido(Pedido pedidos[], int &nPed, Producto productos[], int n, Operacion operaciones[], int &nOp);
void mostrarPedidosPendientes(Pedido pedidos[], int nPed);

// Historial (Pila simulada sobre array)
void registrarOperacion(Operacion operaciones[], int &nOp, string desc);
void mostrarUltimasOperaciones(Operacion operaciones[], int nOp);

// -------------------- MAIN --------------------
int main() {
    
    // Declaración de estructuras de datos estáticas 
    Producto inventario[MAX];
    Pedido colaPedidos[MAX];
    Operacion historial[MAX];
    
    // Contadores de elementos en los arreglos
    int cantProductos = 0;
    int cantPedidos = 0;
    int cantOperaciones = 0;
    
    int opcionPrincipal;
    int opcionSubmenu;

    do {
        system("cls");
        cout << "=================================================" << endl;
        cout << "   ESTRUCTURA DE DATOS: INVENTARIO MULTIVENTAS   " << endl;
        cout << "=================================================" << endl;
        cout << " [1] Gestion de Productos" << endl;
        cout << " [2] Gestion de Stock (Ingresos/Salidas)" << endl;
        cout << " [3] Gestion de Pedidos" << endl;
        cout << " [4] Consultas e Historial" << endl;
        cout << " [0] Salir del Sistema" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << " Elija una opción: ";
        cin >> opcionPrincipal;

        switch(opcionPrincipal) {
            case 1: // SUBMENÚ GESTIÓN DE PRODUCTOS
                do {
                    system("cls");
                    cout << "--- GESTIÓN DE PRODUCTOS ---" << endl;
                    cout << "1. Registrar nuevo producto" << endl;
                    cout << "2. Actualizar producto (Nombre/Precio)" << endl;
                    cout << "3. Eliminar producto de la lista" << endl;
                    cout << "0. Volver al menu principal" << endl;
                    cout << "Opción: ";
                    cin >> opcionSubmenu;
                    
                    if (opcionSubmenu == 1) agregarProducto(inventario, cantProductos, historial, cantOperaciones);
                    else if (opcionSubmenu == 2) actualizarProducto(inventario, cantProductos, historial, cantOperaciones);
                    else if (opcionSubmenu == 3) eliminarProducto(inventario, cantProductos, historial, cantOperaciones);
                } while (opcionSubmenu != 0);
                break;

            case 2: // SUBMENÚ GESTIÓN DE STOCK
                do {
                    system("cls");
                    cout << "--- GESTIÓN DE STOCK ---" << endl;
                    cout << "1. Registrar ingreso de mercaderia (+)" << endl;
                    cout << "2. Registrar salida de productos (-)" << endl;
                    cout << "0. Volver al menu principal" << endl;
                    cout << "Opción: ";
                    cin >> opcionSubmenu;
                    
                    if (opcionSubmenu == 1) actualizarStock(inventario, cantProductos, historial, cantOperaciones, true);
                    else if (opcionSubmenu == 2) actualizarStock(inventario, cantProductos, historial, cantOperaciones, false);
                } while (opcionSubmenu != 0);
                break;

            case 3: // SUBMENÚ GESTIÓN DE PEDIDOS
                do {
                    system("cls");
                    cout << "--- GESTION DE PEDIDOS ---" << endl;
                    cout << "1. Registrar pedido de cliente" << endl;
                    cout << "2. Atender siguiente pedido en cola" << endl;
                    cout << "3. Ver lista de pedidos pendientes" << endl;
                    cout << "0. Volver al menu principal" << endl;
                    cout << "Opción: ";
                    cin >> opcionSubmenu;
                    
                    if (opcionSubmenu == 1) registrarPedido(colaPedidos, cantPedidos, inventario, cantProductos, historial, cantOperaciones);
                    else if (opcionSubmenu == 2) atenderPedido(colaPedidos, cantPedidos, inventario, cantProductos, historial, cantOperaciones);
                    else if (opcionSubmenu == 3) mostrarPedidosPendientes(colaPedidos, cantPedidos);
                } while (opcionSubmenu != 0);
                break;

            case 4: // SUBMENÚ CONSULTAS
                do {
                    system("cls");
                    cout << "--- CONSULTAS GENERALES ---" << endl;
                    cout << "1. Mostrar inventario disponible" << endl;
                    cout << "2. Ver historial de operaciones realizadas" << endl;
                    cout << "0. Volver al menú principal" << endl;
                    cout << "Opción: ";
                    cin >> opcionSubmenu;
                    
                    if (opcionSubmenu == 1) { mostrarInventario(inventario, cantProductos); pausar(); }
                    else if (opcionSubmenu == 2) { mostrarUltimasOperaciones(historial, cantOperaciones); pausar(); }
                } while (opcionSubmenu != 0);
                break;

            case 0:
                cout << "\nSaliendo del sistema de inventario..." << endl;
                break;
            default:
                cout << "\nOpción no valida." << endl;
                pausar();
        }
    } while(opcionPrincipal != 0);

    return 0;
}

// -------------------- AUXILIARES --------------------
void pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
}

// -------------------- LÓGICA DE PRODUCTOS --------------------
int buscarProducto(Producto productos[], int n, int id) {
    for (int i = 0; i < n; i++) {
        if (productos[i].id == id) {
            return i; // Retorna la posición/índice en el arreglo
        }
    }
    return -1; // No encontrado
}

void agregarProducto(Producto productos[], int &n, Operacion operaciones[], int &nOp) {
    if (n >= MAX) {
        cout << "Error: Capacidad maxima del inventario alcanzada." << endl;
        pausar();
        return;
    }
    
    int id, stockInicial;
    string nombre;
    double precio;
    
    cout << "\n[REGISTRAR PRODUCTO]" << endl;
    cout << "Ingrese ID: "; cin >> id;
    
    if (buscarProducto(productos, n, id) != -1) {
        cout << "Error: Ya existe un producto con el ID ingresado." << endl;
        registrarOperacion(operaciones, nOp, "Intento fallido: agregar producto duplicado ID");
        pausar();
        return;
    }
    
    cout << "Nombre del Producto: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Precio: "; cin >> precio;
    cout << "Stock Inicial: "; cin >> stockInicial;
    
    // Guardamos los datos en el índice correspondiente usando referencias (Semana 5 y 7)
    productos[n].id = id;
    productos[n].nombre = nombre;
    productos[n].precio = precio;
    productos[n].stock = stockInicial;
    n++; // Incrementamos el tamaño del arreglo
    
    cout << "¡Producto añadido exitosamente!" << endl;
    registrarOperacion(operaciones, nOp, "Producto registrado: " + nombre);
    pausar();
}

void actualizarProducto(Producto productos[], int n, Operacion operaciones[], int &nOp) {
    int id;
    cout << "\n[ACTUALIZAR PRODUCTO]" << endl;
    cout << "Ingrese el ID del producto a modificar: "; cin >> id;
    
    int pos = buscarProducto(productos, n, id);
    if (pos == -1) {
        cout << "Producto no encontrado." << endl;
        pausar();
        return;
    }
    
    // Uso de Punteros para acceder y modificar directamente (Semana 7)
    Producto* ptrP = &productos[pos];
    
    cout << "Nombre actual: " << ptrP->nombre << " -> Ingrese nuevo nombre: ";
    cin.ignore();
    getline(cin, ptrP->nombre);
    cout << "Precio actual: " << ptrP->precio << " -> Ingrese nuevo precio: ";
    cin >> ptrP->precio;
    
    cout << "¡Producto actualizado correctamente!" << endl;
    registrarOperacion(operaciones, nOp, "Se modificaron datos del producto ID " + to_string(id));
    pausar();
}

void eliminarProducto(Producto productos[], int &n, Operacion operaciones[], int &nOp) {
    int id;
    cout << "\n[ELIMINAR PRODUCTO]" << endl;
    cout << "Ingrese ID del producto a remover: "; cin >> id;
    
    int pos = buscarProducto(productos, n, id);
    if (pos == -1) {
        cout << "Producto no encontrado." << endl;
        pausar();
        return;
    }
    
    // Algoritmo de eliminación en arreglos estáticos (Semana 2 y 4): Mover elementos hacia la izquierda
    for (int i = pos; i < n - 1; i++) {
        productos[i] = productos[i + 1];
    }
    n--; // Se reduce el tamaño lógico del vector
    
    cout << "El producto ha sido removido del inventario." << endl;
    registrarOperacion(operaciones, nOp, "Producto eliminado ID: " + to_string(id));
    pausar();
}

void actualizarStock(Producto productos[], int n, Operacion operaciones[], int &nOp, bool esIncremento) {
    int id, cantidad;
    cout << "\n[GESTIÓN DE STOCK]" << endl;
    cout << "Ingrese ID del producto: "; cin >> id;
    
    int pos = buscarProducto(productos, n, id);
    if (pos == -1) {
        cout << "Producto no encontrado." << endl;
        pausar();
        return;
    }
    
    cout << "Cantidad: "; cin >> cantidad;
    
    if (esIncremento) {
        productos[pos].stock += cantidad;
        cout << "Stock incrementado. Nuevo stock: " << productos[pos].stock << endl;
        registrarOperacion(operaciones, nOp, "Ingreso de mercadería para ID " + to_string(id));
    } else {
        if (productos[pos].stock >= cantidad) {
            productos[pos].stock -= cantidad;
            cout << "Stock disminuido. Nuevo stock: " << productos[pos].stock << endl;
            registrarOperacion(operaciones, nOp, "Salida de mercadería para ID " + to_string(id));
        } else {
            cout << "Error: Stock insuficiente. Solo quedan " << productos[pos].stock << " unidades." << endl;
        }
    }
    pausar();
}

void mostrarInventario(Producto productos[], int n) {
    if (n == 0) {
        cout << "\nEl inventario se encuentra vacio." << endl;
        return;
    }
    cout << "\n=============================================" << endl;
    cout << "            INVENTARIO DE PRODUCTOS          " << endl;
    cout << "=============================================" << endl;
    cout << "ID\tNombre\t\tPrecio\tStock" << endl;
    cout << "---------------------------------------------" << endl;
    for (int i = 0; i < n; i++) {
        cout << productos[i].id << "\t" 
             << productos[i].nombre << "\t\t" 
             << productos[i].precio << "\t" 
             << productos[i].stock << endl;
    }
    cout << "=============================================" << endl;
}

// -------------------- LÓGICA DE PEDIDOS (Arreglo Estático) --------------------
void registrarPedido(Pedido pedidos[], int &nPed, Producto productos[], int n, Operacion operaciones[], int &nOp) {
    if (nPed >= MAX) {
        cout << "Error: La cola de pedidos está llena." << endl;
        pausar();
        return;
    }
    
    string cliente;
    int idProd, cant;
    
    cout << "\n[REGISTRAR PEDIDO]" << endl;
    cout << "Nombre del Cliente: ";
    cin.ignore();
    getline(cin, cliente);
    cout << "ID del Producto solicitado: "; cin >> idProd;
    
    if (buscarProducto(productos, n, idProd) == -1) {
        cout << "Error: El producto indicado no existe en el catalogo." << endl;
        pausar();
        return;
    }
    
    cout << "Cantidad demandada: "; cin >> cant;
    
    pedidos[nPed].cliente = cliente;
    pedidos[nPed].idProducto = idProd;
    pedidos[nPed].cantidad = cant;
    nPed++;
    
    cout << "Pedido registrado para procesamiento." << endl;
    registrarOperacion(operaciones, nOp, "Pedido encolado - Cliente: " + cliente);
    pausar();
}

void atenderPedido(Pedido pedidos[], int &nPed, Producto productos[], int n, Operacion operaciones[], int &nOp) {
    if (nPed == 0) {
        cout << "\nNo existen pedidos pendientes por atender." << endl;
        pausar();
        return;
    }
    
    // El primer elemento ingresado en el arreglo es el primero en salir (Simulación FIFO - Semana 2)
    Pedido primerPedido = pedidos[0];
    int posProd = buscarProducto(productos, n, primerPedido.idProducto);
    
    if (posProd == -1) {
        cout << "El producto del pedido ya no existe. Cancelando pedido..." << endl;
    } else {
        if (productos[posProd].stock >= primerPedido.cantidad) {
            productos[posProd].stock -= primerPedido.cantidad;
            cout << "\nAtendiendo a: " << primerPedido.cliente << endl;
            cout << "Producto despachado: " << productos[posProd].nombre << " x" << primerPedido.cantidad << endl;
            registrarOperacion(operaciones, nOp, "Pedido exitoso - Cliente: " + primerPedido.cliente);
        } else {
            cout << "\nStock insuficiente para surtir el pedido de " << primerPedido.cliente << endl;
            registrarOperacion(operaciones, nOp, "Pedido rechazado por falta de stock - Cliente: " + primerPedido.cliente);
        }
    }
    
    // Desplazar el resto de los pedidos a la izquierda para mantener la estructura estática
    for (int i = 0; i < nPed - 1; i++) {
        pedidos[i] = pedidos[i + 1];
    }
    nPed--; // Reducir cantidad de pedidos activos
    pausar();
}

void mostrarPedidosPendientes(Pedido pedidos[], int nPed) {
    if (nPed == 0) {
        cout << "\nNo hay pedidos en cola de espera." << endl;
        pausar();
        return;
    }
    cout << "\n=============================================" << endl;
    cout << "              PEDIDOS PENDIENTES             " << endl;
    cout << "=============================================" << endl;
    for (int i = 0; i < nPed; i++) {
        cout << (i + 1) << ". Cliente: " << pedidos[i].cliente 
             << " | Prod ID: " << pedidos[i].idProducto 
             << " | Cantidad: " << pedidos[i].cantidad << endl;
    }
    cout << "=============================================" << endl;
    pausar();
}

// -------------------- HISTORIAL DE OPERACIONES --------------------
void registrarOperacion(Operacion operaciones[], int &nOp, string desc) {
    if (nOp < MAX) {
        operaciones[nOp].descripcion = desc;
        nOp++;
    } else {
        // Si el arreglo se llena, desplazamos para emular comportamiento circular/pila estática
        for (int i = 0; i < MAX - 1; i++) {
            operaciones[i] = operaciones[i + 1];
        }
        operaciones[MAX - 1].descripcion = desc;
    }
}

void mostrarUltimasOperaciones(Operacion operaciones[], int nOp) {
    if (nOp == 0) {
        cout << "\nNo se registran movimientos en la sesion." << endl;
        return;
    }
    cout << "\n=============================================" << endl;
    cout << "            HISTORIAL DE OPERACIONES         " << endl;
    cout << "=============================================" << endl;
    // Muestra del más reciente al más antiguo
    for (int i = nOp - 1; i >= 0; i--) {
        cout << "- " << operaciones[i].descripcion << endl;
    }
    cout << "=============================================" << endl;
}