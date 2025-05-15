const fs = require('fs').promises;

// Clase que contiene los métodos que se encargan de la gestión de los usuarios por parte del servidor
class GestionUsuarios {
    constructor(archivoUsuarios) {
        // Constructor de la clase GestionUsuarios
        this.archivoUsuarios = archivoUsuarios || './backend/ficheros/usuarios.json';
    }

    // Método para añadir un nuevo usuario
    async registroUsuario(usuario) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoUsuarios, 'utf-8');
            const data = JSON.parse(contenido);
            const users = data.usuarios || [];

            // Comprobamos si el usuario ya existe
            if (users.find(u => u.email === usuario.email)){
                console.log('El usuario ya existe.');
                return 400;
            }

            // Guardamos el nuevo usuario en el json
            users.push(usuario);

            await fs.writeFile(this.archivoUsuarios, JSON.stringify({ usuarios: users }, null, 2));
            console.log('Usuario añadido correctamente.');

            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al añadir el usuario:', error);
            return 400;
        }
    }

    // Método para validar las credenciales de un usuario
    async inicioSesion(email, password) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoUsuarios, 'utf-8');
            const data = JSON.parse(contenido);
            const users = data.usuarios || [];

            console.log(email, password);

            // Buscamos el usuario por email y contraseña
            const usuario = users.find(u => u.email === email && u.contraseña === password);
            
            console.log(usuario);

            if (usuario) {
                return [usuario.email, usuario.nombre];
            } else {
                console.log('Usuario y/o contraseña incorrectas.');
                return 401; // Código de estado 401 (No autorizado)
            }

        } catch (error) {
            console.error('Error al validar las credenciales:', error);
            return 400;
        }
    }

    // Método para eliminar a un usuario
    async borrarUsuario(email) {
        try {
            // Leemos el archivo json
            const contenido = await fs.readFile(this.archivoUsuarios, 'utf-8');
            const data = JSON.parse(contenido);
            const users = data.usuarios || [];

            // Filtramos los usuarios para eliminar el deseado según su email
            const usuariosFiltrados = users.filter(u => u.email !== email);

            // Guardamos el nuevo array de usuarios en el json
            await fs.writeFile(this.archivoUsuarios, JSON.stringify({ usuarios: usuariosFiltrados }, null, 2));
            console.log('Usuario borrado correctamente.');

            return 200; // Código de estado 200 (OK)

        } catch (error) {
            console.error('Error al borrar el usuario:', error);
            return 400;
        }
    }
}

module.exports = GestionUsuarios;
