HelpDeskApp
============

Resumen
-------
Aplicación de consola en C++ para gestionar tickets de soporte (registro, login, CRUD de tickets). Pensada para compilar con MinGW-w64 (MSYS2) en Windows.

Estructura
---------
- `src/` : código fuente (.cpp/.h)
- `data/`: archivos de datos (`users.txt`, `tickets.txt`)
- `build/`: salida del compilado (`HelpDeskApp.exe`)

Requisitos
---------
- Windows
- MSYS2 con toolchain MinGW-w64 (g++)
  - Instalar MSYS2: https://www.msys2.org/
- (Opcional pero recomendado) OpenSSL para SHA-256 (`mingw-w64-x86_64-openssl`)
- Visual Studio Code (opcional) para editar/depurar

Instalación rápida (MSYS2 MinGW64)
---------------------------------
1. Abre "MSYS2 MinGW 64-bit" (desde Start).
2. Actualiza paquetes:

```bash
pacman -Syu
# si la shell pide reiniciar, ciérrala y reábrela, luego:
pacman -Su
```

3. Instala toolchain y OpenSSL:

```bash
pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-openssl
```

Compilar (MSYS2) — recomendado
-----------------------------
En la shell `MSYS2 MinGW 64-bit` ejecuta:

```bash
cd "/c/Users/migue/OneDrive/Desktop/Cosas varias/Comp Sci/CyberSecurity/HelpDeskApp"
rm -rf build/*
mkdir -p build
g++ -std=c++17 -Wall -Wextra -g -I src src/*.cpp -o build/HelpDeskApp.exe -lcrypto
```

- `-lcrypto` es necesario cuando el proyecto usa OpenSSL (SHA-256).
- Si diera problemas de localización de la librería, añade `-L/mingw64/lib` antes de `-lcrypto`.

Compilar desde PowerShell
-------------------------
Si prefieres PowerShell, añade `mingw64` al `PATH` y pasa la lista de archivos:

```powershell
$env:Path = 'C:\msys64\mingw64\bin;' + $env:Path
Remove-Item -Recurse -Force .\build\* -ErrorAction SilentlyContinue
if (-Not (Test-Path .\build)) { New-Item -ItemType Directory -Path .\build | Out-Null }
$files = (Get-ChildItem -Path .\src -Filter *.cpp | ForEach-Object { $_.FullName })
& g++ -std=c++17 -Wall -Wextra -g -I src $files -o build\HelpDeskApp.exe -lcrypto
```

Ejecutar
-------
Desde MSYS2 o PowerShell:

```bash
./build/HelpDeskApp.exe
# o en PowerShell
.\build\HelpDeskApp.exe
```

Configuración de depuración (VS Code)
-------------------------------------
Recomendado: usar una tarea que ejecute la compilación dentro de MSYS2 MinGW64 para garantizar PATH y expansión de `src/*.cpp`.

Ejemplo de `tasks.json` (reemplaza el actual):

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Build HelpDeskApp (msys2 mingw64)",
      "type": "shell",
      "command": "C:\\msys64\\msys2_shell.cmd",
      "args": [
        "-mingw64",
        "-c",
        "cd '/c/Users/migue/OneDrive/Desktop/Cosas varias/Comp Sci/CyberSecurity/HelpDeskApp' && rm -rf build/* && mkdir -p build && g++ -std=c++17 -Wall -Wextra -g -I src src/*.cpp -o build/HelpDeskApp.exe -lcrypto"
      ],
      "problemMatcher": ["$gcc"],
      "group": { "kind": "build", "isDefault": true }
    }
  ]
}
```

Asegúrate de que en `.vscode/launch.json` `preLaunchTask` coincida con el `label` anterior si quieres que VS Code compile antes de depurar.

Archivos de datos
-----------------
- `data/users.txt` — cada línea: `username,passwordHash,role`
- `data/tickets.txt` — CSV con campos por ticket

Solución de problemas comunes
-----------------------------
- `g++: command not found` → no estás usando la shell MinGW64 o `C:\msys64\mingw64\bin` no está en `PATH`.
- `undefined reference to 'SHA256'` → falta enlazar `libcrypto` (añade `-lcrypto`) o falta instalar `mingw-w64-x86_64-openssl`.
- `preLaunchTask terminated with exit code -1` en VS Code → la tarea de build falló (revisa la salida de la tarea en la pestaña "Terminal" o "Output -> Tasks"). Asegura que la tarea compile todos los `.cpp` y use MSYS2 environment.
- Advertencias de signed/unsigned → algunas funciones usan `size_t` vs `int`; se recomienda mantener `size_t` para comparaciones con `string::size()`.

Buenas prácticas
---------------
- Hacer un `rm -rf build/*` antes de una compilación completa si observas errores de linking extraños.
- Mantener las firmas de funciones sincronizadas entre `.h` y `.cpp`.

Si quieres que yo
-----------------
- Ponga la tarea de VS Code lista (te creo el `tasks.json`), o
- Ejecute la compilación aquí y te pegue la salida (necesito permiso para correr comandos), o
- Genere un `Makefile` simple para simplificar builds.

Contacto
--------
Si tienes problemas con algún paso pega aquí la salida del error y lo reviso.
