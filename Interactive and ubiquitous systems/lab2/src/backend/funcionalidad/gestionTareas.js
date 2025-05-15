const fs = require('fs').promises;

// Clase que se encarga de la gestión de tareas por parte del servidor
class GestionTareas {
    constructor(archivoTareas) {
        // Constructor de la clase gestionTareas
        this.archivoTareas = archivoTareas || './backend/ficheros/tareas.json';
    }

    // Método para añadir una nueva tarea
    async nuevaTarea(emailUsuario, tarea) {
        try {
            if (!emailUsuario) {
                console.log('El email del usuario es obligatorio.');
                return 400; // Código de estado 400 (Bad Request)
            }
            if (!tarea.nombre) {
                console.log('La tarea debe contener un nombre.');
                return 400; // Código de estado 400 (Bad Request)
            };

            // Guardamos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            const tasks = data.tareas || [];

            // Buscamos la posición del usuario en el json
            let indexUsuario = tasks.findIndex(t => t.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, lo añadimos
                tasks.push({ emailUsuario: emailUsuario, listaTareas: [] });
                indexUsuario = tasks.length - 1; // Actualizamos el índice del usuario
            }

            const tareasUsuario = tasks[indexUsuario].listaTareas || [];

            // Comprobamos si la tarea ya existe
            if (tareasUsuario.find(t => t.nombre === tarea.nombre)) {
                console.log('La tarea ya existe.');
                return 400;
            }

            // Guardamos la nueva tarea en la lista
            tareasUsuario.push(tarea);
            // Actualizamos la lista de tareas del usuario en el json
            tasks[indexUsuario].listaTareas = tareasUsuario;

            await fs.writeFile(this.archivoTareas, JSON.stringify({tareas: tasks}, null, 2));
            console.log('Tarea añadida correctamente.');

            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al añadir la tarea:', error);
            return 400;
        }
    }

    // Método para borrar una tarea
    async borrarTarea(emailUsuario, tarea) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            const tasks = data.tareas || [];

            // Obtenemos la posición de las tareas del usuario
            const indexUsuario = tasks.findIndex(t => t.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de tareas del usuario
            let tareasUsuario = tasks[indexUsuario].listaTareas || [];

            // Si no hay tareas, devolvemos un error
            if (tareasUsuario.length === 0) {
                console.log('No hay tareas para borrar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Filtramos las tareas eliminando la tarea deseada
            tareasUsuario = tareasUsuario.filter(t => t.nombre !== tarea.nombre);

            // Actualizamos la lista de tareas del usuario en el json
            tasks[indexUsuario].listaTareas = tareasUsuario;

            await fs.writeFile(this.archivoTareas, JSON.stringify({ tareas: tasks }, null, 2));
            console.log('Tarea borrada correctamente.');
            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al borrar la tarea:', error);
            return 400;
        }
    }

    // Método para modificar una tarea
    async modificarTarea(emailUsuario, tarea) {
        try {
            // Guardamos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            let tasks = data.tareas || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = tasks.findIndex(t => t.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }
            
            // Obtenemos la lista de tareas del usuario
            let tareasUsuario = tasks[indexUsuario].listaTareas || [];
            // Si no hay tareas, devolvemos un error
            if (tareasUsuario.length === 0) {
                console.log('No hay tareas para modificar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Buscamos la posición de la tarea a modificar
            const indexTarea = tareasUsuario.findIndex(t => t.nombre === tarea.nombre);
            if (indexTarea === -1) {
                // Si no existe la tarea, devolvemos un error
                console.log('La tarea no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Modificamos la tarea
            tareasUsuario[indexTarea] = tarea;
            // Actualizamos la lista de tareas del usuario en el json
            tasks[indexUsuario].listaTareas = tareasUsuario;

            // Actualizamos el archivo json
            await fs.writeFile(this.archivoTareas, JSON.stringify({ tareas: tasks }, null, 2));
            console.log('Tarea modificada correctamente.');

            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al modificar la tarea:', error);
            return 400;
        }
    }

    // Método para recuperar las tareas de un usuario por mes
    async recuperarTareasMes(emailUsuario, fechaInicio, fechaFin) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            const tasks = data.tareas || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = tasks.findIndex(t => t.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de tareas del usuario
            const tareasUsuario = tasks[indexUsuario].listaTareas || [];
            // Si no hay tareas, devolvemos un error
            if (tareasUsuario.length === 0) {
                console.log('No hay tareas para recuperar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Filtramos las tareas según el mes
            const tareasMes = tareasUsuario.filter(t => t.fecha >= fechaInicio && t.fecha <= fechaFin && t.completada === false);
            console.log('Tareas del mes recuperadas correctamente.');

            return tareasMes; // Devolvemos las tareas encontradas

        } catch (error) {
            console.error('Error al recuperar las tareas por mes:', error);
            return 400;
        }
    }
    
    // Método para recuperar las tareas de un usuario no completadas
    async recuperarTareasPendientes(emailUsuario) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            const tasks = data.tareas || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = tasks.findIndex(t => t.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de tareas del usuario
            const tareasUsuario = tasks[indexUsuario].listaTareas || [];
            // Si no hay tareas, devolvemos un error
            if (tareasUsuario.length === 0) {
                console.log('No hay tareas para recuperar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Filtramos las tareas por día
            const tareasPendientes = tareasUsuario.filter(t => t.completada === false);
            console.log('Tareas pendientes recuperadas correctamente.');

            return tareasPendientes; // Devolvemos las encontradas

        } catch (error) {
            console.error('Error al recuperar las tareas pendientes:', error);
            return 400;
        }
    }

    async posponerTarea(emailUsuario, tarea, nuevaFecha) {
        try{
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            const tasks = data.tareas || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = tasks.findIndex(t => t.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de tareas del usuario
            let tareasUsuario = tasks[indexUsuario].listaTareas || [];

            // Obtenemos la tarea a completar
            const indexTarea = tareasUsuario.findIndex(t => t.nombre === tarea.nombre);
            if (indexTarea === -1) {
                // Si no existe la tarea, devolvemos un error
                console.log('La tarea no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }
            
            // Marcamos la tarea como completada
            tareasUsuario[indexTarea].fecha = nuevaFecha;

            // Actualizamos la lista de tareas del usuario en el json
            tasks[indexUsuario].listaTareas = tareasUsuario;

            // Guardamos el archivo json
            await fs.writeFile(this.archivoTareas, JSON.stringify({ tareas: tasks }, null, 2));
            console.log('Tarea pospuesta correctamente.');
            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al completar la tarea:', error);
            return 400;
        }
    }

    // Método para completar una tarea
    async completarTarea(emailUsuario, tarea) {
        try{
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            const tasks = data.tareas || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = tasks.findIndex(t => t.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de tareas del usuario
            let tareasUsuario = tasks[indexUsuario].listaTareas || [];

            // Obtenemos la tarea a completar
            const indexTarea = tareasUsuario.findIndex(t => t.nombre === tarea.nombre);
            if (indexTarea === -1) {
                // Si no existe la tarea, devolvemos un error
                console.log('La tarea no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            if (tareasUsuario[indexTarea].completada) {
                // Si la tarea ya está completada, devolvemos un error
                console.log('La tarea ya está completada.');
                return 400; // Código de estado 400 (Bad Request)
            }
            
            // Marcamos la tarea como completada
            tareasUsuario[indexTarea].completada = true;

            // Actualizamos la lista de tareas del usuario en el json
            tasks[indexUsuario].listaTareas = tareasUsuario;

            // Guardamos el archivo json
            await fs.writeFile(this.archivoTareas, JSON.stringify({ tareas: tasks }, null, 2));
            console.log('Tarea completada correctamente.');
            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al completar la tarea:', error);
            return 400;
        }
    }

    // Método para borrar las tareas de un usuario al darse de baja
    async borrarTareasUsuario(emailUsuario) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoTareas, 'utf-8');
            const data = JSON.parse(contenido);
            let tasks = data.tareas || [];

            // Filtramos las tareas eliminando las del usuario
            tasks = tasks.filter(t => t.emailUsuario !== emailUsuario);

            // Guardamos el archivo json
            await fs.writeFile(this.archivoTareas, JSON.stringify({ tareas: tasks }, null, 2));
            console.log('Tareas del usuario borradas correctamente.');
            
            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al borrar las tareas del usuario:', error);
            return 400;
        }
    }
}

module.exports = GestionTareas;
