#!/bin/bash

CLUSTER_NAME=$(/usr/share/google/get_metadata_value attributes/dataproc-cluster-name)

# usunięcie tematów
if kafka-topics.sh --bootstrap-server ${CLUSTER_NAME}-w-1:9092 --list | grep -q "input-topic"; then
    kafka-topics.sh --delete \
    --bootstrap-server ${CLUSTER_NAME}-w-1:9092 \
    --topic input-topic
fi

if kafka-topics.sh --bootstrap-server ${CLUSTER_NAME}-w-1:9092 --list | grep -q "output-topic"; then
    kafka-topics.sh --delete \
    --bootstrap-server ${CLUSTER_NAME}-w-1:9092 \
    --topic output-topic
fi

if kafka-topics.sh --bootstrap-server ${CLUSTER_NAME}-w-1:9092 --list | grep -q "alert-topic"; then
    kafka-topics.sh --delete \
    --bootstrap-server ${CLUSTER_NAME}-w-1:9092 \
    --topic alert-topic
fi

# utworzenie tematów
kafka-topics.sh --create \
--bootstrap-server ${CLUSTER_NAME}-w-1:9092 \
--replication-factor 2 --partitions 3 --topic input-topic

kafka-topics.sh --create \
--bootstrap-server ${CLUSTER_NAME}-w-1:9092 \
--replication-factor 2 --partitions 3 --topic output-topic

kafka-topics.sh --create \
--bootstrap-server ${CLUSTER_NAME}-w-1:9092 \
--replication-factor 2 --partitions 3 --topic alert-topic
