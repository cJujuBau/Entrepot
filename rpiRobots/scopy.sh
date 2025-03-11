#!/bin/bash

# Variables
LOCAL_SRC_DIR="./src"
LOCAL_INCLUDE_DIR="./include"
LOCAL_MAKEFILE="./makefile"
LOCAL_PARAM_FILE="./param.json"
REMOTE_USER="pi"
REMOTE_HOST="172.20.10.6"
# REMOTE_HOST="192.168.1.24"  # Remplacez par l'adresse IP de votre Raspberry Pi
REMOTE_DIR="/home/pi/entrepot"

# Copier le Makefile
rsync -avz --info=stats0 --update "$LOCAL_MAKEFILE" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR/"

# Copier le dossier src
rsync -avz --info=stats0 --update "$LOCAL_SRC_DIR/" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR/src/"

# Copier le dossier include
rsync -avz --info=stats0 --update "$LOCAL_INCLUDE_DIR/" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR/include/"

# Copier le fichier param.json
# rsync -avz --info=stats0 --update "$LOCAL_PARAM_FILE" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR/"




echo "Les dossiers src, include  ont été copiés avec succès sur le Raspberry Pi."