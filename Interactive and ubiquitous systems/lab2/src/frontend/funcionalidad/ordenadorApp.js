const socket = io('http://localhost:3000');

// Meses
const meses = {
    "enero": '01',
    "febrero": '02',
    "marzo": '03',
    "abril": '04',
    "mayo": '05',
    "junio": '06',
    "julio": '07',
    "agosto": '08',
    "septiembre": '09',
    "octubre": '10',
    "noviembre": '11',
    "diciembre": '12'
};

const bienvenidaHeader = document.getElementById('bienvenida');
const diaHeader = document.getElementById("diaImpreso");
const mesHeader = document.getElementById("mesImpreso");

const eventosSection = document.getElementById("eventos")
const eventosDiv = document.getElementById("eventosDiv");

const tareasSection = document.getElementById("tareas");
const tareasDiv = document.getElementById("tareasDiv");

const esperaP = document.getElementById("espera");

let fecha = Date.now(); // Por defecto

// Conexión al servidor
socket.on('connect', () => {
    console.log('Conectado al servidor!');
});
  
socket.on('recuperarFecha', (respuesta) => {
    fecha = respuesta;
});

// Obtención de cookies
function getCookie(nombre) {
    const valor = `; ${document.cookie}`;
    const partes = valor.split(`; ${nombre}=`);
    if (partes.length === 2) return partes.pop().split(';').shift();
}

document.addEventListener('DOMContentLoaded', () => {
    // Función que personaliza el saludo al usuario
    const nombre = getCookie("nombre");

    bienvenidaHeader.innerHTML = bienvenidaHeader.innerHTML.replace(/\[\$nombre\$\]/g, nombre);

    // Enviamos el id del socket al servidor
    const email = getCookie("email");
    socket.emit('registrarSocket', email);
});

// Eventos del día
socket.on('resultadoRecuperarEventosDia', (respuesta) => {
    if (respuesta) {
        console.log("Eventos día recuperados correctamente", respuesta);

        eventosSection.style.display = "block";
        eventosDiv.innerHTML = ""; // Limpiamos el html

        if(respuesta.length > 0){
            // Imprimimos los eventos
            respuesta.forEach(evento => {
                string = evento.nombre + " 🕒 " + evento.hora + (evento.lugar ? " 📍 " + evento.lugar : "") + "<br>";
                eventosDiv.innerHTML += string;
            });

        } else {
            eventosDiv.innerHTML = "No hay eventos";
        }

        // Obtenemos el día
        diaHeader.style.display = "block";
        diaHeader.innerHTML = fecha;   

        esperaP.style.display = "none";
        bienvenidaHeader.style.display = "none";
        mesHeader.style.display = "none";

    } else {
        console.log("Error al recuperar los eventos del día");
        alert("Error al recuperar los eventos del día");
    }
});

// Eventos del mes
socket.on('resultadoRecuperarEventosMes', (respuesta) => {
    if (respuesta) {
        console.log("Eventos mes recuperados correctamente", respuesta);

        eventosSection.style.display = "block";
        eventosDiv.innerHTML = ""; // Limpiamos el html

        if(respuesta.length > 0){
            // Imprimimos los eventos
            respuesta.forEach(evento => {
                string = evento.nombre + " 🕒 " + evento.hora + (evento.lugar ? " 📍 " + evento.lugar : "") + "<br>";
                eventosDiv.innerHTML += string;
            });

        } else {
            eventosDiv.innerHTML = "No hay eventos";
        }

        // Obtenemos el mes
        mesHeader.style.display = "block";
        let Mes = fecha.charAt(0).toUpperCase() + fecha.slice(1);
        mesHeader.innerHTML = Mes;
        
        esperaP.style.display = "none";
        bienvenidaHeader.style.display = "none";
        diaHeader.style.display = "none";

    } else {
        console.log("Error al recuperar los eventos del mes");
        alert("Error al recuperar los eventos del mes");
    }
});

// Tareas del día
socket.on('resultadoRecuperarTareasPendientes', (respuesta) => {
    if (respuesta) {
        console.log("Tareas pendientes recuperadas correctamente", respuesta);

        tareasSection.style.display = "block";
        tareasDiv.innerHTML = ''; // Limpiamos el contenido

        const email = getCookie("email");

        // Imprimimos los eventos
        respuesta.forEach(tarea => {
            const tareaDiv = document.createElement('div');
            tareaDiv.classList.add('tarea');

            // Añadimos un botón para completarla
            const completarTarea = document.createElement('button');
            completarTarea.textContent = 'Completar';
            completarTarea.classList.add('verde');

            // Lógica para cuando el botón sea presionado
            completarTarea.addEventListener('click', () => {
                // Aquí va la lógica para marcar la tarea como completada
                console.log(`Tarea completada: ${tarea.nombre}`);
                // Enviamos peticion al socket
                socket.emit('completarTarea', email, tarea);
            });

            const tareaTexto = document.createElement('span');
            tareaTexto.textContent = `${tarea.nombre} 📅 ${tarea.fecha} 🕒 ${tarea.hora}${tarea.lugar ? ' 📍 ' + tarea.lugar : ''}`;

            // Añadimos los contenedores a la seccion
            tareaDiv.appendChild(completarTarea);
            tareaDiv.appendChild(tareaTexto);

            tareasDiv.appendChild(tareaDiv);
        });

    } else {
        console.log("Error al recuperar las tareas pendientes");
        alert("Error al recuperar las tareas pendientes");
    }
});

// Tareas del mes
socket.on('resultadoRecuperarTareasMes', (respuesta) => {
    if (respuesta) {
        console.log("Tareas del mes recuperadas correctamente", respuesta);

        tareasSection.style.display = "block";
        tareasDiv.innerHTML = ''; // Limpiamos el contenido

        const email = getCookie("email");

        // Imprimimos los eventos
        respuesta.forEach(tarea => {
            const tareaDiv = document.createElement('div');
            tareaDiv.classList.add('tarea');

            // Añadimos un botón para completarla
            const completarTarea = document.createElement('button');
            completarTarea.textContent = 'Completar';
            completarTarea.classList.add('verde');

            // Lógica para cuando el botón sea presionado
            completarTarea.addEventListener('click', () => {
                // Aquí va la lógica para marcar la tarea como completada
                console.log(`Tarea completada: ${tarea.nombre}`);
                // Enviamos peticion al socket
                socket.emit('completarTarea', email, tarea);
            });

            const tareaTexto = document.createElement('span');
            tareaTexto.textContent = `${tarea.nombre} 📅 ${tarea.fecha} 🕒 ${tarea.hora}${tarea.lugar ? ' 📍 ' + tarea.lugar : ''}`;

            // Añadimos los contenedores a la seccion
            tareaDiv.appendChild(completarTarea);
            tareaDiv.appendChild(tareaTexto);

            tareasDiv.appendChild(tareaDiv);
        });

    } else {
        console.log("Error al recuperar las tareas pendientes");
        alert("Error al recuperar las tareas pendientes");
    }
});
