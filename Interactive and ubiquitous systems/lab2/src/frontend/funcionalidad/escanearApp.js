// Conexión con el servidor
const socket = io('http://localhost:3000');

const textoEscaneado = document.getElementById('resultadoTexto');
const seccionEscaneo = document.getElementById('escanearArchivo');
const errorEscaneo = document.getElementById("errorEscaneo");

const mensajeEscaneando = document.getElementById("escaneando");

const mensajeEscaneo = document.getElementById("mensajeEscaneo");
const comandoGuardado = document.getElementById("comandoGuardado");

let imagenSeleccionada = null;

const video = document.getElementById('video');
const canvas = document.getElementById('canvas');
const context = canvas.getContext('2d');
const capturarBtn = document.getElementById('capturarBtn');

let videoStream;
let currentCamera = 'environment';  // 'user' es la cámara delantera, 'environment' es la trasera

// Obtención de cookies
function getCookie(nombre) {
    const valor = `; ${document.cookie}`;
    const partes = valor.split(`; ${nombre}=`);
    if (partes.length === 2) return partes.pop().split(';').shift();
}

document.addEventListener('DOMContentLoaded', () => {
    // Iniciamos la cámara
    accederCamara(currentCamera);
});

// Función para acceder a la cámara
function accederCamara(camara) {
    if (videoStream) {
        videoStream.getTracks().forEach(track => track.stop());
    }

    // Seleccionar la cámara según el parámetro 'camara' ('user' o 'environment')
    navigator.mediaDevices.getUserMedia({
        video: {
            facingMode: camara
        }
    }).then(stream => {
        videoStream = stream;
        video.srcObject = stream;
        console.log('Cámara iniciada correctamente');
    }).catch(err => {
        console.error('Error al acceder a la cámara:', err);
        alert('No se puede acceder a la cámara.');
    });
}



// Cambiar entre cámara delantera y trasera
document.getElementById('cambiarCamaraBtn').addEventListener('click', function() {
    // Alternar entre las cámaras
    currentCamera = (currentCamera === 'user') ? 'environment' : 'user';
    accederCamara(currentCamera);
});

// Capturar imagen cuando el usuario presiona el botón
capturarBtn.addEventListener('click', () => {
    // Establecer las dimensiones del canvas para que coincidan con el video
    canvas.width = video.videoWidth;
    canvas.height = video.videoHeight;
  
    // Dibujar el fotograma actual del video en el canvas
    context.drawImage(video, 0, 0, canvas.width, canvas.height);
    
    // Obtener la imagen del canvas como un dataURL
    const imagenCapturada = canvas.toDataURL('image/png');
    
    // Procesar la imagen capturada con Tesseract
    escanearImagen(imagenCapturada);
});

//Función para dar retroalimentación por voz
function speak(text) {
    const utterance = new SpeechSynthesisUtterance(text);
    utterance.lang = "es-ES";
    utterance.rate = 1; 
    utterance.pitch = 1; 
    utterance.volume = 1; 
    
    speechSynthesis.speak(utterance);
}

// Función para ejecutar OCR con Tesseract
function escanearImagen(imagenCapturada) {
    // Comprobamos que tenemos una imagen válida
    if (!imagenCapturada) {
        errorEscaneo.textContent = 'Imagen invalida.';
        errorEscaneo.style.display = "block";
        return;
    }

    // Mostramos mensaje de escaneando
    mensajeEscaneando.style.display = "block";

    Tesseract.recognize(
        imagenCapturada,
        'spa',
        { logger: m => console.log(m) }
      ).then(({ data: { text } }) => {
        const textoLimpio = text.toUpperCase().replace(/[\t\n\r]+/g, ' ').trim();

        //        text.replace(/\s/g, '');
      
        if (textoLimpio.length === 0) {
            errorEscaneo.innerHTML = 'No se ha detectado texto en la imagen.';
            errorEscaneo.style.display = "block";
            speak("No se ha detectado texto en la imagen. Intenta escanear otra vez.");
            mensajeEscaneando.style.display = "none";
            return;
        }
      
        // Texto válido encontrado
        textoEscaneado.innerHTML = textoLimpio;
        console.log('Texto escaneado:', textoLimpio);

        mensajeEscaneando.style.display = "none";
      
        // Mostrar mensaje de éxito y botones
        mensajeEscaneo.style.display = "block";
        comandoGuardado.style.display = "block";

        speak("Texto escaneado correctamente. Puedes guardarlo cuando quieras.");
        
      }).catch(err => {
        console.error('Error al escanear:', err);
        errorEscaneo.innerHTML = 'Error al escanear la imagen.';
        errorEscaneo.style.display = "block";
      });
      
}

// Función para procesar el texto, clasificarlo como tarea o evento y guardarlo como corresponde
function procesarTexto(){
    const textoLista = textoEscaneado.textContent.trim().split(" ");

    const tipo = textoLista[0]?.toUpperCase();
    
    if (tipo.includes("EVENTO")){
        console.log('Guardando evento');
        guardarEvento(textoLista.slice(1));
    } else if (tipo.includes("TAREA")){
        console.log('Guardando tarea');
        guardarTarea(textoLista.slice(1));
    } else {
        console.log("No se ha reconocido si es evento o tarea.");
        speak("No se ha reconocido si es evento o tarea. Inténtalo de nuevo.");
        alert("El texto escaneado debe comenzar con EVENTO o TAREA.");
    }
}

// Función para guardar el evento
function guardarEvento(textoLista) {
    let nombreInvertido = "";
    let fecha = "";
    let hora = "";
    let lugar = "";

    let i = textoLista.length - 1;

    // Guardado de lugar
    if (i > 1 && textoLista[i-1].toUpperCase().includes("LUGAR")) {
        lugar = textoLista[i];
        i -= 2;
    }

    // Guardado de hora
    if (i > 1 && textoLista[i-1].toUpperCase().includes("HORA")) {
        hora = textoLista[i];
        i -= 2;
    }

    // Guardado de fecha
    if (i > 1 && textoLista[i-1].toUpperCase().includes("FECHA")) {
        fecha = textoLista[i];
        i -= 2;
    }  

    // Guardado de nombre
    while (i >= 0) {
        if (textoLista[i] == "NOMBRE:") {
            break;
        } else {
            nombreInvertido += textoLista[i] + " ";
            i--;
        }
    }
    nombreInvertido = nombreInvertido.trim(); // Eliminamos el espacio final

    const nombre = nombreInvertido.split(" ").reverse().join(" ");

    const evento = {
        nombre: nombre,
        fecha: fecha,
        hora: hora,
        lugar: lugar
    };

    // Imprimir el evento antes de enviarlo
    console.log("Evento a guardar:", evento);

    // Obtener el correo del usuario
    const emailUsuario = getCookie("email");

    // Enviar el evento al servidor
    socket.emit('nuevoEvento', emailUsuario, evento);
}

// Respuesta del servidor
socket.on('resultadoNuevoEvento', (respuesta) => {
    console.log("Respuesta del servidor:", respuesta); // Agrega este log para ver la respuesta del servidor

    if (respuesta) {
        console.log("Evento guardado correctamente");
        speak("Evento guardado correctamente");

        // Redirige al usuario al menú principal
        setTimeout(() => {
            window.location.href = "cliente.html";
        }, 2000); // Espera 2 segundos antes de redirigir
    } else {
        console.log("Error al guardar el evento");
        speak("Error al guardar el evento");
        alert("Error al guardar el evento");
    }
});


// Función para guardar la tarea
function guardarTarea(textoLista) {
    let nombreInvertido = "";
    let descripcion = "";
    let fecha = "";
    let hora = "";
    let lugar = "";

    console.log("Texto a guardar:", textoLista);

    let i = textoLista.length - 1; // Vamos de final a principio

    // Guardado de lugar
    if (i > 1 && textoLista[i-1].toUpperCase().includes("LUGAR")) {
        console.log("Guardando el lugar", textoLista[i]);
        lugar = textoLista[i];
        i -= 2;
    }

    // Guardado de hora
    if (i > 1 && textoLista[i-1].toUpperCase().includes("HORA")) {
        console.log("Guardando la hora", textoLista[i]);
        hora = textoLista[i];
        i -= 2;
    }

    // Guardado de fecha
    if (i > 1 && textoLista[i-1].toUpperCase().includes("FECHA")) {
        console.log("Guardando la fecha", textoLista[i]);
        fecha = textoLista[i];
        i -= 2;
    }  

    // Guardado de descripción
    if (i > 1 && (textoLista[i-1].toUpperCase().includes("DESCRIPCIÓN") || textoLista[i-1].toUpperCase().includes("DESCRIPCION"))) {
        console.log("Guardando la descripción", textoLista[i]);
        descripcion = textoLista[i];
        i -= 2;
    }

    // Guardado de nombre
    while (i >= 0) {
        if (textoLista[i] == "NOMBRE:") {
            break;
        } else {
            nombreInvertido += textoLista[i] + " ";
            i--;
        }
    }
    nombreInvertido = nombreInvertido.trim(); // Eliminamos el espacio final
 
    const nombre = nombreInvertido.split(" ").reverse().join(" ");  
    console.log("Nombre guardado", nombre);

    const tarea = {
        nombre: nombre,
        descripcion: descripcion,
        fecha: fecha,
        hora: hora,
        lugar: lugar,
        completada: false // Por defecto
    };

    // Obtener el correo del usuario
    const emailUsuario = getCookie("email");

    // Enviar la tarea al servidor
    socket.emit('nuevaTarea', emailUsuario, tarea);
}

// Respuesta del servidor
socket.on('resultadoNuevaTarea', (respuesta) => {
    if (respuesta) {
        console.log("Tarea guardada correctamente");

        speak("Tarea guardada correctamente");

        // Redirige al usuario al menú principal
        //setTimeout(() => {
        //    window.location.href = "cliente.html";
        //}, 2000); // Espera 2 segundos antes de redirigir
    } else {
        console.log("Error al guardar la tarea");
        speak("Error al guardar la tarea");
        alert("Error al guardar la tarea");
    }
});



// Detectar movimiento del móvil (acelerómetro)
if ("Accelerometer" in window) {
    try {
        const sensor = new Accelerometer({ frequency: 1 });

        sensor.onreading = () => {
            let x = sensor.x;
            let y = sensor.y;
            
            if (x > 5){ // Inclinación hacia la izquierda
                // Redirigimos a la página de inicio de sesión
                window.location.href = 'cliente.html';
            }

            if (y < -5) { // Inclinación hacia atrás, guardar evento o tarea
                procesarTexto();
            }
        };

        sensor.start();
    } catch (error) {
        console.error('Error con el acelerómetro:', error);
    } 
} else {
    console.error("Accelerometer API no soportada.");
}
