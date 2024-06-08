import re
from collections import defaultdict

from networkx import min_edge_cover

# Fonction pour nettoyer les séquences d'échappement ANSI
def remove_ansi_escape_sequences(text):
    ansi_escape = re.compile(r'\x1b\[([0-9;]*m)')
    return ansi_escape.sub('', text)

# Initialiser un dictionnaire pour stocker les compteurs pour chaque processus
process_counts = defaultdict(lambda: {'is thinking': 0, 'is sleeping': 0, 'has taken a fork': 0, 'is eating': 0})

# Ouvrir et lire le fichier 'caca'
with open('caca', 'r') as file:
    log_data = file.read().strip()

# Nettoyer les séquences d'échappement ANSI
cleaned_data = remove_ansi_escape_sequences(log_data)

# Diviser les lignes correctement
log_lines = re.split(r'\n', cleaned_data)

# Analyser chaque ligne du fichier
for line in log_lines:
    line = line.strip().strip("'")
    match = re.match(r'\d+ (\d+) (.+)', line)
    if match:
        process_id = int(match.group(1))
        message = match.group(2)
        if message in process_counts[process_id]:
            process_counts[process_id][message] += 1

# Demander à l'utilisateur le nombre minimum de fois qu'un processus doit être en train de manger
min_eating_count = int(input("Entrez le nombre minimum de fois qu'un processus doit être en train de manger : "))
# min_eating_count = 26

# Afficher les résultats
for process_id, counts in process_counts.items():
    print(f"Processus {process_id}:")
    for message, count in counts.items():
        print(f"  {message}: {count}")

# Vérifier si chaque processus a mangé au moins le nombre minimum de fois
for process_id, counts in process_counts.items():
    if counts['is eating'] < min_eating_count:
        print(f"Processus {process_id} n'a pas mangé assez de fois : {counts['is eating']} fois (minimum requis : {min_eating_count} fois)")