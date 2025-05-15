const cartaFormulario = document.getElementById('carta-formulario');
const errorMessageCarta = document.getElementById('error-mensaje-carta');

function ocultarError(){errorMessageCarta.style.display = 'none';}
function mostrarError(mensaje) {
    ocultarError();
    errorMessageCarta.textContent = mensaje;
    errorMessageCarta.style.display = 'block';
}

// FORMULARIO PARA ENVIAR LAS CARTAS
cartaFormulario.addEventListener('submit', (event) => {
        event.preventDefault();
        
        const nombre = document.getElementById('nombre').value;
        const correo = document.getElementById('correo-carta').value;
        const ciudad = document.getElementById('ciudad-carta').value;
        const pais = document.getElementById('pais-carta').value;
        const carta = document.getElementById('carta').value;

        // COMPROBAR SI INICIO DE SESION
        const loggedUserEmail = localStorage.getItem('loggedUserEmail');
        if (!loggedUserEmail) {
            mostrarError('Solo se puede enviar una carta si has iniciado sesión.');
            return; 
        }
    
        // COMPROBAR SI EL CORREO COINCIDE
        if (correo !== loggedUserEmail) {
            mostrarError(`El correo electrónico debe coincidir con el que usaste para registrarte. El correo registrado es '${loggedUserEmail}'.`);
            return;
        }

        const nuevaCarta = {
            nombre: nombre,
            correo,
            ciudad: ciudad,
            pais : pais,
            carta,
            usuario: loggedUserEmail // GUARDAMOS LA CARTA PARA EL USUARIO QUE HA INICIADO SESION
        };
        
        const cartasEnviadas = JSON.parse(localStorage.getItem('cartasEnviadas')) || [];
        cartasEnviadas.push(nuevaCarta); //GUARDAMOS CON EL RESTO DE SUS CARTAS ENVIADAS
        localStorage.setItem('cartasEnviadas', JSON.stringify(cartasEnviadas)); 
         
        alert('Carta enviada con éxito');

        //RESETEAMOS FORMULARIO
        cartaFormulario.reset();
        ocultarError();
});



