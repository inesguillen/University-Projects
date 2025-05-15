const socket = io('http://localhost:3000');

let nombre;
let email;

let pendiente = false;
let tareaRecuperada;

// Conexión al servidor
socket.on('connect', () => {
  console.log('Conectado al servidor!');
});

// Obtención de cookies
function getCookie(nombre) {
  const valor = `; ${document.cookie}`;
  const partes = valor.split(`; ${nombre}=`);
  if (partes.length === 2) return partes.pop().split(';').shift();
}

// Función que personaliza el saludo al usuario
document.addEventListener('DOMContentLoaded', () => {
  nombre = getCookie("nombre");
  email = getCookie("email");
  const bienvenida = document.getElementById('bienvenida');

  bienvenida.innerHTML = bienvenida.innerHTML.replace(/\[\$nombre\$\]/g, nombre);

  socket.emit('registrarSocket', email);

  // Comprobamos que no haya tareas pendientes que hayan vencido
  socket.emit('recuperarTareasPendientes', email)
  console.log("Enviada peticion para recuperar tareas");
});
socket.on('resultadoRecuperarTareasPendientes', (respuesta) => {
  if (respuesta) {
      console.log("Tareas pendientes recuperadas correctamente", respuesta);

      const hoy = new Date();
      const fechaFormateada = hoy.toISOString().split('T')[0];

      respuesta.forEach(tarea => {
        const fecha = tarea.fecha;

        if(fecha && fecha <= fechaFormateada){
          document.getElementById('comandosSection').style.display = 'none';
          document.getElementById('tareaDetectada').style.display = 'block';
          document.getElementById('tareaLimite').innerHTML = tarea.nombre + ", " + tarea.fecha + ", " + tarea.hora + ", " + tarea.descripcion + ", " + tarea.lugar;
          pendiente = true;
          tareaRecuperada = tarea;
        }
      });
  } else {
      console.log("Error al recuperar las tareas pendientes");
      alert("Error al recuperar las tareas pendientes");
  }
});

function cambiarComandos() {
  document.getElementById('comandosSection').style.display = 'block';
  document.getElementById('tareaDetectada').style.display = 'none';
}

//Función para dar retroalimentación por voz
function speak(text) {
  const utterance = new SpeechSynthesisUtterance(text);
  utterance.lang = "es-ES";
  utterance.rate = 1; 
  utterance.pitch = 1; 
  utterance.volume = 1; 
  
  speechSynthesis.speak(utterance);
}

// Función para el reconocimiento de voz
function empezarReconocimiento() {
  window.SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
  const speechRecognition = new SpeechRecognition();
  console.log("Reconocimiento de voz iniciado.");

  //Configuración
  speechRecognition.lang = "es-ES";
  speechRecognition.start();

  speechRecognition.onresult = (event) => {
      let comandoVoz = event.results[0][0].transcript.toUpperCase();
      console.log(comandoVoz);
      gestionComandoVoz(comandoVoz);
  };

  speechRecognition.onerror = () => {
      speak("Error en el reconocimiento de voz");
      const p = document.getElementById("comandoVoz");
      p.innerHTML = "Comando no reconocido";
  };
}

// Función para el manejo de comandos de voz
function gestionComandoVoz(comandoVoz) {
  if (comandoVoz.includes("EVENTO")) { // Vamos a la ventana de gestion de eventos
    // Indicamos que se va a añadir un evento
    document.cookie = `fecha=evento`;
    speak("¿Qué evento quieres añadir?")
    // Redirigimos a la página de añadir fechas
    window.location.href = 'fechasController.html';
  } else if (comandoVoz.includes("TAREA")) { // Vamos a la ventana de gestion de tareas
    document.cookie = `fecha=tarea`;
    speak("¿Qué tarea quieres añadir?")
    // Redirigimos a la página de añadir fechas
    window.location.href = 'fechasController.html';
  } else if (comandoVoz.includes("CALENDARIO")) { // Vamos a la ventana de gestion del calendario
      speak("Consultando el calendario");
      // Redirigimos a la página de gestion del calendario
      window.location.href = 'calendarioController.html';
  } else { // Comando no reconocido
    const p = document.getElementById("comandoVoz");
    p.innerHTML = "Comando no reconocido";
    speak("Comando no reconocido");
    return;
  }
  const p = document.getElementById("comandoVoz");
  p.innerHTML = comandoVoz;
}

// Deteccion de gestos
if ("Accelerometer" in window) {
  try {
      const sensor = new Accelerometer({ frequency: 1 });

      sensor.onreading = () => {
        let x = sensor.x;
        let y = sensor.y;

        if (x < -5) { // Inclinación a la derecha
          if (!pendiente){ // Reconocimiento de voz
            speak("Habla");

            setTimeout(() => {
              empezarReconocimiento();
            }, 1000); // Espera 1 segundo antes de empezar reconocimiento

          } else {
            socket.emit('completarTarea', email, tareaRecuperada) // Completar tarea
            pendiente = false;
            cambiarComandos();
          }
          
        } else if (x > 5) { // Inclinación a la izquierda
          if (!pendiente){ // Escanear
            window.location.href = 'escanearController.html';

          } else { // Posponer tarea
            let fecha = new Date(tareaRecuperada.fecha);
            fecha.setDate(fecha.getDate() + 7);
            
            let año = fecha.getFullYear();
            let mes = (fecha.getMonth() + 1).toString().padStart(2, '0'); // los meses empiezan en 0
            let dia = fecha.getDate().toString().padStart(2, '0');
            
            const nuevaFecha = `${año}-${mes}-${dia}`;

            console.log("Posponiendo tarea a la fecha", nuevaFecha);

            cambiarComandos();

            socket.emit('posponerTarea', email, tareaRecuperada, nuevaFecha);
            pendiente = false;
          }
        }

        if (y < -5) { // Inclinación hacia atrás, cierra sesión
          console.log("Cerrando sesión");

          speak("Cerrando sesión. Hasta luego!");
          // Borramos las cookies del usuario
          document.cookie = "email=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
          document.cookie = "nombre=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";

          // Redirigimos a la página de inicio de sesión
          window.location.href = 'usuariosController.html';
        }
      };

      sensor.start();

  } catch (error) {
      console.error("Error con el acelerómetro:", error);
  }
} else {
  console.error("Accelerometer API no soportada.");
}

// Desconexión del servidor
socket.on('disconnect', () => {
  console.log('Desconectado del servidor!');
});