#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include "lines.c"

//PARA PARTE 3
#include <rpc/rpc.h>
#include "server_rpc/server_rpc.h"  


#define MAX_USERNAME 256
#define MAX_USERS 100
#define MAX_FILE_PATH 1024


// aqui van los datos de los archivos publicados por los clientes
typedef struct {
    char filename[MAX_USERNAME]; 	// Usamos MAX_USERNAME para el path (máx 256 bytes)
    char description[MAX_USERNAME]; // Usamos MAX_USERNAME para la descripción (máx 256 bytes)
} archivos;

// esta estructura es para 'guardar' los datos de los clientes que estan registrados en el servidor
typedef struct {
    char username[MAX_USERNAME];
    int conectado;
    int registrado;
    int num_archivos;
    int puerto_escucha; // Nuevo campo para el puerto de escucha del cliente
	char ip_address[MAX_USERNAME]; //
    archivos publicaciones[100]; // Límite de 100 publicaciones por usuario
    int num_publicaciones;
	char timestamp[MAX_STRING];
} InfoUsuario;

// creamos lista con los usuarios registrados
InfoUsuario users[MAX_USERS];
int num_users = 0; // para no sobrepasar el limite

pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;

// variable global para el sd
int g_server_sd;

void sigint_handler(int sig) {
	//printf("\ns> Server shutting down...\n");
	close(g_server_sd);
	// Perform any other cleanup (e.g., mutex destroy)
	exit(0);
}


// ########################### CODIGO PARTE 3 ##################################
/*void connect_to_rpc_server() {
    // Obtener la dirección IP del servidor RPC desde la variable de entorno
    char *rpc_ip = getenv("LOG_RPC_IP");
    
    if (rpc_ip == NULL) {
        // Si no está definida la variable de entorno, mostrar error
        fprintf(stderr, "Error: La variable de entorno LOG_RPC_IP no está definida.\n");
        return;
    }

    // Imprimir la dirección IP obtenida para verificar
    //printf("Conectando al servidor RPC en la IP: %s\n", rpc_ip);
}*/

void handle_operation(char *username, char *operation, char *timestamp) {
    // Conectar al servidor RPC
    //connect_to_rpc_server();
    
    // Aquí se conecta al servidor RPC. Asumimos que 'rpc_ip' es la dirección IP de un servidor RPC
    //CLIENT *clnt = clnt_create(rpc_ip, SERVER_RPC_PROG, SERVER_RPC_VERS, "udp");
	//printf("--DEBUG: Conectando al servidor RCP...");
	
	//CLIENT *clnt = clnt_create("localhost", 99, 1, "tcp");
    char *rpc_ip = getenv("LOG_RPC_IP");
    if (!rpc_ip) {
        fprintf(stderr, "Error: La variable de entorno LOG_RPC_IP no está definida.\n");
        return;
    }

    //printf("Conectando al servidor RPC en la IP: %s\n", rpc_ip);

    CLIENT *clnt = clnt_create(rpc_ip, SERVER_RPC_PROG, SERVER_RPC_VERS, "tcp");
    
	if (clnt == NULL) {
        clnt_pcreateerror("Error al crear el cliente RPC");
        return;
    }
	//printf("--DEBUG: Conexión extablecida");
    // Crear un objeto 'data' que contiene la información que queremos enviar
    data operation_data;

	operation_data.username = malloc(MAX_STRING);
	operation_data.operation = malloc(MAX_STRING);
	operation_data.timestamp = malloc(MAX_STRING);

    strncpy(operation_data.username, username, MAX_STRING);
    strncpy(operation_data.operation, operation, MAX_STRING);
    strncpy(operation_data.timestamp, timestamp, MAX_STRING);

	//printf("--DEBUG:Antes de llamar a RPC");
    // Realizar la llamada RPC
    void *result = rpc_operation_1(&operation_data, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "Error al realizar la llamada RPC");
    }
	//printf("--DEBUG: Después de llamar RPC");
    // Cerrar la conexión con el servidor RPC
    clnt_destroy(clnt);
	free(operation_data.username);
	free(operation_data.operation);
	free(operation_data.timestamp);

}
// ###############################################################################




// buscamos el usuario que queremos encontrar y devolvemos si existe o no
InfoUsuario* find_user(char* username) {
    //printf("SERVER: FIND USER - estamos en find_user, buscamos username %s\n", username);
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            //printf("SERVER: FIND USER -  Existe el usuario %s\n", users[i].username);
            return &users[i];
        }
    }
    //printf("SERVER: FIND USER - No existe ese usuario\n");
    return NULL;
}

// REGISTER USERNAME
void tratar_registro(int client_sock, char* username){
    //printf("SERVER: REGISTRO USER - Estamos en tratar_registro (socket: %d, usuario: %s)\n", client_sock, username);

    int result_code; // resultado

    //printf("SERVER: REGISTRO USER - Número de usuarios registrados antes de procesar: %d (socket: %d)\n", num_users, client_sock);

    InfoUsuario* existing_user = find_user(username);
    if (existing_user) {
        result_code = 1; // USERNAME IN USE
    } else if (num_users < MAX_USERS) {
        // añadimos el nuevo usuario registrado a la lista de usuarios
        //printf("SERVER: REGISTRO USER - Registrando nuevo usuario: %s (socket: %d)\n", username, client_sock);
        strncpy(users[num_users].username, username, MAX_USERNAME - 1);
        users[num_users].username[MAX_USERNAME - 1] = '\0';
        users[num_users].conectado = 0;
        users[num_users].registrado = 1;
        users[num_users].num_archivos = 0;
        num_users++;
        result_code = 0; // OK
    } else {
        //printf("SERVER: REGISTRO USER - Número máximo de usuarios alcanzado (socket: %d).\n", client_sock);
        result_code = 2; // REGISTER FAIL
    }
    //printf("SERVER: REGISTRO USER - Número de usuarios registrados despues de procesar: %d (socket: %d)\n", num_users, client_sock);
}

// UNREGISTER USERNAME
void eliminar_usuario(char* username) {
  	//printf("SERVER: ELIMINAR USER - Estamos eliminar_usuario (usuario: %s)\n", username);
  	//printf("SERVER: ELIMINAR USER - Número de usuarios registrados antes de procesar: %d\n", num_users);

    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            // Mover el último usuario a esta posición
            users[i] = users[num_users - 1];
            num_users--;
            break;
        }
    }
	//printf("SERVER: ELIMINAR USER - Número de usuarios registrados despues de procesar: %d\n", num_users);
}

void tratar_publish(int client_sock, char* username) {
    //printf("SERVER: PUBLISH - Estamos en tratar_publish (socket: %d, usuario: %s)\n", client_sock, username);

    int result_code = 4; // Error genérico
    char filename[MAX_USERNAME];
    char description[MAX_USERNAME];
    ssize_t bytes_filename;
    ssize_t bytes_description;

    // comprobamos si el ususario existe
    InfoUsuario* user = find_user(username);
    if (user == NULL) {
        result_code = 1; // USER DOES NOT EXIST
        //printf("SERVER: PUBLISH - Error: Usuario '%s' no encontrado.\n", username);
    } else if (!user->conectado) {
        result_code = 2; // USER NOT CONNECTED
        //printf("SERVER: PUBLISH - Error: Usuario '%s' no está conectado.\n", username);
    } else {
        // leemos nombre del fichero
        //printf("SERVER: PUBLISH - Esperando nombre de fichero...\n");
        bytes_filename = readLine(client_sock, filename, sizeof(filename));
        //printf("SERVER: PUBLISH - Bytes recibidos para filename: %zd, filename: '%s'\n", bytes_filename, filename);

        if (bytes_filename > 0) {
            //printf("SERVER: PUBLISH - Nombre de fichero recibido: '%s'\n", filename);

            // leemos descripción
            //printf("SERVER: PUBLISH - Esperando descripción del fichero '%s'...\n", filename);
            bytes_description = readLine(client_sock, description, sizeof(description));
            //printf("SERVER: PUBLISH - Bytes recibidos para description: %zd, description: '%s'\n", bytes_description, description);

            if (bytes_description > 0) {
                //printf("SERVER: PUBLISH - Descripción recibida: '%s'\n", description);

                // comprobamos si ya ha sido publicado
                int already_published = 0;
                for (int i = 0; i < user->num_publicaciones; i++) {
                    if (strcmp(user->publicaciones[i].filename, filename) == 0) {
                        already_published = 1;
                        break;
                    }
                }

                if (already_published) {
                    result_code = 3; // CONTENT ALREADY PUBLISHED
                    //printf("SERVER: PUBLISH - Error: El fichero '%s' ya fue publicado por %s.\n", filename, username);
                } else if (user->num_publicaciones < 100) {
                    // guardar
                    strncpy(user->publicaciones[user->num_publicaciones].filename, filename, sizeof(filename) - 1);
                    user->publicaciones[user->num_publicaciones].filename[sizeof(filename) - 1] = '\0';
                    strncpy(user->publicaciones[user->num_publicaciones].description, description, sizeof(description) - 1);
                    user->publicaciones[user->num_publicaciones].description[sizeof(description) - 1] = '\0';
                    user->num_publicaciones++;

                    // creamos fichero físico en /tmp
                    char ruta_fichero[512];
                    snprintf(ruta_fichero, sizeof(ruta_fichero), "/tmp/%s_%s", username, filename);

                    FILE* f = fopen(ruta_fichero, "w");
                    if (f == NULL) {
                        //perror("SERVER: PUBLISH - Error al crear el fichero en /tmp");
                        result_code = 4;
                    } else {
                        fprintf(f, "Descripción: %s\n", description);
                        fclose(f);
                        //printf("SERVER: PUBLISH - Fichero guardado en %s\n", ruta_fichero);
                        result_code = 0; // OK
                        //printf("SERVER: PUBLISH - Usuario %s publicó el fichero: %s con descripción: %s\n", username, filename, description);
                    }
                } else {
                    result_code = 4; // límite de publicaciones
                    //printf("SERVER: PUBLISH - Error: Usuario %s alcanzó el límite de publicaciones.\n", username);
                }
            } else {
                perror("SERVER: PUBLISH - Error al recibir la descripción del fichero");
            }
        } else {
            perror("SERVER: PUBLISH - Error al recibir el nombre del fichero");
        }
    }

    //printf("SERVER: PUBLISH - Enviamos respuesta de PUBLISH\n");
    send(client_sock, &result_code, 1, 0);
    //printf("SERVER: PUBLISH - Resultado enviado: %d\n", result_code);
}

int eliminar_archivo(char* username, char* filename) {
	//printf("SERVER: ELIMINAR_ARCHIVO - Estamos en funcion eliminar_archivo (usuario: %s, fichero: %s)\n", username, filename);

	InfoUsuario* user = find_user(username);
	if (user == NULL) {
		//printf("SERVER: ELIMINAR_ARCHIVO - Error: Usuario '%s' no existe.\n", username);
		return 1; // USER DOES NOT EXIST
	}
	if (!user->conectado) {
		//printf("SERVER: ELIMINAR_ARCHIVO - Error: Usuario '%s' no está conectado.\n", username);
		return 2; // USER NOT CONNECTED
	}

    // buscamos en cada publicacion del usuario
	for (int i = 0; i < user->num_publicaciones; i++) {
		if (strcmp(user->publicaciones[i].filename, filename) == 0) {
			char ruta_fichero[512];
			snprintf(ruta_fichero, sizeof(ruta_fichero), "/tmp/%s_%s", username, filename);

			// intentamos eliminar el fichero
			if (remove(ruta_fichero) != 0) {
				perror("SERVER: ELIMINAR_ARCHIVO - Error al eliminar el fichero físico");
            }

			// eliminamso publicación de la estructura de datos
			user->publicaciones[i] = user->publicaciones[user->num_publicaciones - 1];
			user->num_publicaciones--;
			//printf("SERVER: ELIMINAR_ARCHIVO - Fichero '%s' borrado del usuario '%s'.\n", filename, username);
			return 0; // OK
		}
	}

	//printf("SERVER: ELIMINAR_ARCHIVO - Error: El fichero '%s' no fue publicado por el usuario '%s'.\n", filename, username);
	return 3; // CONTENT NOT PUBLISHED
}

// LIST_USERS
void enviar_lista_usuarios_conectados(int client_socket) {
	unsigned char result_code = 0;
	send(client_socket, &result_code, 1, 0);
	//printf("SERVER: ENVIAR_LISTA_USUARIOS - Enviado código OK.\n");

    // guardamos en la variable el numero de usuarios conectados
	int connected_users_count = 0;
	for (int i = 0; i < num_users; i++) {
		if (users[i].conectado) {
			connected_users_count++;
		}
	}
	//printf("SERVER: ENVIAR_LISTA_USUARIOS - usuarios conectados: %d\n", connected_users_count);

	// enviamos la cantidad de usuarios conectados SEPARADAMENTE
	char num_users_str[32];
	snprintf(num_users_str, sizeof(num_users_str), "%d\n", connected_users_count);
	send(client_socket, num_users_str, strlen(num_users_str), 0);
	//printf("SERVER: ENVIAR_LISTA_USUARIOS - Enviada cantidad de usuarios conectados: %d\n", connected_users_count);

	// enviamos la información de cada usuario conectado
	for (int i = 0; i < num_users; i++) {
		if (users[i].conectado) {
			//printf("SERVER: ENVIAR_LISTA_USUARIOS - usuario conectado: %s\n", users[i].username);

			// nombre de usuario
			send(client_socket, users[i].username, strlen(users[i].username), 0);
			send(client_socket, "\n", 1, 0);
			// IP
			send(client_socket, users[i].ip_address, strlen(users[i].ip_address), 0);
			send(client_socket, "\n", 1, 0);
			// puerto
			char port_str[16];
			snprintf(port_str, sizeof(port_str), "%d", users[i].puerto_escucha);
			send(client_socket, port_str, strlen(port_str), 0);
			send(client_socket, "\n", 1, 0);
			//printf("SERVER: ENVIAR_LISTA_USUARIOS - Enviada información de usuario: %s (IP: %s, puerto: %d)\n", users[i].username, users[i].ip_address, users[i].puerto_escucha);
		}
	}
	//printf("SERVER: ENVIAR_LISTA_USUARIOS - Finalizado el envío de la lista de usuarios.\n");
}

// enviamos las publicaciones del usuario
void enviar_lista_contenido(int client_socket, InfoUsuario* user) {
	// convertir el número de publicaciones a una cadena y enviarla al cliente
	char count_str[32];
	snprintf(count_str, sizeof(count_str), "%d\n", user->num_publicaciones);
	send(client_socket, count_str, strlen(count_str), 0);
	//printf("SERVER: ENVIAR_LISTA_CONTENIDO - Enviada cantidad de publicaciones: %d para usuario %s\n", user->num_publicaciones, user->username);

	// iterar a través de la lista de publicaciones del usuario y enviar cada nombre de archivo
	for (int i = 0; i < user->num_publicaciones; i++) {
		send(client_socket, user->publicaciones[i].filename, strlen(user->publicaciones[i].filename), 0);
		send(client_socket, "\n", 1, 0); // Añadir salto de línea después de cada nombre de archivo
		//printf("SERVER: ENVIAR_LISTA_CONTENIDO - Enviado archivo: %s\n", user->publicaciones[i].filename);
	}
}

//
void tratar_list_users(int client_sock, char* username) {
    //printf("SERVER: TRATAR_LIST_USERS - Procesando LIST_USERS para usuario '%s' en socket %d\n", username, client_sock);
    InfoUsuario* requesting_user = find_user(username);
    unsigned char result_code_byte;

    if (!requesting_user) {
        //printf("SERVER: TRATAR_LIST_USERS - Usuario '%s' no existe.\n", username);
        result_code_byte = 1;
        send(client_sock, &result_code_byte, 1, 0);
    } else if (!requesting_user->conectado) {
        //printf("SERVER: TRATAR_LIST_USERS - Usuario '%s' no está conectado.\n", username);
        result_code_byte = 2;
        send(client_sock, &result_code_byte, 1, 0);
    } else {
        //printf("SERVER: TRATAR_LIST_USERS - Usuario '%s' existe y está conectado. Llamando a enviar_lista_usuarios_conectados.\n", username);
        enviar_lista_usuarios_conectados(client_sock); // llamamos a la funcion que se encarga del envio
        return;
    }
}

// FUNCION QUE SE ENCARGA DE SABER QUE COMANDO REALIZAMOS
void* tratar_cliente(void* arg){
  	printf("\n");
    //printf("SERVER: TRATAR CLIENTE - Estamos en tratar_cliente\n");
    int client_socket = *((int*)arg); // valor del socket del cliente
    free(arg);

    char cadena[MAX_USERNAME * 2];
    char comando[MAX_USERNAME];
    char usuario[MAX_USERNAME];
    char puerto_str[16];
    int puerto_cliente;
	char timestamp[MAX_STRING]; //parte 3
    int result_code; // guardamos resultado
    ssize_t bytes_recibidos;

    while(1){
        bytes_recibidos = readLine(client_socket, cadena, sizeof(cadena));

        if (bytes_recibidos > 0) {
        	//printf("SERVER: TRATAR CLIENTE - Mensaje recibido del cliente %d: %s\n", client_socket, cadena);
        	usuario[0] = '\0'; // eliminar posible basura
			timestamp[0] = '\0'; // limpiar el campo timestamp - parte 3

        	//if (sscanf(cadena, "%s %s", comando, usuario) >= 1) {
			if (sscanf(cadena, "%s %s %[^\n]", comando, usuario, timestamp) >= 1) { //supuestamente con lo de %[^\n] leemos hasta el final de la linea, cogiendo timestamp entero
                //printf("SERVER: TRATAR CLIENTE - Comando recibido: '%s'", comando);
                printf("s> OPERACION %s", comando);
                if (usuario[0] != '\0') {
                    //printf(", Usuario: '%s' (Socket: %d)\n", usuario, client_socket);
                    //printf(" FROM %s\n", usuario);
					//printf("-- DEBUG: Timestamp: %s\n", timestamp);
                }

				// #### PARTE 3 - AQUÍ MANDAMOS LA OPERACION USUARIO Y HORA AL SERVER RPC ####
				handle_operation(usuario, comando, timestamp);

				//NOTA: Teniendo en cuenta que timestamp se coge como lo que habia despeus de usuario ahsta el final, 
				//en el caso de publish y delete también se incluye el path del archivo, por lo que no harían
				//falta modificaciones adicionales, salen los datos en el orden correcto

				
				// ###########################################################################

        		fflush(stdout);
				//printf("-- DEBUG: DEspues de fflush");

                // empezamos con las posibles funciones
            	if (strcmp(comando, "REGISTER") == 0) {
                	//printf("SERVER: TRATAR CLIENTE - Recibido comando REGISTER del username: %s\n", usuario);
					//printf("--DEBUG: Estamos dentro de REGISTER");
                	pthread_mutex_lock(&users_mutex);

                    InfoUsuario* user = find_user(usuario);
                	if (user == NULL) {
                		// añadimos el nuevo usuario
                    	tratar_registro(client_socket, usuario);
                    	result_code = 0; // OK
                	} else {
                    	result_code = 1; // USERNAME IN USE
                	}
                	pthread_mutex_unlock(&users_mutex);

                    //printf("SERVER: TRATAR CLIENTE - enviamos respuesta de registrar: %d\n", result_code);
            	    send(client_socket, &result_code, 1, 0);
            	}
            	else if (strcmp(comando, "UNREGISTER") == 0) {
              		//printf("SERVER: TRATAR CLIENTE - Recibido comando UNREGISTER del username: %s\n", usuario);
                	pthread_mutex_lock(&users_mutex);

                	InfoUsuario* user = find_user(usuario);
              		if (user != NULL) {
                		eliminar_usuario(usuario);
                  		result_code = 0; // OK
              		}
                	else {
                		result_code = 1; // USER DOES NOT EXIST
             		}
             		pthread_mutex_unlock(&users_mutex);

            		//printf("SERVER: TRATAR CLIENTE - enviamos respuesta de unregister: %d\n", result_code);
                	send(client_socket, &result_code, 1, 0);
            	}
				else if (strcmp(comando, "CONNECT") == 0) {
                    //printf("SERVER: TRATAR CLIENTE - Recibido comando CONNECT del usuario %s\n", usuario);

                    pthread_mutex_lock(&users_mutex);
                    InfoUsuario* user = find_user(usuario); // buscamos si existe
                    if (user == NULL) {
                       result_code = 1; // USER DOES NOT EXIST
                    } else if (user->conectado) {
                        result_code = 2; // USER ALREADY CONNECTED
                    } else {
                    	//printf("SERVER: TRATAR CLIENTE - Esperando IP de escucha de %s...\n", usuario);
                    	char ip_str[MAX_USERNAME];
                    	ssize_t bytes_ip = readLine(client_socket, ip_str, sizeof(ip_str)); // leemos la ip
                    	if (bytes_ip > 0) {
                    		strncpy(user->ip_address, ip_str, MAX_USERNAME - 1);
                    		user->ip_address[MAX_USERNAME - 1] = '\0';
                    		//printf("SERVER: TRATAR CLIENTE - IP recibida: %s\n", user->ip_address);

                    		//printf("SERVER: TRATAR CLIENTE - Esperando puerto de escucha de %s...\n", usuario);
                    		char puerto_str[16];
                    		ssize_t bytes_puerto = readLine(client_socket, puerto_str, sizeof(puerto_str)); // leemos puerto
                    		if (bytes_puerto > 0) {
                    			user->puerto_escucha = atoi(puerto_str);
                    			user->conectado = 1;
                    			result_code = 0; // OK
                    			//printf("SERVER: TRATAR CLIENTE - Usuario %s conectado desde IP: %s, Puerto: %d\n", usuario, user->ip_address, user->puerto_escucha);
                    		} else {
                    			result_code = 4; // Error al recibir el puerto
                    			//perror("SERVER: TRATAR CLIENTE - Error al leer el puerto del cliente");
                    		}
                    	} else {
                    		result_code = 3; // Error al recibir la IP
                    		//perror("SERVER: TRATAR CLIENTE - Error al leer la IP del cliente");
                    	}
                    }
                    pthread_mutex_unlock(&users_mutex);

					//printf("SERVER: TRATAR CLIENTE - enviamos respuesta de connect: %d\n", result_code);
                    send(client_socket, &result_code, 1, 0);
                }
                else if (strcmp(comando, "DISCONNECT") == 0) {
                    //printf("SERVER: TRATAR CLIENTE - Recibido comando DISCONNECT del usuario %s\n", usuario);

                    pthread_mutex_lock(&users_mutex);
                    InfoUsuario* user = find_user(usuario);
                    if (user == NULL) {
                        result_code = 1; // USER DOES NOT EXIST
                        //printf("SERVER: TRATAR CLIENTE - Usuario no existe\n");
                    } else if (!user->conectado) {
                        result_code = 2; // USER NOT CONNECTED
                        //printf("SERVER: TRATAR CLIENTE  - Usuario no conectado\n");
                    }
                    else {
                        user->conectado = 0;
                        result_code = 0; // OK
                        //printf("SERVER: TRATAR CLIENTE - Usuario %s desconectado.\n", usuario);
                    }
                    pthread_mutex_unlock(&users_mutex);

                	//printf("SERVER: TRATAR CLIENTE - enviamos respuesta de disconnect: %d\n", result_code);
                    send(client_socket, &result_code, 1, 0);
                }
				else if (strcmp(comando, "PUBLISH") == 0) {
					//printf("SERVER: TRATAR CLIENTE - Recibido comando PUBLISH del cliente %d\n", client_socket);

					char publish_user[MAX_USERNAME]; // nombre del usuario que publica
					ssize_t bytes_user = readLine(client_socket, publish_user, sizeof(publish_user));
					//printf("SERVER: TRATAR CLIENTE - [DEBUG PUBLISH] Username leído: '%s'\n", publish_user);
                    printf(" FROM %s\n", publish_user);

					if (bytes_user > 0) {
						//printf("SERVER: TRATAR CLIENTE - PUBLISH solicitado por usuario: %s\n", publish_user);

						pthread_mutex_lock(&users_mutex);
						//printf("SERVER: TRATAR CLIENTE - llamamos a tratar_publish (pasando socket y username)\n");
						tratar_publish(client_socket, publish_user); // pasamos el socket y el username
						pthread_mutex_unlock(&users_mutex);
					} else {
						//printf("SERVER: TRATAR CLIENTE - Error al leer el username para PUBLISH.\n");
						result_code = 4;
					}

                	//printf("SERVER: TRATAR CLIENTE - enviamos respuesta de publish: %d\n", result_code);
					send(client_socket, &result_code, 1, 0);
                }
                else if (strcmp(comando, "DELETE") == 0) {
                	//printf("SERVER: TRATAR CLIENTE - Recibido comando DELETE del cliente %d\n", client_socket);

                	char publish_user[MAX_USERNAME];
                	ssize_t bytes_user = readLine(client_socket, publish_user, sizeof(publish_user));
                	printf(" FROM %s\n", publish_user);
                	//printf("SERVER: TRATAR CLIENTE - [DEBUG PUBLISH] Username leído: '%s'\n", publish_user);
                	if (bytes_user > 0) {
                		//printf("SERVER: TRATAR CLIENTE - DELETE solicitado por usuario: %s\n", publish_user);
                		char filename[MAX_USERNAME];
                		ssize_t bytes_filename = readLine(client_socket, filename, sizeof(filename)); // leemos el archivo
                		//printf("SERVER: TRATAR CLIENTE - [DEBUG DELETE] Filename leído: '%s'\n", filename);

                        if(bytes_filename > 0) {
                        	//printf("SERVER: TRATAR CLIENTE - Intentando borrar el fichero: '%s' del usuario: %s\n", filename, publish_user);
                			pthread_mutex_lock(&users_mutex);
                			//printf("SERVER: TRATAR CLIENTE - llamamos a eliminar_archivo\n");
                        	int delete_result = eliminar_archivo(publish_user, filename); // eliminamos
                        	//printf("SERVER: TRATAR CLIENTE - enviamos respuesta de delete: %d\n", result_code);
                        	send(client_socket, &delete_result, 1, 0);
                			pthread_mutex_unlock(&users_mutex);
						}
                        else {
                        	//printf("SERVER: TRATAR CLIENTE - Error al leer el nombre del fichero a borrar.\n");
                        	int error_code = 4; // Otro error
                        	//printf("SERVER: TRATAR CLIENTE - enviamos respuesta de delete: %d\n", result_code);
                        	send(client_socket, &error_code, 1, 0);
                        }
                	} else {
                		//printf("SERVER: TRATAR CLIENTE - Error al leer el username para PUBLISH.\n");
                		int error_code = 1; // USER DOES NOT EXIST
                		//printf("SERVER: TRATAR CLIENTE - enviamos respuesta de delete: %d\n", result_code);
                		send(client_socket, &error_code, 1, 0);
                	}
                }
                else if (strcmp(comando, "LIST_USERS") == 0) {
                	//printf("SERVER: TRATAR CLIENTE - Recibido comando LIST_USERS del cliente %d\n", client_socket);
                	char list_user[MAX_USERNAME];
                	ssize_t bytes_user_list = readLine(client_socket, list_user, sizeof(list_user));
					printf(" FROM %s\n", list_user);

                	if (bytes_user_list > 0) {
                		//printf("SERVER: TRATAR CLIENTE - LIST_USERS solicitado por usuario: '%s'\n", list_user);
                		pthread_mutex_lock(&users_mutex);
                		tratar_list_users(client_socket, list_user); // funcion
                        //printf("SERVER: TRATAR CLIENTE - LIST_USERS ya hemos llamado a tratar_list_users\n");
                		pthread_mutex_unlock(&users_mutex);
                	} else {
                		//perror("SERVER: TRATAR CLIENTE - Error al leer el usuario para LIST_USERS");
                		unsigned char error_code = 3;
                		send(client_socket, &error_code, 1, 0);
                	}
                }
                else if (strcmp(comando, "LIST_CONTENT") == 0) {
				    //printf("SERVER: TRATAR CLIENTE - Recibido comando LIST_CONTENT del cliente %d\n", client_socket);

                    // nombre del usuario que está solicitando la lista de contenido
                    char requesting_user[MAX_USERNAME];
				    ssize_t bytes_requesting_user = readLine(client_socket, requesting_user, sizeof(requesting_user));
				    if (bytes_requesting_user <= 0) {
				        //perror("SERVER: TRATAR CLIENTE - Error al leer el nombre del usuario solicitante para LIST_CONTENT");
				        unsigned char error_code = 4; // Otro error
				        send(client_socket, &error_code, 1, 0);
				    }
				    requesting_user[strcspn(requesting_user, "\n")] = 0;
				    //printf("SERVER: TRATAR CLIENTE - Solicitante de LIST_CONTENT: %s\n", requesting_user);
                	printf(" FROM %s\n", requesting_user);
                    // nombre del usuario cuyo contenido se está solicitando
				    char target_user[MAX_USERNAME];
				    ssize_t bytes_target_user = readLine(client_socket, target_user, sizeof(target_user));
				    if (bytes_target_user <= 0) {
				        //perror("SERVER: TRATAR CLIENTE - Error al leer el nombre del usuario objetivo para LIST_CONTENT");
				        unsigned char error_code = 4; // Otro error
				        send(client_socket, &error_code, 1, 0);
				    }
				    target_user[strcspn(target_user, "\n")] = 0;
				    //printf("SERVER: TRATAR CLIENTE - Usuario objetivo para LIST_CONTENT: %s\n", target_user);

				    pthread_mutex_lock(&users_mutex);
                    // comprobamos que los usuarios existan
				    InfoUsuario* req_user_info = find_user(requesting_user);
				    InfoUsuario* tgt_user_info = find_user(target_user);
				    unsigned char result_code = 0;

				    if (req_user_info == NULL) {
				        result_code = 1; // USER DOES NOT EXIST (solicitante)
				    } else if (!req_user_info->conectado) {
				        result_code = 2; // USER NOT CONNECTED (solicitante)
				    } else if (tgt_user_info == NULL) {
				        result_code = 3; // REMOTE USER DOES NOT EXIST
				    } else {
				        result_code = 0; // OK
				        send(client_socket, &result_code, 1, 0);
				        enviar_lista_contenido(client_socket, tgt_user_info);
				    }
				    pthread_mutex_unlock(&users_mutex);

				    if (result_code != 0) {
				        send(client_socket, &result_code, 1, 0);
				    }
				}
            }
         	else if (strncmp(comando, "QUIT", 4) == 0){
                //printf("SERVER: TRATAR CLIENTE - Se ha recibido un \"QUIT\" del cliente %d\n", client_socket);
                break;
            }
            else {
                result_code = 2; // Otro error
                send(client_socket, &result_code, 1, 0);
                //printf("SERVER: TRATAR CLIENTE - Comando desconocido o error, respuesta enviada: %d\n", result_code);
        	}
        }
        else if (bytes_recibidos == 0) {
            //printf("SERVER: TRATAR CLIENTE - Cliente %d cerró la conexión.\n", client_socket);
            break;
        }
        else {
            //perror("SERVER: TRATAR CLIENTE - ERROR al leer del cliente");
            break;
        }
    }

    //printf("SERVER: TRATAR CLIENTE - Cerramos conexión con cliente %d\n", client_socket);
    close(client_socket);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
  	int sd, val, err, client, port;
	socklen_t size;
	pthread_t thid;
	pthread_attr_t attr;
  	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	// comprobamos argumentos
	if (argc != 3 || strcmp(argv[1], "-p") != 0) {
		fprintf(stderr, "Uso: %s -p <puerto>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

  	port = atoi(argv[2]);
  	if (port < 1024 || port > 49151){
    	perror("SERVER: ERROR: port must be between 1024 and 49151");
   	 	exit(1);
  	}

  	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if (sd < 0) {
   		perror("SERVER: ERROR in socket");
   		exit(1);
 	}

  	val = 1;
  	err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
  	if (err < 0) {
    	perror("SERVER: ERROR in opcion");
    	exit(1);
  	}

  	// llenamos la serverAddres de zeros, para evitar datos basura
  	// bzero((char *) &server_addr, sizeof(server_addr));
	memset(&server_addr, 0, sizeof(server_addr));
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_port = htons(port);
  	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // solo escucha en localhost



  	err = bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  	if (err < 0) {
    	printf("SERVER: ERROR en el bind\n");
    	return -1;
  	}

	// obtenemos IP y puerto
	if (getsockname(sd, (struct sockaddr *)&server_addr, &addr_len) == -1) {
		perror("getsockname");
		exit(EXIT_FAILURE);
	}

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &server_addr.sin_addr, ip, sizeof(ip));

  	listen(sd, SOMAXCONN);

  	pthread_attr_init(&attr);
  	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	g_server_sd = sd; // guardamos server socket descriptor globalmente
	signal(SIGINT, sigint_handler);


	// Mostrar IP y puerto (IP real local opcionalmente con getsockname si se desea)
	printf("s> init server %s:%d\n", ip, port);  // O sustituir por IP local real
	printf("s>\n");

  	while(1) {
    	size = sizeof(client_addr);
    	client = accept(sd, (struct sockaddr *) &client_addr, &size);

    	if (client < 0) {
      		printf("SERVER: ERROR en el accept");
      		return -1;
    	}

    	int* new_client = malloc(sizeof(int));
    	if (new_client == NULL) {
      		perror("SERVER: ERROR al asignar memoria para el socket del cliente");
      		close(client);
      		continue;
    	}

    	*new_client = client;
    	err = pthread_create(&thid, &attr, tratar_cliente, (void*)new_client);
    	if (err != 0){
      		printf("SERVER: ERROR al crear un hilo");
      		free(new_client);
      		close(client);
      		return -1;
    	}
  	}

  	sleep(5);

  	//printf("SERVER: Cerramos sd\n");
  	close(sd);

  	return 0;
}