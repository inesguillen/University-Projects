# CONJUNTOS
set TARIFAS;
set AVIONES;
set PISTAS_ATERRIZAJE;
set SLOTS_ATERRIZAJE;
set SLOTS_CONSECUTIVOS;

# PARAMETROS
/* Parte 1 */
param EQUIPAJE{i in TARIFAS};
param PRECIO{i in TARIFAS};
param MIN_BILLETES{i in TARIFAS};
param ASIENTOS{j in AVIONES};
param CAPACIDAD{j in AVIONES};

/* Parte 2 */
param TIEMPOS_ATERRIZAJE{i in PISTAS_ATERRIZAJE, j in SLOTS_ATERRIZAJE};
param SLOTS_ATERRIZAJE_VALIDOS{i in PISTAS_ATERRIZAJE, j in SLOTS_ATERRIZAJE};
param LLEGADA_AVIONES{i in AVIONES};
param LLEGADA_AVIONES_LIM{i in AVIONES};
param LLEGADA_AVIONES_COSTE{i in AVIONES};
param CONSECUTIVOS{i in SLOTS_CONSECUTIVOS, j in SLOTS_ATERRIZAJE};

# VARIABLES
/* Parte 1 */
var billetes_avion_tarifa{i in AVIONES, j in TARIFAS} integer, >= 0;

/* Parte 2 */
var asig_avion_aterrizaje{i in AVIONES, j in PISTAS_ATERRIZAJE, k in SLOTS_ATERRIZAJE}, binary;

# FUNCION OBJETIVO
maximize BENEFICIOS: 
    sum{i in AVIONES, j in TARIFAS} (billetes_avion_tarifa[i,j] * PRECIO[j]) 
    - sum{i in AVIONES, j in PISTAS_ATERRIZAJE, k in SLOTS_ATERRIZAJE} (
        asig_avion_aterrizaje[i,j,k] * max(0, TIEMPOS_ATERRIZAJE[j,k] - LLEGADA_AVIONES[i]) * LLEGADA_AVIONES_COSTE[i]
    );

# RESTRICCIONES
/* Parte 1 */
/* No se deben vender mas billetes que asientos */
s.t. limiteAsientos{i in AVIONES} : sum{j in TARIFAS} billetes_avion_tarifa[i,j] <= ASIENTOS[i];

/* No se debe superar la capacidad maxima de cada avion */
s.t. limiteEquipaje{i in AVIONES} : sum{j in TARIFAS} billetes_avion_tarifa[i,j]*EQUIPAJE[j] <= CAPACIDAD[i];

/* Para cada avion se deben ofertar como minimo 20 billetes leisure y 10 de business */
s.t. limiteTipo{i in AVIONES, j in TARIFAS :  j in {'leisure', 'business'}}: 
    			billetes_avion_tarifa[i,j] >= MIN_BILLETES[j];
				
/* El numero de billetes estándar totales debe ser de al menos un 60% de todos los billetes ofertados */
s.t. limiteProporcion: sum{i in AVIONES} billetes_avion_tarifa[i,'estandar'] >= 0.6 * sum{i in AVIONES, j in TARIFAS} billetes_avion_tarifa[i,j];

/* Parte 2 */
/* Todos los aviones tienen que tener asignado un slot */
s.t. solo_un_slot{i in AVIONES}:  sum{j in PISTAS_ATERRIZAJE, k in SLOTS_ATERRIZAJE} asig_avion_aterrizaje[i,j,k] = 1;

/* Un slot puede estar como maximo asignado a un avion */
s.t. solo_un_slot_avion{j in PISTAS_ATERRIZAJE, k in SLOTS_ATERRIZAJE}:  sum{i in AVIONES} asig_avion_aterrizaje[i,j,k] <= 1;

/* El slot asignado tiene que ser valido */
s.t. solo_un_slot_valido{i in AVIONES,j in PISTAS_ATERRIZAJE, k in SLOTS_ATERRIZAJE}: asig_avion_aterrizaje[i,j,k] <= SLOTS_ATERRIZAJE_VALIDOS[j,k];

/* El inicio del slot de aterrizaje debe ser igual o posterior a la hora de llegada del vuelo */
s.t. inicio{i in AVIONES, j in PISTAS_ATERRIZAJE, k in SLOTS_ATERRIZAJE}:
	asig_avion_aterrizaje[i,j,k]*TIEMPOS_ATERRIZAJE[j,k] >=  asig_avion_aterrizaje[i,j,k]*max(0,LLEGADA_AVIONES[i]);

/* El inicio del slot de aterrizaje debe ser igual o anterior a la hora limite de aterrizaje */
s.t. inicio_lim{i in AVIONES, j in PISTAS_ATERRIZAJE, k in SLOTS_ATERRIZAJE}: 
	asig_avion_aterrizaje[i,j,k]*TIEMPOS_ATERRIZAJE[j,k] <= asig_avion_aterrizaje[i,j,k]*LLEGADA_AVIONES_LIM[i];

/* Por cuestiones de seguridad, no se pueden ocupar dos slots consecutivos en la misma pista. */
s.t. solo_slot_consecutivos{j in PISTAS_ATERRIZAJE, h in SLOTS_CONSECUTIVOS}: 
	sum{i in AVIONES, k in SLOTS_ATERRIZAJE} asig_avion_aterrizaje[i,j,k] * CONSECUTIVOS[h,k] <= 1;

end;
