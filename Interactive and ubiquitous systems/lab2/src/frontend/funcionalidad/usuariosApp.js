const socket = io('http://localhost:3000');

// Funciones para mostrar y ocultar formularios
function mostrarInicioSesion() {
  document.getElementById('inicioSesion').style.display = 'block';
  document.getElementById('registroUsuario').style.display = 'none';
  document.getElementById('bajaUsuario').style.display = 'none';
}
function mostrarBajaUsuario() {
  document.getElementById('inicioSesion').style.display = 'none';
  document.getElementById('registroUsuario').style.display = 'none';
  document.getElementById('bajaUsuario').style.display = 'block';
}
function mostrarRegistroUsuario() {
  document.getElementById('inicioSesion').style.display = 'none';
  document.getElementById('registroUsuario').style.display = 'block';
  document.getElementById('bajaUsuario').style.display = 'none';
}

// Botón de vuelta al inicio de sesion
document.querySelectorAll('.botonVolver').forEach(btn => {
  btn.addEventListener('click', () => {
    mostrarInicioSesion()});
  });


// Registro del usuario
document.getElementById('botonRegistro').addEventListener('click', () => {
  mostrarRegistroUsuario();
});
const registroForm = document.getElementById('registroForm');
registroForm.addEventListener('submit', (e) => {
  e.preventDefault();

  const email = document.getElementById('registroEmail').value;
  const nombre = document.getElementById('registroNombre').value;
  const contraseña = document.getElementById('registroContraseña').value;

  if (!email || !nombre || !contraseña) {
    alert("Por favor, completa todos los campos.");
    return;
  }

  const usuario = {
    email: email,
    nombre: nombre,
    contraseña: contraseña
  };

  // Envíamos los datos del usuario al servidor
  socket.emit('registroUsuario', usuario);
});
const mensajeRegistro = document.getElementById('resultadoRegistro');
// Recibimos la respuesta del servidor del registro
socket.on('resultadoRegistroUsuario', (resultado) => {
    if (resultado === 200) {
      mensajeRegistro.textContent = "Registro exitoso";
      mensajeRegistro.style.color = "green";
    } else if (resultado === 400) {
      mensajeRegistro.textContent = "El usuario ya existe";
      mensajeRegistro.style.color = "red";
    } else {
      mensajeRegistro.textContent = "Error en el registro";
      mensajeRegistro.style.color = "orange";
    }
  });

// Función que detecta si es un ordenador o un movil
function esDispositivoMovil() {
  return /Mobi|Android|iPhone/i.test(navigator.userAgent);
}

// Inicio de sesion
const loginForm = document.getElementById('loginForm');
loginForm.addEventListener('submit', (e) => {
  e.preventDefault();

  const email = document.getElementById('loginEmail').value;
  const password = document.getElementById('loginContraseña').value;

  // Enviamos las credenciales al servidor
  socket.emit('inicioSesion', email, password);
});
// Recibimos la respuesta del servidor del inicio de sesion
const mensajeLogin = document.getElementById('resultadoLogin');
socket.on('resultadoInicioSesion', (resultado) => {
  if (Array.isArray(resultado)) {
    mensajeLogin.textContent = "Inicio de sesión exitoso";
    mensajeLogin.style.color = "green";

    // Guardamos los datos del usuario en cookies
    const email = resultado[0];
    const nombre = resultado[1];
    document.cookie = `email=${email}`;
    document.cookie = `nombre=${nombre}`;
    console.log(`Bienvenido ${nombre}, tu email es ${email}`);

    // Redirigimos a la página de inicio dependiendo de si es un ordenador o un movil
    if (esDispositivoMovil()) {
      window.location.href = 'cliente.html';
      document.cookie = `dispositivo=movil`;
    } else {
      window.location.href = 'clienteOrdenador.html';
      document.cookie = `dispositivo=ordenador`;
    }    
  }
  else if (resultado === 401) {
    mensajeLogin.textContent = "Usuario y/o contraseña incorrectos";
    mensajeLogin.style.color = "red";
  } else {
    mensajeLogin.textContent = "Error al validar las credenciales";
    mensajeLogin.style.color = "orange";
  } 
  });

// Borrado de usuario
document.getElementById('botonBaja').addEventListener('click', () => {
  mostrarBajaUsuario();
});
const deleteForm = document.getElementById('deleteForm');
deleteForm.addEventListener('submit', (e) => {
  e.preventDefault();

  const email = document.getElementById('deleteEmail').value;

  // Enviamos el email al servidor
  socket.emit('borrarUsuario', email);
});
// Recibimos la respuesta del servidor del borrado de usuario por partes
const mensajeDelete = document.getElementById('resultadoDelete');
socket.on('resultadoBorrarUsuario', (resultado) => {
    if (resultado === 200) {
      mensajeDelete.textContent = "Usuario borrado correctamente";
      mensajeDelete.style.color = "green";
    } else if (resultado === 400) {
      mensajeDelete.textContent = "Error al borrar el usuario";
      mensajeDelete.style.color = "red";
    }
  });
