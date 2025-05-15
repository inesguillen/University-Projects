const registroButton = document.querySelector('.btn1');
const registroPopup = document.getElementById('registro-popup');
const cancelarRegistro = document.getElementById('cancelar-registro');
const registroFormulario = document.getElementById('registro-formulario');
const errorMessageRegistro = document.getElementById('error-mensaje-reg');
const limpiarFormulario = document.getElementById('limpiar-campos');
const hijosCont = document.getElementById('hijos-formulario');

function mostrarError(mensaje) {
    ocultarError();
    errorMessageRegistro.textContent = mensaje;
    errorMessageRegistro.style.display = 'block';
}

function ocultarError() {
    errorMessageRegistro.style.display = 'none';
}

registroButton.addEventListener('click', () => {
    registroPopup.style.display = 'block';
    ocultarError();}
);

cancelarRegistro.addEventListener('click', () => {
    const confirmacion = confirm('¿Está seguro de que desea cancelar el registro?');
    if (confirmacion) {
        registroPopup.style.display = 'none';
        ocultarError();
    }
});

limpiarFormulario.addEventListener('click', () => {
    const confirmacion = confirm('¿Está seguro de que desea limpiar todos los campos?');
    if (confirmacion) {
        registroFormulario.reset();
        ocultarError();
        hijosCont.innerHTML = '';
    }
});

// FORMULARIO DE HIJOS
document.getElementById('hijos').addEventListener('input', (event) => {
    const numeroHijos = parseInt(event.target.value) || 0;
    hijosCont.innerHTML = '';

    if (numeroHijos > 0) {
        for (let i = 0; i < numeroHijos; i++) {
            const hijoDiv = document.createElement('div');
            hijoDiv.classList.add('hijo');
            hijoDiv.innerHTML = `
                <h3>Datos del Hijo ${i + 1}</h3>
                <label for="nombre-hijo-${i}">Nombre:</label>
                <input type="text" id="nombre-hijo-${i}" class="nombre-hijo" required>
                <label for="edad-hijo-${i}">Edad:</label>
                <input type="number" id="edad-hijo-${i}" class="edad-hijo" min="0" required>
                <label for="juguetes-hijo-${i}">Juguetes favoritos:</label>
                <input type="text" id="juguetes-hijo-${i}" class="juguetes-hijo" required>
            `;
            hijosCont.appendChild(hijoDiv);
			
        }
    }
});

//FORMULARIO DE REGISTRO
registroFormulario.addEventListener('submit', (event) => {
    event.preventDefault();

    const submitButton = document.querySelector('.btnFormulario[type="submit"]');
    submitButton.disabled = true;

    const nombreUsuario = document.getElementById('nombre-usuario').value;
    const contrasenaRegistro = document.getElementById('contrasena-registro').value;
    const repetirContrasena = document.getElementById('repetir-contrasena').value;
    const correo = document.getElementById('correo').value;
    const ciudad = document.getElementById('ciudad').value;
    const pais = document.getElementById('pais').value;
    const genero = document.getElementById('genero').value;
    const hijos  = parseInt(document.getElementById('hijos').value) || 1;

    // CONFIRMAR QUE LOS DATOS SEAN CORRECTOS
    if (nombreUsuario.length < 3) {
        mostrarError('El nombre de usuario debe tener al menos 3 caracteres.');
        submitButton.disabled = false;
        return;
    }

    const contrasenaValida = /^(?=.*\d{2,})(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%^&*()_+])[A-Za-z\d!@#$%^&*()_+]{12,}$/;
    if (!contrasenaValida.test(contrasenaRegistro)) {
        mostrarError('La contraseña debe tener al menos 12 caracteres, 2 números, 1 carácter especial, 1 letra mayúscula y 1 letra minúscula.');
        submitButton.disabled = false;
        return;
    }

    if (contrasenaRegistro !== repetirContrasena) {
        mostrarError('Las contraseñas no coinciden.');
        submitButton.disabled = false;
        return;
    }

    const correoValido = /^[^@\s]+@[^@\s]+\.[^@\s]+$/;
    if (!correoValido.test(correo)) {
        mostrarError('El formato del correo electrónico no es válido.');
        submitButton.disabled = false;
        return;
    }

    if (ciudad.length < 3 || pais.length < 3) {
        mostrarError('La ciudad y el país deben tener al menos 3 caracteres.');
        submitButton.disabled = false;
        return;
    }

    // GUARDAR DATOS DE LOS HIJOS POR USUARIO
    const hijosDatos = [];
    if (hijos > 0) {
        for (let i = 0; i < hijos; i++) {
            const nombreHijo = document.getElementById(`nombre-hijo-${i}`).value;
            const edadHijo = document.getElementById(`edad-hijo-${i}`).value;
            const juguetesHijo = document.getElementById(`juguetes-hijo-${i}`).value;

            if (nombreHijo.length < 3) {
                mostrarError(`El nombre del hijo ${i + 1} debe tener al menos 3 caracteres.`);
                submitButton.disabled = false;
                return;
            }

            if (edadHijo < 1) {
                mostrarError(`La edad del hijo ${i + 1} debe ser mayor que 0.`);
                submitButton.disabled = false;
                return;
            }

            if (juguetesHijo.length === 0) {
                mostrarError(`Por favor, indica los juguetes favoritos del hijo ${i + 1}.`);
                submitButton.disabled = false;
                return;
            }

            hijosDatos.push({nombre: nombreHijo, edad: edadHijo, juguetes: juguetesHijo});
        }
    }

    const usuarios = JSON.parse(localStorage.getItem('usuarios')) || {};

    // CONFIRMAR QUE EL CORREO NO SE HA LOGGEADO ANTERIORMENTE
    if (usuarios[correo]) {
        mostrarError('Este correo electrónico ya está registrado.');
        submitButton.disabled = false;
        return;
    }

    // GUARDAR DATOS DEL USUARIO
    usuarios[correo] = {
        nombreUsuario,
        contrasena: contrasenaRegistro,
        ciudad,
        pais,
        genero,
        hijos,
		hijosDatos,
        correo
    };

    localStorage.setItem('usuarios', JSON.stringify(usuarios));
    alert('Registro exitoso. Puedes iniciar sesión ahora.');

    registroPopup.style.display = 'none';
    ocultarError();

    // RESETEAR FORMULARIO
    registroFormulario.reset();
    hijosCont.innerHTML = '';
});

