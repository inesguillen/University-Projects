import socket
import sys
import traceback
from enum import Enum
import argparse
import threading
import time
import os
import shutil

MAX_USERNAME = 256

#################### PARA IMPLEMENTACIÓN PARTE 2 ####################
import requests

def get_timestamp(): #con esta funcion el cliente llama al servicio web
    try:
        response = requests.get('http://127.0.0.1:5000/get_time') #localhost:5000
        if response.status_code == 200:
            return response.json()['timestamp']
        else:
            return "00/00/0000 00:00:00" #cadena vacia si no contacta
            # devolvemos esto para que el funcione aunque no estuviera activo
    except Exception as e:
        print(f"CLIENT: Error getting timestamp: {e}")
        return "00/00/0000 00:00:00"


class Client :

    # ******************** TYPES *********************
    # *
    # * @brief Return codes for the protocol methods

    class RC(Enum) :
        OK = 0
        ERROR = 1
        USER_ERROR = 2

    # ****************** ATTRIBUTES ******************
    _server = None
    _port = -1

    _connected_user = None
    _listening_port = -1
    connected = False # para rastrear la conexión
    _user_info_cache = {} # diccionario para almacenar la información de los usuarios

    # ****************** AUX FUNCTIONS ******************

    @staticmethod
    def _start_listening_thread(listen_socket, port):
        # iniciamos un hilo para escuchar conexiones entrantes en el puerto dado
        def listener(sock, p):
            try:
                sock.listen(5)
                #print(f"c> Escuchando para descargas en el puerto {p}...")
                while True:
                    client_socket, addr = sock.accept()
                    #print(f"c> Recibida conexión para descarga desde {addr}")

                    cliente_thread = threading.Thread(target=Client._handle_client, args=(client_socket,))
                    cliente_thread.start()

            except socket.error as e:
                print(f"c> Error al iniciar la escucha en el puerto {p}: {e}")
            finally:
                if 'sock' in locals():
                    sock.close()

        thread = threading.Thread(target=listener, args=(listen_socket, port))
        thread.daemon = True
        thread.start()
        #print(f"c> Hilo de escucha iniciado en el puerto {port}")

    @staticmethod
    def _handle_client(cliente_socket):
        try:
            # recibimos la solicitud completa
            solicitud_completa_bytes = b""
            while True:
                data = cliente_socket.recv(1024)
                if not data:
                    # cliente cerró la conexión inesperadamente
                    #print("c> Cliente remoto cerró la conexión de forma inesperada")
                    return
                solicitud_completa_bytes += data
                if b'\n' in data:  # Check for newline in the received chunk
                    break
            solicitud_completa = solicitud_completa_bytes.decode().strip()
            #print(f"c> Datos recibidos: {solicitud_completa}")

            solicitud_lines = solicitud_completa.split('\n')

            if solicitud_lines[0].startswith("GET_FILE"):
                if len(solicitud_lines) >= 2:
                    remote_file_name = solicitud_lines[1].strip()  # Get filename and remove extra spaces
                    #print(f"c> Solicitud de archivo: {remote_file_name}")

                    # construimos la ruta completa al archivo
                    user_dir = f"/tmp/archivos_{Client._connected_user}"
                    file_path = os.path.join(user_dir, remote_file_name)

                    # verificamos si el archivo existe
                    if os.path.isfile(file_path):
                        # archivo existe, enviar código 0
                        cliente_socket.sendall(b'\x00')

                        # enviamos el tamaño del archivo
                        tamano_archivo = os.path.getsize(file_path)
                        tamano_archivo_bytes = f"{tamano_archivo}\n".encode()
                        cliente_socket.sendall(tamano_archivo_bytes)

                        # contenido del archivo
                        with open(file_path, "rb") as f:
                            bytes_enviados = 0
                            while bytes_enviados < tamano_archivo:
                                data = f.read(4096)
                                if not data:
                                    # error al leer el archivo local
                                    #print("c> Error al leer el archivo local")
                                    cliente_socket.sendall(b'\x02')  # Send error code
                                    return
                                cliente_socket.sendall(data)
                                bytes_enviados += len(data)
                        #print(f"c> Archivo '{remote_file_name}' enviado correctamente ({bytes_enviados} bytes)")

                    else:
                        # archivo no existe, enviar código 1
                        cliente_socket.sendall(b'\x01')
                        #print(f"c> Archivo no encontrado: {remote_file_name}")
                else:
                    #print("c> Solicitud GET FILE incompleta")
                    cliente_socket.sendall(b'\x02')  # Indicate a general error

        except Exception as e:
            #print(f"c> Error al manejar la descarga: {e}")
            traceback.print_exc()
        finally:
            try:
                cliente_socket.close()
            except socket.error:
                pass



    @staticmethod
    def _get_user_info(username):
        # obtenemos la info de un usuario (IP, puerto)
        return Client._user_info_cache.get(username)

    @staticmethod
    def _update_user_info(username, ip, port):
        # actualizamos la info de un usuario
        Client._user_info_cache[username] = {'ip': ip, 'port': port}

    @staticmethod
    def _process_list_users_response(data):
        # procesamos la respuesta del servidor al comando LIST_USERS y actualizamos los datos
        lines = data.strip().split('\n')  # dividimos la respuesta en líneas eliminando espacios en blanco
        if not lines or lines[0] != "c> LIST_USERS OK": # tiene que empezar con este comando
            return
        try:
            num_users = int(lines[1]) if len(lines) > 1 else 0 # leemos num de usuarios
            for i in range(num_users):
                if len(lines) > 2 + (i * 3) + 2:
                    # obtenemos datos de cada usuario
                    username = lines[2 + (i * 3)]
                    ip = lines[3 + (i * 3)]
                    port = lines[4 + (i * 3)]

                    Client._update_user_info(username, ip, port) # llamamos a la func aux
        except ValueError:
            print("CLIENT: Error processing LIST_USERS response - invalid number of users.")
        except IndexError:
            print("CLIENT: Error processing LIST_USERS response - incomplete user information.")




    # ******************** METHODS *******************

    @staticmethod
    def  register(user) :
        sock = None
        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()

            # creamos socket tcp
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            sock.connect(server_address)

            # mensaje que enviamos
            #message = f"REGISTER {user}\n".encode('utf-8')
            message = f"REGISTER {user} {timestamp}\n".encode('utf-8')
            #ahora el server recibe tambien el timestamp
            sock.sendall(message)

            # respuesta del server
            result_code = sock.recv(1)

            if not result_code:
                print("c> REGISTER FAIL")
            else:
                # convertimos byte a int
                code = int.from_bytes(result_code, byteorder='big')
                if code == 0:
                    print("c> REGISTER OK")
                elif code == 1:
                    print("c> USERNAME IN USE")
                elif code == 2:
                    print("c> REGISTER FAIL")
                else:
                    print("c> REGISTER FAIL")

        except socket.error as e:
            print(f"c> REGISTER FAIL")
            print(f"c>Error de socket: {e}")
        finally:
            # cerramos socket
            if 'sock' in locals():
                sock.close()

        return Client.RC.ERROR

    @staticmethod
    def  unregister(user) :
        sock = None
        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()

            # creamos socket
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            sock.connect(server_address)

            # mensaje para el servidor
            #message = f"UNREGISTER {user}\n".encode('utf-8')
            message = f"UNREGISTER {user} {timestamp}\n".encode('utf-8')
            sock.sendall(message)

            # respuesta
            result_code = sock.recv(1)

            if not result_code:
                print("c> UNREGISTER FAIL")
            else:
                code = int.from_bytes(result_code, byteorder='big')
                if code == 0:
                    print("c> UNREGISTER OK")
                    # eliminamos carpeta del usuario
                    user_dir = f"/tmp/archivos_{user}"
                    if os.path.exists(user_dir) and os.path.isdir(user_dir):
                        try:
                            shutil.rmtree(user_dir)
                            #print(f"c> Carpeta '{user_dir}' eliminada correctamente")
                        except Exception as e:
                            print(f"c> Error al eliminar la carpeta '{user_dir}': {e}")
                elif code == 1:
                    print("c> USER DOES NOT EXIST")
                elif code == 2:
                    print("c> UNREGISTER FAIL")
                else:
                    print("c> UNREGISTER FAIL")
        except socket.timeout:
            print("c> UNREGISTER FAIL - Tiempo de espera agotado")
        except socket.error as e:
            print(f"c> UNREGISTER FAIL")
            print(f"c> CLIENT: ERROR de socket: {e}")
        finally:
            if 'sock' in locals():
                sock.close()

        return Client.RC.ERROR


    @staticmethod
    def connect(user):
        sock = None
        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()

            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            #print(f"c> Conectando a {server_address}...")
            sock.connect(server_address)

            #message = f"CONNECT {user}\n".encode('utf-8')
            message = f"CONNECT {user} {timestamp}\n".encode('utf-8')
            sock.sendall(message)

            # enviamos la ip
            local_ip = Client._server
            sock.sendall(f"{local_ip}\n".encode('utf-8'))
            #print(f"c> DEBUG: Enviando IP: {local_ip}")

            # creamos un socket para escuchar conexiones entrantes y lo enlazamos a un puerto libre
            listen_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            listen_sock.bind(('localhost', 0))
            _, listening_port = listen_sock.getsockname()
            Client._listening_port = listening_port
            Client._start_listening_thread(listen_sock, listening_port) # iniciamos hilo de escucha

            time.sleep(0.5) # esperamos un poco para asegurar que el hilo se inicia antes de enviar el puerto

            # enviamos el puerto de escucha al servidor
            sock.sendall(f"{listening_port}\n".encode('utf-8'))
            #print(f"c> DEBUG: Enviando puerto de escucha: {listening_port}")

            # recibimos respuesta
            result_code_bytes = sock.recv(1)
            code = int.from_bytes(result_code_bytes, byteorder='big')

            if code == 0:
                print("c> CONNECT OK")
                Client._connected_user = user
                user_dir = f"/tmp/archivos_{user}"
                os.makedirs(user_dir, exist_ok=True)
                #print(f"c> Carpeta creada (si no existía) para el usuario en: {user_dir}")
                Client.connected = True
                return Client.RC.OK
            elif code == 1:
                print("c> CONNECT FAIL, USER DOES NOT EXIST")
            elif code == 2:
                print("c> USER ALREADY CONNECTED")
            elif code == 3:
                print("c> CONNECT FAIL")
            else:
                print("c> CONNECT FAIL")
            return Client.RC.ERROR

        except socket.error as e:
            print(f"c> Error de socket al conectar: {e}")
            Client.connected = False
            return Client.RC.ERROR
        except ConnectionRefusedError:
            print("c> La conexión fue rechazada por el servidor")
            Client.connected = False
            return Client.RC.ERROR
        except TimeoutError:
            print("c> Se agotó el tiempo de espera para la conexión")
            Client.connected = False
            return Client.RC.ERROR
        except Exception as e:
            print(f"c> Error inesperado al conectar: {e}")
            Client.connected = False
            return Client.RC.ERROR
        finally:
            if sock:
                sock.close()

    @staticmethod
    def disconnect(user):
        sock = None
        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()

            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            sock.connect(server_address)

            message = f"DISCONNECT {user} {timestamp}\n".encode('utf-8')
            sock.sendall(message)

            result_code_bytes = sock.recv(1)
            if not result_code_bytes:
                print("c> DISCONNECT FAIL - No response from server")
                return Client.RC.ERROR
            result_code = int.from_bytes(result_code_bytes, byteorder='big')
            if result_code == 0:
                print("c> DISCONNECT OK")
                Client._connected_user = None
                Client.connected = False
                return Client.RC.OK
            elif result_code == 1:
                print("c> DISCONNECT FAIL, USER DOES NOT EXIST")
                return Client.RC.USER_ERROR
            elif result_code == 2:
                print("c> DISCONNECT FAIL, USER NOT CONNECTED")
                return Client.RC.USER_ERROR
            elif result_code == 3:
                print("c> DISCONNECT FAIL, CANNOT DISCONNECT ANOTHER USER")
                return Client.RC.USER_ERROR
            else:
                print("c> DISCONNECT FAIL")
                return Client.RC.ERROR

        except socket.error as e:
            print(f"c> DISCONNECT FAIL - Socket error: {e}")
            Client.connected = False
            return Client.RC.ERROR
        except Exception as e:
            print(f"c> DISCONNECT FAIL - Unexpected error: {e}")
            Client.connected = False
            return Client.RC.ERROR
        finally:
            if sock:
                sock.close()

    @staticmethod
    def publish(fileName, description):
        sock = None
        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()

            # obtenemos username
            user = Client._connected_user
            ruta_file = f"/tmp/archivos_{user}/{fileName}" #ruta completa
            # la extensión de haberla iría incluida en el filename

            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            sock.connect(server_address)

            #message = f"PUBLISH {timestamp}\n".encode('utf-8')
            message = f"PUBLISH {user} {ruta_file}\t{timestamp}\n".encode('utf-8')
            sock.sendall(message)

            # verificamos que el usuario esté conectado
            if Client._connected_user:
                user_message = f"{Client._connected_user}\n".encode('utf-8')
                sock.sendall(user_message)
                #print(f"c> DEBUG: Enviando usuario para PUBLISH: {Client._connected_user}")
            else:
                print("c> PUBLISH FAIL, USER NOT CONNECTED")
                return Client.RC.ERROR

            time.sleep(0.1)

            # validamos del nombre de archivo
            if ' ' in fileName:
                print("c> PUBLISH FAIL, FILE PATH CANNOT CONTAIN SPACES")
                return Client.RC.ERROR
            # y su longitud
            if len(fileName) > 256:
                print("c> PUBLISH FAIL, FILE PATH TOO LONG")
                return Client.RC.ERROR

            # enviamos el nombre del fichero
            file_path_message = f"{fileName}\n".encode('utf-8')
            sock.sendall(file_path_message)
            #print(f"c> DEBUG: Enviando nombre de fichero para PUBLISH: {fileName}")

            # validamos la longitud de la descripción
            if len(description) > 256:
                print("c> PUBLISH FAIL, DESCRIPTION TOO LONG")
                return Client.RC.ERROR

            # enviamos la descripción
            description_message = f"{description}\n".encode('utf-8')
            sock.sendall(description_message)
            #print(f"c> DEBUG: Enviando descripción para PUBLISH: {description}")

            # recibimos respuesta
            result_code_bytes = sock.recv(1)
            if not result_code_bytes:
                print("c> PUBLISH FAIL - No response from server")
                return Client.RC.ERROR

            result_code = int.from_bytes(result_code_bytes, byteorder='big')
            if result_code == 0:
                print("c> PUBLISH OK")

                # Creamos el archivo en la carpeta del usuario
                user_dir = f"/tmp/archivos_{Client._connected_user}"
                os.makedirs(user_dir, exist_ok=True)
                file_path = os.path.join(user_dir, fileName)

                # Creamos el archivo con contenido vacío (o puedes añadir contenido dummy)
                with open(file_path, "w", encoding="utf-8") as f:
                    f.write(description + "\n")
                    #print(f"c> Archivo '{fileName}' guardado en {file_path}")

                #print(f"c> Archivo '{fileName}' creado correctamente en {user_dir}")
                return Client.RC.OK

            elif result_code == 1:
                print("c> PUBLISH FAIL, USER DOES NOT EXIST")
                return Client.RC.USER_ERROR
            elif result_code == 2:
                print("c> PUBLISH FAIL, USER NOT CONNECTED")
                return Client.RC.USER_ERROR
            elif result_code == 3:
                print("c> PUBLISH FAIL, CONTENT ALREADY PUBLISHED")
                return Client.RC.USER_ERROR
            else:
                print("c> PUBLISH FAIL")
                return Client.RC.ERROR

        except socket.error as e:
            print(f"c> PUBLISH FAIL - Socket error: {e}")
            return Client.RC.ERROR
        except Exception as e:
            print(f"c> PUBLISH FAIL - Unexpected error: {e}")
            return Client.RC.ERROR
        finally:
            if sock:
                sock.close()

    @staticmethod
    def delete(fileName):
        sock = None

        # comprobamos que el cliente esté conectado
        if not Client.connected or Client._connected_user is None:
            print("c> DELETE FAIL , NOT CONNECTED TO SERVER")
            return Client.RC.ERROR

        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()

            # obtenemos username
            user = Client._connected_user
            ruta_file = f"/tmp/archivos_{user}/{fileName}"

            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            sock.connect(server_address)

            message = f"DELETE {user} {ruta_file}\t{timestamp}\n".encode('utf-8')
            sock.sendall(message)

            # enviamos nombre del usuario
            user_message = f"{Client._connected_user}\n".encode('utf-8')
            sock.sendall(user_message)
            #print(f"c> DEBUG: Enviando usuario para DELETE: {Client._connected_user}")

            # nombre del fichero a eliminar
            filename_message = f"{fileName}\n".encode('utf-8')
            sock.sendall(filename_message)
            #print(f"c> DEBUG: Enviando nombre de fichero para DELETE: {fileName}")

            # recibimos respuesta
            result_code_bytes = sock.recv(1)
            if not result_code_bytes:
                print("c> DELETE FAIL - No response from server")
                return Client.RC.ERROR

            result_code = int.from_bytes(result_code_bytes, 'big')
            if result_code == 0:
                print("c> DELETE OK")

                # Eliminamos el archivo local del usuario
                user_dir = f"/tmp/archivos_{Client._connected_user}"
                file_path = os.path.join(user_dir, fileName)

                try:
                    if os.path.exists(file_path):
                        os.remove(file_path)
                        #print(f"c> Archivo eliminado correctamente de: {file_path}")
                except Exception as e:
                    print(f"c> ERROR al eliminar el archivo local: {e}")
                return Client.RC.OK

            elif result_code == 1:
                print("c> DELETE FAIL , USER DOES NOT EXIST")
                return Client.RC.USER_ERROR
            elif result_code == 2:
                print("c> DELETE FAIL , USER NOT CONNECTED")
                return Client.RC.USER_ERROR
            elif result_code == 3:
                print("c> DELETE FAIL , CONTENT NOT PUBLISHED")
                return Client.RC.USER_ERROR
            else:
                print("c> DELETE FAIL")
                return Client.RC.ERROR

        except socket.error as e:
            print(f"c> DELETE FAIL - Socket error: {e}")
            return Client.RC.ERROR
        except Exception as e:
            print(f"c> DELETE FAIL - Unexpected error: {e}")
            return Client.RC.ERROR
        finally:
            if sock:
                sock.close()

    @staticmethod
    def listusers():
        sock = None

        # comprobamos q este conectado y esta activo
        if not Client.connected or Client._connected_user is None:
            print("c> LIST_USERS FAIL , NOT CONNECTED TO SERVER")
            return Client.RC.ERROR
        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()
            #obtener usuario
            user = Client._connected_user

            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            sock.connect(server_address)

            message = f"LIST_USERS {user} {timestamp}\n".encode('utf-8')
            sock.sendall(message)

            user_message = f"{Client._connected_user}\n".encode('utf-8')
            sock.sendall(user_message)
            #print(f"c> DEBUG: Enviando usuario para LIST_USERS: {Client._connected_user}")

            # esperamos respuesta del servidor
            #print("c> DEBUG (LIST_USERS): Esperando result_code...")
            result_code_bytes = sock.recv(1)
            if not result_code_bytes:
                print("c> LIST_USERS FAIL (no result_code)")
                return Client.RC.ERROR
            result_code = int.from_bytes(result_code_bytes, byteorder='big')
            #print(f"c> DEBUG (LIST_USERS): Recibido result_code: {result_code}")

            #
            if result_code == 0:
                print("c> LIST_USERS OK")
                #print("c> DEBUG (LIST_USERS): Esperando num_users...")

                # leemos num de usuarios conectados
                num_users_bytes = b""
                while True:
                    chunk = sock.recv(1)
                    if not chunk: # si no recibimos ningún byte, la conexión probablemente se cerró inesperadamente
                        print("c> LIST_USERS FAIL (error al leer num_users)")
                        sock.close()
                        return Client.RC.ERROR
                    num_users_bytes += chunk
                    if b'\n' in num_users_bytes:
                        break
                #print(f"c> DEBUG (LIST_USERS): Bytes recibidos para num_users: {num_users_bytes!r}")
                try:
                    num_users_str = num_users_bytes.decode('utf-8').strip()
                    num_users = int(num_users_str) # convertir la cadena que representa la cantidad de usuarios a un entero
                    #print(f"c> DEBUG (LIST_USERS): Recibido num_users: {num_users}")
                    user_list_output = "c> LIST_USERS OK\n" + str(num_users) + "\n"

                    # por cada usuario leemos su nombre, IP y puerto
                    for _ in range(num_users):
                        #print("c> DEBUG (LIST_USERS): Esperando username...")
                        username_bytes = b"" # nombre
                        while True:
                            chunk = sock.recv(1)
                            if not chunk:
                                print("c> LIST_USERS FAIL (error al leer username)")
                                sock.close()
                                return Client.RC.ERROR
                            username_bytes += chunk
                            if b'\n' in username_bytes:
                                break
                        username = username_bytes.decode('utf-8').rstrip('\n\0')
                        user_list_output += username + "\n"

                        #print("c> DEBUG (LIST_USERS): Esperando ip_address...")
                        ip_address_bytes = b"" # ip
                        while True:
                            chunk = sock.recv(1)
                            if not chunk:
                                print("c> LIST_USERS FAIL (error al leer ip_address)")
                                sock.close()
                                return Client.RC.ERROR
                            ip_address_bytes += chunk
                            if b'\n' in ip_address_bytes:
                                break
                        ip_address = ip_address_bytes.decode('utf-8').rstrip('\n\0')
                        user_list_output += ip_address + "\n"

                        #print("c> DEBUG (LIST_USERS): Esperando port...")
                        port_bytes = b"" # puerto
                        while True:
                            chunk = sock.recv(1)
                            if not chunk:
                                print("c> LIST_USERS FAIL (error al leer port)")
                                sock.close()
                                return Client.RC.ERROR
                            port_bytes += chunk
                            if b'\n' in port_bytes:
                                break
                        port = port_bytes.decode('utf-8').rstrip('\n\0')
                        user_list_output += port + "\n"

                        print(f"\t{username} {ip_address} {port}")

                    Client._process_list_users_response(user_list_output) # procesamos la lista completa
                    return Client.RC.OK

                except ValueError:
                    print("c> LIST_USERS FAIL (ValueError num_users)")
                    sock.close()
                    return Client.RC.ERROR

            elif result_code == 1:
                print("c> LIST_USERS FAIL , USER DOES NOT EXIST")
            elif result_code == 2:
                print("c> LIST_USERS FAIL , USER NOT CONNECTED")
            else:
                print("c> LIST_USERS FAIL (unknown result_code)")

            return Client.RC.ERROR

        except socket.error as e:
            print(f"c> LIST_USERS FAIL - Socket error: {e}")
            return Client.RC.ERROR
        except Exception as e:
            print(f"c> LIST_USERS FAIL - Unexpected error: {e}")
            return Client.RC.ERROR
        finally:
            if sock:
                sock.close()

    @staticmethod
    def listcontent(user_name_remoto):
        sock = None
        if not Client.connected:
            print("c> LIST_CONTENT FAIL, NOT CONNECTED TO SERVER")
            return Client.RC.ERROR
        try:
            #obtenemos fecha y hora
            timestamp = get_timestamp()
            #obtener usuario
            user = Client._connected_user

            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (Client._server, Client._port)
            sock.connect(server_address)

            message = f"LIST_CONTENT {user} {timestamp}\n".encode('utf-8')
            sock.sendall(message)

            # enviamos el nombre del usuario que realiza la operación
            user_op = Client._connected_user
            user_op_message = f"{user_op}\n".encode('utf-8')
            sock.sendall(user_op_message)

            # enviamos el usuario cuyo contenido queremos conocer
            user_remoto_message = f"{user_name_remoto}\n".encode('utf-8')
            sock.sendall(user_remoto_message)

            # recibimos respuesta
            result_code_bytes = sock.recv(1)
            if not result_code_bytes:
                print("c> LIST_CONTENT FAIL")
                return Client.RC.ERROR

            code = int.from_bytes(result_code_bytes, byteorder='big')
            if code == 0:
                print("c> LIST_CONTENT OK")

                # leemos número de archivos publicados
                num_files_bytes = b""
                while True:
                    chunk = sock.recv(1)
                    if not chunk:
                        print("c> LIST_CONTENT FAIL - Error al leer num_files")
                        return Client.RC.ERROR
                    num_files_bytes += chunk
                    if b'\n' in num_files_bytes:
                        break
                #print(f"c> DEBUG (LIST_CONTENT): num_files_bytes received: {num_files_bytes!r}")
                try:
                    num_files = int(num_files_bytes.decode('utf-8').strip())
                    #print(f"c> DEBUG (LIST_CONTENT): num_files: {num_files}")

                    # para cada archivo, leemos su nombre
                    for _ in range(num_files):
                        filename_bytes = b""
                        while True:
                            chunk = sock.recv(1)
                            if not chunk:
                                print("c> LIST_CONTENT FAIL - Error al leer filename")
                                return Client.RC.ERROR
                            filename_bytes += chunk
                            if b'\n' in filename_bytes:
                                break
                        filename = filename_bytes.decode('utf-8').rstrip('\n\0')
                        print(f"\t{filename}")
                    return Client.RC.OK
                except ValueError:
                    print("c> LIST_CONTENT FAIL - ValueError converting num_files")
                    return Client.RC.ERROR
            elif code == 1:
                print("c> LIST_CONTENT FAIL , USER DOES NOT EXIST")
            elif code == 2:
                print("c> LIST_CONTENT FAIL , USER NOT CONNECTED")
            elif code == 3:
                print(f"c> LIST_CONTENT FAIL , REMOTE USER DOES NOT EXIST")
            else:
                print("c> LIST_CONTENT FAIL")
            return Client.RC.ERROR

        except socket.error as e:
            print(f"c> LIST_CONTENT FAIL - Socket error: {e}")
            return Client.RC.ERROR
        finally:
            # 7. Cierra la conexión.
            if sock:
                sock.close()


    @staticmethod
    def getfile(user_name_remoto, remote_file_name, local_file_name):
        sock = None
        try:
            # conectamos al cliente, de acuerdo a la dirección IP y puerto de escucha de dicho cliente
            # obtenemos la info del usuario remoto de la cache
            remote_user_info = Client._get_user_info(user_name_remoto)
            if not remote_user_info:
                print(f"c> GET_FILE FAIL: No se encontró información para el usuario {user_name_remoto}")
                return Client.RC.ERROR

            remote_ip = remote_user_info['ip']
            remote_port = int(remote_user_info['port'])

            #obtenemos fecha y hora
            timestamp = get_timestamp()

            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((remote_ip, remote_port))

            # cadena “GET FILE”
            sock.sendall(f"GET_FILE {timestamp}\n".encode())

            # cadena con el path absoluto del fichero que se desea descargar
            sock.sendall(f"{remote_file_name}\n".encode())

            # byte que codifica el resultado de la operación
            respuesta = sock.recv(1)

            if respuesta == b'\x00':  # (OK)
                # cadena de caracteres codificando el tamaño del fichero en bytes en decimal
                tamano_archivo_str = b""
                while True:
                    chunk = sock.recv(1)
                    if not chunk:
                        raise Exception("Conexión cerrada inesperadamente al recibir el tamaño del archivo")
                    tamano_archivo_str += chunk
                    if b'\n' in tamano_archivo_str:
                        break
                tamano_archivo = int(tamano_archivo_str.decode().strip())

                # contenido del archivo
                user_dir = f"/tmp/archivos_{Client._connected_user}"
                os.makedirs(user_dir, exist_ok=True)
                local_file_path = os.path.join(user_dir, local_file_name)  # Correct path

                with open(local_file_path, 'wb') as archivo_local:
                    bytes_recibidos = 0
                    while bytes_recibidos < tamano_archivo:
                        datos = sock.recv(1024)
                        if not datos:
                            raise Exception("Conexión cerrada inesperadamente durante la transferencia del archivo")
                        archivo_local.write(datos)
                        bytes_recibidos += len(datos)

                print("c> GET_FILE OK")

                ##### ES NECESARIO HACER PUBLISH PARA QUE SE GUARDE EN EL USUARIO EL NUEVO ARCHIVO COPIADO ##########
                try:
                    with open(local_file_path, 'r', encoding='utf-8') as archivo_local:
                        descripcion = archivo_local.readline().strip()
                except Exception as e:
                    print(f"c> ERROR al leer la descripción del archivo: {e}")
                    return Client.RC.ERROR

                Client.publish(local_file_name, descripcion) # publicamos el nuevo archivo
                ######################


                return Client.RC.OK

            elif respuesta == b'\x01':  # Código 1 (FILE NOT EXIST)
                print("c> GET_FILE FAIL, FILE NOT EXIST")
                return Client.RC.USER_ERROR

            else:  # Código 2 (ERROR) o cualquier otro error
                print("c> GET_FILE FAIL")
                return Client.RC.ERROR

        except Exception as e:
            print(f"c> GET_FILE FAIL: {e}")
            return Client.RC.ERROR

        finally:
            if sock:
                sock.close()
                # En caso de fallo, borra el archivo local
                if os.path.exists(local_file_name) and respuesta != b'\x00':
                    os.remove(local_file_name)



    # *
    # **
    # * @brief Command interpreter for the client. It calls the protocol functions.

    @staticmethod
    def shell():
        while (True) :
            try :
                command = input("c> ")
                line = command.split(" ")

                if (len(line) > 0):
                    line[0] = line[0].upper()
                    if (line[0]=="REGISTER") :
                        if (len(line) == 2) :
                            Client.register(line[1])
                        else :
                            print("CLIENT: Syntax error. Usage: REGISTER <userName>")

                    elif(line[0]=="UNREGISTER") :
                        if (len(line) == 2) :
                            Client.unregister(line[1])
                        else :
                            print("CLIENT: Syntax error. Usage: UNREGISTER <userName>")

                    elif(line[0]=="CONNECT") :
                        if (len(line) == 2) :
                            Client.connect(line[1])
                            #print(f"DEBUG: Client.connected = {Client.connected}, Client.sock = {Client.sock}")
                        else :
                            print("CLIENT: Syntax error. Usage: CONNECT <userName>")

                    elif(line[0]=="PUBLISH") :
                        if (len(line) >= 3) :
                            #  Remove first two words
                            description = ' '.join(line[2:])
                            Client.publish(line[1], description)
                        else :
                            print("CLIENT: Syntax error. Usage: PUBLISH <fileName> <description>")

                    elif(line[0]=="DELETE") :
                        if (len(line) == 2) :
                            Client.delete(line[1])
                        else :
                            print("CLIENT: Syntax error. Usage: DELETE <fileName>")

                    elif(line[0]=="LIST_USERS") :
                        if (len(line) == 1) :
                            Client.listusers()
                        else :
                            print("CLIENT: Syntax error. Use: LIST_USERS")

                    elif(line[0]=="LIST_CONTENT") :
                        if (len(line) == 2) :
                            Client.listcontent(line[1])
                        else :
                            print("CLIENT: Syntax error. Usage: LIST_CONTENT <userName>")

                    elif(line[0]=="DISCONNECT") :
                        if (len(line) == 2) :
                            Client.disconnect(line[1])
                        else :
                            print("CLIENT: Syntax error. Usage: DISCONNECT <userName>")

                    elif(line[0]=="GET_FILE") :
                        if (len(line) == 4) :
                            Client.getfile(line[1], line[2], line[3])
                        else :
                            print("CLIENT: Syntax error. Usage: GET_FILE <userName> <remote_fileName> <local_fileName>")

                    elif(line[0]=="QUIT") :
                        if (len(line) == 1) :
                            break
                        else :
                            print("CLIENT: Syntax error. Use: QUIT")

                    else :
                        print("CLIENT: ERROR: command " + line[0] + " not valid.")

            except Exception as e:
                print("CLIENT: Exception: " + str(e))



    # *
    # * @brief Prints program usage
    @staticmethod
    def usage() :
        print("CLIENT: Usage: python3 client.py -s <server> -p <port>")


    # *

    # * @brief Parses program execution arguments

    @staticmethod
    def  parseArguments(argv) :
        parser = argparse.ArgumentParser()
        parser.add_argument('-s', type=str, required=True, help='Server IP')
        parser.add_argument('-p', type=int, required=True, help='Server Port')
        args = parser.parse_args()

        if (args.s is None):
            parser.error("CLIENT: Usage: python3 client.py -s <server> -p <port>")
            return False

        if ((args.p < 1024) or (args.p > 65535)):
            parser.error("CLIENT: ERROR: Port must be in the range 1024 <= port <= 65535");
            return False;

        Client._server = args.s
        Client._port = args.p
        return True

    # ******************** MAIN *********************

    @staticmethod
    def main(argv) :
        if (not Client.parseArguments(argv)) :
            Client.usage()
            return

        #  Write code here



        Client.shell()

        print("+++ FINISHED CLIENT +++")


if __name__=="__main__":
    if Client.parseArguments(sys.argv[1:]):
        Client.shell()