const inicioButton = document.querySelector('.btn0');
const inicioPopup = document.getElementById('inicio-popup');
const inicioFormulario = document.getElementById('inicio-formulario');
const cancelarInicio = document.getElementById('cancelar-inicio');
const errorMessage = document.getElementById('error-mensaje-inicio');
const newButton = document.getElementById('new-button');

const editarPopup = document.getElementById('editar-popup');
const editarFormulario = document.getElementById('editar-formulario');
const cancelarEditar = document.getElementById('cancelar-editar');
const errorMensajeEditar = document.getElementById('error-mensaje-editar');
const hijosContainer = document.getElementById('hijos-container'); // El contenedor de hijos

const cartasPopup = document.getElementById('cartas-popup');
const cartasContainer = document.getElementById('cartas-container');
const cerrarCartasPopup = document.getElementById('cerrar-cartas-popup');

function mostrarPopup(display) {
    inicioPopup.style.display = display;
    errorMessage.style.display = 'none';
}

inicioButton.addEventListener('click', () => {
    mostrarPopup('block');
});

cancelarInicio.addEventListener('click', () => {mostrarPopup('none');});

//FORMULARIO INICIO DE SESION
inicioFormulario.addEventListener('submit', (event) => {
    event.preventDefault();

    const usuario = document.getElementById('usuario').value;
    const contrasena = document.getElementById('contrasena').value;
    const usuarios = JSON.parse(localStorage.getItem('usuarios')) || {};
    const storedUserData = Object.values(usuarios).find(user => user.nombreUsuario == usuario);

    //CONFIRMAMOS QUE LOS DATOS ESTAN REGISTRADOS Y CORRESPONDEN
    if (storedUserData && contrasena == storedUserData.contrasena) {
        localStorage.setItem('isLoggedIn', 'true');
        localStorage.setItem('loggedUserEmail', storedUserData.correo);

        alert('Inicio de sesión exitoso');
        mostrarPopup('none');
        
        // OCULTAMOS BOTONES DE REGISTRO E INICIO DE SESION
        inicioButton.style.display = 'none';
        registroButton.style.display = 'none';

        //CREAMOS EL BOTON DE PERFIL
        crearButtonPerfil();
    } 
    else {
        errorMessage.textContent = 'Error: Usuario o contraseña incorrectos';
        errorMessage.style.display = 'block';
    }
});

//CREAMOS BOTON DE PERFIL
function crearButtonPerfil() {
    const perfilButton = document.createElement("button");
    perfilButton.innerText = "Perfil"; 
    perfilButton.id = "perfilButton";
    perfilButton.addEventListener('click', mostrarOpciones); //MOSTRAMOS LAS OPCIONES AL PULSAR EL PERFIL

    newButton.appendChild(perfilButton);
    newButton.style.display = 'flex';
}

function mostrarOpciones() {
    const opcionesContainer = document.getElementById("opcionesContainer") || crearOpcionesContainer(); //LAMAR A LA FUNCION
    opcionesContainer.style.display = opcionesContainer.style.display === "none" ? "flex" : "none";
}

// CREAMOS LOS BOTONES DE EDITAR PERFIL, VER CARTAS Y CERRAR SESION
function crearOpcionesContainer() {
    const newContainer = document.createElement("div");
    newContainer.id = "opcionesContainer";
    newContainer.style.display = "flex"; 
    newContainer.style.flexDirection = "column";

    // EDITAR PERFIL
    const editarButton = document.createElement("button");
    editarButton.innerText = "Editar perfil";
    editarButton.addEventListener('click', mostrarEditarPerfil); //LLAMAMOS A LA FUNCION

    //MOSTRAR CARTAS
    const cartasButton = document.createElement("button");
    cartasButton.innerText = "Cartas";
    cartasButton.addEventListener('click', mostrarCartasPopup); //LLAMAMOS A LA FUNCION
    
    //CERRAR SESION
    const cerrarSesionButton = document.createElement("button");
    cerrarSesionButton.innerText = "Cerrar Sesión";
    cerrarSesionButton.addEventListener('click', cerrarSesion); //LLAMAMOS A LA FUNCION

    newContainer.appendChild(editarButton);
    newContainer.appendChild(cartasButton);
    newContainer.appendChild(cerrarSesionButton);
    newButton.appendChild(newContainer);

    return newContainer;
}

function cerrarSesion() {
    //PEDIMOS CONFIRMACIÓN AL USUARIO
    const confirmacion = confirm('¿Está seguro de que desea cerrar sesion?');
    if (confirmacion) {
        localStorage.setItem('isLoggedIn', 'false'); // INICIO DE SESION ES FALSE AHORA
        localStorage.removeItem('loggedUserEmail');

        // QUITAMOS LOS BOTONES DEL PERFIL DE USUARIO
        const perfilButton = document.getElementById('perfilButton');
        if (perfilButton) {
            perfilButton.remove();
        }
        const opcionesContainer = document.getElementById("opcionesContainer");
        if (opcionesContainer) {
            opcionesContainer.remove();
        }

        // ENSEÑAMOS BOTONES DE INICIO DE SESION Y REGISTRO
        inicioButton.style.display = 'block';
        registroButton.style.display = 'block';
        alert('Sesión cerrada');
    }
}

// MOSTRAMOS FORMULARIO DE EDITAR DATOS
function mostrarEditarPerfil() {
    editarPopup.style.display = 'flex';
    const loggedUserEmail = localStorage.getItem('loggedUserEmail');
    const usuarios = JSON.parse(localStorage.getItem('usuarios')) || {};
    const storedUserData = usuarios[loggedUserEmail];

    //CAMBIAMOS DATOS
    if (storedUserData){
        document.getElementById('editar-nombre').value = storedUserData.nombreUsuario; 
        document.getElementById('editar-ciudad').value = storedUserData.ciudad || ''; 
        document.getElementById('editar-pais').value = storedUserData.pais || ''; 
        document.getElementById('editar-genero').value = storedUserData.genero || 'Masculino';
        actualizarHijosFormulario(storedUserData.hijosDatos ); //ACTUALIZAMOS HIJOS EN UNA FUNCION APARTE
    }
}

cancelarEditar.addEventListener('click', (event) => {
    event.preventDefault();
    editarPopup.style.display = 'none';
    errorMensajeEditar.style.display = 'none';
});

// FORMULARIO PARA AÑADIR UN HIJO NUEVO
function nuevoHijo() {
    const hijosFormulario = document.getElementById('editar-hijos-formulario');
    const divHijo = document.createElement('div');

    //NOMBRE
    const nombreInput = document.createElement('input');
    nombreInput.type = 'text';
    nombreInput.placeholder = 'Nombre del hijo';
    nombreInput.required = true;
    divHijo.appendChild(nombreInput);

    //EDAD
    const edadInput = document.createElement('input');
    edadInput.type = 'number';
    edadInput.placeholder = 'Edad del hijo';
    edadInput.min = 0;
    edadInput.required = true;
    divHijo.appendChild(edadInput);

    //JUEGUETE FAVORITO
    const jugueteInput = document.createElement('input');
    jugueteInput.type = 'text';
    jugueteInput.placeholder = 'Juguete favorito del hijo';
    jugueteInput.required = true;
    divHijo.appendChild(jugueteInput);

    // SI QUEREMOS ELIMINAR EL HIJO: BOTON
    const eliminarHijoButton = document.createElement('button');
    eliminarHijoButton.innerText = 'Eliminar Hijo';
    eliminarHijoButton.type = 'button';
    eliminarHijoButton.onclick = () => {
        divHijo.remove(); // ELIMINA DIV DEL HIJO
        actualizarNumeroHijos(); // ACTUALIZA NUM HIJOS
    };
    divHijo.appendChild(eliminarHijoButton); //MOSTRAMOS BOTON DE ELIMINAR

    //AÑADIMOS LA ACTUALIZACION AL FORMULARIO: AÑADIMOS HIJO Y SU POSIBILIDAD DE BORRAR
    hijosFormulario.appendChild(divHijo);
    actualizarNumeroHijos(); //ACTUALIZAMOS NUMERO DE HIJOS
}

// FUNCION PARA ACTUALIZAR LOS DATOS DE LOS HIJOS DEL USUARIO
function actualizarHijosFormulario(hijos) {
    const hijosFormulario = document.getElementById('editar-hijos-formulario');
    hijosFormulario.innerHTML = ''; 

    //ITERAMOS POR CADA HIJO
    hijos.forEach((hijo) => {
        const divHijo = document.createElement('div');

        //NOMBRE
        const nombreInput = document.createElement('input');
        nombreInput.type = 'text';
        nombreInput.value = hijo.nombre;
        nombreInput.placeholder = 'Nombre del hijo';
        nombreInput.required = true;
        divHijo.appendChild(nombreInput);

        //EDAD
        const edadInput = document.createElement('input');
        edadInput.type = 'number';
        edadInput.value = hijo.edad;
        edadInput.placeholder = 'Edad del hijo';
        edadInput.min = 0;
        edadInput.required = true;
        divHijo.appendChild(edadInput);

        //JUGUETE
        const jugueteInput = document.createElement('input');
        jugueteInput.type = 'text';
        jugueteInput.value = hijo.jugueteFavorito;
        jugueteInput.placeholder = 'Juguete favorito del hijo';
        jugueteInput.required = true;
        divHijo.appendChild(jugueteInput);

        //BOTON PARA ELIMINAR EL HIJO
        const eliminarHijoButton = document.createElement('button');
        eliminarHijoButton.innerText = 'Eliminar Hijo';
        eliminarHijoButton.type = 'button'; 
        eliminarHijoButton.onclick = () => {
            divHijo.remove(); 
            actualizarNumeroHijos(); 
        };
        divHijo.appendChild(eliminarHijoButton); 

        //AÑADIMOS AL FORMULARIO EL HIJO
        hijosFormulario.appendChild(divHijo);
    });
    actualizarNumeroHijos(); //ACTUALIZAMOS NUM TOTAL DE HIJOS DEL USUARIO
}

//ACTUALIZAR NUM DE HIJOS
function actualizarNumeroHijos() {
    const hijosFormulario = document.getElementById('editar-hijos-formulario');
    const totalHijos = hijosFormulario.children.length; // NUMERO TOTAL
    const numeroHijosDisplay = document.getElementById('numero-hijos'); 
    numeroHijosDisplay.innerText = `Total de hijos: ${totalHijos}`; //MOSTRAMOS EN EL POPUP EL NUMERO

    //SACAMOS DATOS DEL USUARIO
    const loggedUserEmail = localStorage.getItem('loggedUserEmail');
    let usuarios = JSON.parse(localStorage.getItem('usuarios')) || {};

    // BUSACMOS USUARIO LOGGEADO
    let usuarioActual = null;
    for (let key in usuarios) {
        if (usuarios[key].correo === loggedUserEmail) {
            usuarioActual = key;
        }
    }
    if (usuarioActual) {
        usuarios[usuarioActual].hijos = totalHijos; //ACTUALIZA NUM DE HIJOS
        localStorage.setItem('usuarios', JSON.stringify(usuarios)); // GUARDAMOS
    }
}

// FORMULARIO PARA EDITAR LOS DATOS DEL USUARIO
editarFormulario.addEventListener('submit', (event) => {
    event.preventDefault();

    const nuevoNombre = document.getElementById('editar-nombre').value;
    const nuevaCiudad = document.getElementById('editar-ciudad').value;
    const nuevoPais = document.getElementById('editar-pais').value;
    const nuevoGenero = document.getElementById('editar-genero').value;

    // CREAMOS UN ARRAY DONDE GUARDAMOS LOS DATOS DEL HIJO
    const nuevosHijos = [];
    document.querySelectorAll('#editar-hijos-formulario div').forEach(div => {
        const nombre = div.children[0].value;
        const edad = div.children[1].value;
        const jugueteFavorito = div.children[2].value;
        
        nuevosHijos.push({ nombre, edad, jugueteFavorito }); //GUARDAMOS
    }); 

    const loggedUserEmail = localStorage.getItem('loggedUserEmail');
    let usuarios = JSON.parse(localStorage.getItem('usuarios')) || {};

    //BUSCAMOS USUARIO LOGGEADO
    let usuarioActual;
    for (let key of Object.keys(usuarios)) {
        if (usuarios[key].correo === loggedUserEmail) {
            usuarioActual = key;
            //break; // Salir del bucle una vez encontrado
        }
    }

    //GUARDAMOS DATOS NUEVOS
    if (usuarioActual) {
        usuarios[usuarioActual].nombre = nuevoNombre;
        usuarios[usuarioActual].ciudad = nuevaCiudad;
        usuarios[usuarioActual].pais = nuevoPais;
        usuarios[usuarioActual].genero = nuevoGenero;
        usuarios[usuarioActual].hijosDatos = nuevosHijos;
        usuarios[usuarioActual].hijos = nuevosHijos.length;

        localStorage.setItem('usuarios', JSON.stringify(usuarios));

        alert('Perfil actualizado correctamente');
        editarPopup.style.display = 'none';
    } else {
        errorMensajeEditar.textContent = "Error al actualizar el perfil.";
        errorMensajeEditar.style.display = 'block';
    }
});

// MOSTRAMOS LAS CARTAS ENVIADAS DEL USUARIO EN UN POPUP
function mostrarCartasPopup() {
    cartasContainer.innerHTML = '';
    const loggedUserEmail = localStorage.getItem('loggedUserEmail');
    const cartasEnviadas = JSON.parse(localStorage.getItem('cartasEnviadas')) || [];
    const cartasUsuario = [];
    for (const carta of cartasEnviadas) {
        if (carta.correo === loggedUserEmail) {
            cartasUsuario.push(carta);
        }
    }

    if (cartasUsuario.length > 0) {
        //LEEMOS CADA CARTA
        cartasUsuario.forEach((carta, index) => {
            const cartaElement = document.createElement('div');
            cartaElement.className = 'carta';
            cartaElement.draggable = true; //SE PUEDE MOVER
            cartaElement.dataset.index = index;
            
            // FORMATO CARTAS
            cartaElement.innerHTML = `
                <article class="carta">
                    <div class="carta-header">Carta</div>
                    <div class="carta-cuerpo">
                        <img src="imagenes/reno.webp" alt="Foto de quien envía la carta">
                        <div class="contenido">
                            <h3>${carta.nombre}</h3>
                            <p>Ciudad: ${carta.ciudad}, País: ${carta.pais}</p>
                            <p>${carta.carta}</p>
                        </div>
                    </div>
                    <button class="borrar-carta">Borrar Carta</button>
                </article>
            `;

            //BOTON PARWA BORRAR LA CARTA SELECCIONADA
            cartaElement.querySelector('.borrar-carta').addEventListener('click', () => {
                //PEDIMOS CONFIRMACION DEL USUARIO
                const confirmacion = confirm('¿Estás seguro de que deseas borrar esta carta?');
                if (confirmacion) {
                    cartasUsuario.splice(index, 1);
                    localStorage.setItem('cartasEnviadas', JSON.stringify(cartasUsuario));
                    mostrarCartasPopup(); //RECARGAR POPUP
                }
            });

            //DRAG AND DROP
            cartaElement.addEventListener('dragstart',  (event) => dragStart(event, index));
            cartaElement.addEventListener('dragover', dragOver);
            cartaElement.addEventListener('drop', (event) => drop(event, index));
            
            cartasContainer.appendChild(cartaElement);
        });
    } else {
        //NO HAY CARTAS, MOSTRAMOS ERROR
        const noCartasElement = document.createElement('p');
        noCartasElement.textContent = 'No has enviado cartas aún.';
        cartasContainer.appendChild(noCartasElement);
    }
    cartasPopup.style.display = 'flex'; //MOSTRAR POPUP
}

function dragStart(event, index) {
    event.dataTransfer.setData("text/plain", index); // GUARDA INDICE INICIAL
}

function dragOver(event) {
    event.preventDefault(); // PERMITE ARRASTRAR LAS CARTAS
}

function drop(event, dropIndex) {
    const dragStartIndex = parseInt(event.dataTransfer.getData("text/plain"));
    swapCartas(dragStartIndex, dropIndex); //LLAMA FUNCION
    mostrarCartasPopup(); // ACTUALIZA POPUP
}

function swapCartas(fromIndex, toIndex) {
    const cartas = JSON.parse(localStorage.getItem('cartasEnviadas'));
    const loggedUserEmail = localStorage.getItem('loggedUserEmail');
    const cartasUsuario = [];
    for (const carta of cartas) {
        if (carta.correo === loggedUserEmail) {
            cartasUsuario.push(carta);
        }
    }

    // CAMBIA POS DE LAS CARTAS
    [cartasUsuario[fromIndex], cartasUsuario[toIndex]] = [cartasUsuario[toIndex], cartasUsuario[fromIndex]];

    // GUARDA CAMBIOS EN LOCALSTORAGE PARA EL USUARIO ACTUAL
    cartas.forEach((carta, index) => {
        if (carta.correo === loggedUserEmail) {
            cartas[index] = cartasUsuario.shift();
        }
    });

    localStorage.setItem('cartasEnviadas', JSON.stringify(cartas));
}

//CERRAMOS POPUP DE LAS CARTAS ENVIADAS
document.getElementById('cerrar-cartas-popup').addEventListener('click', () => {
    cartasPopup.style.display = 'none';
});

//CUANDO ACTUALIZAMOS LA PAG WEB, SEGUIMOS INICIADOS SESION
window.addEventListener('load', () => {
    if (localStorage.getItem('isLoggedIn') === 'true') {
        crearButtonPerfil();
        inicioButton.style.display = 'none';
        registroButton.style.display = 'none';
    }
});
