const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
const http = require('http').Server(app);

const GestionEventos = require('./funcionalidad/gestionEventos');
const GestionUsuarios = require('./funcionalidad/gestionUsuarios');
const GestionTareas = require('./funcionalidad/gestionTareas');

// Instanciamos los gestores de eventos, tareas y recordatorios
const gestorEventos = new GestionEventos('./ficheros/eventos.json');
const gestorUsuarios = new GestionUsuarios('./ficheros/usuarios.json');
const gestorTareas = new GestionTareas('./ficheros/tareas.json');

const io = require('socket.io')(http, {
    cors: {
      origin: '*',
      methods: ['GET', 'POST']
    }
  });

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, '../frontend/')));

// Rutas de las APIs
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../frontend/usuariosController.html'));
});

const socketsPorEmail = {};

// Configuración de Socket.IO (puedes añadir más aquí)
io.on('connection', (socket) => {
    console.log('Nuevo usuario conectado');

    // Registro del socket
    socket.on('registrarSocket', async (email) => {
        if (!socketsPorEmail[email]) {
            socketsPorEmail[email] = [];
        }

        socketsPorEmail[email].push(socket);

        console.log(`Guardando un socket ${socket.id} registrado para ${email}`);
        console.log('Sockets actuales:', socketsPorEmail);
    });

    // Usuarios
    socket.on('registroUsuario', async (usuario) => {
        const resultado = await gestorUsuarios.registroUsuario(usuario);
        socket.emit('resultadoRegistroUsuario', resultado);
    });
    socket.on('inicioSesion', async (email, password) => {
        const resultado = await gestorUsuarios.inicioSesion(email, password);
        socket.emit('resultadoInicioSesion', resultado);
    });
    socket.on('borrarUsuario', async (email) => {
        let resultado = await gestorTareas.borrarTareasUsuario(email);
        if (resultado !== 200) {
            socket.emit('resultadoBorrarUsuario', resultado);
            return;
        }

        resultado = await gestorEventos.borrarEventosUsuario(email);
        if (resultado !== 200) {
            socket.emit('resultadoBorrarUsuario', resultado);
            return;
        }

        resultado = await gestorUsuarios.borrarUsuario(email);
        socket.emit('resultadoBorrarUsuario', resultado);
    });

    // Eventos
    socket.on('nuevoEvento', async (emailUsuario, evento) => {
        const resultado = await gestorEventos.nuevoEvento(emailUsuario, evento);
        socket.emit('resultadoNuevoEvento', resultado);
    });
    socket.on('recuperarEventosDia', async (emailUsuario, fecha) => {
        console.log(fecha);
        const eventos = await gestorEventos.recuperarEventosDia(emailUsuario, fecha);
        
        // Enviar el resultado a los demás sockets del mismo usuario
        const sockets = socketsPorEmail[emailUsuario];
        if (sockets){
            sockets.forEach(s => {
                s.emit('resultadoRecuperarEventosDia', eventos);
            });
        }
    });
    socket.on('recuperarEventosMes', async (emailUsuario, fechaInicio, fechaFin) => {
        console.log(fechaInicio, fechaFin);
        const eventos = await gestorEventos.recuperarEventosMes(emailUsuario, fechaInicio, fechaFin);

        // Enviar el resultado a los demás sockets del mismo usuario
        const sockets = socketsPorEmail[emailUsuario];
        if (sockets){
            sockets.forEach(s => {
                s.emit('resultadoRecuperarEventosMes', eventos);
            });
        }
    });
    socket.on('borrarEvento', async (emailUsuario, evento) => {
        const resultado = await gestorEventos.borrarEvento(emailUsuario, evento);
        socket.emit('resultadoBorrarEvento', resultado);
    });
    socket.on('modificarEvento', async (emailUsuario, evento) => {
        const resultado = await gestorEventos.modificarEvento(emailUsuario, evento);
        socket.emit('resultadoModificarEvento', resultado);
    });

    // Tareas
    socket.on('nuevaTarea', async (emailUsuario, tarea) => {
        const resultado = await gestorTareas.nuevaTarea(emailUsuario, tarea);
        socket.emit('resultadoNuevaTarea', resultado);
    });
    socket.on('recuperarTareasPendientes', async (emailUsuario) => {
        const tareas = await gestorTareas.recuperarTareasPendientes(emailUsuario);

        // Enviar el resultado a los demás sockets del mismo usuario
        const sockets = socketsPorEmail[emailUsuario];
        if (sockets){
            sockets.forEach(s => {
                s.emit('resultadoRecuperarTareasPendientes', tareas);
            });
        }
    });
    socket.on('recuperarTareasMes', async (emailUsuario, fechaInicio, fechaFin) => {
        console.log(fechaInicio, fechaFin)
        const tareas = await gestorTareas.recuperarTareasMes(emailUsuario, fechaInicio, fechaFin);

        // Enviar el resultado a los demás sockets del mismo usuario
        const sockets = socketsPorEmail[emailUsuario];
        if (sockets){
            sockets.forEach(s => {
                s.emit('resultadoRecuperarTareasMes', tareas);
            });
        }
    });
    socket.on('borrarTarea', async (emailUsuario, tarea) => {
        const resultado = await gestorTareas.borrarTarea(emailUsuario, tarea);
        socket.emit('resultadoBorrarTarea', resultado);
    });
    socket.on('modificarTarea', async (emailUsuario, tarea) => {
        const resultado = await gestorTareas.modificarTarea(emailUsuario, tarea);
        socket.emit('resultadoModificarTarea', resultado);
    });
    socket.on('posponerTarea', async (emailUsuario, tarea, nuevaFecha) => {
        const resultado = await gestorTareas.posponerTarea(emailUsuario, tarea, nuevaFecha);
        socket.emit('resultadoPosponerTarea', resultado);
    });
    socket.on('completarTarea', async (emailUsuario, tarea) => {
        const resultado = await gestorTareas.completarTarea(emailUsuario, tarea);
        socket.emit('resultadoCompletarTarea', resultado);
    });

    // Envio de fecha
    socket.on('envioFecha', async (emailUsuario, fecha) => {
        // Enviar el resultado a los demás sockets del mismo usuario
        console.log("mandando la fecha", fecha);
        const sockets = socketsPorEmail[emailUsuario];
        if (sockets){
            sockets.forEach(s => {
                s.emit('recuperarFecha', fecha);
            });
        }
    })

});

// Iniciar el servidor
const PORT = 3000;
http.listen(PORT, () => {
    console.log(`Servidor escuchando en puerto http://localhost:${PORT}`);
});
