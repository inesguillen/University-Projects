# Grupo 14

# Información relevante
## Videos 
Se puede encontrar un enlace en la memoria

## Ejecutar el servidor
Desde la carpeta src, ejecutar el comando

    npm run server

## Calendario
Para poder consultar el calendario, se debe acceder a la aplicación desde un móvil y desde un ordenador, estando loggeados con la misma cuenta.

## Escanear
El formato para guardar eventos debe ser el siguiente:

    Evento: <nombre_evento>
    Fecha: <fecha_evento>   //Formato YYYY-MM-DD
    Hora: <hora_evento>     //Opcional
    Lugar: <lugar_evento>   //Opcional

Por otro lado, el formato para guardar tareas debe ser el siguiente:

    Tarea: <nombre_tarea>
    Descripcion: <descripcion_tarea>    //Opcional
    Fecha: <fecha_tarea>    //Opcional, formato YYYY-MM-DD
    Hora: <hora_evento>     //Opcional
    Lugar: <lugar_tarea>    //Opcional

## Guardado eventos / tareas por voz
Los botones en verde son campos obligatorios. Los que están en gris son opcionales.

## Posponer / completar tareas emergentes
Si se inicia sesión con el usuario prueba (email: prueba@prueba.es y contraseña: Prueba), en cuanto entre al menú principal, saltará un ejemplo de evento que ha sobrepasado la fecha límite, pidiendo posponerla o completarla.

# Estructura del proyecto
## backend
Contiene todos los ficheros relacionados con las funcionalidades del servidor, incluyendo los ficheros donde guarda información con sus respectivos gestores.
- ficheros/: actúa de base de datos. Almacena los json que recopilan los eventos, tareas y usuarios registrados en el sistema.
- funcionalidad/: recopila todos los gestores. Cada gestor se encarga de acceder a su json correspondiente y realizar las operaciones sobre estos datos.
- server.js: ejecuta el servidor

## frontend
COntiene todos los ficheros relacionados con las funcionalidades del cliente, incluyendo tanto los html como sus respectivos JavaScript.
- funcionalidad/: contiene todos los ficheros que se encargan de ejecutar la lógica de sus respectivos html.
- .tml: contienen las páginas de la aplicación

