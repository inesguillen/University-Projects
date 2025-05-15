const fs = require('fs').promises;

// Clase que contiene todos los métodos relacionados con la gestión de estos por parte del servidor
class GestionEventos {
    constructor(archivoEventos) {
        // Constructor de la clase GestionEventos
        this.archivoEventos = archivoEventos || './backend/ficheros/eventos.json';
    }

    // Método para añadir un nuevo evento
    async nuevoEvento(emailUsuario, evento) {
        try {
            if (!emailUsuario) {
                console.log('El email del usuario es obligatorio.');
                return 400; // Código de estado 400 (Bad Request)
            }
            if (!evento.nombre || !evento.fecha || !evento.hora) {
                console.log('El evento debe contener un nombre, fecha y hora.');
                return 400; // Código de estado 400 (Bad Request)
            };

            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoEventos, 'utf-8');
            const data = JSON.parse(contenido);
            const events = data.eventos || [];

            // Buscamos la posición del usuario en el json
            let indexUsuario = events.findIndex(e => e.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, lo añadimos
                events.push({ emailUsuario: emailUsuario, listaEventos: [] });
                indexUsuario = events.length - 1; // Actualizamos el índice del usuario
            }

            const eventosUsuario = events[indexUsuario].listaEventos || [];

            // Comprobamos si el evento ya existe
            if (eventosUsuario.find(e => e.nombre === evento.nombre)){
                console.log('El evento ya existe.');
                return 400;
            }

            // Guardamos el nuevo evento en la lista
            eventosUsuario.push(evento);
            // Actualizamos la lista de eventos del usuario en el json
            events[indexUsuario].listaEventos = eventosUsuario;

            await fs.writeFile(this.archivoEventos, JSON.stringify({ eventos: events }, null, 2));
            console.log('Evento añadido correctamente.');

            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al añadir el evento:', error);
            return 400;
        }
    }

    // Método para borrar un evento
    async borrarEvento(emailUsuario, evento) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoEventos, 'utf-8');
            const data = JSON.parse(contenido);
            const events = data.eventos || [];

            // Obtenemos la posición de los eventos del usuario
            const indexUsuario = events.findIndex(e => e.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de eventos del usuario
            let eventosUsuario = events[indexUsuario].listaEventos || [];

            // Si no hay eventos, devolvemos un error
            if (eventosUsuario.length === 0) {
                console.log('No hay eventos para borrar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Filtramos los eventos eliminando el evento deseado
            eventosUsuario = eventosUsuario.filter(e => e.nombre !== evento.nombre || e.fecha !== evento.fecha);

            // Actualizamos la lista de eventos del usuario en el json
            events[indexUsuario].listaEventos = eventosUsuario;

            await fs.writeFile(this.archivoEventos, JSON.stringify({ eventos: events }, null, 2));
            console.log('Evento borrado correctamente.');
            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al borrar el evento:', error);
            return 400;
        }
    }

    // Método para modificar un evento
    async modificarEvento(emailUsuario, evento) {
        try {
            // Guardamos el archivo json
            const contenido = await fs.readFile(this.archivoEventos, 'utf-8');
            const data = JSON.parse(contenido);
            let events = data.eventos || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = events.findIndex(e => e.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }
            
            // Obtenemos la lista de eventos del usuario
            let eventosUsuario = events[indexUsuario].listaEventos || [];
            // Si no hay eventos, devolvemos un error
            if (eventosUsuario.length === 0) {
                console.log('No hay eventos para modificar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Buscamos la posición del evento a modificar
            const indexEvento = eventosUsuario.findIndex(e => e.nombre === evento.nombre && e.fecha === evento.fecha);
            if (indexEvento === -1) {
                // Si no existe el evento, devolvemos un error
                console.log('El evento no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Modificamos el evento
            eventosUsuario[indexEvento] = evento;
            // Actualizamos la lista de eventos del usuario en el json
            events[indexUsuario].listaEventos = eventosUsuario;

            // Actualizamos el archivo json
            await fs.writeFile(this.archivoEventos, JSON.stringify({ eventos: events }, null, 2));
            console.log('Evento modificado correctamente.');

            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al modificar el evento:', error);
            return 400;
        }
    }

    // Método para recuperar los eventos de un usuario por mes
    async recuperarEventosMes(emailUsuario, fechaInicio, fechaFin) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoEventos, 'utf-8');
            const data = JSON.parse(contenido);
            const events = data.eventos || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = events.findIndex(e => e.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de eventos del usuario
            const eventosUsuario = events[indexUsuario].listaEventos || [];
            // Si no hay eventos, devolvemos un error
            if (eventosUsuario.length === 0) {
                console.log('No hay eventos para recuperar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Filtramos los eventos según el mes
            const eventosMes = eventosUsuario.filter(e => e.fecha >= fechaInicio && e.fecha <= fechaFin);
            console.log('Eventos del mes recuperados correctamente.');

            return eventosMes; // Devolvemos los eventos encontrados

        } catch (error) {
            console.error('Error al recuperar los eventos por mes:', error);
            return 400;
        }
    }
    
    // Método para recuperar los eventos de un usuario por día
    async recuperarEventosDia(emailUsuario, fecha) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoEventos, 'utf-8');
            const data = JSON.parse(contenido);
            const events = data.eventos || [];

            // Buscamos la posición del usuario en el json
            const indexUsuario = events.findIndex(e => e.emailUsuario === emailUsuario);
            if (indexUsuario === -1) {
                // Si no existe el usuario, devolvemos un error
                console.log('El usuario no existe.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Obtenemos la lista de eventos del usuario
            const eventosUsuario = events[indexUsuario].listaEventos || [];
            // Si no hay eventos, devolvemos un error
            if (eventosUsuario.length === 0) {
                console.log('No hay eventos para recuperar.');
                return 404; // Código de estado 404 (No encontrado)
            }

            // Filtramos los eventos por día
            const eventosDia = eventosUsuario.filter(e => e.fecha === fecha);
            console.log('Eventos del día recuperados correctamente.');

            return eventosDia; // Devolvemos los eventos encontrados

        } catch (error) {
            console.error('Error al recuperar los eventos por día:', error);
            return 400;
        }
    }

    // Método para borrar los eventos de un usuario si este se da de baja
    async borrarEventosUsuario(emailUsuario){
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoEventos, 'utf-8');
            const data = JSON.parse(contenido);
            let events = data.eventos || [];

            // Filtramos las tareas eliminando las del usuario
            events = events.filter(e => e.emailUsuario !== emailUsuario);

            // Actualizamos el archivo json
            await fs.writeFile(this.archivoEventos, JSON.stringify({ eventos: events }, null, 2));
            console.log('Eventos eliminados correctamente.');

            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al borrar los eventos del usuario:', error);
            return 400;
        }
    }
}

module.exports = GestionEventos;

