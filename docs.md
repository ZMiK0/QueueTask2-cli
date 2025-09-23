El programa empieza con un menú donde puedes:
- Comenzar cola
    - Completar tarea
- Editar cola
    - Añadir tarea
    - Editar tarea por id
    - Eliminar tarea por id
    - Purgar cola


Las tareas se guardan en una base de datos sqlite.

Las tareas se llaman desde la base de datos cada vez que se inicia el programa y se guardan en una LinkedList.
    - Primero se llama a la base de datos, si esta vacia se 

Al terminar el programa Y SOLO SI SE TERMINA CORRECTAMENTE, se purga la tabla de la base de datos y la LinkedList se guarda en la base de datos (en una nueva tabla).

Cada tarea tiene un ID, un título y una descripción.

La descripción solo aparece al momento de comenzar la cola.


VIDEOS:

https://youtu.be/xFMXIgvlgcY?si=k8Syu9G-6xmy28qO