const socket = io('http://localhost:3000');

// Conexión al servidor
socket.on('connect', () => {
    console.log('Conectado al servidor!');
});
  
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

let inicio = true; // Controla las acciones del giroscopio
let email;

// Obtención de cookies
function getCookie(nombre) {
    const valor = `; ${document.cookie}`;
    const partes = valor.split(`; ${nombre}=`);
    if (partes.length === 2) return partes.pop().split(';').shift();
}

document.addEventListener('DOMContentLoaded', () => {
    // Enviamos el id del socket al servidor
    email = getCookie("email");
    socket.emit('registrarSocket', email);
});

function mostrarComandos() {
    document.getElementById("comandosIniciales").style.display = 'none';
    document.getElementById("comandosCambios").style.display = 'block';
    inicio = false;
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
        let comandoVoz = event.results[0][0].transcript.toLowerCase().trim();
        console.log("Comando de voz reconocido:", comandoVoz);
        speak("Comando de voz reconocido: " + comandoVoz);
        // Limpiar posibles palabras adicionales como "mes de enero"
        Object.keys(meses).forEach(mes => {
            if (comandoVoz.includes(mes)) {
                consultarMes(mes);
            }
        });

        // consultarMes(comandoVoz)
    };
  
    speechRecognition.onerror = () => {
        console.log("Comando no reconocido");
        speak("Comando no reconocido");
    };
}

let videoElement = document.getElementById('video');

const hands = new Hands({
  locateFile: (file) => `https://cdn.jsdelivr.net/npm/@mediapipe/hands/${file}`,
});

// Ajustamos las opciones de MediaPipe Hands
hands.setOptions({
  maxNumHands: 1,
  modelComplexity: 1,
  minDetectionConfidence: 0.7,
  minTrackingConfidence: 0.7,
});

// Configuramos la cámara
const camera = new Camera(videoElement, {
  onFrame: async () => {
    await hands.send({ image: videoElement });
    console.log('Enviando imagen al modelo');
  },
  width: 320,
  height: 240
});
camera.start();

let currentMonth = null;

// Función para actualizar el mes en el MOVIL
function actualizarMes() {
  if (currentMonth === null) return; // No actualizamos nada al inicio
  document.getElementById('headerTarea').innerHTML = `Mes actual: ${Object.keys(meses)[currentMonth]}`;
}

let lastGestureTime = 0;
const gestureCooldown = 3000; //1500; // milisegundos

function detectHandMovement(results) {
    if (!results.multiHandLandmarks || results.multiHandLandmarks.length === 0) return;

    const landmarks = results.multiHandLandmarks[0];
    const now = Date.now();

    // Comprobaciones por dedo
    const indexUp = (
        landmarks[8].y < landmarks[6].y &&
        landmarks[6].y < landmarks[5].y
    );

    const thumbRight = (
        landmarks[4].x < landmarks[3].x && // Mano derecha: pulgar extendido a la izquierda en pantalla
        Math.abs(landmarks[4].y - landmarks[3].y) < 0.1 // alineado verticalmente
    );

    // El resto de dedos NO deben estar extendidos
    const middleUp = landmarks[12].y < landmarks[10].y && landmarks[10].y < landmarks[9].y;
    const ringUp = landmarks[16].y < landmarks[14].y && landmarks[14].y < landmarks[13].y;
    const pinkyUp = landmarks[20].y < landmarks[18].y && landmarks[18].y < landmarks[17].y;

    const otherFingersUp = middleUp || ringUp || pinkyUp;

    // Avanzar mes: solo el índice extendido
    if (indexUp && !thumbRight && !otherFingersUp) {
        if (now - lastGestureTime > gestureCooldown) {
            console.log("Índice extendido → avanzar mes");
            speak("Avanzando al siguiente mes");
            avanzarMes();
            lastGestureTime = now;
        }
    }

    // Retroceder mes: solo el pulgar extendido
    else if (thumbRight && !indexUp && !otherFingersUp) {
        if (now - lastGestureTime > gestureCooldown) {
            console.log("Pulgar extendido → retroceder mes");
            speak("Retrocediendo al mes anterior");
            retrocederMes();
            lastGestureTime = now;
        }
    }
}

// Función para avanzar al siguiente mes
function avanzarMes() {
    if (currentMonth === null) {
        currentMonth = 0;
    } else {
    currentMonth = (currentMonth + 1) % 12;  // Avanza al siguiente mes (circular)
    const mesActual = Object.keys(meses)[currentMonth];
    consultarMes(mesActual);  // Llama a la función para consultar el mes
    }
}

// Función para retroceder al mes anterior
function retrocederMes() {
    if (currentMonth === null) {
        currentMonth = 11;
    } else {
        currentMonth = (currentMonth - 1 + 12) % 12;
        const mesActual = Object.keys(meses)[currentMonth];
        consultarMes(mesActual);  // Llama a la función para consultar el mes
    }
}

// Función que se ejecuta cuando hay resultados de la detección de la mano
hands.onResults((results) => {
  detectHandMovement(results);
});

// Función de inicialización
function init() {
  actualizarMes();
}

init();

// Función para consultar el mes (usada en comandos de voz)
function consultarMes(comandoVoz) {
    const mes = meses[comandoVoz];

    if (!mes) {
        console.warn("Mes no reconocido:", mes);
        speak("No he entendido el mes, intenta de nuevo");
        return;
    }

    currentMonth = Object.keys(meses).indexOf(comandoVoz);
    actualizarMes();  // Muestra el mes en pantalla
    //speak(`Mostrando el mes de ${comandoVoz}`);  // Retroalimentación por voz

    const fechaInicio = `2025-${mes}-01`;
    const ultimoDia = new Date(2025, parseInt(mes), 0).getDate(); // 0 da el último día del mes anterior
    const fechaFin = `2025-${mes}-${ultimoDia.toString().padStart(2, '0')}`;

    document.cookie = `fechaActual=${fechaInicio}`;
    document.cookie = `consulta=mes`;

    // Enviar la petición al servidor para recuperar eventos y tareas
    socket.emit('envioFecha', email, comandoVoz);
    socket.emit('recuperarEventosMes', email, fechaInicio, fechaFin);
    socket.emit('recuperarTareasMes', email, fechaInicio, fechaFin);

}

// Deteccion de gestos
if ("Accelerometer" in window) {
    try {
        const sensor = new Accelerometer({ frequency: 1 });

        sensor.onreading = () => {
            let x = sensor.x;
            let y = sensor.y;

            if (x < -5){ // Inclinación a la derecha
                if (inicio){ // Ver día actual
                    console.log("Mostrando día actual");
                    speak("Mostrando día actual");

                    const hoy = new Date();
                    const fechaFormateada = hoy.toISOString().split('T')[0];

                    document.cookie = `fechaActual=${fechaFormateada}`;
                    document.cookie = `consulta=dia`;

                    // Enviamos la peticion de que mande la fecha al ordenador
                    socket.emit('envioFecha', email, fechaFormateada);

                    // Enviamos la fecha al servidor para que recupere los eventos y las tareas
                    socket.emit('recuperarEventosDia', email, fechaFormateada);
                    socket.emit('recuperarTareasPendientes', email);
                    // Los eventos y tareas le llegarán al ordenador para que los procese y los muestre
                    mostrarComandos();
                } else { // Navegar al mes o día siguiente
                    console.log("Navegando al siguiente");
                    speak("Siguiente");

                    const fechaActual = getCookie("fechaActual");
                    const consulta = getCookie("consulta");

                    // Día
                    if (consulta === "dia"){
                        let fecha = new Date(fechaActual);
                        fecha.setDate(fecha.getDate() + 1);

                        let año = fecha.getFullYear();
                        let mes = (fecha.getMonth() + 1).toString().padStart(2, '0'); // los meses empiezan en 0
                        let dia = fecha.getDate().toString().padStart(2, '0');

                        const nuevaFecha = `${año}-${mes}-${dia}`;

                        document.cookie = `fechaActual=${nuevaFecha}`;
                        
                        // Enviamos la peticion de que mande la fecha al ordenador
                        socket.emit('envioFecha', email, nuevaFecha);

                        // Enviamos la fecha al servidor para que recupere los eventos y las tareas
                        socket.emit('recuperarEventosDia', email, nuevaFecha);
                        socket.emit('recuperarTareasPendientes', email);

                    } else { // Mes
                        const fechaSplit = fechaActual.split("-");

                        let mes = parseInt(fechaSplit[1]);
                        mes = (mes+1)%12;

                        const mesNumero = mes.toString().padStart(2, '0');

                        consultarMes(Object.keys(meses).find(clave => meses[clave] === mesNumero));
                    }
                }

            } else if (x > 5) { // Inclinación a la izquierda
                if (inicio) { // Reconocer mes
                    console.log("Esperando mes");
                    empezarReconocimiento();
                    mostrarComandos();
                } else { // Navegar al mes o día anterior
                    console.log("Navegando al anterior");
                    speak("Anterior");

                    const fechaActual = getCookie("fechaActual");
                    const consulta = getCookie("consulta");

                    // Día
                    if (consulta === "dia"){
                        let fecha = new Date(fechaActual);
                        fecha.setDate(fecha.getDate() - 1);

                        let año = fecha.getFullYear();
                        let mes = (fecha.getMonth() + 1).toString().padStart(2, '0'); // los meses empiezan en 0
                        let dia = fecha.getDate().toString().padStart(2, '0');

                        const nuevaFecha = `${año}-${mes}-${dia}`;

                        document.cookie = `fechaActual=${nuevaFecha}`;
                        
                        // Enviamos la peticion de que mande la fecha al ordenador
                        socket.emit('envioFecha', email, nuevaFecha);
                        
                        // Enviamos la fecha al servidor para que recupere los eventos y las tareas
                        socket.emit('recuperarEventosDia', email, nuevaFecha);
                        socket.emit('recuperarTareasPendientes', email);

                    } else { // Mes
                        const fechaSplit = fechaActual.split("-");

                        let mes = parseInt(fechaSplit[1]);
                        mes = (mes-1)%12;

                        const mesString = mes.toString().padStart(2, '0');

                        consultarMes(Object.keys(meses).find(clave => meses[clave] === mesString));
                    }
                }
            } 

            if (y < -5) { // Inclinación hacia atrás, volver al menú
            console.log("Volviendo al menú");
            window.location.href = 'cliente.html';
            }
        };

        sensor.start();

    } catch (error) {
        console.error("Error con el acelerómetro:", error);
    }
} else {
console.error("Accelerometer API no soportada.");
}

