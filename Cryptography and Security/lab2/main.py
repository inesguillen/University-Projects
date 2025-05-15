import os
import json
from registro import RegistroUsuario
from inicio_sesion import InicioSesion
from gestion_archivos import GestionArchivos
from gestion_firmas import GestionFirmas
#from gestion_certificados import GestionCertificados
from certificado_ac1 import GenerarAC1

def menu():
    print("Bienvenido a su gestor de archivos médicos. Por favor, seleccione una opción:")
    print("1. Iniciar Sesión\n2. Registro\n3. Salir")

# Generamos el AC1 UNA SOLA VEZ
# porque solo nosotras "las desarrolladoras de la app" tendremos acceso a esta clave privada,
# que usaremos, junto con el certificado ac1, para firmar los certificados de los usuarios
generar_ac1 = GenerarAC1()
certificado_ac1, clave_privada_ac1 = generar_ac1.generar_certificado_ac1()


salir = False

while not salir:
    menu()

    try: # usamos un try para evitar al excepcion que surge si nuestro input no fuera un número
        opcion = int(input("\nOpción: "))
        if opcion == 3:
            print("Saliendo...")
            salir = True

        elif opcion == 2:
            print("Regístrese en la base de datos")
            usuario = input("Nombre de usuario: ")
            password = input("Contraseña: ")
            password2 = input("Repita contraseña: ")
            nombre = input("Nombre: ")
            apellido1 = input("Primer apellido: ")
            apellido2 = input("Segundo apellido (opcional): ")
            mail = input("Correo electrónico: ")
            centro_medico = input("Centro médico: ")

            nuevo_usuario = RegistroUsuario(usuario, password, password2, nombre, apellido1, apellido2, mail, centro_medico)
            nuevo_usuario.registrar_usuario()

            #print(nuevo_usuario)

        elif opcion == 1:
            usuario = input("Nombre de usuario: ")
            print("Revisando usuario...")

            iniciar_sesion = InicioSesion(usuario)
            # PRIMERO, VALIDAR QUE EL USUARIO EXISTE
            validado = iniciar_sesion.validar_usuario()

            if validado:
                print("Usuario existente. Elija un método de inicio de sesión\n")
                print("1. Contraseña\n2. Certificado\n")
                eleccion = int(input("Opción: "))
                sesion_iniciada = False # de primeras iniciamos con esto falso

                if eleccion == 1:
                    intentos = 0
                    while intentos < 3 and not sesion_iniciada: # maximo tres intentos de contraseña
                        password = input("Contraseña: ")
                        # a parte de checkear los datos, devuelve un bool que es True si se inicó sesión correctamente
                        sesion_iniciada, clave = iniciar_sesion.iniciar_sesion_contrasena(password)

                        if not sesion_iniciada: # contraseña incorrecta
                            intentos += 1
                            print("Contraseña incorrecta, pruebe de nuevo\n")
                            print("Intentos restantes: " + str(3 - intentos) + "\n")

                    # tras gastar todos los intentos
                    if not sesion_iniciada:  # Si no logró iniciar sesión
                        print("Demasiados intentos fallidos, inténtelo más tarde")
                        break

                elif eleccion == 2:
                    print("Iniciando sesión con certificado...")
                    if iniciar_sesion.autenticar_con_certificado():
                        sesion_iniciada = True
                    else:
                        print("Autenticación con certificado fallida.")
                        break

                else:
                    print("Opción no válida")
                    break


            else:
                print("Saliendo de la aplicación...")
                break #usuario no existente


            if sesion_iniciada:
                gestion_archivos = GestionArchivos(usuario, clave)
                gestion_firma = GestionFirmas(usuario, clave, gestion_archivos, certificado_ac1, clave_privada_ac1)
                #certificado_usuario = gestion_firma.generar_certificado_usuario

                if gestion_firma.private_key:
                    print("Clave privada cargada correctamente.")
                else:
                    print("Error: La clave privada no se cargó.")

                while True:
                    print("\nBienvenido, " + usuario)
                    print("\nPor favor, seleccione una opción:")
                    print("1. Ver archivos PDF\n2. Abrir archivo\n3. Subir archivo PDF\n4. Eliminar archivo PDF\n5. Firmar "
                          "archivo.\n6. Verificar firma de un archivo.\n7. Renovar certificado.\n8. Salir")
                    select = int(input("Selección: "))

                    if select == 1:
                        gestion_archivos.ver_archivos()
                    elif select == 2:
                        gestion_archivos.abrir_archivo()
                    elif select == 3:
                        gestion_archivos.subir_archivos()
                    elif select == 4:
                        gestion_archivos.eliminar_archivo()
                    elif select == 5:
                        archivo = input("Indique el archivo que desea firmar (incluyendo la ruta): ")
                        gestion_firma.firmar_archivo(archivo)
                    elif select == 6:
                        archivo = input("Indique el archivo que desea verificar (incluyendo la ruta): ")
                        gestion_firma.verificar_firma(archivo, "certificados/"+usuario+ "_cert.pem")
                    elif select == 7:
                        #print("Aquí solicitamos nuevo certificado (de momento nada jiji). Saliendo...")
                        #break
                        #necesitamos los datos de nuestro usuario
                        if os.path.exists("usuarios.json"):
                            with open("usuarios.json", 'r') as file:
                                datos = json.load(file)  # Cargar todo el JSON
                                datos_usuario = datos["usuarios"].get(usuario, None)  # Buscar solo el usuario específico

                        gestion_firma.renovar_certificado_usuario(usuario, datos_usuario)

                    elif select == 8:
                        print("\nCerrando sesión...\n")
                        break
                    else:
                        print("Opción no válida.")

        else:
            print("Opción no válida")


    except ValueError:
        print("\nOpción no válida. Debe introducir un número\n")