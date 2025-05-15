import sys
import math
import time
from heapq import heappush, heappop

# leemos el archivo CSV en el formato dado
def leer_csv(nombre_archivo):
    with open(nombre_archivo, mode='r') as archivo:
        lineas = archivo.read().splitlines()

    # Se procesa el archivo línea por línea
    num_aviones = int(lineas[0])
    inicio = []
    meta = []

    # leemos coordenadas de inicio y meta del primer avion
    inicio_meta = lineas[1].split()
    inicio.append(tuple(map(int, inicio_meta[0].strip("()").split(','))))
    meta.append(tuple(map(int, inicio_meta[1].strip("()").split(','))))

    # leemos del resto de los aviones
    for i in range(2, 2 + num_aviones - 1):
        coords = lineas[i].split()
        inicio.append(tuple(map(int, coords[0].strip("()").split(','))))
        meta.append(tuple(map(int, coords[1].strip("()").split(','))))

    # leemos mapa
    mapa = [linea.split(';') for linea in lineas[2 + num_aviones - 1:]]

    #verificamos que todas las filas tengan el mismo número de columnas
    num_columnas = len(mapa[0])
    for fila in mapa:
        if len(fila) != num_columnas:
            # se muestra un error y el programa se detiene
            print("Error: Las filas del mapa no tienen el mismo número de columnas.")
            sys.exit(1)

    return num_aviones, inicio, meta, mapa

# Heurística Manhattan
def distancia_manhattan(p1, p2):
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])

# Heurística Máximo de Manhattan
def maximo_manhattan(posiciones, metas):
    return max(distancia_manhattan(posiciones[i], metas[i]) for i in range(len(posiciones)))

# verificamos si una celda es valida
def es_valida(mapa, x, y):
    # que este dentro de los limites del mapa
    return 0 <= x < len(mapa) and 0 <= y < len(mapa[0]) and mapa[x][y] != 'G'

# expandimos los movimientos posibles de un avión
def movimientos_posibles(mapa, x, y, tipo):
    # devolvemos todas las celdas accesibles desde (x, y)
    dirs = [(0, 1), (1, 0), (0, -1), (-1, 0)]  # Movimientos posibles (→, ↓, ←, ↑)
    movimientos = []
    for dx, dy in dirs:
        nx, ny = x + dx, y + dy
        if es_valida(mapa, nx, ny):
            movimientos.append((nx, ny))
    # En blanco (B), puede detenerse
    if tipo == 'B':
        movimientos.append((x, y)) #añadimos la posibilidad de quedarse parado en celda B
    return movimientos

# convertimos una lista de posiciones en una secuencia de movimientos legibles →, ↓, w, etc
def formatear_trayectoria(trayectoria, inicio, meta):
    movimientos = [f"({inicio[0]},{inicio[1]})"] # guardamos las pos iniciales
    x1, y1 = inicio

    for i in range(1, len(trayectoria)):
        x1, y1 = trayectoria[i - 1]
        x2, y2 = trayectoria[i]
        if x1 == x2 and y1 == y2:
            movimientos.append(f"w ({x1},{y1})") #wait si se queda en la misma coordenada
        elif x1 == x2:
            # derecha si la y de la siguiente celda es mayor
            # izq si es menor
            movimientos.append(f"→ ({x2},{y2})" if y2 > y1 else f"← ({x2},{y2})")
        else:
            # baja si la x de la siguiente celda es mayor
            # sube si es menor
            movimientos.append(f"↓ ({x2},{y2})" if x2 > x1 else f"↑ ({x2},{y2})")

    x1, y1 = meta
    if trayectoria[-1] != meta:
        movimientos.append(f"w ({x1},{y1})") #si hemos llegado al final pero no todos los aviones
        # han llegado, paramos hasta terminar
    return " ".join(movimientos)

# Algoritmo A* modificado para evitar intercambios de posiciones entre aviones
def a_estrella(mapa, inicio, meta, heuristica_func):
    visitados = set() # almacena posiciones ya exploradas para evitar ciclos
    cola = [] # cola de prioridad
    estado_inicial = (0, inicio, [inicio])  # (costo acumulado, posiciones iniciales del avion, trayectoria inicial)
    heappush(cola, estado_inicial) #inserta el estado inicial en la cola
    nodos_expandidos = 0

    while cola:
        costo, posiciones, trayectoria = heappop(cola) #extrae el estado con la menor heurística de la cola
        if tuple(posiciones) in visitados:
            continue # si el estado ya fue visitado, se omite
        visitados.add(tuple(posiciones))
        nodos_expandidos += 1 #marca el estado como visitado y aumenta el contador de nodos expandidos

        # verificamos si todos los aviones alcanzaron su meta
        if all(pos == meta[i] for i, pos in enumerate(posiciones)):
            return trayectoria, nodos_expandidos #devolvemos trayectoria completa y el número de nodos expandidos

        movimientos = []
        for i, (x, y) in enumerate(posiciones): #identificamos coordenadas actuales x,y
            tipo = mapa[x][y] #determina los movimientos posibles basados en el tipo de celda actual
            #lista de listas, donde cada sublista contiene las posibles posiciones del avión correspondiente
            movimientos.append(movimientos_posibles(mapa, x, y, tipo))

        # Si hay más de un avión, generar combinaciones de movimientos para evitar colisiones
        if len(movimientos) > 1:
            for mov_comb in [(mov1, mov2) for mov1 in movimientos[0] for mov2 in movimientos[1]]:
                mov1, mov2 = mov_comb
                if mov1 != mov2:  # Evitar colisiones
                    # Restricción adicional: no se pueden intercambiar posiciones entre aviones
                    if mov1 == posiciones[1] and mov2 == posiciones[0]:
                        continue

                    nuevo_costo = costo + 1 #calculamos nuevo costo
                    nueva_posicion = [mov1, mov2] #actualizamos posiciones
                    nueva_trayectoria = trayectoria + [nueva_posicion]
                    # nueva heurística sumando el costo al valor devuelto por heuristica_func
                    heuristica = (
                        nuevo_costo +
                        heuristica_func(nueva_posicion, meta)
                    )
                    heappush(cola, (heuristica, nueva_posicion, nueva_trayectoria))
        else:
            # Si solo hay un avión, no necesitamos combinar movimientos
            for mov1 in movimientos[0]:
                nuevo_costo = costo + 1
                nueva_posicion = [mov1]
                nueva_trayectoria = trayectoria + [nueva_posicion]
                heuristica = (
                    nuevo_costo +
                    heuristica_func(nueva_posicion, meta)
                )
                heappush(cola, (heuristica, nueva_posicion, nueva_trayectoria))

    return None, nodos_expandidos  # No se encontró solución

#programa principal
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python nombredelarchivo.py <ruta_csv> <heuristica>")
        print("  <ruta_csv>: Ruta al archivo CSV con el mapa.")
        print("  <heuristica>: 1 para Manhattan, 2 para Máximo de Manhattan.")
        sys.exit(1)

    ruta_csv = sys.argv[1] #ruta de archivo .csv
    heuristica = int(sys.argv[2]) # Seleccion de heuristica
    if heuristica == 1:
        heuristica_func = lambda posiciones, metas: sum(distancia_manhattan(posiciones[i], metas[i]) for i in range(len(posiciones)))
    elif heuristica == 2:
        heuristica_func = maximo_manhattan
    else:
        print("Error: Heurística no válida. Usa 1 para Manhattan o 2 para Máximo de Manhattan.")
        sys.exit(1)

    # leemos el archivo CSV
    num_aviones, inicio, meta, mapa = leer_csv(ruta_csv)

    nombre_base = ruta_csv.split("/")[-1].split(".")[0]
    archivo_output = f"./ASTAR-tests/{nombre_base}-{heuristica}.output" #creamos archivo .output
    archivo_stat = f"./ASTAR-tests/{nombre_base}-{heuristica}.stat" #creamos archivo .stat

    tiempo_inicio = time.time() #tiempo de ejecucion
    resultado, nodos_expandidos = a_estrella(mapa, inicio, meta, heuristica_func) #ejecutamos algoritmo
    tiempo_total = time.time() - tiempo_inicio

    if resultado:
        makespan = len(resultado) - 1
        solucion = [ # lista con las coordenadas y direcciones de los aviones
            formatear_trayectoria([resultado[i][j] for i in range(len(resultado))], inicio[j], meta[j])
            for j in range(num_aviones)
        ]

        # guardamos datos en los archivos de salida
        with open(archivo_output, "w", encoding="utf-8") as output_file:
            output_file.write("\n".join(solucion))

        with open(archivo_stat, "w", encoding="utf-8") as stat_file:
            stat_file.write(f"Tiempo total: {tiempo_total:.2f}s\n")
            stat_file.write(f"Makespan: {makespan}\n")
            stat_file.write(f"h inicial: {heuristica_func(inicio, meta)}\n")
            stat_file.write(f"Nodos expandidos: {nodos_expandidos}\n")

    else:
        #no hay solucion
        print("No se encontró solución.")
