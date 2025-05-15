const socket = io('http://localhost:3000');

const palabrasANumeros = {
  "uno": '01',
  "dos": '02',
  "tres": '03',
  "cuatro": '04',
  "cinco": '05',
  "seis": '06',
  "siete": '07',
  "ocho": '08',
  "nueve": '09',
  "diez": '10',
  "once": '11',
  "doce": '12',
  "trece": '13',
  "catorce": '14',
  "quince": '15',
  "dieciséis": '16',
  "diecisiete": '17',
  "dieciocho": '18',
  "diecinueve": '19',
  "veinte": '20',
  "veintiuno": '21',
  "veintidos": '22',
  "veintitres": '23',
  "veinticuatro": '24',
  "veinticinco": '25',
  "veintiseis": '26',
  "veintisiete": '27',
  "veintiocho": '28',
  "veintinueve": '29',
  "treinta": '30',
  "cuarenta": '40',
  "cincuenta": '50',
};

// Obtención de cookies
function getCookie(nombre) {
  const valor = `; ${document.cookie}`;
  const partes = valor.split(`; ${nombre}=`);
  if (partes.length === 2) return partes.pop().split(';').shift();
}

// Función que personaliza el saludo al usuario
document.addEventListener('DOMContentLoaded', () => {
  const fecha = getCookie("fecha");
  
  if (fecha == "evento"){
    // Muestra el header correspondiente a eventos y oculta el de tareas
    document.getElementById('headerEvento').style.display = 'block';
    document.getElementById('headerTarea').style.display = 'none';

    // Muestra el formulario de eventos y oculta el de tareas
    document.getElementById('formEvento').style.display = 'flex';
    document.getElementById('formTarea').style.display = 'none';
  }
  else if (fecha == "tarea"){
    // Muestra el header correspondiente a tareas y oculta el de eventos
    document.getElementById('headerEvento').style.display = 'none';
    document.getElementById('headerTarea').style.display = 'block';

    // Muestra el formulario de tareas y oculta el de eventos
    document.getElementById('formEvento').style.display = 'none';
    document.getElementById('formTarea').style.display = 'flex';
  }

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

// Función para el reconocimiento de voz
function empezarReconocimiento(callback) {
  window.SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
  const speechRecognition = new SpeechRecognition();
  console.log("Reconocimiento de voz iniciado.");

  //Configuración
  speechRecognition.lang = "es-ES";
  speechRecognition.start();

  speechRecognition.onresult = (event) => {
      let comandoVoz = event.results[0][0].transcript.toUpperCase();
      console.log(comandoVoz);
      callback(comandoVoz);
  };

  speechRecognition.onerror = () => {
      console.log("Comando no reconocido");
      speak("Comando no reconocido");
  };
}

// Deteccion de gestos
if ("Accelerometer" in window) {
  try {
      const sensor = new Accelerometer({ frequency: 1 });

      sensor.onreading = () => {
        let x = sensor.x;
        let y = sensor.y;

        if (x > 5) { // Inclinación a la izquierda, volver al menu principal
          console.log("Regresando al menú");
          window.location.href = 'cliente.html';;
        }

        if (y < -5) { // Inclinación hacia atrás, guardar evento o tarea
          const fecha = getCookie("fecha");

          if (fecha == "evento") {
            console.log("Guardando evento");
            guardarEvento();
          }
          else if (fecha == "tarea") {
            console.log("Guardando tarea");
            guardarTarea();
          }
        }
      };

      sensor.start();

  } catch (error) {
      console.error("Error con el acelerómetro:", error);
  }
} else {
  console.error("Accelerometer API no soportada.");
}


// Formulario de eventos
// Nombre
document.getElementById('nombreEventoButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Nombre recibido:", resultado);
    // Muestra el nombre por pantalla
    const p = document.getElementById("nombreEvento");
    p.innerHTML = resultado;
  });
});
// Fecha
document.getElementById('fechaEventoButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Fecha recibida:", resultado);
    // Muestra la fecha por pantalla
    const p = document.getElementById("fechaEvento");
    p.innerHTML = resultado;
  });
});
// Hora
document.getElementById('horaEventoButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Hora recibida:", resultado);
    // Muestra la fecha por pantalla
    const p = document.getElementById("horaEvento");
    p.innerHTML = resultado;
  });
});
// Lugar
document.getElementById('lugarEventoButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Lugar recibido:", resultado);
    // Muestra la fecha por pantalla
    const p = document.getElementById("lugarEvento");
    p.innerHTML = resultado;
  });
});


// Formulario de tareas
// Nombre
document.getElementById('nombreTareaButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Nombre recibido:", resultado);
    // Muestra el nombre por pantalla
    const p = document.getElementById("nombreTarea");
    p.innerHTML = resultado;
  });
});
// Descripcion
document.getElementById('descripcionTareaButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Descripción recibida:", resultado);
    // Muestra el nombre por pantalla
    const p = document.getElementById("descripcionTarea");
    p.innerHTML = resultado;
  });
});
// Fecha límite
document.getElementById('fechaTareaButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Fecha recibida:", resultado);
    // Muestra la fecha por pantalla
    const p = document.getElementById("fechaTarea");
    p.innerHTML = resultado;
  });
});
// Hora límite
document.getElementById('horaTareaButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Hora recibida:", resultado);
    // Muestra la fecha por pantalla
    const p = document.getElementById("horaTarea");
    p.innerHTML = resultado;
  });
});
// Lugar
document.getElementById('lugarTareaButton').addEventListener('click', () => {
  // Comienza reconocimiento de voz
  empezarReconocimiento((resultado) => {
    console.log("Lugar recibido:", resultado);
    // Muestra la fecha por pantalla
    const p = document.getElementById("lugarTarea");
    p.innerHTML = resultado;
  });
});


// Función para convertir palabras a números
function convertirPalabraANumero(palabra) {
  return palabrasANumeros[palabra.toLowerCase()] ?? "00"; // Devuelve 00 si no se encuentra la palabra
}

// Función para validar si es un entero o un carácter
function validarNumeros(valor){
  if (!Number.isInteger(valor)) {
    return convertirPalabraANumero(valor);
  } else {
    return valor.toString();
  }
}

// Función para procesar la fecha y convertirla al formato YYYY-MM-DD
function procesarFecha(fecha) {
  const fechaLista = fecha.split(" ");

  // Validamos que se haya introducido bien
  if (fechaLista.length != 5) {
    return "";
  }

  const dia = validarNumeros(fechaLista[0]);
  const mes = validarNumeros(fechaLista[2]);
  const anio = fechaLista[4].toString(); // Suele ser un número directamente

  return `${anio}-${mes}-${dia}`; // Formato YYYY-MM-DD  
}

// Función para procesar la hora y convertirla al formato HH:MM
function procesarHora(horaString) {
  const horaLista = horaString.split(" ");

  // Validamos que se haya introducido una hora
  if (horaLista.length > 2 || horaLista[0] === "[Ejemplo:") {
    return "09:00"; // Hora por defecto
  } 

  const hora = validarNumeros(horaLista[0]);

  let minutos = "00";
  if (horaLista.length == 2) { // Si solo tiene una palabra, se toman los minutos como '00'
    minutos = validarNumeros(horaLista[1]);
  }

  return `${hora}:${minutos}`; // Formato HH:MM
}


// Función para procesar los campos del evento y mandarselos al servidor para su almacenamiento
function guardarEvento() {
  const nombre = document.getElementById("nombreEvento").innerHTML;

  const fechaString = document.getElementById("fechaEvento").innerHTML;
  const fecha = procesarFecha(fechaString); // Formato YYYY-MM-DD

  // Validamos que estos campos esten introducidos
  if (nombre.startsWith("Cualquier") || fecha === "") { 
    console.log("Nombre o fecha incorrectos");
    speak("Nombre o fecha incorrectos");
    alert("Nombre o fecha incorrectos");
    return;
  }

  const horaString = document.getElementById("horaEvento").innerHTML;
  const hora = procesarHora(horaString); // Formato HH:MM
  
  let lugar = document.getElementById("lugarEvento").innerHTML;
  // Validamos que se haya introducido un lugar
  if (lugar.startsWith("[Cualquier")) {
    lugar = ""; // Lugar por defecto
  }
  
  const evento = {
    nombre: nombre,
    fecha: fecha,
    hora: hora,
    lugar: lugar
  };

  // Enviar datos al servidor
  socket.emit('nuevoEvento', getCookie("email"), evento);
}
// Respuesta del servidor
socket.on('resultadoNuevoEvento', (respuesta) => {
  if (respuesta) {
    console.log("Evento guardado correctamente");

    speak("Evento guardado correctamente");
    document.cookie = "fecha=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";

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

// Función para procesar los campos de la tarea y mandarselos al servidor para su almacenamiento
function guardarTarea() {
  const nombre = document.getElementById("nombreTarea").innerHTML;
  // Validamos que el nombre se haya introducido
  if (nombre.startsWith("Cualquier")) { 
    console.log("Nombre obligatorio");
    speak("Nombre obligatorio");
    alert("Nombre obligatorio");
    return;
  }

  let descripcion = document.getElementById("descripcionTarea").innerHTML;
  // Validamos que se haya introducido una descripcion
  if (descripcion.startsWith("[Cualquier")) {
    descripcion = ""; // Lugar por defecto
  }

  const fechaString = document.getElementById("fechaTarea").innerHTML;
  const fecha = procesarFecha(fechaString); // Formato YYYY-MM-DD

  const horaString = document.getElementById("horaTarea").innerHTML;
  const hora = procesarHora(horaString); // Formato HH:MM

  let lugar = document.getElementById("lugarTarea").innerHTML;
  // Validamos que se haya introducido un lugar
  if (lugar.startsWith("[Cualquier")) {
    lugar = ""; // Lugar por defecto
  }

  const tarea = {
    nombre: nombre,
    descripcion: descripcion,
    fecha: fecha,
    hora: hora,
    lugar: lugar,
    completada: false // Por defecto
  };

  // Enviar datos al servidor
  socket.emit('nuevaTarea', getCookie("email"), tarea);
}
// Respuesta del servidor
socket.on('resultadoNuevaTarea', (respuesta) => {
  if (respuesta) {
    console.log("Tarea guardada correctamente");

    speak("Tarea guardada correctamente");
    document.cookie = "fecha=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";

    // Redirige al usuario al menú principal
    setTimeout(() => {
      window.location.href = "cliente.html";
    }, 2000); // Espera 2 segundos antes de redirigir
  } else {
    console.log("Error al guardar la tarea");
    speak("Error al guardar la tarea");
    alert("Error al guardar la tarea");
  }
});
