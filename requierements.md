# Project: Snake

---

## Background

En 1976, un ingeniero de Atari llamado Dennis Koble creó un juego llamado Blockade. Una serpiente se mueve por una pantalla, come, crece, y muere si choca. Décadas después, ese juego llegó a los Nokia 3310 y se volvió legendario. Pero nadie te dijo lo más interesante: la razón por la que ese juego funciona de forma tan natural es porque el problema que resuelve tiene una estructura de datos perfecta y obvia. Y esa estructura es una **queue**.

Piensa en cómo se mueve la serpiente. Tienes una cabeza que avanza hacia una nueva casilla, y una cola que abandona la casilla más antigua. La cabeza siempre es lo más nuevo. La cola siempre es lo más viejo. Lo nuevo entra por un lado, lo viejo sale por el otro. Eso tiene un nombre: **FIFO**. First In, First Out. Es exactamente la definición de una queue.

Ahora piensa en cómo crece. Cuando la serpiente come una manzana, la cabeza avanza hacia la nueva casilla igual que siempre, pero la cola no desaparece ese turno. El segmento más antiguo se queda. La serpiente tiene un nodo más. No agregaste nada especial, simplemente omitiste el `dequeue` de ese tick.

Eso es todo. Un juego completo reducido a dos operaciones:

- **Avanzar**: `enqueue` en la cabeza + `dequeue` en la cola
- **Comer**: `enqueue` en la cabeza + no hacer `dequeue`

Ahora la parte que lo hace diferente a hacer esto en tu computadora: lo vas a ver físico. Cada nodo de tu queue va a ser un LED encendido en una matriz real. Cuando hagas `enqueue`, un LED se enciende. Cuando hagas `dequeue`, un LED se apaga. No es una abstracción en pantalla, es hardware respondiendo a tus estructuras de datos en tiempo real.

Eso es lo que hace este proyecto. No estás aprendiendo a hacer un juego. Estás aprendiendo que las estructuras de datos no son conceptos de libro, son la solución natural a problemas reales. Snake no usa una queue porque alguien lo decidió. Usa una queue porque es la única estructura que describe exactamente lo que pasa.

---

## Learning Goals

Al terminar serás capaz de:

- Implementar una queue con linked list desde cero
- Entender enqueue, dequeue y sus casos borde
- Mapear una estructura de datos abstracta a un espacio físico bidimensional
- Manejar colisiones como un problema de búsqueda en una estructura de datos
- Generar posiciones aleatorias evitando conflictos con datos existentes

---

## Hardware Required

- Arduino Uno o Nano
- Matriz LED 8x8 con módulo MAX7219 x1
- Joystick analógico KY-023 x1 (o 4 botones como alternativa)
- Buzzer pasivo x1
- Protoboard + jumpers
- Cable USB al PC

---

## Concepts Involved

| Concepto | Dónde aparece |
|---|---|
| Queue | Cuerpo completo de la serpiente |
| Linked list | Implementación interna de la queue |
| Enqueue | Nueva cabeza al avanzar |
| Dequeue | Eliminar la cola al avanzar |
| Collision detection | Buscar si la cabeza ya existe en la queue |
| Random + constraint | Generar manzana fuera del cuerpo |

---

## Specification

**El juego debe:**

1. Iniciar con una serpiente de 3 segmentos en el centro de la matriz
2. Moverse automáticamente en la dirección actual cada 300ms
3. Cambiar de dirección con el joystick, sin permitir reversa directa (si vas a la derecha, no puedes ir inmediatamente a la izquierda)
4. Generar una manzana en una posición aleatoria que no esté ocupada por el cuerpo
5. Al comer la manzana, crecer un segmento y generar una nueva manzana inmediatamente
6. Detectar colisión con los bordes del tablero y terminar el juego
7. Detectar colisión con el propio cuerpo y terminar el juego
8. Al terminar, mostrar una animación de game over (todos los LEDs parpadean 3 veces)
9. Después del game over, reiniciar automáticamente tras 2 segundos
10. El buzzer emite un tono corto al comer y un tono grave al morir

**El juego no debe:**

- Usar arrays para representar el cuerpo de la serpiente
- Usar una matriz de 8x8 en memoria para rastrear posiciones del cuerpo
- Permitir que la serpiente se mueva fuera de los 64 píxeles de la matriz

---

## Implementation Details

**La queue del cuerpo:**
Cada nodo almacena una coordenada (x, y). El frente de la queue es la cabeza. El final es la cola. En cada tick del juego, calculas la nueva cabeza según la dirección, haces enqueue al frente, y dequeue al final, salvo que se haya comido una manzana ese turno.

**Collision detection:**
Para saber si la nueva cabeza colisiona con el cuerpo, debes recorrer toda la queue buscando si algún nodo tiene las mismas coordenadas. No hay atajo. Eso es O(n) y es intencional.

**La manzana:**
Generas coordenadas aleatorias y verificas que no estén en la queue. Si están, generas de nuevo. En un tablero de 64 casillas con una serpiente larga esto se vuelve interesante.

**Velocidad progresiva:**
Cada vez que la serpiente come, el intervalo de movimiento reduce 15ms. Empieza en 300ms, se vuelve más rápido con cada manzana. Mínimo 100ms.

---

## How to think about it

Antes de escribir una línea de código, dibuja en papel una queue de 3 nodos con coordenadas. Dibuja a mano qué pasa en cada tick: qué nodo entra, qué nodo sale, cómo se ve en la matriz. Haz esto para 5 ticks antes de tocar el teclado. Si no puedes dibujarlo, no puedes programarlo.