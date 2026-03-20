# Reporte de Práctica 2: Algoritmos de Deduplicación de Transacciones

**Autores:** González González Erick Emiliano & De La Rosa Hernández Tania  
**Materia:** Análisis y Diseño de Algoritmos  
**Fecha:** Marzo 2026

---

## 1. Descripción del Problema
Se requiere desarrollar una solución eficiente para un pipeline de datos que recibe registros de transacciones financieras. Debido a errores en el proceso de captura, los registros pueden presentar identificadores duplicados (`transaction_id`). 

El objetivo es implementar una función que filtre estos duplicados, manteniendo únicamente la **primera aparición** de cada ID, preservando el orden original y garantizando la integridad de los montos asociados a esa primera entrada.

## 2. Soluciones Implementadas

Para esta práctica se presentan dos enfoques en lenguajes distintos, priorizando la eficiencia en el tiempo de ejecución.

### A. Implementación en Python (Alta Abstracción)
Se utilizó la estructura de datos `set` de Python. Al estar implementada internamente como una tabla hash, permite realizar búsquedas y membresías en tiempo constante.

```python
import json
import csv

def deduplicate_transactions(transaction_list):
    seen_ids = set()
    unique_transactions = []
    
    for tx in transaction_list:
        tx_id = tx.get('transaction_id')
        if tx_id not in seen_ids:
            unique_transactions.append(tx)
            seen_ids.add(tx_id)
            
    return unique_transactions
```

### B. Implementación en C (Control de Memoria y Bajo Nivel)
En C, ante la falta de una librería estándar para conjuntos, se optó por construir una **Tabla Hash con Encadenamiento**. Esta estructura es vital para manejar colisiones y asegurar que el rendimiento no se degrade a medida que el volumen de datos (como el archivo de 20k registros) aumenta.

```c
// Fragmento principal de la lógica en C
bool is_duplicate_and_insert(Node** table, char* id) {
    unsigned int index = hash_function(id);
    Node* current = table[index];
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) return true;
        current = current->next;
    }
    // Inserción de nuevo ID no visto
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->id, id);
    newNode->next = table[index];
    table[index] = newNode;
    return false;
}
```

## 3. Análisis de Complejidad
Sea $n$ el número total de transacciones en el archivo de entrada:

| Tipo | Complejidad | Justificación |
|----------|----------|----------|
| **Tiempo**    | $O(n)$  | El algoritmo recorre la lista una sola vez. Gracias a la Tabla Hash (o Set), cada verificación de duplicado toma $O(1)$ en promedio.   |
| **Memoria**    | $O(k)$   | Donde $k$ es el número de IDs únicos. Debemos almacenar cada identificador único en una estructura auxiliar para comparaciones futuras.  |


## 4. Consideraciones de Diseño
Para que este algoritmo sea robusto en un entorno real, se consideraron los siguientes puntos:

1. **Escalabilidad:** Si el volumen de datos superara la memoria RAM (Big Data), el diseño pasaría de una tabla hash en memoria a un procesamiento por lotes (Batch processing) o el uso de un almacén de llaves externo como Redis.

2. **Estabilidad:** La solución mantiene el orden relativo de los elementos originales, lo cual es crítico en sistemas contables donde el tiempo de llegada importa.

3. **Manejo de Archivos:** Se implementó lógica para leer archivos `.csv` y `.json`, gestionando correctamente los encabezados y los diferentes tipos de parseo.

4. **Normalización:** Se asume que los IDs son sensibles a mayúsculas. Para mayor seguridad, el diseño contempla la posibilidad de normalizar los strings antes de generar el hash.

## 5. Pruebas y Resultados
Se realizaron pruebas con dos datasets principales:

* Input de Ejemplo: 5 registros, resultando en 3 únicos.

* Dataset de Gran Escala (transaction_data.csv): 5,000 registros procesados en milisegundos.

* Dataset JSON: 20,000 registros procesados exitosamente mediante el script de Python.

**Resultado de ejecución en consola (C):**

```
Procesando archivo...
--- Reporte de Ejecucion (C) ---
Total de registros leidos: 5000
Registros unicos encontrados: 4085
Duplicados eliminados: 915
```
