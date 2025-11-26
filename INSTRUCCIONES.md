# Instrucciones para Ejecutar HelpDeskApp

## Requisitos del Sistema
- **Sistema Operativo:** Windows 10/11
- **MSYS2:** Requerido para compilar (descarga de https://www.msys2.org/)
- **Espacio en disco:** ~500 MB para MSYS2 + dependencias

---

## 1. Instalación Inicial (Primera vez)

### Paso 1: Instalar MSYS2

1. **Descargar MSYS2:**
   - Ve a https://www.msys2.org/
   - Descarga el instalador para Windows (64-bit)

2. **Instalar MSYS2:**
   - Ejecuta el instalador
   - Instala en la ubicación predeterminada: `C:\msys64`
   - Completa la instalación

3. **Actualizar MSYS2:**
   - Abre "MSYS2 MSYS" desde el menú de inicio
   - Ejecuta:
   ```bash
   pacman -Syu
   ```
   - Si pide cerrar la ventana, ciérrala y vuelve a abrir
   - Ejecuta nuevamente:
   ```bash
   pacman -Su
   ```

### Paso 2: Instalar Herramientas de Compilación

1. **Abrir "MSYS2 MinGW 64-bit"** (NO "MSYS2 MSYS")

2. **Instalar toolchain y OpenSSL:**
   ```bash
   pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-openssl
   ```
   - Presiona `Enter` cuando pregunte por selección (instala todos)
   - Escribe `Y` para confirmar

### Paso 3: Clonar o Descargar el Repositorio

**Opción A - Con Git:**
```bash
git clone https://github.com/TU_USUARIO/HelpDeskApp.git
cd HelpDeskApp
```

**Opción B - Descarga ZIP:**
1. Descarga el ZIP del repositorio
2. Extrae en una carpeta de tu elección
3. Abre "MSYS2 MinGW 64-bit" y navega a esa carpeta:
   ```bash
   cd "/c/ruta/a/tu/carpeta/HelpDeskApp"
   ```

### Paso 4: Compilar el Proyecto

En la shell **MSYS2 MinGW 64-bit**:

```bash
# Limpiar compilaciones anteriores
rm -rf build/*
mkdir -p build

# Compilar
g++ -std=c++17 -Wall -Wextra -g -I src src/*.cpp -o build/HelpDeskApp.exe -lcrypto

# Verificar que compiló correctamente
ls -lh build/HelpDeskApp.exe
```

Si no hay errores, verás el archivo `HelpDeskApp.exe` creado.

---

## 2. Ejecutar el Programa

### Opción A: Desde MSYS2 MinGW 64-bit (Recomendado)

1. **Abrir "MSYS2 MinGW 64-bit"**

2. **Navegar al proyecto:**
   ```bash
   cd "/c/ruta/a/tu/carpeta/HelpDeskApp"
   ```

3. **Ejecutar:**
   ```bash
   ./build/HelpDeskApp.exe
   ```

### Opción B: Desde PowerShell

1. **Abrir PowerShell** (`Win + X` → Windows PowerShell)

2. **Navegar al proyecto:**
   ```powershell
   cd "C:\ruta\a\tu\carpeta\HelpDeskApp"
   ```

3. **Ejecutar:**
   ```powershell
   .\build\HelpDeskApp.exe
   ```

### Opción C: Doble Clic (con archivo .bat)

1. **Crear archivo `run.bat`** en la raíz del proyecto con este contenido:
   ```batch
   @echo off
   cd /d "%~dp0"
   build\HelpDeskApp.exe
   pause
   ```

2. **Hacer doble clic en `run.bat`** para ejecutar

---

## 3. Uso del Programa

### Primer Uso - Registrarse

1. Ejecutar el programa (ver Opción A, B o C arriba)
2. Seleccionar opción **2** (Registrarse)
3. Ingresar username (3-20 caracteres) y password (mínimo 6 caracteres)
4. El usuario se guardará en `data/users.txt`

### Uso Normal - Iniciar Sesión

1. Seleccionar opción **1** (Iniciar sesión)
2. Gestionar tickets:
   - Crear nuevo ticket
   - Ver todos los tickets
   - Actualizar estado de ticket
   - Eliminar ticket
   - Reasignar ticket (solo administradores)
3. Todas las acciones se registran en `data/audit.log`

---

## 4. Solución de Problemas

### ❌ Error: "libcrypto-3-x64.dll no encontrado"

**Causa:** Las DLLs de MSYS2 no están accesibles

**Solución 1 - Copiar DLLs (recomendado):**

En PowerShell:
```powershell
Copy-Item "C:\msys64\mingw64\bin\libcrypto-3-x64.dll" ".\build\"
Copy-Item "C:\msys64\mingw64\bin\libgcc_s_seh-1.dll" ".\build\"
Copy-Item "C:\msys64\mingw64\bin\libstdc++-6.dll" ".\build\"
```

En MSYS2 MinGW 64-bit:
```bash
cp /mingw64/bin/libcrypto-3-x64.dll build/
cp /mingw64/bin/libgcc_s_seh-1.dll build/
cp /mingw64/bin/libstdc++-6.dll build/
```

**Solución 2 - Agregar al PATH:**
1. `Win + R` → `sysdm.cpl` → Enter
2. "Opciones avanzadas" → "Variables de entorno"
3. En "Variables del sistema", seleccionar "Path" → "Editar"
4. "Nuevo" → agregar: `C:\msys64\mingw64\bin`
5. Reiniciar PowerShell/terminal

---

### ❌ Error de compilación: "g++ no se reconoce..."

**Causa:** Intentas compilar desde PowerShell sin MSYS2

**Solución:** Usa **MSYS2 MinGW 64-bit** para compilar (ver Paso 4)

---

### ❌ Los datos no se guardan (usuarios/tickets)

**Causa:** Directorio de trabajo incorrecto o falta carpeta `data/`

**Solución:**

1. Crear carpeta `data/` si no existe:
   ```powershell
   mkdir data -Force
   ```

2. **Asegurarte de ejecutar desde la raíz del proyecto:**
   - ❌ NO hacer doble clic en `build\HelpDeskApp.exe`
   - ✅ Usar `run.bat` o ejecutar desde PowerShell/MSYS2

3. Verificar estructura:
   ```
   HelpDeskApp/
   ├── build/
   │   └── HelpDeskApp.exe
   ├── data/          ← debe existir aquí
   └── src/
   ```

---

### ❌ Error: "Permission denied" al compilar

**Causa:** El programa está en ejecución

**Solución:** Cerrar todas las instancias de `HelpDeskApp.exe` y volver a compilar

---

## 5. Recompilar el Proyecto

Si haces cambios al código fuente:

**Desde MSYS2 MinGW 64-bit:**
```bash
cd "/c/ruta/a/tu/carpeta/HelpDeskApp"
rm -rf build/*
mkdir -p build
g++ -std=c++17 -Wall -Wextra -g -I src src/*.cpp -o build/HelpDeskApp.exe -lcrypto
```

**Desde PowerShell (requiere MSYS2 instalado):**
```powershell
cd "C:\ruta\a\tu\carpeta\HelpDeskApp"
Remove-Item -Recurse -Force .\build\* -ErrorAction SilentlyContinue
if (-Not (Test-Path .\build)) { New-Item -ItemType Directory -Path .\build | Out-Null }
C:\msys64\msys2_shell.cmd -mingw64 -defterm -no-start -c "g++ -std=c++17 -Wall -Wextra -g -I src src/*.cpp -o build/HelpDeskApp.exe -lcrypto"
```

---

## 6. Archivos de Datos

### `data/users.txt`
- **Formato:** `username,passwordHash,role`
- **Ejemplo:** `admin,5e884898da2804715...,admin`
- Hash SHA-256 (64 caracteres)

### `data/tickets.txt`
- **Formato:** `id,title,description,priority,status,createdBy,assignedTo`
- **Ejemplo:** `1,Error login,Descripcion,Alta,Abierto,user1,admin`

### `data/audit.log`
- **Formato:** `[timestamp] user | action | details`
- **Ejemplo:** `[2024-01-15 10:30:45] admin | LOGIN | Successful login`

---

## 7. Notas de Seguridad

- **SHA-256:** Contraseñas hasheadas con SHA-256 (64 chars hex)
- **CSV Injection Prevention:** Campos sanitizados automáticamente
- **Validación:** Usernames (3-20 chars), passwords (6-30 chars)
- **Roles:** `user` (normal) y `admin` (administrador)
- **Reasignación:** Solo admins pueden reasignar tickets
3. Ingresar usuario, contraseña (mínimo 6 caracteres) y rol (`user` o `agent`)
4. Seleccionar opción **3** (Salir)

### Uso normal:
1. Ejecutar el programa
2. Seleccionar opción **1** (Iniciar sesión)
3. Ingresar credenciales
4. Usar el menú principal para gestionar tickets
5. Seleccionar opción **5** (Cerrar sesión) cuando termines

---

## Notas Importantes

- **Siempre ejecuta desde la raíz del proyecto** para que encuentre las carpetas `data/`
- **Los hashes SHA-256 deben tener 64 caracteres** - si ves hashes más cortos, recompila y recrea los usuarios
- **El rol `agent` tiene más permisos** que el rol `user` (puede reasignar tickets, etc.)
- **Todos los cambios se registran** en `data/audit.log`

---

Para más detalles técnicos, consulta `README.md`.
