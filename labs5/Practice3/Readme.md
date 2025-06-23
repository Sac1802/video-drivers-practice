### 🔧 **Práctica: Driver con contador de accesos y comando RESET**

#### 🧠 ¿Qué debe hacer el driver?

Crea un driver de carácter que:

1. Lleve un **contador** de cuántas veces se ha leído el dispositivo.
2. Al hacer `cat /dev/practice`, se debe mostrar el mensaje actual y el número de accesos.
3. Si escribes el comando especial `"RESET"`, el contador se reinicia a cero y muestra un mensaje en `dmesg`.

---

#### ✍️ Requisitos

* Implementa `.read` y `.write` como ya lo hiciste.
* Declara una variable global `static int access_count = 0;`
* En la función `.read`, **incrementa el contador** y devuelve el mensaje con el número de lecturas, por ejemplo:

  ```
  Status: DEVICE ON
  Reads: 3
  ```
* En la función `.write`, si se detecta que el usuario escribió `"RESET"`, reinicia el contador a 0 y cambia el estado a `"DEVICE RESET"`.

---

#### 💡 Tip Extra

Para formatear el mensaje combinado (`status + contador`), puedes usar `snprintf()` en una variable `char output[64]`.

---