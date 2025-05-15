from constraint import Problem, AllDifferentConstraint
import re

# Leer y parsear el archivo de entrada
def parse_input(file_path):
    try:
        with open(file_path, 'r') as file: #abre el archivo en modo lectura
            lines = file.readlines() #almacena las lines como lista de cadenas

        # Validación básica del formato
        if len(lines) < 5:
            raise ValueError("El archivo de entrada tiene menos de 5 líneas necesarias.")

        num_franjas = int(lines[0].strip().split(":")[1].strip()) #franjas horarias
        dimensiones = tuple(map(int, lines[1].strip().split('x'))) #tamaño de la matriz
        talleres_std = parse_positions(lines[2].strip()) #posiciones talleres estándar
        talleres_spc = parse_positions(lines[3].strip()) #posiciones talleres especialistas
        parkings = parse_positions(lines[4].strip()) #posiciones parkings
        aviones = parse_planes(lines[5:])

        return num_franjas, dimensiones, talleres_std, talleres_spc, parkings, aviones
    except Exception as e:
        print(f"Error al leer o procesar el archivo de entrada: {e}")
        exit(1)

#procesa las posiciones de parkings y talleres
#las posiciones dispoinbles son los valores que las variables de los aviones pueden tomar
def parse_positions(line): #recibe una linea de coordenadas
    try:
        return [tuple(map(int, match)) for match in re.findall(r'\((\d+),(\d+)\)', line)]
        #re.findall  busca parejas encerradas entre ()
        #devuelve una lista de tuplas de numeros enteros, para que sea más facil de manejar
    except Exception as e:
        raise ValueError(f"Error al parsear posiciones: {e}")

def parse_planes(lines): #hace algo similar con las lineas de los aviones
    aviones = []
    for line in lines:
        parts = line.strip().split('-') #divide cada linea en partes separadas por -
        #porque el formato es este: ID-TIPO-RESTR-T1-T2
        if len(parts) == 5: #si no tiene 5 partes, se omite la línea
            avion_id = f"{parts[0]}-{parts[1]}-{parts[2]}-{parts[3]}"
            aviones.append({
                'id': avion_id, #identificador del avion
                'tipo': parts[1], #tipo: jmb o stn
                'restr': parts[2] == 'T', #restriccion especial: true or false
                't1': int(parts[3]), #tiempo para mantenimiento estándar
                't2': int(parts[4]), #tiempo para mantenimiento especializado
            })
    return aviones #devuelve la lista de los aviones y sus datos
    #se estructura de tal manera que se pueda utilizar en el modelo csp

def model_csp(num_franjas, dimensiones, talleres_std, talleres_spc, parkings, aviones):
    problem = Problem()

    # Crear variables por avión y franja horaria
    for plane in aviones:
        for t in range(num_franjas):
            variable = f"{plane['id']}_{t}" #nombra la var con el id del avion y la franja horaria corresp
            problem.addVariable(variable, talleres_std + talleres_spc + parkings)
            #asigna un dominio (posiciones disponibles) a cada variable

    # Restricción 1: Asegurar que en cada franja los aviones estén en diferentes posiciones
    for t in range(num_franjas):
        plane_variables = [f"{plane['id']}_{t}" for plane in aviones]
        problem.addConstraint(AllDifferentConstraint(), plane_variables)
        #garantiza que las variables dadas no compartan el mismo valor

    # Restricción 2: Validar capacidad de talleres (máximo 2 aviones por taller)
    def validar_capacidad(*positions): #recibe las posiciones de los aviones
        talleres = {}
        for pos in positions: #esto se hace para agrupar
            if pos not in talleres:
                talleres[pos] = [] #si la posicion no esta en talleres, lo agrega como clave con un valor vacío(lista)
            talleres[pos].append(pos) #agrega la posicion actual que representa al avión

        for planes_at_pos in talleres.values():
            if len(planes_at_pos) > 2:  # Máximo 2 aviones por taller
                return False  #devuelve true si solo tiene dos aviones y false si tiene más

            # Contar aviones JUMBO en talleres especialistas
            if sum(1 for p in planes_at_pos if p in talleres_spc) > 1:  # Máximo 1 JMB
                return False #false si hay mas de 1 jmb
        return True

    for t in range(num_franjas):
        problem.addConstraint(validar_capacidad, [f"{plane['id']}_{t}" for plane in aviones])

    # Restricción 3: Talleres especialistas para tareas T2
    for plane in aviones:
        if plane['restr']:
            for i in range(plane['t2']):
                problem.addConstraint(lambda x: x in talleres_spc, [f"{plane['id']}_{i}"])
            for i in range(plane['t2'], plane['t2'] + plane['t1']):
                problem.addConstraint(lambda x: x in talleres_std + talleres_spc, [f"{plane['id']}_{i}"])

    # Restricción 4: Orden de tareas de mantenimiento
    def ordenar_tareas(plane, *args):
        tareas_tipo_2 = [i for i, tarea in enumerate(args) if "tipo 2" in tarea]
        tareas_tipo_1 = [i for i, tarea in enumerate(args) if "tipo 1" in tarea]
        return all(j > i for i in tareas_tipo_2 for j in tareas_tipo_1)

    for plane in aviones:
        tareas = [f"{plane['id']}_{i}" for i in range(plane['t2'] + plane['t1'])]
        if plane['t2'] > 0:
            for t in range(min(plane['t2'] + plane['t1'], num_franjas)):
                problem.addConstraint(ordenar_tareas, tareas)

    # Restricción 5: Maniobrabilidad y JMB no adyacentes
    def validar_maniobrabilidad(pos, *adj_pos):
        x, y = pos
        adyacentes = [(x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)]
        adyacentes_validos = [(x, y) for x, y in adyacentes if 0 <= x < dimensiones[0] and 0 <= y < dimensiones[1]]
        return all(adj not in adj_pos for adj in adyacentes_validos)

    for t in range(num_franjas):
        for plane in aviones:
            problem.addConstraint(validar_maniobrabilidad, [f"{plane['id']}_{t}"] + [f"{adj_plane['id']}_{t}" for adj_plane in aviones if adj_plane != plane])

    # Restricción 6: Jumbos no pueden estar en talleres adyacentes
    def no_jumbos_adyacentes(*args):
        jumbos = [pos for pos in args if "JMB" in pos]
        for i, pos1 in enumerate(jumbos):
            x1, y1 = pos1
            for j, pos2 in enumerate(jumbos):
                if i != j:
                    x2, y2 = pos2
                    if abs(x1 - x2) <= 1 and abs(y1 - y2) <= 1:  # Adyacentes
                        return False
        return True

    for t in range(num_franjas):
        variables = [f"{plane['id']}_{t}" for plane in aviones]
        problem.addConstraint(no_jumbos_adyacentes, variables)

    return problem



def write_output(output_path, solutions, aviones, talleres_std, talleres_spc, parkings):
    # Abrir el archivo para escribir los resultados
    with open(output_path, 'w') as file:
        file.write(f"N. Sol: {len(solutions)}\n")  # Escribir el número de soluciones
        for idx, solution in enumerate(solutions, 1):  # Iterar sobre cada solución
            file.write(f"Solucion {idx}:\n")
            result = []
            for plane in aviones:  # Iterar sobre cada avión
                asignaciones = []
                for t in range(len(solution) // len(aviones)):  # Suponiendo que cada avión tiene una franja en cada paso
                    var = f"{plane['id']}_{t}"  # Clave para el avión en la franja t
                    pos = solution.get(var)  # Obtener la posición asignada en esa franja
                    if pos:
                        # Determinar el tipo de posición
                        pos_str = (
                            f"STD{pos}" if pos in talleres_std else
                            f"SPC{pos}" if pos in talleres_spc else
                            f"PRK{pos}" if pos in parkings else "ERROR"
                        )
                        asignaciones.append(pos_str)  # Agregar la asignación a la lista
                # Unir las asignaciones y agregarlas a la lista de resultados para esa solución
                result.append(f"{plane['id']}: {', '.join(asignaciones)}")
            # Escribir la solución con todas las asignaciones de aviones
            file.write(f"{', '.join(result)}\n")


def main():
    import sys
    if len(sys.argv) != 2:
        print("Uso: python CSPMaintenance.py <path maintenance>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = input_path.replace('.txt', '.csv')

    num_franjas, dimensiones, talleres_std, talleres_spc, parkings, aviones = parse_input(input_path)
    problem = model_csp(num_franjas, dimensiones, talleres_std, talleres_spc, parkings, aviones)
    solutions = problem.getSolutions()
    write_output(output_path, solutions, aviones, talleres_std, talleres_spc, parkings)

if __name__ == "__main__":
    main()
