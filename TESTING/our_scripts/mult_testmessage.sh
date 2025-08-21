#!/bin/bash

# Crée un dossier "out" pour stocker les réponses
mkdir -p out

# Lance 50 clients simultanément
for ((i=1; i<=50; i++)); do
	./TESTING/our_scripts/testmessage.sh > "out/client_${i}.txt" &
done

# Attend que tous les clients aient terminé
wait