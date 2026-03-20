import json
import csv

def deduplicate_transactions(transaction_list):
    """
    Algoritmo de deduplicación con complejidad O(n).
    
    """
    seen_ids = set()
    unique_transactions = []
    
    for tx in transaction_list:
        # extraer el transaction_id
        tx_id = tx.get('transaction_id')
        
        if tx_id not in seen_ids:
            unique_transactions.append(tx)
            seen_ids.add(tx_id)
            
    return unique_transactions

def load_data(file_path):
    if file_path.endswith('.json'):
        with open(file_path, 'r') as f:
            return json.load(f)
    elif file_path.endswith('.csv'):
        with open(file_path, 'r') as f:
            return list(csv.DictReader(f))
    return []

# --- Ejecución ---
if __name__ == "__main__":
    
    # Cambia el nombre del archivo según sea necesario (soporta .json y .csv)
    input_file = 'transaction_data.json' 
    
    data = load_data(input_file)
    print(f"Registros cargados: {len(data)}")
    
    cleaned_data = deduplicate_transactions(data)
    print(f"Registros únicos: {len(cleaned_data)}")
    
    print("Muestra del resultado:", cleaned_data[:3])