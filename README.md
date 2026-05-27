# 🧾 Sistema de Inventario - Multiventas Masid's EIRL

Proyecto desarrollado para el curso de **Estructura de Datos** – Universidad Continental.  
Implementa un sistema de control de inventario usando listas enlazadas, colas FIFO y pilas LIFO en C++.

## 📌 Descripción del problema
La empresa *Multiventas Masid's EIRL* (depósito de gaseosas y cervezas) realizaba su control de inventario de forma manual, lo que generaba:
- Errores en registros de ingresos y salidas (duplicados, incorrectos).
- Pérdidas económicas y desorden en el almacén.
- Atención a clientes sin respetar el orden de llegada.

## ✅ Solución propuesta
Se desarrolló un sistema automatizado con:
- **Lista enlazada** → Almacenamiento y gestión del inventario de productos.
- **Cola FIFO** → Registro y atención de pedidos por orden de llegada.
- **Pila LIFO** → Historial de las últimas operaciones realizadas.

## 🚀 Características
- Registro, actualización y eliminación de productos.
- Control de ingresos y salidas de mercadería.
- Registro de pedidos de clientes (respetando orden).
- Atención automática del siguiente pedido en la cola.
- Consulta de stock disponible.
- Historial de movimientos recientes (pila).
- Menú principal con submenús organizados por función.

## 🛠️ Requisitos de compilación
- Compilador C++ compatible con el estándar **C++98/03** (por ejemplo: Dev-C++ 5.11, Turbo C++, g++ con flag `-std=c++98`).
- Sistema operativo **Windows** (usa `system("cls")` para limpiar pantalla).  
  *Si usas Linux/Mac, reemplaza `system("cls")` por `system("clear")` en el código.*

## 📦 Cómo compilar y ejecutar

### Usando g++ (MinGW / Linux)
```bash
g++ -o inventario.exe src/main.cpp
./inventario.exe
