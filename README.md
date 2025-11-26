# HelpDesk App - Sistema de Tickets

**Proyecto:** Help Desk App
**Curso:** Ciberseguridad  
**Autor:** Miguel Angel Monsalve Valencia  
**Lenguaje:** C++

## ¿Qué hace este programa?

Sistema de tickets estilo help desk con:
- Registro e inicio de sesión de usuarios
- Creación y gestión de tickets (CRUD)
- Contraseñas hasheadas con SHA-256
- Validación de inputs contra inyecciones
- Logging de acciones

## Requisitos

- **MSYS2** con MinGW-w64: https://www.msys2.org/
- **OpenSSL** para hashing

## Instalación Rápida

1. Instalar MSYS2 desde https://www.msys2.org/

2. Abrir **MSYS2 MinGW 64-bit** y ejecutar:
```bash
pacman -Syu
pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-openssl
```

## Compilar

 **MSYS2 MinGW 64-bit**:

``bash
cd /c/ruta/a/tu/proyecto/HelpDeskApp
g++ -std=c++17 -Wall -Wextra -g -I src src/*.cpp -o build/HelpDeskApp.exe -lcrypto
```

## Ejecutar

```bash
./build/HelpDeskApp.exe
```

## Estructura del Proyecto

```
HelpDeskApp/
├── src/           # Código fuente (.cpp, .h)
├── data/          # Archivos de datos (users.txt, tickets.txt, audit.log)
├── build/         # Ejecutable compilado
└── README.md
```

## Características de Seguridad

- ✅ SHA-256 para contraseñas
- ✅ Validación de inputs (SQL injection básica)
- ✅ Prevención de CSV injection
- ✅ Audit logging
- ✅ Control de roles (admin/user)

## Errores Comunes

**`undefined reference to SHA256`**  
→ Falta `-lcrypto` en el comando de compilación o no instalaste OpenSSL

**`g++: command not found`**  
→ Usa **MSYS2 MinGW 64-bit**, no MSYS2 normal

**Los datos no se guardan**  
→ Ejecuta desde la raíz del proyecto, no desde `build/`

---

**Documentación completa:** Ver `INSTRUCCIONES.md` para guía paso a paso
