### 🧠 **Reto: Historial de estados en un driver de caracteres**

#### 📝 Descripción:

Vas a crear un driver de caracteres que, además de mostrar el estado actual del dispositivo, mantenga un **historial de los últimos 5 mensajes** escritos por el usuario. Cada vez que escribas algo con `echo`, el mensaje se guarda en un pequeño buffer circular de 5 posiciones. Cuando hagas `cat`, el driver debe mostrar todos los mensajes guardados, del más nuevo al más viejo.

#### 🛠️ Objetivos del reto:

* Usar un arreglo de strings dentro del kernel.
* Implementar un buffer circular simple.
* Mostrar múltiples líneas en una lectura (`read`).
* Reforzar el uso de `copy_from_user`, `copy_to_user`, `snprintf`, offsets, etc.

#### 🧪 Ejemplo de uso:

```bash
echo "Hello" > /dev/history_dev
echo "System ON" > /dev/history_dev
echo "Charging..." > /dev/history_dev
cat /dev/history_dev
```

**Salida esperada:**

```
Charging...
System ON
Hello
(empty)
(empty)
```

Si luego escribes otros dos:

```bash
echo "Battery full" > /dev/history_dev
echo "Disconnected" > /dev/history_dev
cat /dev/history_dev
```

**Salida esperada:**

```
Disconnected
Battery full
Charging...
System ON
Hello
```

---

Este reto refuerza la lógica de estructuras de datos, manejo de memoria del kernel (con punteros), `read` con múltiples líneas, y cómo simular comportamientos útiles sin salirte de lo que has aprendido.
